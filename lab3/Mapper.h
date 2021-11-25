#ifndef MAPPER_H_
#define MAPPER_H_

#include <vector>

#include "Circuit.h"
#include "Arch.h"

using namespace std;

enum Solution_Type {
	SingleType_Simple = 1,
	SingleType_Shared,
	SingleType_ModifiedCostFunction,
	SingleType_ModifiedCostFunctionWithSharing
};

/*
	Encompasses the entire problem space.
	Contains all the inputs in circuits and architecture.
*/
class Mapper {
private:
	vector<Circuit> circuits;
	Architecture architecture;
	
	Solution_Type solution_type;

public:
	Mapper(Architecture architecture_n) : architecture(architecture_n) {}
	
	// Get functions
	vector<Circuit>& getCircuits() { return circuits; }
	Architecture& getArchitecture() { return architecture; }
	
	// Set functions
	void addCircuit(unsigned int circuit_id_n, unsigned int circuit_logic_block_count_n);
	void setSolutionType(Solution_Type solution_type_n) { solution_type = solution_type_n; }
	
	// Computation functions
	void initCircuits(string logical_rams_f, string logic_block_count_f);
	void mapAll_Wrapper();
	void mapAll_SingleType_Simple();
	void mapAll_SingleType_Shared();
	void mapAll_SingleType_ModifiedCostFunction();
	void mapAll_SingleType_ModifiedCostFunctionWithSharing();
	void mapAll_SingleType_ModifiedCostFunctionWithSharing_SortedLogicalRam();
	
	void computeCircuitAreas();
	
	// Print functions
	void printCircuits();
	void printCircuitSolutions();
	void printCircuitWidthLogicalRamMap();
	void printCircuitAreas();
	
	void printCircuits_raw();
	void printLogicalRam_raw();
	void printCircuitSolutions_raw();
};


#endif /* MAPPER_H_ */