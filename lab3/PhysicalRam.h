#ifndef PHYSICALRAM_H_
#define PHYSICALRAM_H_

#include <vector>
#include <map>

#include "LogicalRam.h"

using namespace std;

enum P_RamType {
	LUTRAM = 1,
	BRAM8192,
	BRAM128K,
	BRAM_CUSTOM_1,
	BRAM_CUSTOM_2
};

class PhysicalRam {
private:
	unsigned int max_bits;
	unsigned int max_depth;
	unsigned int max_width;
	P_RamType type;
	double ratio;
	double area;
	vector<unsigned int> possible_widths;
	map<unsigned int, unsigned int> width_to_depths;
	
public:
	PhysicalRam(unsigned int max_bits_n, unsigned int max_depth_n, unsigned int max_width_n, P_RamType type_n, double ratio);
	
	// Get functions
	unsigned int getMaxBits() { return max_bits; }
	unsigned int getMaxDepth() { return max_depth; }
	unsigned int getMaxWidth() { return max_width; }
	unsigned int getMaxWidth_mode(L_RamMode mode_n) { return (mode_n == TrueDualPort) ? max_width/2 : max_width; }
	P_RamType getType() { return type; }
	double getRatio() { return ratio; }
	double getArea() { return area; }
	vector<unsigned int>& getPossibleWidths() { return possible_widths; }
	map<unsigned int, unsigned int>& getWidthToDepths() { return width_to_depths; }
	
	// Set functions
	void addPossibleWidth(unsigned int width_n);
	void addToWidthToDepths(unsigned int depth_n, unsigned int width_n);
		
};


#endif /* PHYSICALRAM_H_ */