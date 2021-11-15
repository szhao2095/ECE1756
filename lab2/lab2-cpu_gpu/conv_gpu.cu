#include <stdio.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
#include <cstdlib>

#include "utils.h"

// A variable that holds the number of CPU clock cycles per second
uint64_t clocks_per_sec;
// Declare the filter coefficient as read-only values that can be stored in the GPU constant memory
// for faster access than global memory.
__device__ __constant__ int device_filter[FILTER_SIZE*FILTER_SIZE*MAX_NUM_FILTERS];

/*********************************************************************************************
 * This function reads the time stamp using inline assembly code to measure code runtime.    *
 *********************************************************************************************/
inline uint64_t rdtsc(){
	uint32_t lo, hi;
	__asm__ volatile (".byte 0x0f, 0x31" : "=a" (lo), "=d" (hi));
	return (uint64_t)(((uint64_t)hi)<<32LL) | (uint64_t) lo;
}

/*********************************************************************************************
 * This is the CUDA 2D convolution kernel. It takes as an input pointers to the input and    *
 * output images in the GPU memory, along with the image dimensions. It divides up the image *
 * into tiles of size TILE_LOAD_SIZE x TILE_LOAD_SIZE, and loads each tile into the shared   *
 * memory where a group of threads can work on it.   		     			     *
 *********************************************************************************************/
__global__ void gpu_conv2d(int* input_image, int* output_image, int image_width, int image_height) {

	// Allocate space in shared memory to load an image tile
	__shared__ int image_tile[TILE_LOAD_SIZE][TILE_LOAD_SIZE];

	// Declare variables to use throughout the kernel
	int thread_id, tile_location_x, tile_location_y;
	int image_location_x, image_location_y, pixel_id;
	
	// Load tiles from Global memory to Shared memory for faster access
	for (int itr = 0; itr <= (TILE_LOAD_SIZE * TILE_LOAD_SIZE) / (TILE_SIZE * TILE_SIZE); itr++){
		// Calculate destination x and y indecies
		thread_id = (threadIdx.y * TILE_SIZE) + threadIdx.x + (itr * TILE_SIZE * TILE_SIZE);
		tile_location_y = thread_id / TILE_LOAD_SIZE;
		tile_location_x = thread_id % TILE_LOAD_SIZE;
		
		// Calculate source pixel index
		image_location_y = blockIdx.y * TILE_SIZE + tile_location_y - FILTER_RADIUS;
		image_location_x = blockIdx.x * TILE_SIZE + tile_location_x - FILTER_RADIUS;
		pixel_id = (image_location_y * image_width) + image_location_x;
		
		// Load pixels
		if (tile_location_y < TILE_LOAD_SIZE && tile_location_x < TILE_LOAD_SIZE){
			if (image_location_y >= 0 && image_location_y < image_height 
			    && image_location_x >= 0 && image_location_x < image_width){
				image_tile[tile_location_y][tile_location_x] = input_image[pixel_id];
			} else {
				image_tile[tile_location_y][tile_location_x] = 0;
			}
		}
	}
	__syncthreads();

	// Perform the 2D convolution
	int accum = 0;
	int y, x, z;
	z = blockIdx.z;
	for (y = 0; y < FILTER_SIZE; y++) {
		for (x = 0; x < FILTER_SIZE; x++) {
			accum += image_tile[threadIdx.y + y][threadIdx.x + x] * device_filter[(z * FILTER_SIZE * FILTER_SIZE) + (y * FILTER_SIZE) + x];
		}
	}

	// Write the output
	y = blockIdx.y * TILE_SIZE + threadIdx.y;
	x = blockIdx.x * TILE_SIZE + threadIdx.x;
	if (y < image_height && x < image_width){
		output_image[(z * image_width * image_height) + (y * image_width) + x] = accum;
	}
	__syncthreads();
}


int main(int argc, char *argv[]) {
	// Parse user command line arguments
	int num_filters;		// Number of filters for convolution
	std::string input_image_name;	// Name of input image
	if (argc < 3) {
		std::cout << "Missing command line arguments. Try running:" << std::endl;
		std::cout << "./conv_gpu <image_name> <num_filters>" << std::endl;
		return 0;
	}
	input_image_name = argv[1];
	num_filters = atoi(argv[2]);

	// Get the number of CPU cycles per second
	uint64_t start_clk = rdtsc();
	usleep(100000);
	uint64_t end_clk = rdtsc();
	clocks_per_sec = (end_clk - start_clk) * 10;

	// Declare pointers for all needed buffers
	int image_width; 		// Input image width
	int image_height; 		// Input image height
	int* host_input_image;		// Input image in CPU memory
	int* host_filter;		// Convolution filter in CPU memory
	int* host_output_image_gpu;	// GPU output image in CPU memory
	int* device_input_image;	// Input image in GPU memory
	int* device_output_image;	// GPU output image in GPU memory

	// Read filter data from file
	host_filter = (int*) malloc(sizeof(int) * FILTER_SIZE * FILTER_SIZE * num_filters);
	read_filters<int>("filters", FILTER_SIZE, host_filter, num_filters);

	// Read input image size
	if(!read_pgm_image_size(input_image_name, image_width, image_height)){
		return 0;
	}

	// Read image in PGM format
	host_input_image = (int*) malloc(sizeof(int) * image_width * image_height);
	read_pgm_image<int>(input_image_name, image_width, image_height, host_input_image, 0);

	// Allocate memory for GPU and CPU output images
	host_output_image_gpu = (int*) malloc(sizeof(int) * image_width * image_height * num_filters);
	
	// Allocate GPU memory for the input and output images
	cudaCheck(cudaMalloc((void **) &device_input_image, image_width * image_height * sizeof(int)));
	cudaCheck(cudaMalloc((void **) &device_output_image, image_width * image_height * num_filters * sizeof(int)));
	
	// Create CUDA grids and blocks with size depending on the tiles of the image
	dim3 dimGrid(((image_width-1)/TILE_SIZE)+1, ((image_height-1)/TILE_SIZE)+1, num_filters);
	dim3 dimBlock(TILE_SIZE, TILE_SIZE, 1);

	// Run the convolution for multiple times and get the average runtime
	double gpu_runtime_in_ms = 0;
	for(int run = 0; run < NUM_RUNS; run++){
		// Copy input image and convolution filter from host memory to GPU memory
	   	cudaCheck(cudaMemcpy(device_input_image, host_input_image, image_width * image_height * sizeof(int), cudaMemcpyHostToDevice));
		cudaCheck(cudaMemcpyToSymbol(device_filter, host_filter, FILTER_SIZE * FILTER_SIZE * num_filters * sizeof(int), 0));
		
		// Call the CUDA kernel to perform the 2D convolution on the GPU. Record time stamp before and after to measure runtime
		start_clk = rdtsc();
		gpu_conv2d<<<dimGrid,dimBlock>>>(device_input_image, device_output_image, image_width, image_height);
		cudaDeviceSynchronize();
		end_clk = rdtsc();

		// Copy back the output image (i.e. result of convolution) from GPU memory to host memory
		cudaCheck(cudaMemcpy(host_output_image_gpu, device_output_image, image_width * image_height * num_filters * sizeof(int), cudaMemcpyDeviceToHost));
		
		gpu_runtime_in_ms += 1000.0 * (end_clk-start_clk) / clocks_per_sec;
	}
	gpu_runtime_in_ms /= NUM_RUNS;

	// Write the output images in PGM format
	for(int k = 0; k < num_filters; k++){
		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << k;
		std::string filename = std::string("out_gpu/") + oss.str() + std::string(".pgm");
		write_pgm_image<int>(filename, image_width, image_height, host_output_image_gpu + (k * image_width * image_height), 0);
	}

	// Print average GPU runtime
	std::cout << "Runtime for " << num_filters << " filter(s) averaged accross " << NUM_RUNS << " runs = " << gpu_runtime_in_ms << " ms" << std::endl;
	
	std::cout << "------------------------" << std::endl;

	// Free GPU memory
	cudaFree(device_input_image);
	cudaFree(device_output_image);
	cudaFree(device_filter);
	
	// Free CPU memory
	free(host_filter);
	free(host_input_image);
	free(host_output_image_gpu);
	
	return 0;
}
