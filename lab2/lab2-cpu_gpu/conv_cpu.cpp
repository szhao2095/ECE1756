#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
#include <immintrin.h>
#include <cstring>
#include <math.h>
#include <cstdlib>

#include "utils.h"

// A variable that holds the number of CPU clock cycles per second
uint64_t clocks_per_sec;

/*********************************************************************************************
 * This function reads the time stamp using inline assembly code to measure code runtime.    *
 *********************************************************************************************/
inline uint64_t rdtsc(){
	uint32_t lo, hi;
	__asm__ volatile (".byte 0x0f, 0x31" : "=a" (lo), "=d" (hi));
	return (uint64_t)(((uint64_t)hi)<<32LL) | (uint64_t) lo;
}

/*********************************************************************************************
 * This function implements the 2D convolution simply using nested for loops assuming that   *
 * the input image is already padded. It takes as an input pointers for input and output     *
 * images as well as convolution filter coefficients and number of filters used. 	     *
 *********************************************************************************************/
void cpu_conv2d(float* input_image, float* filter, float* output_image, int image_width, int image_height, int num_filters){
	// Declare variables used throughout the function
	float accum;
	int xx, yy;

	// Calculate the padded image size
	int padded_image_width = image_width + (2*FILTER_RADIUS);

	for(int filter_id = 0; filter_id < num_filters; filter_id++){ // For each filter
		for(int image_y = 0; image_y < image_height; image_y++){ // For each y location of the output image
			for(int image_x = 0; image_x < image_width; image_x++){ // For each x location of the output image
				// Start a new accumulation result
				accum = 0;
				
				// Compute the dot-product of the filter and the input image
				for(int filter_y = 0; filter_y < FILTER_SIZE; filter_y++){ 
					for(int filter_x = 0; filter_x < FILTER_SIZE; filter_x++){
						xx = image_x + filter_x;
						yy = image_y + filter_y;
						accum += input_image[(yy * padded_image_width) + xx] * filter[(filter_id * FILTER_SIZE * FILTER_SIZE) + (filter_y * FILTER_SIZE) + filter_x];
					}
				}

				// Store the convolution result in the corresponding pixel location
				output_image[(filter_id * image_width * image_height) + (image_y * image_width) + image_x] = accum;
			}
		}
	}
}

/*********************************************************************************************
 * This is a multi-threaded version of the simple convolution implementation in which the    *
 * different threads are working on convolution with different filters at the same time.     *
 *********************************************************************************************/
void cpu_conv2d_multithreaded(float* input_image, float* filter, float* output_image, int image_width, int image_height, int num_filters){
	// Declare variables used throughout the function
	float accum;
	int xx, yy;

	// Calculate the padded image size
	int padded_image_width = image_width + (2*FILTER_RADIUS);

	// Same for loops from the cpu_conv2d function but adding the parallel for OpenMP pragma
	// to multi-thread the computation of multiple filters at the same time.
	#pragma omp parallel for
	for(int filter_id = 0; filter_id < num_filters; filter_id++){
		for(int image_y = 0; image_y < image_height; image_y++){
			for(int image_x = 0; image_x < image_width; image_x++){
				accum = 0;
				for(int filter_y = 0; filter_y < FILTER_SIZE; filter_y++){
					for(int filter_x = 0; filter_x < FILTER_SIZE; filter_x++){
						xx = image_x + filter_x;
						yy = image_y + filter_y;
						accum += input_image[(yy * padded_image_width) + xx] * filter[(filter_id * FILTER_SIZE * FILTER_SIZE) + (filter_y * FILTER_SIZE) + filter_x];
					}
				}
				output_image[(filter_id * image_width * image_height) + (image_y * image_width) + image_x] = accum;
			}
		}
	}
}

/*********************************************************************************************
 * This function is a hand-vectorized implementation of the dot-product of length 3 vectors. *
 * We use this to compute one row of the 3x3 convolution filter in parallel.		     *
 *********************************************************************************************/
float dot3 (float* a, float* b){
	// Load 128-bit (4x float values) to vector registers
	const __m128 v_a = _mm_loadu_ps(a);
	const __m128 v_b = _mm_loadu_ps(b);
	
	// Perform vector dot-product operation of size 3 (one element is discarded)
	__m128 acc = _mm_dp_ps(v_a, v_b, 0x71);

	// Carve out the result from the least significant 32 bits of the vector register
	float result = _mm_cvtss_f32(acc);
	return result;
}

/*********************************************************************************************
 * This function uses the hand-vectorized dot3 function above to implement convolution. The  *
 * vector instructions are used to compute one row of the filter in parallel.                *
 *********************************************************************************************/
void cpu_conv2d_vectorized(float* input_image, float* filter, float* output_image, int image_width, int image_height, int num_filters){
	// Declare variables for the dot-product results of the 3 filter rows
	float r1_dp, r2_dp, r3_dp;

	// Calculate the padded image size
	int padded_image_width = image_width + (2*FILTER_RADIUS);

	for(int filter_id = 0; filter_id < num_filters; filter_id++){ // For each filter
		for(int image_y = 0; image_y < image_height; image_y++){ // For each y location in the image
			for(int image_x = 0; image_x < image_width; image_x++){ // For each x location in the image

				// Calculate the dot-product of the 3 rows of the filter using the hand-vectorized dot3 function
				r1_dp = dot3(&filter[(filter_id * FILTER_SIZE * FILTER_SIZE)], &input_image[((image_y) * padded_image_width) + image_x]);
				r2_dp = dot3(&filter[(filter_id * FILTER_SIZE * FILTER_SIZE) + FILTER_SIZE], &input_image[((image_y+1) * padded_image_width) + image_x]);
				r3_dp = dot3(&filter[(filter_id * FILTER_SIZE * FILTER_SIZE) + 2*FILTER_SIZE], &input_image[((image_y+2) * padded_image_width) + image_x]);
				
				// Store the summation of the results of the 3 filter rows in the corresponding pixel location
				output_image[(filter_id * image_width * image_height) + (image_y * image_width) + image_x] = r1_dp + r2_dp + r3_dp;
			}
		}
	}
}

/*********************************************************************************************
 * This is a multi-threaded version of the hand-vectorized convolution implementation in     *
 * which different threads are working on convolution with different filters in parallel.    *
 *********************************************************************************************/
void cpu_conv2d_vectorized_multithreaded(float* input_image, float* filter, float* output_image, int image_width, int image_height, int num_filters){
	// Declare variables for the dot-product results of the 3 filter rows
	float r1_dp, r2_dp, r3_dp;

	// Calculate the padded image size
	int padded_image_width = image_width + (2*FILTER_RADIUS);
	
	#pragma omp parallel for
	for(int filter_id = 0; filter_id < num_filters; filter_id++){ // For each filter
		for(int image_y = 0; image_y < image_height; image_y++){ // For each y location in the image
			for(int image_x = 0; image_x < image_width; image_x++){ // For each x location in the image

				// Calculate the dot-product of the 3 rows of the filter using the hand-vectorized dot3 function
				r1_dp = dot3(&filter[(filter_id * FILTER_SIZE * FILTER_SIZE)], &input_image[((image_y) * padded_image_width) + image_x]);
				r2_dp = dot3(&filter[(filter_id * FILTER_SIZE * FILTER_SIZE) + FILTER_SIZE], &input_image[((image_y+1) * padded_image_width) + image_x]);
				r3_dp = dot3(&filter[(filter_id * FILTER_SIZE * FILTER_SIZE) + 2*FILTER_SIZE], &input_image[((image_y+2) * padded_image_width) + image_x]);
				
				// Store the summation of the results of the 3 filter rows in the corresponding pixel location
				output_image[(filter_id * image_width * image_height) + (image_y * image_width) + image_x] = r1_dp + r2_dp + r3_dp;
			}
		}
	}
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
	usleep(1000000);
	uint64_t end_clk = rdtsc();
	clocks_per_sec = (end_clk - start_clk);

	// Declare pointers for all needed buffers
	int image_width; 		// Input image width
	int image_height; 		// Input image height
	int padded_image_width; 	// Padded input image width
	int padded_image_height; 	// Padded input image height
	float* host_input_image;	// Input image in CPU memory
	float* host_filter;		// Convolution filter in CPU memory
	float* host_output_image_cpu;	// CPU output image in CPU memory

	// Read filter data from file
	host_filter = (float*) malloc(sizeof(float) * FILTER_SIZE * FILTER_SIZE * num_filters);
	read_filters<float>("filters", FILTER_SIZE, host_filter, num_filters);

	// Read input image size
	if(!read_pgm_image_size(input_image_name, image_width, image_height)){
		return 0;
	}
	padded_image_width = image_width + (2 * FILTER_RADIUS);
	padded_image_height = image_height + (2 * FILTER_RADIUS);

	// Read image in PGM format
	host_input_image = (float*) malloc(sizeof(float) * padded_image_width * padded_image_height);
	read_pgm_image<float>(input_image_name, image_width, image_height, host_input_image, 1);

	// Allocate memory for output image
	host_output_image_cpu = (float*) malloc(sizeof(float) * image_width * image_height * num_filters);

	// Run simple convolution
	std::cout << "Runtime of simple convolution for " << num_filters << " filter(s) averaged accross " << NUM_RUNS << " runs = ";
	double cpu_runtime_in_ms;
	cpu_runtime_in_ms = 0;
	for(int run = 0; run < NUM_RUNS; run++){
		start_clk = rdtsc();
		cpu_conv2d(host_input_image, host_filter, host_output_image_cpu, image_width, image_height, num_filters);
		end_clk = rdtsc();
		cpu_runtime_in_ms += 1000.0 * (end_clk-start_clk) / clocks_per_sec;
	}
	cpu_runtime_in_ms /= NUM_RUNS;
	std::cout << cpu_runtime_in_ms << " ms" << std::endl;

	// Write the output images in PGM format
	for(int k = 0; k < num_filters; k++){
		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << k;
		std::string filename = std::string("out_cpu/simple_") + oss.str() + std::string(".pgm");
		write_pgm_image<float>(filename, image_width, image_height, host_output_image_cpu + (k * image_width * image_height), 0);
	}
	
	// Run hand-vectorized convolution
	std::cout << "Runtime of hand vectorized convolution for " << num_filters << " filter(s) averaged accross " << NUM_RUNS << " runs = ";
	cpu_runtime_in_ms = 0;
	for(int run = 0; run < NUM_RUNS; run++){
		start_clk = rdtsc();
		cpu_conv2d_vectorized(host_input_image, host_filter, host_output_image_cpu, image_width, image_height, num_filters);
		end_clk = rdtsc();
		cpu_runtime_in_ms += 1000.0 * (end_clk-start_clk) / clocks_per_sec;
	}
	cpu_runtime_in_ms /= NUM_RUNS;
	std::cout << cpu_runtime_in_ms << " ms" << std::endl;

	// Write the output images in PGM format
	for(int k = 0; k < num_filters; k++){
		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << k;
		std::string filename = std::string("out_cpu/vectorized_") + oss.str() + std::string(".pgm");
		write_pgm_image<float>(filename, image_width, image_height, host_output_image_cpu + (k * image_width * image_height), 0);
	}

	std::cout << "------------------------" << std::endl;

	// Free CPU memory
	free(host_filter);
	free(host_input_image);
	free(host_output_image_cpu);
	
	return 0;
}
