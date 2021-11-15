#ifndef _UTILS_H_
#define _UTILS_H_

#define MAX_NUM_FILTERS		64
#define FILTER_SIZE   		3
#define FILTER_RADIUS 		int(FILTER_SIZE/2)
#define TILE_SIZE		32
#define TILE_LOAD_SIZE		(TILE_SIZE+FILTER_SIZE-1)
#define NUM_RUNS		20

#define cudaCheck(cudaCall)  do {                               \
	cudaError_t err = cudaCall;                             \
	if (err != cudaSuccess) {                               \
	    printf( "Failed to run a CUDA call at line %d \n", __LINE__);    \
	    printf( "Error:  %s \n", cudaGetErrorString(err));    \
	    return -1;                                          \
	}                                                       \
} while(0)

/*int read_pgm_image(std::string filename, int image_width, int image_height, int* pixels, bool add_padding);
int read_pgm_image(std::string filename, int image_width, int image_height, float* pixels, bool add_padding);
int write_pgm_image(std::string filename, int image_width, int image_height, int* pixels, bool remove_padding);
int write_pgm_image(std::string filename, int image_width, int image_height, float* pixels, bool remove_padding);
int read_filters(std::string filename, int filter_size, int* filter, int num_filters);
int read_filters(std::string filename, int filter_size, float* filter, int num_filters);*/
template<typename T> int read_filters(std::string filename, int filter_size, T* filter, int num_filters);
template<typename T> int read_pgm_image(std::string filename, int image_width, int image_height, T* pixels, bool add_padding);
template<typename T> int write_pgm_image(std::string filename, int image_width, int image_height, T* pixels, bool remove_padding);
int read_pgm_image_size(std::string filename, int &image_width, int &image_height);

#endif
