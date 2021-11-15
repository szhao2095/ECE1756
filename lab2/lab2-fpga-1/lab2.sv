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

/////////////////////////////////////////// Start of your code ///////////////////////////////////////////

localparam PIC_WIDTH = 512+2; // How wide is our picture

logic [PIXEL_DATAW-1:0] SIPO [PIC_WIDTH*2+FILTER_SIZE];			// SIPO, shift reg for data input
																					// Data is sampled at positions 0, 1, 2, 514, 515, 516, 1028, 1029, 1030
																					// for convolution calculation, w.r.t. the filter they are arranged:
																					//	1030, 1029, 1028
																					//	516 , 515 , 514
																					// 2   , 1   , 0
logic data_valid [PIC_WIDTH*2+FILTER_SIZE];							// Data valid propagate

// Intermediate outputs
logic signed [31:0] mult_out [9];				// Output of multiplier
logic data_valid_mult;								// Valid bit for if multiplier output is valid

logic signed [31:0] addr_stage_1_out [4];		// Output of adders in stage 1
logic signed [31:0] addr_stage_2_out [2];		// Output of adders in stage 2
logic signed [31:0] addr_stage_3_out;			// Output of adder in stage 3
logic signed [31:0] addr_stage_4_out;			// Output of adder in stage 4

// Pipeline registers
logic signed [31:0] mult_out_pipe [9];
logic data_valid_mult_pipe;

logic signed [31:0] addr_stage_1_out_pipe [5];
logic data_valid_addr_stage_1_pipe;
logic signed [31:0] addr_stage_2_out_pipe [3];
logic data_valid_addr_stage_2_pipe;
logic signed [31:0] addr_stage_3_out_pipe [2];
logic data_valid_addr_stage_3_pipe;
logic unsigned [PIXEL_DATAW-1:0] addr_stage_4_out_pipe;
logic data_valid_addr_stage_4_pipe; // Final, same level (in terms of pipelining) as o_valid

// Counter to ensure correctness, no computation is to happen for the first 2 valid inputs
// for each new row of pixels
logic [15:0] counter;

// Instantiating multipliers. All products are computed in one and the same clock cycle
mult8x8 convo_mult_0 (.i_dataa(SIPO[2+PIC_WIDTH*2]), 	.i_datab(r_f[0][0]), .o_res(mult_out[0]));	// Row 0, Col 0
mult8x8 convo_mult_1 (.i_dataa(SIPO[1+PIC_WIDTH*2]), 	.i_datab(r_f[0][1]), .o_res(mult_out[1]));	// Row 0, Col 1
mult8x8 convo_mult_2 (.i_dataa(SIPO[0+PIC_WIDTH*2]), 	.i_datab(r_f[0][2]), .o_res(mult_out[2]));	// Row 0, Col 2
mult8x8 convo_mult_3 (.i_dataa(SIPO[2+PIC_WIDTH*1]), 	.i_datab(r_f[1][0]), .o_res(mult_out[3]));	// Row 1, Col 0
mult8x8 convo_mult_4 (.i_dataa(SIPO[1+PIC_WIDTH*1]), 	.i_datab(r_f[1][1]), .o_res(mult_out[4]));	// Row 1, Col 1
mult8x8 convo_mult_5 (.i_dataa(SIPO[0+PIC_WIDTH*1]), 	.i_datab(r_f[1][2]), .o_res(mult_out[5]));	// Row 1, Col 2
mult8x8 convo_mult_6 (.i_dataa(SIPO[2]), 					.i_datab(r_f[2][0]), .o_res(mult_out[6]));	// Row 2, Col 0
mult8x8 convo_mult_7 (.i_dataa(SIPO[1]), 					.i_datab(r_f[2][1]), .o_res(mult_out[7]));	// Row 2, Col 1
mult8x8 convo_mult_8 (.i_dataa(SIPO[0]), 					.i_datab(r_f[2][2]), .o_res(mult_out[8]));	// Row 2, Col 2

// Instantiating adders. Each grouping of adders in one clock cycle each
addr32p32 convo_addr_stage_1_0 (.i_dataa(mult_out_pipe[0]), .i_datab(mult_out_pipe[1]), .o_res(addr_stage_1_out[0]));
addr32p32 convo_addr_stage_1_1 (.i_dataa(mult_out_pipe[2]), .i_datab(mult_out_pipe[3]), .o_res(addr_stage_1_out[1]));
addr32p32 convo_addr_stage_1_2 (.i_dataa(mult_out_pipe[4]), .i_datab(mult_out_pipe[5]), .o_res(addr_stage_1_out[2]));
addr32p32 convo_addr_stage_1_3 (.i_dataa(mult_out_pipe[6]), .i_datab(mult_out_pipe[7]), .o_res(addr_stage_1_out[3]));

addr32p32 convo_addr_stage_2_0 (.i_dataa(addr_stage_1_out_pipe[0]), .i_datab(addr_stage_1_out_pipe[1]), .o_res(addr_stage_2_out[0]));
addr32p32 convo_addr_stage_2_1 (.i_dataa(addr_stage_1_out_pipe[2]), .i_datab(addr_stage_1_out_pipe[3]), .o_res(addr_stage_2_out[1]));

addr32p32 convo_addr_stage_3_0 (.i_dataa(addr_stage_2_out_pipe[0]), .i_datab(addr_stage_2_out_pipe[1]), .o_res(addr_stage_3_out));

addr32p32 convo_addr_stage_4_0 (.i_dataa(addr_stage_3_out_pipe[0]), .i_datab(addr_stage_3_out_pipe[1]), .o_res(addr_stage_4_out));

// Valid for computed values is only true if all inputs are valid
// otherwise, similar to lab 1, these are just propagated along with computed values
always_comb begin
	// Multiplier output only valid if all 9 data bytes are valid
	data_valid_mult = data_valid[2+PIC_WIDTH*2] & data_valid[1+PIC_WIDTH*2] & data_valid[0+PIC_WIDTH*2] &
							data_valid[2+PIC_WIDTH*1] & data_valid[1+PIC_WIDTH*1] & data_valid[0+PIC_WIDTH*1] &
							data_valid[2]             & data_valid[1]             & data_valid[0] &
							(counter >= FILTER_SIZE); // Need at least 3 element in the row before computing
end

logic enable;
always_comb begin
	enable = i_ready & i_valid;
end


int i;
always_ff @ (posedge clk) begin

	if(reset)begin
		// Reset counter
		counter <= 16'b0;
	
		// Reset SIPO and valid propagation
		for(i = 0; i < $size(SIPO); i = i + 1) begin
			SIPO[i] <= 8'b0;	
			data_valid[i] <= 1'b0;
		end
		
	end else begin
		if(enable)begin	
			// Counter
			// If counter at 514 means current row complete, reset to 1 since it's the first entry of the next row
			if (counter == PIC_WIDTH) begin
				counter <= 16'b1;
			end else begin
				counter <= counter + 1'b1;
			end
		
			SIPO[0] <= i_x; 					// If input is valid, load into SIPO[0]
			data_valid[0] <= i_valid;		// Similarly with valids
			for(i = 1; i < $size(SIPO); i = i + 1) begin
				SIPO[i] <= SIPO[i-1];					// Propagate signal in shift reg
				data_valid[i] <= data_valid[i-1];	// Propagate valid signal
			end
		
			// Pipeline reg for multiplier out, adder stage 1 in
			mult_out_pipe <= mult_out;
			data_valid_mult_pipe <= data_valid_mult;
			
						
			data_valid_addr_stage_1_pipe <= data_valid_mult_pipe;
			data_valid_addr_stage_2_pipe <= data_valid_addr_stage_1_pipe;
			data_valid_addr_stage_3_pipe <= data_valid_addr_stage_2_pipe;
			data_valid_addr_stage_4_pipe <= data_valid_addr_stage_3_pipe;
			
			// Pipeline reg for adder stage 1 out, adder stage 2 in
			addr_stage_1_out_pipe[0] <= addr_stage_1_out[0];
			addr_stage_1_out_pipe[1] <= addr_stage_1_out[1];
			addr_stage_1_out_pipe[2] <= addr_stage_1_out[2];
			addr_stage_1_out_pipe[3] <= addr_stage_1_out[3];
			addr_stage_1_out_pipe[4] <= mult_out_pipe[8];
			
			// Pipeline reg for adder stage 2 out, adder stage 3 in
			addr_stage_2_out_pipe[0] <= addr_stage_2_out[0];
			addr_stage_2_out_pipe[1] <= addr_stage_2_out[1];
			addr_stage_2_out_pipe[2] <= addr_stage_1_out_pipe[4];
			
			// Pipeline reg for adder stage 3 out, adder stage 4 in
			addr_stage_3_out_pipe[0] <= addr_stage_3_out;
			addr_stage_3_out_pipe[1] <= addr_stage_2_out_pipe[2];
			
			// Pipeline reg for adder stage 4 out, final result, clipping performed here
			if (addr_stage_4_out > 32'sd255) begin
				addr_stage_4_out_pipe <= 8'd255;
			end else if (addr_stage_4_out < 32'sd0) begin
				addr_stage_4_out_pipe <= 8'd0;
			end else begin
				addr_stage_4_out_pipe <= addr_stage_4_out[7:0];
			end
		end
	end
	
end

assign o_y = addr_stage_4_out_pipe;
assign o_ready = i_ready;
assign o_valid = data_valid_addr_stage_4_pipe & i_ready;	

/////////////////////////////////////////// End of your code ///////////////////////////////////////////

endmodule

/*******************************************************************************************/

// Multiplier module for 8x8 multiplication
// Taken from lab 1 with modifications for signed multiplication
module mult8x8 ( /* synthesis multstyle = "dsp" */
	input unsigned [7:0] i_dataa,
	input signed [7:0] i_datab,
	output signed [31:0] o_res
);

logic signed [31:0] result;

always_comb begin
	result = i_datab * $signed({1'b0,i_dataa});
end

assign o_res = result;

endmodule

/*******************************************************************************************/


/*******************************************************************************************/

// Adder module for 32b+32b addition operations 
// Taken from lab 1 with modifications for signed addition
module addr32p32 (
	input signed [31:0] i_dataa,
	input signed [31:0] i_datab,
	output signed [31:0] o_res
);

assign o_res = i_dataa + i_datab;

endmodule

/*******************************************************************************************/

