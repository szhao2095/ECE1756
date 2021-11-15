transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -sv -work work +incdir+C:/Users/ZaKaye/Desktop/Pixiv/MEng\ Study\ 2021\ Fall/FPGA/lab1/lab1_pipe {C:/Users/ZaKaye/Desktop/Pixiv/MEng Study 2021 Fall/FPGA/lab1/lab1_pipe/lab1_pipe.sv}

vlog -vlog01compat -work work +incdir+C:/Users/ZaKaye/Desktop/Pixiv/MEng\ Study\ 2021\ Fall/FPGA/lab1/lab1_pipe {C:/Users/ZaKaye/Desktop/Pixiv/MEng Study 2021 Fall/FPGA/lab1/lab1_pipe/lab1_tb.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L twentynm_ver -L twentynm_hssi_ver -L twentynm_hip_ver -L rtl_work -L work -voptargs="+acc"  lab1_tb

add wave *
view structure
view signals
run -all
