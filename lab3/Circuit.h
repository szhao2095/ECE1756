#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include <vector>

#include "LogicalRam.h"
#include "MappedRam.h"

using namespace std;

class Circuit {
private:
	unsigned int id;
	unsigned int logic_block_num;
	
	vector<LogicalRam> logical_rams;
	vector<MappedRam> mapped_rams;
	
	double area;
	map<P_RamType, unsigned int> physical_ram_counts;
	
	// Optimization vars
	// Map of width to vector of logical ram ids with that width in descending order (largest depth first) 
	map<unsigned int, vector<pair<unsigned int, unsigned int>>> width_logical_ram_map;
	
public:
	Circuit(unsigned int id_n, unsigned int logic_block_num_n) : id(id_n), logic_block_num(logic_block_num_n) {}
	
	// Get functions
	unsigned int getID() { return id; }
	unsigned int getLogicBlockNum() { return logic_block_num; }
	vector<LogicalRam>& getLogicalRams() { return logical_rams; }
	vector<LogicalRam> getLogicalRams_Copy() { return logical_rams; }
	vector<MappedRam>& getMappedRams() { return mapped_rams; }
	double getArea() { return area; }
	map<P_RamType, unsigned int>& getPhysicalRamCounts() { return physical_ram_counts; }
	map<unsigned int, vector<pair<unsigned int, unsigned int>>>& getWidthLogicalRamMap() { return width_logical_ram_map; }
	
	// Set functions
	void addLogicalRam(unsigned int logical_ram_id_n, unsigned int depth_n, unsigned int width_n, L_RamMode mode_n);
	void addMappedRam(MappedRam mapped_ram_n);
	void setArea(double area_n) { area = area_n; }
	
	// Computation functions
	void computeArea(vector<PhysicalRam>& physical_rams, unsigned int arch_N, double arch_LB_area);

	// Print functions
	void printInfo_LogicalRam();
	void printInfo_MappedRam();
	void printInfo_WidthLogicalRamMap();
	void printInfo_Area();
};


#endif /* CIRCUIT_H_ */