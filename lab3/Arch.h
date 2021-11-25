#ifndef ARCH_H_
#define ARCH_H_

#include <vector>

#include "PhysicalRam.h"

using namespace std;

class Architecture {
private:
	unsigned int k;
	unsigned int N;
	double logic_block_area;
	vector<PhysicalRam> physical_rams;
	
public:
	Architecture(unsigned int k_n, unsigned int N_n, double logic_block_area_n) : k(k_n), N(N_n), logic_block_area(logic_block_area_n) {}
		
	// Get functions
	unsigned int getK() { return k; }
	unsigned int getN() { return N; }
	double getLogicBlockArea() { return logic_block_area; }
	vector<PhysicalRam>& getPhysicalRams() { return physical_rams; }
	
	// Set functions
	void addPhysicalRam(PhysicalRam physical_ram_n);
	
	// Print functions
	void printInfo();
};


#endif /* ARCH_H_ */