#include "Arch.h"
#include "helper.h"

using namespace std;

// Set functions
void Architecture::addPhysicalRam(PhysicalRam physical_ram_n) {
	physical_rams.push_back(physical_ram_n);
}

// Print functions
void Architecture::printInfo() {	
	for (auto i: physical_rams) {
		cout << "Max bits: " << i.getMaxBits();
		cout << ", Max depth: " << i.getMaxDepth();
		cout << ", Max width: " << i.getMaxWidth();
		cout << ", Type: " << getPRAMType_S(i.getType());
		
		map<unsigned int, unsigned int>& z = i.getWidthToDepths();
		cout << ", Possible widths::depths: ";
		for (auto j: i.getPossibleWidths()) {
			cout << j << "::" << z[j] << ", ";
		}
		cout << endl;
	}
}