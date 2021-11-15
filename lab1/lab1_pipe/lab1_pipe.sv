module lab1_pipe #
(
	parameter WIDTHIN = 16,		// Input format is Q2.14 (2 integer bits + 14 fractional bits = 16 bits)
	parameter WIDTHOUT = 32,	// Intermediate/Output format is Q7.25 (7 integer bits + 25 fractional bits = 32 bits)
	// Taylor coefficients for the first five terms in Q2.14 format
	parameter [WIDTHIN-1:0] A0 = 16'b01_00000000000000, // a0 = 1
	parameter [WIDTHIN-1:0] A1 = 16'b01_00000000000000, // a1 = 1
	parameter [WIDTHIN-1:0] A2 = 16'b00_10000000000000, // a2 = 1/2
	parameter [WIDTHIN-1:0] A3 = 16'b00_00101010101010, // a3 = 1/6
	parameter [WIDTHIN-1:0] A4 = 16'b00_00001010101010, // a4 = 1/24
	parameter [WIDTHIN-1:0] A5 = 16'b00_00000010001000  // a5 = 1/120
)
(
	input clk,
	input reset,	
	
	input i_valid,
	input i_ready,
	output o_valid,
	output o_ready,
	
	input [WIDTHIN-1:0] i_x,
	output [WIDTHOUT-1:0] o_y
);
//Output value could overflow (32-bit output, and 16-bit inputs multiplied
//together repeatedly).  Don't worry about that -- assume that only the bottom
//32 bits are of interest, and keep them.
logic [WIDTHIN-1:0] x_m0_in;	// Register to hold input X, as input to m0
logic [WIDTHIN-1:0] x_m0_hold;// To extend propagation chain	
logic [WIDTHIN-1:0] x_m1_in;	// Pipeline register for x, as input to m1
logic [WIDTHIN-1:0] x_m1_hold;	
logic [WIDTHIN-1:0] x_m2_in;	// Pipeline register for x, as input to m2
logic [WIDTHIN-1:0] x_m2_hold;	
logic [WIDTHIN-1:0] x_m3_in;	// Pipeline register for x, as input to m3
logic [WIDTHIN-1:0] x_m3_hold;	
logic [WIDTHIN-1:0] x_m4_in;	// Pipeline register for x, as input to m4

logic [WIDTHOUT-1:0] m0_out_a0_in_pipe;	// Pipeline register after m0
logic [WIDTHOUT-1:0] a0_out_m1_in_pipe;	// Pipeline register after a0
logic [WIDTHOUT-1:0] m1_out_a1_in_pipe;	// Pipeline register after m1
logic [WIDTHOUT-1:0] a1_out_m2_in_pipe;	// Pipeline register after a1
logic [WIDTHOUT-1:0] m2_out_a2_in_pipe;	// Pipeline register after m2
logic [WIDTHOUT-1:0] a2_out_m3_in_pipe;	// Pipeline register after a2
logic [WIDTHOUT-1:0] m3_out_a3_in_pipe;	// Pipeline register after m3
logic [WIDTHOUT-1:0] a3_out_m4_in_pipe;	// Pipeline register after a3
logic [WIDTHOUT-1:0] m4_out_a4_in_pipe;	// Pipeline register after m4
logic [WIDTHOUT-1:0] a4_out_final;			// Register to hold output Y, a4_out goes directly into y_Q

logic valid_input_x;			// Output of register x is valid
logic valid_m0;				// Output of m0 is valid
logic valid_a0;				// Output of a0 is valid
logic valid_m1;				// Output of m1 is valid
logic valid_a1;				// Output of a1 is valid
logic valid_m2;				// Output of m2 is valid
logic valid_a2;				// Output of a2 is valid
logic valid_m3;				// Output of m3 is valid
logic valid_a3;				// Output of a3 is valid
logic valid_m4;				// Output of m4 is valid
logic valid_a4_final;		// Output of a4 (final result) is valid

// signal for enabling sequential circuit elements
logic enable;

// Signals for computing the y output
logic [WIDTHOUT-1:0] m0_out; // A5 * x
logic [WIDTHOUT-1:0] a0_out; // A5 * x + A4
logic [WIDTHOUT-1:0] m1_out; // (A5 * x + A4) * x
logic [WIDTHOUT-1:0] a1_out; // (A5 * x + A4) * x + A3
logic [WIDTHOUT-1:0] m2_out; // ((A5 * x + A4) * x + A3) * x
logic [WIDTHOUT-1:0] a2_out; // ((A5 * x + A4) * x + A3) * x + A2
logic [WIDTHOUT-1:0] m3_out; // (((A5 * x + A4) * x + A3) * x + A2) * x
logic [WIDTHOUT-1:0] a3_out; // (((A5 * x + A4) * x + A3) * x + A2) * x + A1
logic [WIDTHOUT-1:0] m4_out; // ((((A5 * x + A4) * x + A3) * x + A2) * x + A1) * x
logic [WIDTHOUT-1:0] a4_out; // ((((A5 * x + A4) * x + A3) * x + A2) * x + A1) * x + A0

// compute y value
mult16x16 Mult0 (.i_dataa(A5), 		.i_datab(x_m0_in), 	.o_res(m0_out));
addr32p16 Addr0 (.i_dataa(m0_out_a0_in_pipe), 	.i_datab(A4), 	.o_res(a0_out));

mult32x16 Mult1 (.i_dataa(a0_out_m1_in_pipe), 	.i_datab(x_m1_in), 	.o_res(m1_out));
addr32p16 Addr1 (.i_dataa(m1_out_a1_in_pipe), 	.i_datab(A3), 	.o_res(a1_out));

mult32x16 Mult2 (.i_dataa(a1_out_m2_in_pipe), 	.i_datab(x_m2_in), 	.o_res(m2_out));
addr32p16 Addr2 (.i_dataa(m2_out_a2_in_pipe), 	.i_datab(A2), 	.o_res(a2_out));

mult32x16 Mult3 (.i_dataa(a2_out_m3_in_pipe), 	.i_datab(x_m3_in), 	.o_res(m3_out));
addr32p16 Addr3 (.i_dataa(m3_out_a3_in_pipe), 	.i_datab(A1), 	.o_res(a3_out));

mult32x16 Mult4 (.i_dataa(a3_out_m4_in_pipe), 	.i_datab(x_m4_in), 	.o_res(m4_out));
addr32p16 Addr4 (.i_dataa(m4_out_a4_in_pipe), 	.i_datab(A0), 	.o_res(a4_out));


// Combinational logic
always_comb begin
	// signal for enable
	enable = i_ready;
end

// Infer the registers
always_ff @(posedge clk or posedge reset) begin
	if (reset) begin
		x_m0_in <= 0;
		x_m0_hold <= 0;
		x_m1_in <= 0;
		x_m1_hold <= 0;
		x_m2_in <= 0;
		x_m2_hold <= 0;
		x_m3_in <= 0;
		x_m3_hold <= 0;
		x_m4_in <= 0;
		
		valid_input_x <= 1'b0;
		valid_m0 <= 1'b0;
		valid_a0 <= 1'b0;
		valid_m1 <= 1'b0;
		valid_a1 <= 1'b0;
		valid_m2 <= 1'b0;
		valid_a2 <= 1'b0;
		valid_m3 <= 1'b0;
		valid_a3 <= 1'b0;
		valid_m4 <= 1'b0;
		valid_a4_final <= 1'b0;
		
		m0_out_a0_in_pipe <= 1'b0;
		a0_out_m1_in_pipe <= 1'b0;
		m1_out_a1_in_pipe <= 1'b0;
		a1_out_m2_in_pipe <= 1'b0;
		m2_out_a2_in_pipe <= 1'b0;
		a2_out_m3_in_pipe <= 1'b0;
		m3_out_a3_in_pipe <= 1'b0;
		a3_out_m4_in_pipe <= 1'b0;
		m4_out_a4_in_pipe <= 1'b0;
		a4_out_final <= 1'b0;
		
	end else if (enable) begin
		// As long as circuit is operating
		// read in new x value and propagate
		x_m0_in <= i_x;
		x_m0_hold <= x_m0_in;
		x_m1_in <= x_m0_hold;
		x_m1_hold <= x_m1_in;
		x_m2_in <= x_m1_hold;
		x_m2_hold <= x_m2_in;
		x_m3_in <= x_m2_hold;
		x_m3_hold <= x_m3_in;
		x_m4_in <= x_m3_hold;
	
		// propagate the valid value
		valid_input_x <= i_valid;
		valid_m0 <= valid_input_x;
		valid_a0 <= valid_m0;
		valid_m1 <= valid_a0;
		valid_a1 <= valid_m1;
		valid_m2 <= valid_a1;
		valid_a2 <= valid_m2;
		valid_m3 <= valid_a2;
		valid_a3 <= valid_m3;
		valid_m4 <= valid_a3;
		valid_a4_final <= valid_m4;
		
		// update outs
		m0_out_a0_in_pipe <= m0_out;
		a0_out_m1_in_pipe <= a0_out;
		m1_out_a1_in_pipe <= m1_out;
		a1_out_m2_in_pipe <= a1_out;
		m2_out_a2_in_pipe <= m2_out;
		a2_out_m3_in_pipe <= a2_out;
		m3_out_a3_in_pipe <= m3_out;
		a3_out_m4_in_pipe <= a3_out;
		m4_out_a4_in_pipe <= m4_out;
		a4_out_final <= a4_out;
		
	end
end

// assign outputs
assign o_y = a4_out_final;
// ready for inputs as long as receiver is ready for outputs */
assign o_ready = i_ready;   		
// the output is valid as long as the corresponding input was valid and 
//	the receiver is ready. If the receiver isn't ready, the computed output
//	will still remain on the register outputs and the circuit will resume
//  normal operation with the receiver is ready again (i_ready is high)*/
assign o_valid = valid_a4_final & i_ready;	

endmodule

/*******************************************************************************************/

// Multiplier module for the first 16x16 multiplication
module mult16x16 (
	input  [15:0] i_dataa,
	input  [15:0] i_datab,
	output [31:0] o_res
);

logic [31:0] result;

always_comb begin
	result = i_dataa * i_datab;
end

// The result of Q2.14 x Q2.14 is in the Q4.28 format. Therefore we need to change it
// to the Q7.25 format specified in the assignment by shifting right and padding with zeros.
assign o_res = {3'b000, result[31:3]};

endmodule

/*******************************************************************************************/

// Multiplier module for all the remaining 32x16 multiplications
module mult32x16 (
	input  [31:0] i_dataa,
	input  [15:0] i_datab,
	output [31:0] o_res
);

logic [47:0] result;

always_comb begin
	result = i_dataa * i_datab;
end

// The result of Q7.25 x Q2.14 is in the Q9.39 format. Therefore we need to change it
// to the Q7.25 format specified in the assignment by selecting the appropriate bits
// (i.e. dropping the most-significant 2 bits and least-significant 14 bits).
assign o_res = result[45:14];

endmodule

/*******************************************************************************************/

// Adder module for all the 32b+16b addition operations 
module addr32p16 (
	input [31:0] i_dataa,
	input [15:0] i_datab,
	output [31:0] o_res
);

// The 16-bit Q2.14 input needs to be aligned with the 32-bit Q7.25 input by zero padding
assign o_res = i_dataa + {5'b00000, i_datab, 11'b00000000000};

endmodule

/*******************************************************************************************/
