transcript on
if {[file exists gate_work]} {
	vdel -lib gate_work -all
}
vlib gate_work
vmap work gate_work

vlog -vlog01compat -work work +incdir+. {lab1_pipe.vo}

vlog -vlog01compat -work work +incdir+C:/Users/ZaKaye/Desktop/Pixiv/MEng\ Study\ 2021\ Fall/FPGA/lab1/lab1_pipe {C:/Users/ZaKaye/Desktop/Pixiv/MEng Study 2021 Fall/FPGA/lab1/lab1_pipe/lab1_pipe_tb.v}

vsim -t 1ps -L altera_ver -L altera_lnsim_ver -L twentynm_ver -L lpm_ver -L sgate_ver -L twentynm_hssi_ver -L twentynm_hip_ver -L gate_work -L work -voptargs="+acc"  lab1_pipe_tb

source lab1_pipe_dump_all_vcd_nodes.tcl
add wave *
view structure
view signals
run -all
