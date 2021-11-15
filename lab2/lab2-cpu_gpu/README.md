### How to compile and run?

To compile the GPU code, use the following command:

`nvcc conv_gpu.cu utils.cpp -o conv_gpu`

To run the executable, use the following command:

`./conv_gpu <image_name> <num_filters>`

Keep in mind that the maximum number of filters is set to 64. For example, you can run the code using the test image provided by running:

`./conv_gpu hinton.pgm 4`

This will run the convolution on the GPU with 4 filters and you can see the output in the [out_gpu/](out_gpu) directory.

To run the GPU code for several fiter sizes, you just have to run the provided shell script as follows:

`./run_gpu.sh <list_of_num_filters_you_want_to_try>`

For example:

`./run_gpu.sh 1 2 4 8 16 32 64`


Similarly for the CPU code, you can compile it using:

`g++ -mavx -fopenmp conv_cpu.cpp utils.cpp -o conv_cpu`

You can also add the `-O2` or `-O3` for different compiler optimization levels. To run it, you can follow the same instructions for the GPU code but using the `conv_cpu` executable and the `run_cpu.sh` script. The output images can be found in the [out_cpu/](out_cpu) directory.

To run the multi-threaded version, you will have to manually replace the functions `cpu_conv2d` and `cpu_conv2d_vectorized` in `conv_cpu.cpp` with `cpu_conv2d_multithreaded` and `cpu_conv2d_vectorized_multithreaded`, respectively. Then, you can recompile and run the code again.

