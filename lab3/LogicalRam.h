#ifndef LOGICALRAM_H_
#define LOGICALRAM_H_

#include <iostream>
#include <string>

using namespace std;

enum L_RamMode {
	ROM = 1,
	SinglePort,
	SimpleDualPort,
	TrueDualPort
};

class LogicalRam {
private:
	unsigned int id;
	unsigned int depth;
	unsigned int width;
	L_RamMode mode;
	
	// Optimization vars
	bool solved = false;
	unsigned int total_size;
	
public:
	LogicalRam(unsigned int id_n, unsigned int depth_n, unsigned int width_n, L_RamMode mode_n) : id(id_n), depth(depth_n), width(width_n), mode(mode_n), total_size(depth_n*width_n) {}
	
	// Get functions
	unsigned int getID() { return id; }
	unsigned int getDepth() { return depth; }
	unsigned int getWidth() { return width; }
	L_RamMode getMode() { return mode; }
	bool getSolved() { return solved; }
	unsigned int getTotalSize() { return total_size; }
	
	// Set functions
	void setSolved() { solved = true; }
	
	// Print functions
	void printInfo();

	// Sort functions
	bool operator < (const LogicalRam& logical_ram) {
		return total_size < logical_ram.total_size;
	}
		
};

#endif /* LOGICALRAM_H_ */