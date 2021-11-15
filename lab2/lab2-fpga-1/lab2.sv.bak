// This module implements 2D covolution between a 3x3 filter and a 512-pixel-wide image of any height.
// It is assumed that the input image is padded with zeros such that the input and output images have
// the same size. The filter coefficients are symmetric in the x-direction (i.e. f[0][0] = f[0][2], 
// f[1][0] = f[1][2], f[2][0] = f[2][2] for any filter f) and their values are limited to integers
// (but can still be positive of negative). The input image is grayscale with 8-bit pixel values ranging
// from 0 (black) to 255 (white).
module lab2 (
	input  clk,			// Operating clock
	input  reset,			// Active-high reset signal (reset when set to 1)
	input  [71:0] i_f,		// Nine 8-bit signed convolution filter coefficients in row-major format (i.e. i_f[7:0] is f[0][0], i_f[15:8] is f[0][1], etc.)
	input  i_valid,			// Set to 1 if input pixel is valid
	input  i_ready,			// Set to 1 if consumer block is ready to receive a new pixel
	input  [7:0] i_x,		// Input pixel value (8-bit unsigned value between 0 and 255)
	output o_valid,			// Set to 1 if output pixel is valid
	output o_ready,			// Set to 1 if this block is ready to receive a new pixel
	output [7:0] o_y		// Output pixel value (8-bit unsigned value between 0 and 255)
);

localparam FILTER_SIZE = 3;	// Convolution filter dimension (i.e. 3x3)
localparam PIXEL_DATAW = 8;	// Bit width of image pixels and filter coefficients (i.e. 8 bits)

// The following code is intended to show you an example of how to use paramaters and
// for loops in SytemVerilog. It also arrages the input filter coefficients for you
// into a nicely-arranged and easy-to-use 2D array of registers. However, you can ignore
// this code and not use it if you wish to.

logic signed [PIXEL_DATAW-1:0] r_f [FILTER_SIZE-1:0][FILTER_SIZE-1:0]; // 2D array of registers for filter coefficients
integer col, row; // variables to use in the for loop
always_ff @ (posedge clk) begin
	// If reset signal is high, set all the filter coefficient registers to zeros
	// We're using a synchronous reset, which is recommended style for recent FPGA architectures
	if(reset)begin
		for(row = 0; row < FILTER_SIZE; row = row + 1) begin
			for(col = 0; col < FILTER_SIZE; col = col + 1) begin
				r_f[row][col] <= 0;
			end
		end
	// Otherwise, register the input filter coefficients into the 2D array signal
	end else begin
		for(row = 0; row < FILTER_SIZE; row = row + 1) begin
			for(col = 0; col < FILTER_SIZE; col = col + 1) begin
				// Rearrange the 72-bit input into a 3x3 array of 8-bit filter coefficients.
				// signal[a +: b] is equivalent to signal[a+b-1 : a]. You can try to plug in
				// values for col and row from 0 to 2, to understand how it operates.
				// For example at row=0 and col=0: r_f[0][0] = i_f[0+:8] = i_f[7:0]
				//	       at row=0 and col=1: r_f[0][1] = i_f[8+:8] = i_f[15:8]
				r_f[row][col] <= i_f[(row * FILTER_SIZE * PIXEL_DATAW)+(col * PIXEL_DATAW) +: PIXEL_DATAW];
			end
		end
	end
end

// Start of your code


// End of your code

endmodule

