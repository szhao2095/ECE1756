#include <iostream>

#include "LogicalRam.h"
#include "PhysicalRam.h"
#include "MappedRam.h"
#include "helper.h"

using namespace std;

// Get functions
P_RamType MappedRam::getType_output() {
	switch (type) {
		case BRAM8192:
			return BRAM8192;
		case BRAM128K:
			return BRAM128K;
		case BRAM_CUSTOM_1:
			return BRAM8192;	// For checker since there's no type 4
		case BRAM_CUSTOM_2:
			return BRAM128K;	// For checker since there's no type 5
		default:
			return LUTRAM;
	}
}
	
// Print functions
void MappedRam::printInfo() {
	cout << "Logical Ram ID: " << logical_ram_id;
	cout << ", ID: " << id;
	cout << ", S: " << S;
	cout << ", P: " << P;
	cout << ", P_RamType: " << getPRAMType_S(type);
	cout << ", W: " << W;
	cout << ", D: " << D;
	cout << ", L_RamMode: " << getLRAMMode_S(mode);
	cout << ", Additional LUTs: " << add_luts;
	cout << ", Area: " << area;
	cout << ", Cost: " << cost;
	cout << endl; 
}
