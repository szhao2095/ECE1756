#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>

#include "Mapper.h"

using namespace std;

enum Architecture_Type {
	Stratix_iv = 1,
	SingleRamType_no_LUTRAM,
	SingleRamType_50_LUTRAM,
	CustomArch_w_LUTRAM
};

void print_help() {
	cout << endl;
	cout << "Usage: ./mapper <architecture_type> <options>" << endl;
	
	cout << "Architecture type: " << endl;
	cout << "1: Stratix-IV" << endl;
	cout << "2: Single RAM type with no LUTRAM" << endl;
	cout << "3: Single RAM type with 50% LUTRAM" << endl;
	cout << "4: Custom architecture with LUTRAM" << endl;
	cout << endl;
	
	cout << "Detailed usage:" << endl;
	cout << "./mapper 1" << endl;
	cout << "./mapper 2|3 <max_Kbit> <max_width> <ratio>" << endl;
	cout << "./mapper 4 <lbs_per_lutram> <max_Kbit_1> <max_width_1> <ratio_1> <max_Kbit_2> <max_width_2> <ratio_2>" << endl;
	cout << endl;
	
	cout << "For 2, 3 and 4, enter details for max_Kbit, max_width and ratio for desired BRAM as well as LUTRAM ratio if applicable." << endl;
	cout << "For 3, LUTRAM specs are same as Stratix-IV" << endl;
	cout << endl;
	
	cout << "<lbs_per_lutram>: Number of LBs where there's 1 LUTRAM" << endl;
	cout << "<max_Kbit>: In number of 1024bits, e.g. 1, 2, 4, = 1K, 2K, 4K etc" << endl;
	cout << "<max_width>: In powers of 2 up to <max_Kbit>" << endl;
	cout << "<ratio>: Number of LBs for each BRAM" << endl;
	cout << endl;
	cout << endl;
		
	exit(-1);
}
	

int main(int argc, char **argv) {
	/*
		Control panel
	*/
	string logical_ram_file = "logical_rams.txt";
	string logical_block_file = "logic_block_count.txt";
	
	Architecture_Type arch_type = Stratix_iv;
	
	double ratio_LUT = 1/0.5;	
	double ratio_8192 = 10;
	double ratio_128K = 300;		
	
	unsigned int max_bits_custom_1 = 1*1024;
	unsigned int max_depth_custom_1 = 1*1024;
	unsigned int max_width_custom_1 = 32;
	double ratio_custom_1 = 10;
	unsigned int max_bits_custom_2 = 1*1024;
	unsigned int max_depth_custom_2 = 1*1024;
	unsigned int max_width_custom_2 = 32;
	double ratio_custom_2 = 10;
	
	if (argc < 2) {
		print_help();
	}
	switch (atoi(argv[1])) {
		case 2:
			arch_type = SingleRamType_no_LUTRAM;
			
			if (argc < 5) {
				print_help();
			}
			if (atoi(argv[3]) > atoi(argv[2]) * 1024) {
				print_help();
			}
			
			max_bits_custom_1 = atoi(argv[2]) * 1024;
			max_depth_custom_1 = max_bits_custom_1;
			max_width_custom_1 = atoi(argv[3]);
			ratio_custom_1 = atoi(argv[4]);
			
			break;
		case 3:
			arch_type = SingleRamType_50_LUTRAM;
			
			if (argc < 5) {
				print_help();
			}
			if (atoi(argv[3]) > atoi(argv[2]) * 1024) {
				print_help();
			}
			
			max_bits_custom_1 = atoi(argv[2]) * 1024;
			max_depth_custom_1 = max_bits_custom_1;
			max_width_custom_1 = atoi(argv[3]);
			ratio_custom_1 = (double)atoi(argv[4]);
			
			break;
		case 4:
			arch_type = CustomArch_w_LUTRAM;
			
			if (argc < 9) {
				print_help();
			}
			/* if (atoi(argv[2]) > 100 or atoi(argv[2]) <= 0) {
				print_help();
			} */
			if (atoi(argv[4]) > atoi(argv[3]) * 1024) {
				print_help();
			}
			if (atoi(argv[7]) > atoi(argv[6]) * 1024) {
				print_help();
			}
			
			// ratio_LUT = 1/((double)atoi(argv[2])/100);
			ratio_LUT = (double)atoi(argv[2]);
			
			max_bits_custom_1 = atoi(argv[3]) * 1024;
			max_depth_custom_1 = max_bits_custom_1;
			max_width_custom_1 = atoi(argv[4]);
			ratio_custom_1 = (double)atoi(argv[5]);
			
			max_bits_custom_2 = atoi(argv[6]) * 1024;
			max_depth_custom_2 = max_bits_custom_1;
			max_width_custom_2 = atoi(argv[7]);
			ratio_custom_2 = (double)atoi(argv[8]);
		
			break;
		case 1:
		default:
			break;
	}
	
	// SingleType_Simple = 1,
	// SingleType_Shared,
	// SingleType_ModifiedCostFunction,
	// SingleType_ModifiedCostFunctionWithSharing
	Solution_Type solution_type = SingleType_ModifiedCostFunctionWithSharing;
	
	/*
		Switch blocks that controls what physical memory gets inserted
	*/
	bool use_lutram = true;
	bool use_bram8192 = true;
	bool use_bram128k = true;
	bool use_bram_custom_1 = false;
	bool use_bram_custom_2 = false;
	switch (arch_type) {
		case SingleRamType_no_LUTRAM:
			use_lutram = false;
			use_bram8192 = false;
			use_bram128k = false;
			use_bram_custom_1 = true;
			break;
		case SingleRamType_50_LUTRAM:
			use_bram8192 = false;
			use_bram128k = false;
			use_bram_custom_1 = true;
			break;
		case CustomArch_w_LUTRAM:
			use_bram8192 = false;
			use_bram128k = false;
			use_bram_custom_1 = true;
			use_bram_custom_2 = true;
			break;
		case Stratix_iv:
		default:
			break;
	}
	
	/*
		Contains all the test cases needed
	*/
	// Base: Stratix-IV 
	// Rams: LUTRAM, BRAM8192, BRAM128K
	
	
	unsigned int k = 6;
	unsigned int N = 10;
	double area = 35000;
	Architecture arch(k, N, area);
	
	if (use_lutram) {
		unsigned int max_bits_LUT = 640;
		unsigned int max_depth_LUT = 64;
		unsigned int max_width_LUT = 20;
		PhysicalRam new_physical_ram_LUT(max_bits_LUT, max_depth_LUT, max_width_LUT, LUTRAM, ratio_LUT);
		new_physical_ram_LUT.addPossibleWidth(10);
		new_physical_ram_LUT.addPossibleWidth(20);
		new_physical_ram_LUT.addToWidthToDepths(64, 10);
		new_physical_ram_LUT.addToWidthToDepths(32, 20);
		arch.addPhysicalRam(new_physical_ram_LUT);
	}
	
	if (use_bram8192) {
		unsigned int max_bits_8192 = 8192;
		unsigned int max_depth_8192 = 8192;
		unsigned int max_width_8192 = 32;
		PhysicalRam new_physical_ram_8192(max_bits_8192, max_depth_8192, max_width_8192, BRAM8192, ratio_8192);
		for (unsigned int i_width = 1; i_width <= max_width_8192; i_width *= 2) {
			new_physical_ram_8192.addPossibleWidth(i_width);
			new_physical_ram_8192.addToWidthToDepths(max_bits_8192/i_width, i_width);
		}
		arch.addPhysicalRam(new_physical_ram_8192);
	}
	
	if (use_bram128k) {
		unsigned int max_bits_128K = 131072; // 128*1024
		unsigned int max_depth_128K = 131072;
		unsigned int max_width_128K = 128;
		PhysicalRam new_physical_ram_128K(max_bits_128K, max_depth_128K, max_width_128K, BRAM128K, ratio_128K);
		for (unsigned int i_width = 1; i_width <= max_width_128K; i_width *= 2) {
			new_physical_ram_128K.addPossibleWidth(i_width);
			new_physical_ram_128K.addToWidthToDepths(max_bits_128K/i_width, i_width);
		}
		arch.addPhysicalRam(new_physical_ram_128K);
	}
	
	if (use_bram_custom_1) {
		PhysicalRam new_physical_ram_custom_1(max_bits_custom_1, max_depth_custom_1, max_width_custom_1, BRAM_CUSTOM_1, ratio_custom_1);
		for (unsigned int i_width = 1; i_width <= max_width_custom_1; i_width *= 2) {
			new_physical_ram_custom_1.addPossibleWidth(i_width);
			new_physical_ram_custom_1.addToWidthToDepths(max_bits_custom_1/i_width, i_width);
		}
		arch.addPhysicalRam(new_physical_ram_custom_1);
	}
	
	if (use_bram_custom_2) {
		PhysicalRam new_physical_ram_custom_2(max_bits_custom_2, max_depth_custom_2, max_width_custom_2, BRAM_CUSTOM_2, ratio_custom_2);
		for (unsigned int i_width = 1; i_width <= max_width_custom_2; i_width *= 2) {
			new_physical_ram_custom_2.addPossibleWidth(i_width);
			new_physical_ram_custom_2.addToWidthToDepths(max_bits_custom_2/i_width, i_width);
		}
		arch.addPhysicalRam(new_physical_ram_custom_2);
	}
	// arch.printInfo();	
	
	/*
		Mapping computation
	*/
	
    /* clock_t tStart = clock();
	unsigned int run_count = 20;
	
	for (unsigned int i = 0; i < run_count; i++) {
		Mapper mapper(arch);
		mapper.initCircuits(logical_ram_file, logical_block_file);
		mapper.setSolutionType(solution_type);
		mapper.mapAll_Wrapper();
	}
	
	printf("Average time taken for %d runs: %.2fs\n", run_count, (double)(clock() - tStart)/CLOCKS_PER_SEC/run_count); */
	
	Mapper mapper(arch);
	mapper.initCircuits(logical_ram_file, logical_block_file);
	mapper.setSolutionType(solution_type);
	mapper.mapAll_Wrapper();
	
	// mapper.computeCircuitAreas();
	
	/*
		Print/Output
	*/
	// mapper.printCircuits();
	// mapper.printCircuitSolutions();
	// mapper.printCircuitWidthLogicalRamMap();
	// mapper.printCircuitAreas();

	// mapper.printCircuits_raw();
	// mapper.printLogicalRam_raw();
	mapper.printCircuitSolutions_raw();
	
	return 0;
}