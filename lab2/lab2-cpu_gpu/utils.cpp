#include <stdio.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
#include <cstdlib>

#include "utils.h"

/*********************************************************************************************
 * This function reads the size of the input image given in PGM format                       *
 * Function arguments:                                                                       *
 * - string filename: a string specifying the name of the image (including extension .pgm)   *
 * - int* image_width: pointer for storing the input image width                             *
 * - int* image_height: pointer for storing the input image height                           *
 *********************************************************************************************/
int read_pgm_image_size(std::string filename, int &image_width, int &image_height){
	// Declare variables used throughout the function
	std::ifstream infile;    
	infile.open(filename.c_str(), std::ios::in);
	if(!infile){
		std::cout << "Cannot find specified file: " << filename << std::endl;
		return 0;
	} 
	std::stringstream ss;
	std::string line = "";

	// Check for the format of the PGM file. It must be "P2"!
	std::getline(infile, line);
	if(line.compare("P2") != 0){
		std::cout << "PGM format error: PGM type is not P2" << std::endl;
		return 0;
	}

	// The 2nd line is usually a comment with the file name and creator. Read & ignore!
	std::getline(infile, line);

	// Read the width and height of the image
	int width, height;
	ss << infile.rdbuf();
	ss >> width >> height;
	image_width = width;
	image_height = height;

	// Read and discard maximum pixel value
	int temp;
	ss >> temp;
	
	infile.close();
	return 1;
}

/*********************************************************************************************
 * This function reads a grayscale image in the ASCII Portable Gray Map (PGM). You can learn *
 * more about this format at: https://people.sc.fsu.edu/~jburkardt/data/pgma/pgma.html       *
 * Function arguments:                                                                       *
 * - filename: a string specifying the name of the image (including extension .pgm)          *
 * - image_width: the number of pixels in one row of the image                               *
 * - image_height: the number of pixels in one column of the image                           *
 * - pixels: a pre-allocated array of integers with the size (image_width x image_height)    *
 *   to store the pixel values of the image						     *
 * - add_padding: a boolean value that specifies whether to add padding or not               *
 *********************************************************************************************/
template<typename T>
int read_pgm_image(std::string filename, int image_width, int image_height, T* pixels, bool add_padding){
	// Declare variables used throughout the function
	int row = 0, col = 0, num_rows = 0, num_cols = 0;
	std::ifstream infile(filename);
	std::stringstream ss;
	std::string line = "";

	// Check for the format of the PGM file. It must be "P2"!
	std::getline(infile, line);
	if(line.compare("P2") != 0){
		std::cout << "PGM format error: PGM type is not P2" << std::endl;
		return 0;
	}

	// The 2nd line is usually a comment with the file name and creator. Read & ignore!
	std::getline(infile, line);

	// Check that the width and height specified in the PGM file match function inputs
	ss << infile.rdbuf();
	ss >> num_cols >> num_rows;
	if((num_cols != image_width) | (num_rows != image_height)){
		std::cout << "PGM dimensions (" << num_rows << "x" << num_cols << 
			") is different from the specified dimensions (" << image_height << 
			"x" << image_width << ")" << std::endl;
		return 0;	
	}

	// Read and discard maximum pixel value
	int temp;
	ss >> temp;
	
	// Adjust start and end of for loops if the image is required to be padded
	int row_start = 0, col_start = 0, row_end = num_rows, col_end = num_cols, width = image_width;
	if(add_padding){
		int padded_image_width = image_width + (2*FILTER_RADIUS);
		int padded_image_height = image_height + (2*FILTER_RADIUS);
		row_start = FILTER_RADIUS;
		row_end = num_rows + FILTER_RADIUS;
		col_start = FILTER_RADIUS;
		col_end = num_cols + FILTER_RADIUS;
		width = width + FILTER_RADIUS + FILTER_RADIUS;
		int idx = 0;
		// Initialize borders to zeros
		while(idx < padded_image_width * padded_image_height){
			if((idx < padded_image_width) || (idx >= ((padded_image_height-1)*padded_image_width))){
				pixels[idx] = 0;
				idx++;
			} else {
				pixels[idx] = 0;
				pixels[idx+padded_image_width-1] = 0;
				idx += padded_image_width;
			}
		}
	}

	// Read the pixel values and populate the array
	for (row = row_start; row < row_end; ++row){
		for (col = col_start; col < col_end; ++col){
			ss >> pixels[(row * width) + col];
		}
	}
	infile.close();
	return 1;
}

/**********************************************************************************************
 * This function writes a grayscale image in the ASCII Portable Gray Map (PGM). You can learn *
 * more about this format at: https://people.sc.fsu.edu/~jburkardt/data/pgma/pgma.html        *
 * Function arguments:                                                                        *
 * - filename: a string specifying the name of the image to write (including extension .pgm)  *
 * - image_width: the number of pixels in one row of the image                                *
 * - image_height: the number of pixels in one column of the image                            *
 * - pixels: the image's pixel values stored as an unrolled 1D array of integers with size    *
 *   (image_width x image_height). Values below 0 and above 255 are clipped.                  *
 * - remove_padding: a boolean value that specifies whether to remove padding or not          *
 **********************************************************************************************/
template<typename T>
int write_pgm_image(std::string filename, int image_width, int image_height, T* pixels, bool remove_padding){
	// Declare variables used throughout the function
	int row = 0, col = 0;
	std::ofstream outfile(filename, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	T max_pixel_value = 255;

	// Write the PGM file header
	outfile << "P2\n";
	outfile << "# " << filename << " created by ECE1756 Assignment 2\n";
	outfile << image_width << " " << image_height << "\n";
	outfile << max_pixel_value << "\n";
	
	// Adjust the start and end of the for loops if the image contains padding that should be removed
	int row_start = 0, col_start = 0, row_end = image_height, col_end = image_width, width = image_width;
	if(remove_padding){
		row_start = FILTER_RADIUS;
		row_end = image_height + FILTER_RADIUS;
		col_start = FILTER_RADIUS;
		col_end = image_width + FILTER_RADIUS;
		width = width + FILTER_RADIUS + FILTER_RADIUS;
	}

	// Write pixel values of the image & clip any values below 0 and above max_pixel_value
	T pixel;
	for (row = row_start; row < row_end; ++row){
		for (col = col_start; col < col_end; ++col){
			pixel = pixels[(row * width) + col];
			if(pixel < 0){
				pixel = 0;
			} else if (pixel > max_pixel_value){
				pixel = max_pixel_value;
			}
			outfile << pixel << " ";
		}
		outfile << "\n";
	}
	outfile << std::flush;
	return 1;
}

/**********************************************************************************************
 * This function reads the values of the convolution filter coefficients from file.           *
 * Function arguments:                                                                        *
 * - string filename: a string specifying the name of the file storing the filter values      *
 * - int filter_size: the size of the convolution filter                                      *
 * - int* filter: pointer to the memory allocated for storing the filter coefficients         *
 * - int num_filters: the number of convolution filters to parse                              *
 **********************************************************************************************/
template<typename T>
int read_filters(std::string filename, int filter_size, T* filter, int num_filters) {
	std::ifstream infile(filename);
	std::stringstream ss;
	
	ss << infile.rdbuf();
	// Read the filter values and populate the array
	for (int k = 0; k < num_filters; k++){
		for (int i = 0; i < filter_size; ++i){
			for (int j = 0; j < filter_size; ++j){
				ss >> filter[(k * filter_size * filter_size)+(i * filter_size) + j];
			}
		}
	}

	infile.close();
	return 1;
}

template int read_filters<int>(std::string filename, int filter_size, int* filter, int num_filters);
template int read_filters<float>(std::string filename, int filter_size, float* filter, int num_filters);
template int read_pgm_image<int>(std::string filename, int image_width, int image_height, int* pixels, bool add_padding);
template int read_pgm_image<float>(std::string filename, int image_width, int image_height, float* pixels, bool add_padding);
template int write_pgm_image<int>(std::string filename, int image_width, int image_height, int* pixels, bool remove_padding);
template int write_pgm_image<float>(std::string filename, int image_width, int image_height, float* pixels, bool remove_padding);
