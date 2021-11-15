module lab1_fsm #
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

// FSM control signals 
logic store_x;
logic store_y;

logic mmux;
logic [2:0] amux;

// Register declarations
logic [WIDTHIN-1:0] x_reg;
logic [WIDTHOUT-1:0] y_reg; // Doubles as register for intermediate steps of the calculation

// Wires
logic [WIDTHOUT-1:0] m_in;
logic [WIDTHOUT-1:0] m_out;
logic [WIDTHIN-1:0] a_in;
logic [WIDTHOUT-1:0] a_out;


fsm fsm0(.clk(clk), .reset(reset), .i_valid(i_valid), .i_ready(i_ready), 
				.o_valid(o_valid), .o_ready(o_ready), .store_x(store_x), .store_y(store_y), .mmux(mmux), .amux(amux));
				
// Multiplexers for inputs to multiplier and adder
mux2x1 mux2x1_mmux(.sel(mmux), .a({5'b0, A5, 11'b0}), .b(y_reg), .out(m_in));
mux8x1 mux8x1_amux(.sel(amux), .a(A4), .b(A3), .c(A2), .d(A1), .e(A0), .f(16'b0), .g(16'b0), .h(16'b0), .out(a_in));
				
// Only one set of mulitplier and adder
mult32x16 Mult1 (.i_dataa(m_in), 	.i_datab(x_reg), 		.o_res(m_out));
addr32p16 Addr1 (.i_dataa(m_out), 	.i_datab(a_in), 	.o_res(a_out));

// Registers
always_ff @(posedge clk or posedge reset) begin
	if (reset) begin
		x_reg <= 0;
		y_reg <= 0;
	end else if (store_x) begin
		x_reg <= i_x;
	end else if (store_y) begin
		y_reg <= a_out;
	end
end

assign o_y = y_reg;

endmodule

/*******************************************************************************************/

module fsm(
	input clk,
	input reset,
	
	input i_valid,
	input i_ready,
	output logic o_valid,
	output logic o_ready,
	
	output logic store_x,
	output logic store_y,
	output logic mmux, 		//	Mux control signal for multiplier
	output logic [2:0] amux // Mux control signal for adder
);

typedef enum logic [2:0] { input_x, compute_0, compute_1, compute_2, compute_3, compute_4, output_y } State;
State currentState, nextState;

logic input_valid;
logic enable;

// State register updates
always_ff @(posedge clk or posedge reset) begin
	if (reset) begin
		currentState <= input_x;
	end else begin
		currentState <= nextState;	// Logic to check for enable/i_ready in comb 
	end
end

// Some signal assignments
always_comb begin
	input_valid = i_valid;
	enable = i_ready;
end

// FSM next state computation
always_comb begin
	case(currentState)
		input_x: if(input_valid) begin	// input_x state waits for valid input
			nextState = compute_0;
		end else begin
			nextState = input_x;
		end
		compute_0: nextState = compute_1; 	// compute_<x> states computes the corresponding o_y regardless if
		compute_1: nextState = compute_2;	// ready to output to downstream or not
		compute_2: nextState = compute_3;
		compute_3: nextState = compute_4;
		compute_4: nextState = output_y;
		output_y: if(enable) begin 			// o_y is computed, o_valid is toggled, waits for i_ready before moving on
			nextState = input_x;
		end else begin
			nextState = output_y;
		end
	endcase
end

// FSM control signals
always_comb begin
	// Base case
	o_valid = 1'b0;
	o_ready = 1'b0;
	
	store_x = 1'b0;			// Controls x_reg
	store_y = 1'b0;			// Controls y_reg
	
	mmux = 1'b0;				// Controls multiplier mux
	amux = 3'b000;				// Controls adder mux
	
	case(currentState)
		input_x: begin
			o_valid = 1'b0;	// Repeating for clarity, output not ready
			o_ready = 1'b1;	// Ready for new input
			store_x = 1'b1;	// Store x into register
		end
		compute_0: begin
			mmux = 1'b0;		// Repeating for clarity, selecting a5 for multiplier, a5 padded to Q7.25
			amux = 3'b000;		// Selecting a4 for adder
			store_y = 1'b1;	// Store intermediate result into register
		end
		compute_1: begin
			mmux = 1'b1;		// Selecting previously computed value for multiplier
			amux = 3'b001;		// Selecting a3 for adder
			store_y = 1'b1;	// Store intermediate result into register
		end
		compute_2: begin
			mmux = 1'b1;		// Selecting previously computed value for multiplier
			amux = 3'b010;		// Selecting a2 for adder
			store_y = 1'b1;	// Store intermediate result into register
		end
		compute_3: begin
			mmux = 1'b1;		// Selecting previously computed value for multiplier
			amux = 3'b011;		// Selecting a1 for adder
			store_y = 1'b1;	// Store intermediate result into register
		end
		compute_4: begin
			mmux = 1'b1;		// Selecting previously computed value for multiplier
			amux = 3'b100;		// Selecting a0 for adder
			store_y = 1'b1;	// Store final result into register
		end
		output_y: begin
			o_valid = 1'b1;	// Output ready
			o_ready = 1'b0;	// Repeating for clarity. not ready for new input
		end
	endcase
end

endmodule

/*******************************************************************************************/

module mux2x1 (
	input sel,
	input [31:0] a,
	input [31:0] b,
	
	output logic [31:0] out
);

always_comb begin
	case (sel)
		1'b0: out = a;
		1'b1: out = b;
	endcase
end

endmodule

/*******************************************************************************************/

module mux8x1 (
	input [2:0] sel,
	input [15:0] a,
	input [15:0] b,
	input [15:0] c,
	input [15:0] d,
	input [15:0] e,
	input [15:0] f,
	input [15:0] g,
	input [15:0] h,
	
	output logic [15:0] out
);

always_comb begin
	case (sel[2:0])
		3'b000: out = a;
		3'b001: out = b;
		3'b010: out = c;
		3'b011: out = d;
		3'b100: out = e;
		3'b101: out = f;
		3'b110: out = g;
		3'b111: out = h;
	endcase
end

endmodule

/*******************************************************************************************/

//// Multiplier module for the first 16x16 multiplication
//module mult16x16 (
//	input  [15:0] i_dataa,
//	input  [15:0] i_datab,
//	output [31:0] o_res
//);
//
//logic [31:0] result;
//
//always_comb begin
//	result = i_dataa * i_datab;
//end
//
//// The result of Q2.14 x Q2.14 is in the Q4.28 format. Therefore we need to change it
//// to the Q7.25 format specified in the assignment by shifting right and padding with zeros.
//assign o_res = {3'b000, result[31:3]};
//
//endmodule

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
