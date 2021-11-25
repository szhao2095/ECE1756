# ECE1756 Lab 3 - RAM mapper
## Compilation
```sh
make all
```
## Run
Ensure that logical_rams.txt and logic_block_count.txt are present in the same folder.
Note that if the file names are not the same, changes have to be made in main.cpp.
```sh
./mapper # Shows help
./mapper 1 # Runs with Stratix-IV architecture
./mapper 2|3 <max_Kbit> <max_width> <ratio> # Runs with Single BRAM Type architecture, without and with LUTRAM respectively
./mapper 4 <lbs_per_lutram> <max_Kbit_1> <max_width_1> <ratio_1> <max_Kbit_2> <max_width_2> <ratio_2> # Runs with custom architecture, Stratix-IV LUTRAM and 2 BRAMs
```
## Example
```sh
# Run mapping tool with Stratix-IV architecture, pipe into mapping file, run checker on mapping file
./mapper 1 > mapping_stratix_iv
./checker -d -t logical_rams.txt logic_block_count.txt mapping_stratix_iv > checker_stratix_iv

# Run mapping tool with single BRAM type with LUTRAM
# BRAM 4K, 64bit max width, 50 LBs/BRAM
./mapper 3 4 64 50 > mapping_single_BRAM_w_LUTRAM
./checker -t -l 1 1 -b 4096 64 50 1 logical_rams.txt logic_block_count.txt mapping_single_BRAM_w_LUTRAM > checker_single_BRAM_w_LUTRAM

# Run mapping tool with single BRAM type without LUTRAM
# BRAM 64K, 256bit max width, 200 LBs/BRAM
# Note that LUTRAM is still defined to align the type field of the mapping output
./mapper 2 64 256 200 > mapping_single_BRAM_w_LUTRAM
./checker -t -l 1 1 -b 65536 256 200 1 logical_rams.txt logic_block_count.txt mapping_single_BRAM_wo_LUTRAM > checker_single_BRAM_wo_LUTRAM

# Run mapping tool with custom architecture
# 20 LBs/LUTRAM
# BRAM 16K, 32bit max width, 10 LBs/BRAM
# BRAM 32K, 64bit max width, 100 LBs/BRAM
./mapper 4 20 16 32 10 32 64 100 > mapping_single_BRAM_w_LUTRAM
./checker -t -l 19 1 -b 16384 32 10 1 -b 32768 64 100 1 logical_rams.txt logic_block_count.txt mapping_custom_arch > checker_custom_arch
```

## Python Scripts
A total of 6 python scripts are written to assist in the evaluation of the mapping tool.  
File names will have to be changed within each script.
| Script | Purpose |
| ------ | ------ |
| check_pram_usage.py | Graphs the utilization of physical RAMs for the 4 implementations |
| check_tile_usage.py | Graphs the utilization of tiles for the 4 implementations |
| check_circuit_area.py | Graphs the area usage across the 69 circuits |
| compare_area.py | Graphs the difference in area between two checker output (in table mode) files |
| explore_single_bram.py | Sets up, runs, collect and evaluates results based on different settings for the single BRAM architecture |
| explore_custom_arch.py | Sets up, runs, collect and evaluates results based on different settings for the custom (2 BRAM, LUTRAM) architecture |
### Run
```sh
python2.7 <script_name>
```












