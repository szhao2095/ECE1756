`timescale 1 ns / 1 ps

module lab2_tb();

// Define some local parameters useful for simulation
localparam CLK_PERIOD = 20;					// Clock period is 20ns
localparam QSTEP = CLK_PERIOD/4;				// Time step of a quarter of a clock period
localparam TIMESTEP = CLK_PERIOD/10;		// Time step of one tenth of a clock period
localparam FILTER_SIZE = 3;					// Convolution filter size (3x3)
localparam PADDING_SIZE = FILTER_SIZE/2;	// Image padding size 
// You have to set the project directory for the testbench to operate correctly
localparam PROJECT_DIR = "C:/Users/ZaKaye/Desktop/Pixiv/MEng_Study_2021_Fall/FPGA/lab2/lab2-fpga-1/dse/dse1/dse1_14";

// Declare signals for the DUT interface
logic clk;
logic reset;
logic i_valid;
logic i_ready;
logic o_valid;
logic o_ready;
logic [71:0] i_f;
logic [7:0]  i_x;
logic [7:0]  o_y;

// Define some filters for testing
logic [71:0] identity_filter;	// Identity filter (output image is the same as input image)
initial identity_filter = {8'd0, 8'd0, 8'd0, 8'd0, 8'd1, 8'd0, 8'd0, 8'd0, 8'd0};
logic [71:0] edge_filter;			// Edge detection filter
initial edge_filter = {-8'd1, -8'd1, -8'd1, -8'd1, 8'd8, -8'd1, -8'd1, -8'd1, -8'd1};
logic [71:0] sharpen_filter;		// Image sharpening filter
initial sharpen_filter = {8'd0, -8'd1, 8'd0, -8'd1, 8'd5, -8'd1, 8'd0, -8'd1, 8'd0};

// Define the test image and the golden results you want to use for testing
// PS: These tests are ordered in increasing difficulty and size. Following the same order when you
// test your design will make it easier to debug. You can uncomment the parameters of the test you 
// want to use.

// Test 1: Delta Dot (3 x 512) & Identity filter
// This test uses a 3 x 512 black image with one white pixel in the middle
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/01_delta_point.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/01_delta_point_identity_golden.pgm"};
//initial i_f = identity_filter;

// Test 2: Delta Line (3 x 512) & Identity filter
// This test uses a 3 x 512 black image with a white vertical line of pixels in the middle
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/02_delta_line.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/02_delta_line_identity_golden.pgm"};
//initial i_f = identity_filter;

// Test 3a: Step (3 x 512) & Identity filter
// This test uses a 3 x 512 half black and half white image
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/03_step.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/03_step_identity_golden.pgm"};
//initial i_f = identity_filter;

// Test 3b: Step (3 x 512) & Edge filter
// This test uses a 3 x 512 half black and half white image
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/03_step.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/03_step_edge_golden.pgm"};
//initial i_f = edge_filter;

// Test 4a: Pixel Checker Board (3 x 512) & Identity filter
// This test uses a 3 x 512 image with alternating black and white pixels in a checker board pattern
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/04_pixelchecker.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/04_pixelchecker_identity_golden.pgm"};
//initial i_f = identity_filter;

// Test 4b: Pixel Checker Board (3 x 512) & Edge filter
// This test uses a 3 x 512 image with alternating black and white pixels in a checker board pattern
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/04_pixelchecker.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/04_pixelchecker_edge_golden.pgm"};
//initial i_f = edge_filter;

// Test 5a: Checker Board (50 x 512) & Identity filter
// This test uses a 50 x 512 image of a coarse checker board pattern
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/05_checker.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/05_checker_identity_golden.pgm"};
//initial i_f = identity_filter;

// Test 5b: Checker Board (50 x 512) & Edge filter
// This test uses a 50 x 512 image of a coarse checker board pattern
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/05_checker.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/05_checker_edge_golden.pgm"};
//initial i_f = edge_filter;

// Test 6a: Small Hinton (50 x 512) & Edge filter
// This test uses a 50 x 512 snippet of the Geoffrey Hinton's image
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/06_hinton_small.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/06_hinton_small_edge_golden.pgm"};
//initial i_f = edge_filter;

// Test 6b: Small Hinton (50 x 512) & Sharpen filter
// This test uses a 50 x 512 snippet of the Geoffrey Hinton's image
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/06_hinton_small.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/06_hinton_small_sharpen_golden.pgm"};
//initial i_f = sharpen_filter;

// Test 7a: Hinton (512 x 512) & Edge filter
// This test uses a full 512 x 512 image of Geoffrey Hinton
localparam TEST_IMAGE = {PROJECT_DIR, "/tests/07_hinton.pgm"};
localparam GOLDEN_RES = {PROJECT_DIR, "/tests/07_hinton_edge_golden.pgm"};
initial i_f = edge_filter;

// Test 7b: Hinton (512 x 512) & Sharpen filter
// This test uses a full 512 x 512 image of Geoffrey Hinton
//localparam TEST_IMAGE = {PROJECT_DIR, "/tests/07_hinton.pgm"};
//localparam GOLDEN_RES = {PROJECT_DIR, "/tests/07_hinton_sharpen_golden.pgm"};
//initial i_f = sharpen_filter; 


// Generate a 50MHz clock
initial clk = 1'b1;
always #(CLK_PERIOD/2) clk = ~clk;

// This is the function to calculate the error between the produced output and golden result
function logic[7:0] error(input logic[7:0] x0, input logic[7:0] x1);
begin
	if(x0 > x1) begin
		error = x0 - x1;
	end else begin
		error = x1 - x0;
	end
end
endfunction

// Instantiation of the DUT circuit
lab2 dut (
	.clk(clk),
	.reset(reset),
	.i_valid(i_valid),
	.i_ready(i_ready),
	.o_valid(o_valid),
	.o_ready(o_ready),
	.i_f(i_f),
	.i_x(i_x),
	.o_y(o_y)
);

// Declare more signals used for testing
integer test_image, result_image, difference_image;
integer image_width, image_height;
integer char, temp, pixel_id, pixel_val;
logic [8000:0] line;
logic valid_stall_tested = 0;
logic [7:0] saved_i_x;
integer out_id;
integer current_time = 0;
logic ready_stall_tested = 0;
logic fail = 1'b0;
integer golden_result, sim_log, temp1;
logic [7:0] golden_pixel;

// Producer Process: Sequential testbench code to initialize the circuit and
// send input values to it
initial begin
	// Open specified test image
	test_image = $fopen(TEST_IMAGE, "r");
	
	// Read the image PGM header
	temp = $fgets(line, test_image);
	temp = $fgets(line, test_image);
	temp = $fscanf(test_image, "%d %d\n", image_width, image_height);
	$display("Image width = %d, Image height = %d\n", image_width, image_height);
	temp = $fgets(line, test_image);
	
	// i_valid starts low
	i_valid = 1'b0;
	i_x = 0;
	
	// Hold the reset high for 3.5 cycles
	// Doing this to ensure the design is fully reset if someone uses a multi-cycle 
	// reset strategy, and that the reset goes low away from any clock edges to 
	// avoid any subtle timing issues when doing gate-level (timing) simulation.
	reset = 1'b1;
	#(3*CLK_PERIOD);
	#(2*QSTEP);
	reset = 1'b0;
	#(2*QSTEP);
	
	// Stream in a row of zero pixels for padding
	for(pixel_id = 0; pixel_id < (image_width+2*PADDING_SIZE); pixel_id = pixel_id + 1) begin
		#(QSTEP);
		i_x = 0;
		#(2*QSTEP);
		i_valid = 1'b1;
		while(!o_ready) begin
			i_valid = 1'b0;
			#(CLK_PERIOD);
			i_valid = 1'b1;
		end
		#(QSTEP);
	end
	
	// Stream in the values of the image pixels
	for(pixel_id = 0; pixel_id < ((image_width+2*PADDING_SIZE)*image_height); pixel_id = pixel_id + 1) begin		
		// Start at positive edge. Advance quarter cycle
		#(QSTEP);
		
		// Prepare an input value (if it is first or last pixel of the row, insert zero for padding in case of a 3x3 filte. Otherwise, insert pixel)
		if(((pixel_id % (image_width+2*PADDING_SIZE) == 0) || ((pixel_id+1) % (image_width+2*PADDING_SIZE) == 0))) begin
			pixel_val = 0;
		end else begin
			temp = $fscanf(test_image, "%d ", pixel_val); 
		end
		i_x = pixel_val;
		saved_i_x = i_x;
		
		// Advance half a cycle
		#(2*QSTEP);
		
		// Check o_ready and set i_valid
		i_valid = 1'b1;
		while(!o_ready) begin
			// If DUT claims to not be ready, then it shouldn't be an issue if we 
			// give it an erroneous value.
			i_x = 128;
			i_valid = 1'b0;
			
			// Wait for a clock period before checking o_ready again
			#(CLK_PERIOD);
			
			// Restore the correct value of i_x if we're going to go out of this while loop
			i_x = saved_i_x;
			i_valid = 1'b1;
		end
		
		// Test that DUT stalls properly if we don't give it a valid input
		if (pixel_id == image_width/4 && !valid_stall_tested) begin
			i_valid = 1'b0;
			i_x = 23; // Strange input to make sure it is ignored.
			#(3*CLK_PERIOD);
			
			// DUT may not be ready at this point, so wait until it is
			while(!o_ready) begin
				#(CLK_PERIOD);
			end

			i_x = saved_i_x;
			i_valid = 1'b1;
			valid_stall_tested = 1;
		end
		
		// Advance another quarter cycle to next positive edge
		#(QSTEP);
	end
	
	// Stream in a row of zero pixels for padding
	for(pixel_id = 0; pixel_id < (image_width+2*PADDING_SIZE); pixel_id = pixel_id + 1) begin
		#(QSTEP);
		i_x = 0;
		#(2*QSTEP);
		i_valid = 1'b1;
		while(!o_ready) begin
			i_valid = 1'b0;
			#(CLK_PERIOD);
			i_valid = 1'b1;
		end
		#(QSTEP);
	end
	
	$fclose(test_image);
end

// Consumer process: Sequential testbench code to receive output
// from the circuit and verify it. Also stops the simulation when
// all outputs are delivered.
initial begin
	// Downstream consumer device is initially ready to receive data
	i_ready = 1'b1;

	// Delay until just before the first posedge
	#(CLK_PERIOD - TIMESTEP);
	current_time = current_time + (CLK_PERIOD - TIMESTEP);
	
	// Open files to write the result image, difference image and simulation log
	result_image = $fopen({PROJECT_DIR, "/verilog_out.pgm"}, "w");
	difference_image = $fopen({PROJECT_DIR, "/verilog_diff.pgm"}, "w");
	sim_log = $fopen({PROJECT_DIR, "/verilog_sim_log"}, "w");
	
	// Open file to read golden results
	golden_result = $fopen(GOLDEN_RES, "r");
	
	// Write result image PGM header
	$fwrite(result_image, "P2\n");
	$fwrite(result_image, "# Generated by ECE1756 Verilog testbench\n");
	$fwrite(result_image, "%d %d\n", image_width, image_height);
	$fwrite(result_image, "255\n");
	
	// Write difference image PGM header
	$fwrite(difference_image, "P2\n");
	$fwrite(difference_image, "# Generated by ECE1756 Verilog testbench\n");
	$fwrite(difference_image, "%d %d\n", image_width, image_height);
	$fwrite(difference_image, "255\n");
	
	// Read and ignore the golden result PGM header
	temp1 = $fgets(line, golden_result);
	temp1 = $fgets(line, golden_result);
	temp1 = $fgets(line, golden_result);
	temp1 = $fgets(line, golden_result);
	
	
	for (out_id = 0; out_id < image_width * image_height; out_id = out_id) begin
		// We are now at the point just before the posedge. Check o_valid and compare results.
		if (o_valid) begin
			// Read a pixel from the golden result
			temp1 = $fscanf(golden_result, "%d ", golden_pixel);
			
			// Write the output pixel and difference between output and golden result
			$fwrite(result_image, "%d ", o_y);
			$fwrite(difference_image, "%d ", error(o_y, golden_pixel));
			if(out_id % image_width == 0) begin
				$fwrite(result_image, "\n");
				$fwrite(difference_image, "\n");
			end
			
			// Display the comparison result and write it to the simulation log file
			if(golden_pixel == o_y) begin
				$display("Pixel %d matching! \tExpected: %d \tGot: %d", out_id, golden_pixel, o_y);
				$fwrite(sim_log, "Pixel %d matching! \tExpected: %d \tGot: %d\n", out_id, golden_pixel, o_y);
			end else begin
				$display("Pixel %d doesn't match! \tExpected: %d \tGot: %d", out_id, golden_pixel, o_y);
				$fwrite(sim_log, "Pixel %d doesn't match! \tExpected: %d \tGot: %d\n", out_id, golden_pixel, o_y);
				fail = 1'b1;
			end
			
			// Increment our loop counter
			out_id = out_id + 1;
		end
		
		// Advance to positive edge
		#(TIMESTEP);
		current_time = current_time + (TIMESTEP);

		// Then advance another quarter cycle
		#(QSTEP);
		current_time = current_time + (QSTEP);

		// Set i_ready
		i_ready = 1'b1;
		
		// Test that DUT stalls properly if receiver isn't ready
		if (out_id == image_height/2 && !ready_stall_tested) begin
			i_ready = 1'b0;
			// Wait for 6 clock periods
			#(6*CLK_PERIOD);
			current_time = current_time + (6*CLK_PERIOD);
			// Then restore i_ready
			i_ready = 1'b1;
			ready_stall_tested = 1;
		end
		
		// Then advance to just before the next posedge
		#(3*QSTEP - TIMESTEP);
		current_time = current_time + (3*QSTEP - TIMESTEP);
	end
	
	// Display the result of the simulation and write it to the simulation log file
	if(fail) begin
		$display("%s", "TEST FAILED! SOME PIXELS ARE NOT MATCHING GOLDEN RESULT!");
		$fwrite(sim_log, "TEST FAILED! SOME PIXELS ARE NOT MATCHING GOLDEN RESULT!\n");
	end else begin
		$display("%s", "TEST PASSED! ALL PIXELS ARE MATCHING GOLDEN RESULT!");
		$fwrite(sim_log, "TEST PASSED! ALL PIXELS ARE MATCHING GOLDEN RESULT!\n");
	end
	
	$fclose(result_image);
	$fclose(difference_image);
	$fclose(sim_log);
	$fclose(golden_result);
	
	$stop(0);
end


endmodule
