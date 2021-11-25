#ifndef MAPPEDRAM_H_
#define MAPPEDRAM_H_

#include "PhysicalRam.h"

using namespace std;

class MappedRam {
private:
	unsigned int logical_ram_id;
	unsigned int logical_ram_width;
	unsigned int logical_ram_depth;
	unsigned int id;
	unsigned int S;
	unsigned int P;
	P_RamType type;
	unsigned int W;
	unsigned int D;
	L_RamMode mode;
	
	unsigned int add_luts;
	double area;
	
	double cost;
	
public:
	MappedRam(unsigned int logical_ram_id_n, unsigned int logical_ram_width_n, unsigned int logical_ram_depth_n, unsigned int id_n, unsigned int S_n, unsigned int P_n, P_RamType type_n, unsigned int W_n, unsigned int D_n, L_RamMode mode_n, unsigned int add_luts_n, double area_n, double cost_n) : logical_ram_id(logical_ram_id_n), logical_ram_width(logical_ram_width_n), logical_ram_depth(logical_ram_depth_n), id(id_n), S(S_n), P(P_n), type(type_n), W(W_n), D(D_n), mode(mode_n), add_luts(add_luts_n), area(area_n), cost(cost_n) {}

	// Get functions
	unsigned int getLogicalRamID() { return logical_ram_id; }
	unsigned int getLogicalRamWidth() { return logical_ram_width; }
	unsigned int getLogicalRamDepth() { return logical_ram_depth; }
	unsigned int getID() { return id; }
	unsigned int getS() { return S; }
	unsigned int getP() { return P; }
	P_RamType getType() { return type; }
	P_RamType getType_output(); // Hacking this for output print
	unsigned int getW() { return W; }
	unsigned int getD() { return D; }
	L_RamMode getMode() { return mode; }
	unsigned int getAddLuts() { return add_luts; }
	double getArea() { return area; }
	double getCost() { return cost; }
	
	// Set functions
	
	// Print functions
	void printInfo();

	// Sort functions
	bool operator < (const MappedRam& mapped_ram) {
		return cost < mapped_ram.cost;
	}
};

#endif /* MAPPEDRAM_H_ */