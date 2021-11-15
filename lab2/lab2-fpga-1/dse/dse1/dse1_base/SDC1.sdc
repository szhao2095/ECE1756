# Set the target clock period to 1 ns for the one clock in this design (clk).
# This is unachievably fast, telling Quartus to optimize as much as possible for timing.
# Some CAD tools do not behave well with impossible constraints, but they work fine with
# Quartus and are interpreted as "go as fast as possible".

create_clock -period 1 -name clk clk