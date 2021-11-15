`timescale 1 ns / 1 ps

module lab1_pipe_tb();

// Define timesteps that will help proceed through the simulation
localparam CLK_PERIOD = 24; 				//ns
localparam QSTEP = CLK_PERIOD/4;			//a timestep of quarter clock cycle
localparam TIMESTEP = CLK_PERIOD/8;	   //a small timestep 
// Define input & output precision and number of test inputs
localparam WIDTHIN = 16;
localparam WIDTHOUT = 32;
localparam N_INPUTS = 50;
// Initialize the random number to 2, so the first input has an easy to compute output.
localparam SEED = 32'd2;   
// Define scaling factors to convert inputs and outputs from binary to decimal values with specific fixed point format
localparam i_scale_factor = 2.0**-14.0; // Q2.14 output format
localparam o_scale_factor = 2.0**-25.0; // Q7.25 output format
// Define the error margin allowed in the produced results
localparam error_threshold = 0.045; 

reg clk;
reg reset;
reg i_valid;
reg i_ready;
wire o_valid;
wire o_ready;
reg [WIDTHIN-1:0] i_x;
wire [WIDTHOUT-1:0] o_y;

// Generate a 42MHz clock
initial clk = 1'b1;
always #(CLK_PERIOD/2) clk = ~clk;

// Instantiate the circuit
lab1_pipe
// Passing down the parameters to the instantiated module is good
// practice, but won't work for gate-level simulation, so 
// commented out.
//#
//(	// parameter assignments
//	.WIDTH(WIDTH),
//	.A(A),
//	.B(B),
//	.C(C),
// .D(D)
//)
dut	// instance name
(
	.clk(clk),
	.reset(reset),
	.i_valid(i_valid),
	.i_ready(i_ready),
	.o_valid(o_valid),
	.o_ready(o_ready),
	.i_x(i_x),
	.o_y(o_y)
);

// A behavioral function to calculate the ground truth results.
// It calculates the Taylor expansion polynomial without factorization
// and it will be used to make sure that all three parts of the
// assignment produce results that are within a specific error margin.
function real exp_taylor(input real x);
	real a0, a1, a2, a3, a4, a5;
begin
	a0 = 1.0;
	a1 = 1.0;
	a2 = 0.5;
	a3 = 0.16667;
	a4 = 0.04167;
	a5 = 0.00833;
	exp_taylor = a0 + (a1 * x) + (a2 * (x**2)) + (a3 * (x**3)) + (a4 * (x**4)) + (a5 * (x**5));
end
endfunction

// A behavioral function to calculate the exponential function directly.
// The produced values will be used to create the exp vs. hardware comparison graph later
function real exp(input real x);
	real base;
begin
	base = 2.71828;
	exp = base ** x;
end
endfunction

// A behavioral function to calculate the error in the produced results compared to the
// ground truth values to make sure it is less than the specified margin [Error = abs(x0 - x1)].
function real error(input real x0, input real x1);
begin
	if(x0 - x1 > 0) begin
		error = x0 - x1;
	end else begin
		error = x1 - x0;
	end
end
endfunction

// Producer Process: Sequential testbench code to initialize the circuit and
// send input values to it
reg [31:0] prod_rand = SEED;
reg [WIDTHIN-1:0] saved_i_x;
reg valid_stall_tested = 0;
integer prod_i;
initial begin
	
	//i_valid starts low
	i_valid = 1'b0;
	
	// Toggle the reset for a cycle
	reset = 1'b1;
	#(CLK_PERIOD);
	reset = 1'b0;
	
	// Generate N_INPUTS random inputs and deliver to circuit
	for (prod_i = 0; prod_i < N_INPUTS; prod_i = prod_i + 1) begin
		
		//start on posedge
		
		//advance quarter cycle
		#(QSTEP);
		//generate an input value
		i_x = prod_rand[WIDTHIN-1:0];
		saved_i_x = i_x;
		prod_rand = $random(prod_rand);
	
		//advance quarter cycle (now on negedge)
		#(QSTEP);
		//nothing to do here
		
		//advance another quarter cycle
		#(QSTEP);
		//check o_ready and set i_valid
		i_valid = 1'b1;
		while(!o_ready) begin
			//if DUT claims to not be ready, then it shouldn't be an issue if we 
			//give it an erroneous value.
			i_x = 13;
			i_valid = 1'b0;
			
			//wait for a clock period before checking o_ready again
			#(CLK_PERIOD);
			//restore the correct value of i_x if we're going to go out of this while loop
			i_x = saved_i_x;
			i_valid = 1'b1;
		end
		
		//test that DUT stalls properly if we don't give it a valid input
		if (prod_i == N_INPUTS / 2 && !valid_stall_tested) begin
			i_valid = 1'b0;
			i_x = 23; // Strange input to make sure it is ignored.
			#(3*CLK_PERIOD);
			
			//DUT may not be ready at this point, so wait until it is
			while(!o_ready) begin
				#(CLK_PERIOD);
			end

			i_x = saved_i_x;
			i_valid = 1'b1;
			valid_stall_tested = 1;
		end
		
		//advance another quarter cycle to next posedge
		#(QSTEP);
	end
end

// Consumer process: Sequential testbench code to receive output
// from the circuit and verify it. Also stops the simulation when
// all outputs are delivered.
reg [31:0] cns_rand = SEED;
reg [WIDTHOUT-1:0] cns_x;
real good_y;
reg fail = 1'b0;
integer cns_i;
integer current_time = 0;
reg ready_stall_tested = 0;
integer fexp, ftest;
real itr;
initial begin
	// downstream consumer device is initially ready to receive data
	i_ready = 1'b1;

	//delay until just before the first posedge
	#(CLK_PERIOD - TIMESTEP);
	current_time = current_time + (CLK_PERIOD - TIMESTEP);
	
	ftest = $fopen("testcase.txt", "w");
	
	// We generate the same numbers as the producer process
	// by using the same random seed sequence
	for (cns_i = 0; cns_i < N_INPUTS; cns_i = cns_i/*cns_i = cns_i + 1*/) begin

		//we are now at the point just before the posedge
		//check o_valid and compare results
		if (o_valid) begin
			// Use our copy of X to calculate the correct Y
			cns_x = cns_rand[WIDTHIN-1:0];
			cns_rand = $random(cns_rand);
			good_y = exp_taylor($itor(cns_x)*i_scale_factor);
			
			// Display and compare the answer to the known good value
			// We print cns_i, not i_x, as cns_i is the input that led to o_y, 
			// not the current input (which may be out of sync in pipelined designs).
			// Check for equality to pass, so that o_y being unknown (X) is a fail.
			if (error(good_y, $itor(o_y)*o_scale_factor) < error_threshold) begin
				$display("at %dns SUCCESS\t X: %9.6f\t Expected Y: %9.6f\t Got Y: %9.6f\t Error: %9.6f\t < %9.6f", current_time, $itor(cns_x)*i_scale_factor, good_y, $itor(o_y)*o_scale_factor, error(good_y, $itor(o_y)*o_scale_factor), error_threshold);
			end
			else begin
				$display("at %dns FAIL\t X: %9.6f\t Expected Y: %9.6f\t Got Y: %9.6f\t Error: %9.6f\t < %9.6f", current_time, $itor(cns_x)*i_scale_factor, good_y, $itor(o_y)*o_scale_factor, error(good_y, $itor(o_y)*o_scale_factor), error_threshold);
				fail = 1'b1;
			end
			$fwrite(ftest, "%f\t%f\n", $itor(cns_x)*i_scale_factor, $itor(o_y)*o_scale_factor);
		
			//increment our loop counter
			cns_i = cns_i + 1;
		end
		
		//advance to posedge
		#(TIMESTEP);
		current_time = current_time + (TIMESTEP);

		//then advance another quarter cycle
		#(QSTEP);
		current_time = current_time + (QSTEP);

		//set i_ready
		i_ready = 1'b1;
		
		//test that DUT stalls properly if receiver isn't ready
		if (cns_i == N_INPUTS / 8 && !ready_stall_tested) begin
			i_ready = 1'b0;
			//wait for 6 clock periods
			#(6*CLK_PERIOD);
			current_time = current_time + (6*CLK_PERIOD);
			//then restore i_ready
			i_ready = 1'b1;
			ready_stall_tested = 1;
		end
		
		//then advance to just before the next posedge
		#(3*QSTEP - TIMESTEP);
		current_time = current_time + (3*QSTEP - TIMESTEP);

	end
	
	$fclose(ftest);
	$display("%s", fail? "SOME TESTS FAILED" : "ALL TESTS PASSED");
	
	fexp = $fopen("exp.txt", "w");
	
	for(itr = 0; itr < 4.0; itr = itr + 0.02) begin
		$fwrite(fexp, "%f\t%f\n", itr, exp(itr));
	end
	
	$fclose(fexp);
	
	$stop(0);
end

endmodule
