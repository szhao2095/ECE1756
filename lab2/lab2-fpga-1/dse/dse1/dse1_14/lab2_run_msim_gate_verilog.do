transcript on
if {[file exists gate_work]} {
	vdel -lib gate_work -all
}
vlib gate_work
vmap work gate_work

vlog -vlog01compat -work work +incdir+. {lab2.vo}

vlog -sv -work work +incdir+C:/Users/ZaKaye/Desktop/Pixiv/MEng_Study_2021_Fall/FPGA/lab2/lab2-fpga-1/dse/dse1/dse1_14 {C:/Users/ZaKaye/Desktop/Pixiv/MEng_Study_2021_Fall/FPGA/lab2/lab2-fpga-1/dse/dse1/dse1_14/lab2_tb.sv}

vsim -t 1ps -L altera_ver -L altera_lnsim_ver -L twentynm_ver -L lpm_ver -L sgate_ver -L twentynm_hssi_ver -L twentynm_hip_ver -L gate_work -L work -voptargs="+acc"  lab2_tb

source lab2_dump_all_vcd_nodes.tcl
add wave *
view structure
view signals
run -all
