#include <algorithm>
#include <cmath>

#include "Circuit.h"
#include "LogicalRam.h"
#include "MappedRam.h"
#include "helper.h"

// Set functions
void Circuit::addLogicalRam(unsigned int logical_ram_id_n, unsigned int depth_n, unsigned int width_n, L_RamMode mode_n) {
	LogicalRam new_logical_ram(logical_ram_id_n, depth_n, width_n, mode_n);
	logical_rams.push_back(new_logical_ram);
}

void Circuit::addMappedRam(MappedRam mapped_ram_n) {
	mapped_rams.push_back(mapped_ram_n);
}

// Computation functions

void Circuit::computeArea(vector<PhysicalRam>& physical_rams, unsigned int arch_N, double arch_LB_area) {
	map<P_RamType, unsigned int> physical_ram_to_lb_count_map;
	map<P_RamType, unsigned int> physical_ram_to_ratio_map;
	for (auto i: physical_rams) {
		physical_ram_to_lb_count_map[i.getType()] = 0;
		physical_ram_to_ratio_map[i.getType()] = i.getRatio();
		
		physical_ram_counts[i.getType()] = 0;
	}
	
	unsigned int total_add_luts = 0;
	for (auto i: mapped_rams) {
		physical_ram_to_lb_count_map[i.getType()] += physical_ram_to_ratio_map[i.getType()];
		total_add_luts += i.getAddLuts();
		
		physical_ram_counts[i.getType()] += 1;
	}
	
	// Now we find out which of the three is the limiting value
	//P_RamType limiting_lb_PRAM_type;
	unsigned int largest_lb_count = 0;
	for (auto i: physical_rams) {
		if (physical_ram_to_lb_count_map[i.getType()] > largest_lb_count) {
			//limiting_lb_PRAM_type = i.getType();
			largest_lb_count = physical_ram_to_lb_count_map[i.getType()];
		}
	}
	
	// One more check against the total logic blocks needed for circuit and additional LUTs
	unsigned int total_circuit_lb = logic_block_num + (unsigned int)ceil((double)total_add_luts / (double)arch_N);
	
	// Blanket check for all non-LUTRAM memories, ensuring that we have at least enough LB for all the logic needed
	if (total_circuit_lb > largest_lb_count) {
		largest_lb_count = total_circuit_lb;
	}
	for (auto i: physical_rams) {
		if (i.getType() == LUTRAM) {
			// If LUTRAM, we need to check how much of the LB count we computed is still free for logic
			// If the amount for logic is not enough, we need to find the new total LB count
			double used_by_lutram = ceil((double)physical_ram_to_lb_count_map[i.getType()] * (1/i.getRatio()));
			double free_for_logic = (double)largest_lb_count - used_by_lutram;
			
			if (total_circuit_lb > free_for_logic) {
				// Remaining LB count not enough for logic
				// Increment largest_lb_count (total needed)
				largest_lb_count = (unsigned int)used_by_lutram + (unsigned int)total_circuit_lb;
			}
			break;
		}
	}
	
	// With the largest type and count known, we find total area
	// LB area calculation, includes LUTRAM
	area = (double)largest_lb_count * arch_LB_area;
	
	for (auto i: physical_rams) {
		if (i.getType() == LUTRAM) {
			// Add on the additional 5000 area per logic block that can do LUTRAM
			area += ceil((double)largest_lb_count * (1/i.getRatio())) * (i.getArea() - arch_LB_area);
			continue;
		}
		area += ceil((double)largest_lb_count/i.getRatio()) * i.getArea();
	}
}

// Print functions
void Circuit::printInfo_LogicalRam() {
	for (auto i: logical_rams) {
		cout << "Circuit id: " << id; 
		cout << ", Logic block count: " << logic_block_num;
		cout << " :: ";
		i.printInfo();
	}
}

void Circuit::printInfo_MappedRam() {
	for (auto i: mapped_rams) {
		cout << "Circuit id: " << id; 
		cout << ", Logic block count: " << logic_block_num;
		cout << " :: ";
		i.printInfo();
	}
}

void Circuit::printInfo_WidthLogicalRamMap() {
	for (auto it = width_logical_ram_map.cbegin(); it != width_logical_ram_map.cend(); ++it) {
		for (auto i: it->second) {
			cout << "Circuit id: " << id; 
			cout << ", Width: " << it->first; 
			cout << ", Logical Ram ID: " << i.first;
			cout << ", Depth: " << i.second;
			cout << endl;
		}
		cout << "===End of Width: " << it->first << "===" << endl;
	}
}

void Circuit::printInfo_Area() {
	cout << "Circuit id: " << id; 
	cout << ", Area: " << area;
	for (auto it = physical_ram_counts.cbegin(); it != physical_ram_counts.cend(); ++it) {
		cout << ", Type: " << getPRAMType_S(it->first);
		cout << ", Count: " << it->second;
	}
	cout << endl;
}





