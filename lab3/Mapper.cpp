#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>

#include "Mapper.h"
#include "helper.h"

using namespace std;

// Set functions
void Mapper::addCircuit(unsigned int circuit_id_n, unsigned int circuit_logic_block_count_n) {
	Circuit new_circuit(circuit_id_n, circuit_logic_block_count_n);
	circuits.push_back(new_circuit);
}

// Computation functions
void Mapper::initCircuits(string logical_rams_f, string logic_block_count_f) {
	ifstream logical_rams_fd(logical_rams_f);
	ifstream logic_block_count_fd(logic_block_count_f);
	
	string line;
	getline(logic_block_count_fd, line); // Skip first line since is field headings
	string circuit_id_s, circuit_logic_block_count_s;
	while (logic_block_count_fd >> circuit_id_s >> circuit_logic_block_count_s)
	{
		unsigned int circuit_id_n = stoi(circuit_id_s);
		unsigned int circuit_logic_block_count_n = stoi(circuit_logic_block_count_s);
		addCircuit(circuit_id_n, circuit_logic_block_count_n);
	}
	
	string logical_ram_id_s, depth_s, width_s, mode_s;
	L_RamMode mode_n;
	getline(logical_rams_fd, line);	// Skip first line since is total ciruit count
	getline(logical_rams_fd, line);	// Skip first line since is field headings
	while (logical_rams_fd >> circuit_id_s >> logical_ram_id_s >> mode_s >> depth_s >> width_s)
	{
		unsigned int circuit_id_n = stoi(circuit_id_s);
		unsigned int logical_ram_id_n = stoi(logical_ram_id_s);
		unsigned int depth_n = stoi(depth_s);
		unsigned int width_n = stoi(width_s);
		if (mode_s == "ROM") {
			mode_n = ROM;
		}
		else if (mode_s == "SinglePort") {
			mode_n = SinglePort;
		}
		else if (mode_s == "SimpleDualPort") {
			mode_n = SimpleDualPort;
		}
		else if (mode_s == "TrueDualPort") {
			mode_n = TrueDualPort;
		}
		else {
			cout << "Mapper.cpp: Unknown logical ram type." << endl;
		}
		
		// This line assumes circuits are entered in the sorted order
		// Assumes circuit id == position in vector array
		circuits[circuit_id_n].addLogicalRam(logical_ram_id_n, depth_n, width_n, mode_n);
		
		// Optimization var prep
		// Only do for non TrueDualPort
		if (mode_s != "TrueDualPort" && mode_s != "SimpleDualPort") {
			
			// Retrieve the corresponding map for current circuit
			map<unsigned int, vector<pair<unsigned int, unsigned int>>>& width_logical_ram_map = circuits[circuit_id_n].getWidthLogicalRamMap();
			
			if (width_logical_ram_map.find(width_n) == width_logical_ram_map.end()) {
				// Width not yet in map
				vector<pair<unsigned int, unsigned int>> logical_ram_same_width;
				logical_ram_same_width.push_back(make_pair(logical_ram_id_n, depth_n));
				width_logical_ram_map[width_n] = logical_ram_same_width;
			} else {
				// Doing this sort manually since there's some const issue with sort
				for (size_t i = 0; i < width_logical_ram_map[width_n].size(); i++) {
					if (width_logical_ram_map[width_n][i].second > depth_n) {
						continue;
					}
					// At this point we have reach the point where depth_n is larger than or equal to the rest
					width_logical_ram_map[width_n].insert(width_logical_ram_map[width_n].begin()+i, make_pair(logical_ram_id_n, depth_n));
					break;
				}
			}
			
		}
	}
	// sortCircuitWidthLogicalRamMap();
}


void Mapper::mapAll_Wrapper() {	
	switch (solution_type) {
		case SingleType_Simple:
			mapAll_SingleType_Simple();
			break;
		case SingleType_Shared:
			mapAll_SingleType_Shared();
			break;
		case SingleType_ModifiedCostFunction:
			mapAll_SingleType_ModifiedCostFunction();
			break;
		case SingleType_ModifiedCostFunctionWithSharing:
			mapAll_SingleType_ModifiedCostFunctionWithSharing();
			break;
		default:
			cout << "Mapper.cpp: Solution type not defined within bounds: " << solution_type << endl;
	}
}	

void Mapper::mapAll_SingleType_Simple() {
	for (auto& circuit: circuits) {
		unsigned int counter = 0;
			
		for (auto logical_ram: circuit.getLogicalRams()) {
			if (logical_ram.getSolved()) {
				// Skip if solved
				continue;
			}
			
			// Find solution (as long as legal) for every physical ram ...
			vector<MappedRam> possible_sol_across_phy_ram;
				
			for (auto physical_ram: architecture.getPhysicalRams()) {
				
				// Find solution (as long as legal) for every possible width for a given physical ram ...
				vector<MappedRam> possible_sol_given_phy_ram;
				
				/// Legality checks
				// If TrueDualPort mode, and current physical ram is LUTRAM, we skip
				if (logical_ram.getMode() == TrueDualPort && physical_ram.getType() == LUTRAM){
					continue;
				}
				
				for (auto current_width: physical_ram.getPossibleWidths()) { // Assuming there is at least one solution where S <= 16
					
					/// Legality checks
					// If TrueDualPort mode, and current width is max, we skip
					if (logical_ram.getMode() == TrueDualPort && current_width == physical_ram.getMaxWidth()){
						continue;
					}
					
					/// Computations
					unsigned int add_decoder_luts = 0;
					unsigned int add_mux_luts = 0;
					
					// Initialize accordingly
					unsigned int logical_ram_id = logical_ram.getID();
					unsigned int logical_ram_width = logical_ram.getWidth();
					unsigned int logical_ram_depth = logical_ram.getDepth();
					unsigned int id = counter++;
					unsigned int S = 1;
					unsigned int P = 1;
					P_RamType type = physical_ram.getType();
					unsigned int W = current_width;
					unsigned int D = physical_ram.getWidthToDepths()[W];
					L_RamMode mode = logical_ram.getMode();
					
					unsigned int add_luts = 0;
					double area = 0.0;
					
					double cost = 0.0;
					
					// First we check for width
					if (logical_ram.getWidth() > W) {	
						// If physical ram width is not enough, need to go parallel first
						
						// First find out how many in parallel
						P = (unsigned int)ceil((double)logical_ram.getWidth()/(double)W);
					}
						
					// Check if depth is enough
					if (logical_ram.getDepth() > D) {
						// If depth is not enough, we go series
						S = (unsigned int)ceil((double)logical_ram.getDepth()/(double)D);
						if (S > 16) {
							// Skip if S > 16
							continue;
						}
						// If some in series, additional luts needed
						if (S > 1) {
							// Additional decoder LUTs calculation
							if (S == 2) {
								add_decoder_luts = 1;
							} else {
								// Since log2(R):R decoder = R log2(R)-LUTs
								// With R <= 16, log2(R) <= 4 
								// FPGA LUTs are 6-LUTs
								add_decoder_luts = S;
							}
							
							// Additional mux LUTs calculation
							// One 6-LUTs makes a 4:1 mux at maximum
							if (S > 1 && S <= 4) {
								// One 4:1 mux takes 4 inputs
								add_mux_luts = 1;
							} else if (S > 4 && S <= 7) {
								// Two 4:1 mux, first one takes 4 inputs, second one takes 3 more
								add_mux_luts = 2;
							} else if (S > 7 && S <= 10) {
								// Three 4:1 mux, first two takes 4 inputs each, second one takes 2 more
								add_mux_luts = 3;
							} else if (S > 10 && S <= 13) {
								// Four 4:1 mux, first three takes 4 inputs each, second one takes 1 more
								add_mux_luts = 4;
							} else if (S > 13 && S <= 16) {
								// Five 4:1 mux, first four takes 4 inputs each, second one takes none
								add_mux_luts = 5;
							} else {
								// Exceeding limit of S < 16
								cout << "Mapper.cpp: S exceeds limit of 16." << endl;
							}
							// Make sure to account for width
							add_mux_luts *= (W*P);
						}
					}
						
					add_luts = add_decoder_luts + add_mux_luts;
					// If TrueDualPort mode we need to double additional LUTs
					if (logical_ram.getMode() == TrueDualPort) {
						add_luts *= 2;
					}
					
					// Area calculation
					area = ((double)S * (double)P * physical_ram.getArea()) + (ceil((double)add_luts / (double)architecture.getN()) * architecture.getLogicBlockArea());
					cost = area;
					
					MappedRam new_mapped_ram(logical_ram_id, logical_ram_width, logical_ram_depth, id, S, P, type, W, D, mode, add_luts, area, cost);
					possible_sol_given_phy_ram.push_back(new_mapped_ram);
				} /* End for (auto current_width: physical_ram.getPossibleWidths()) */
				
				// At this point we have all possible solutions for current physical ram
				// Sort and take least area solution
				if (!possible_sol_given_phy_ram.empty()) {
					sort(possible_sol_given_phy_ram.begin(), possible_sol_given_phy_ram.end());
					possible_sol_across_phy_ram.push_back(possible_sol_given_phy_ram[0]);
				}
				
			} /* End for (auto physical_ram: architecture.getPhysicalRams()) */
			
			// At this point we have all best solutions for each type of physical ram
			// Sort and take smallest solution
			if (!possible_sol_across_phy_ram.empty()) {
				sort(possible_sol_across_phy_ram.begin(), possible_sol_across_phy_ram.end());
				circuit.addMappedRam(possible_sol_across_phy_ram[0]);
				logical_ram.setSolved();
			}
			
		} /* End for (auto logical_ram: circuit.getLogicalRams()) */
		
		// At this point we have all solutions for current circuit
		// Carrying on with next circuit ...
		
	} /* End for (auto circuit: circuits) */
}

void Mapper::mapAll_SingleType_Shared() {
	for (auto& circuit: circuits) {
		unsigned int counter = 0;
		map<unsigned int, vector<pair<unsigned int, unsigned int>>>& width_logical_ram_map = circuit.getWidthLogicalRamMap();
			
		for (auto& logical_ram: circuit.getLogicalRams()) {
			if (logical_ram.getSolved()) {
				// Skip if solved
				continue;
			}
			
			// Find solution (as long as legal) for every physical ram ...
			vector<MappedRam> possible_sol_across_phy_ram;
			// Map for <MappedRam ID, LogicalRam ID that's to share physical ram>
			map<unsigned int, unsigned int> possible_ram_share;
				
			for (auto physical_ram: architecture.getPhysicalRams()) {
				
				// Find solution (as long as legal) for every possible width for a given physical ram ...
				vector<MappedRam> possible_sol_given_phy_ram;
				
				/// Legality checks
				// If TrueDualPort mode, and current physical ram is LUTRAM, we skip
				if (logical_ram.getMode() == TrueDualPort && physical_ram.getType() == LUTRAM){
					continue;
				}
				
				for (auto current_width: physical_ram.getPossibleWidths()) { // Assuming there is at least one solution where S <= 16
					
					/// Legality checks
					// If TrueDualPort mode, and current width is max, we skip
					if (logical_ram.getMode() == TrueDualPort && current_width == physical_ram.getMaxWidth()){
						continue;
					}
					
					/// Computations
					unsigned int add_decoder_luts = 0;
					unsigned int add_mux_luts = 0;
					
					// Initialize accordingly
					unsigned int logical_ram_id = logical_ram.getID();
					unsigned int logical_ram_width = logical_ram.getWidth();
					unsigned int logical_ram_depth = logical_ram.getDepth();
					unsigned int id = counter++;
					unsigned int S = 1;
					unsigned int P = 1;
					P_RamType type = physical_ram.getType();
					unsigned int W = current_width;
					unsigned int D = physical_ram.getWidthToDepths()[W];
					L_RamMode mode = logical_ram.getMode();
					
					unsigned int add_luts = 0;
					double area = 0.0;
					
					double cost = 0.0;
					
					// First we check for width
					if (logical_ram.getWidth() > W) {	
						// If physical ram width is not enough, need to go parallel first
						
						// First find out how many in parallel
						P = (unsigned int)ceil((double)logical_ram.getWidth()/(double)W);
					}
						
					// Check if depth is enough
					if (logical_ram.getDepth() > D) {
						// If depth is not enough, we go series
						S = (unsigned int)ceil((double)logical_ram.getDepth()/(double)D);
						if (S > 16) {
							// Skip if S > 16
							continue;
						}
						// If some in series, additional luts needed
						if (S > 1) {
							// Additional decoder LUTs calculation
							if (S == 2) {
								add_decoder_luts = 1;
							} else {
								// Since log2(R):R decoder = R log2(R)-LUTs
								// With R <= 16, log2(R) <= 4 
								// FPGA LUTs are 6-LUTs
								add_decoder_luts = S;
							}
							
							// Additional mux LUTs calculation
							// One 6-LUTs makes a 4:1 mux at maximum
							if (S > 1 && S <= 4) {
								// One 4:1 mux takes 4 inputs
								add_mux_luts = 1;
							} else if (S > 4 && S <= 7) {
								// Two 4:1 mux, first one takes 4 inputs, second one takes 3 more
								add_mux_luts = 2;
							} else if (S > 7 && S <= 10) {
								// Three 4:1 mux, first two takes 4 inputs each, second one takes 2 more
								add_mux_luts = 3;
							} else if (S > 10 && S <= 13) {
								// Four 4:1 mux, first three takes 4 inputs each, second one takes 1 more
								add_mux_luts = 4;
							} else if (S > 13 && S <= 16) {
								// Five 4:1 mux, first four takes 4 inputs each, second one takes none
								add_mux_luts = 5;
							} else {
								// Exceeding limit of S < 16
								cout << "Mapper.cpp: S exceeds limit of 16." << endl;
							}
							// Make sure to account for width
							add_mux_luts *= (W*P);
						}
					}
						
					add_luts = add_decoder_luts + add_mux_luts;
					// If TrueDualPort mode we need to double additional LUTs
					if (logical_ram.getMode() == TrueDualPort) {
						add_luts *= 2;
					}
					
					// Area calculation
					area = ((double)S * (double)P * physical_ram.getArea()) + (((double)add_luts / (double)architecture.getN()) * architecture.getLogicBlockArea());
					cost = area;
					
					// At this point we have a potential solution
					// To optimize mapping, we want to see if we can fit in another logical ram in this
					// if this is not LUTRAM and not already in TrueDualPort mode and not operating at the max width
					if (physical_ram.getType() != LUTRAM && logical_ram.getMode() != SimpleDualPort && logical_ram.getMode() != TrueDualPort && current_width != physical_ram.getMaxWidth()){
						
						for (unsigned int width_i = current_width; current_width >= 1; current_width /= 2) {
						
							// Look for other logical rams that has the same (current) width 
							vector<pair<unsigned int, unsigned int>> potential_matches = width_logical_ram_map[width_i];
							
							unsigned int remaining_depth = D*S - logical_ram_depth;
							unsigned int remaining_space = S*P*W*D - logical_ram_depth*logical_ram_width;
							for (auto potential_match: potential_matches) {
								// For each potential match, we check if the depth can fit
								// Note that potential_matches is in sorted order: descending depth size
								
								if (potential_match.first == logical_ram_id) {
									// Skip if found self
									continue;
								}
								
								// Not enough space/depth left, skip
								if (potential_match.second > remaining_depth) {
									continue;
								}
								if (width_i*potential_match.second > remaining_space) {
									continue;
								}
								
								// Assuming there's a possible match
								// Retrieve from circuit's logical_ram vector based on id
								if (circuit.getLogicalRams()[potential_match.first].getSolved()){
									// If solved, move on
									continue;
								}
								
								// At this point we have a logical ram that can share physical ram
								// We record this possible ram share in our map
								// Map is used for look up later after all widths are explored
								possible_ram_share[id] = potential_match.first;
								
								// Update cost value
								cost = area * ((double)logical_ram_depth/((double)logical_ram_depth+(double)potential_match.second));
								
								// If choose to share, mode changes to either TrueDualPort or ROM
								mode = TrueDualPort;
								
								// Only can share ram with one other logical ram
								// Since potential_matches is already sorted, 
								// the first one we find is the largest possible depth that fits
								break; 
							}
							// Note that if no potential match is found, we leave the loop and there is no difference compared
							// to the simple single ram mapping function above
						}
					}
					
					MappedRam new_mapped_ram(logical_ram_id, logical_ram_width, logical_ram_depth, id, S, P, type, W, D, mode, add_luts, area, cost);
					possible_sol_given_phy_ram.push_back(new_mapped_ram);
						
					
				} /* End for (auto current_width: physical_ram.getPossibleWidths()) */
				
				// At this point we have all possible solutions for current physical ram
				// Sort and take least area solution
				if (!possible_sol_given_phy_ram.empty()) {
					sort(possible_sol_given_phy_ram.begin(), possible_sol_given_phy_ram.end());
					possible_sol_across_phy_ram.push_back(possible_sol_given_phy_ram[0]);
				}
				
			} /* End for (auto physical_ram: architecture.getPhysicalRams()) */
			
			// At this point we have all best solutions for each type of physical ram
			// Sort and take smallest solution
			if (!possible_sol_across_phy_ram.empty()) {
				sort(possible_sol_across_phy_ram.begin(), possible_sol_across_phy_ram.end());
				
				// Check if chosen solution does any ram sharing
				if (possible_ram_share.find(possible_sol_across_phy_ram[0].getID()) != possible_ram_share.end()) {
					// If ram sharing
					unsigned int other_logical_ram_id = possible_ram_share[possible_sol_across_phy_ram[0].getID()];
					LogicalRam other_logical_ram = circuit.getLogicalRams()[other_logical_ram_id];
					
					MappedRam shared_mapped_ram(other_logical_ram_id, other_logical_ram.getWidth(), other_logical_ram.getDepth(), possible_sol_across_phy_ram[0].getID(), possible_sol_across_phy_ram[0].getS(), possible_sol_across_phy_ram[0].getP(), possible_sol_across_phy_ram[0].getType(), possible_sol_across_phy_ram[0].getW(), possible_sol_across_phy_ram[0].getD(), possible_sol_across_phy_ram[0].getMode(), possible_sol_across_phy_ram[0].getAddLuts(), possible_sol_across_phy_ram[0].getArea(), possible_sol_across_phy_ram[0].getArea()-possible_sol_across_phy_ram[0].getCost());
					circuit.addMappedRam(shared_mapped_ram);
							
					circuit.getLogicalRams()[other_logical_ram_id].setSolved();
				}
				
				circuit.addMappedRam(possible_sol_across_phy_ram[0]);
				logical_ram.setSolved();
			}
			
		} /* End for (auto logical_ram: circuit.getLogicalRams()) */
		
		// At this point we have all solutions for current circuit
		// Carrying on with next circuit ...
		
	} /* End for (auto circuit: circuits) */
}

void Mapper::mapAll_SingleType_ModifiedCostFunction() {
	for (auto& circuit: circuits) {
		unsigned int counter = 0;		
		
		// Used for cost function
		map<P_RamType, unsigned int> total_physical_ram_used;
		map<P_RamType, unsigned int> available_physical_ram;
		map<P_RamType, double> physical_ram_ratio;
		for (auto physical_ram: architecture.getPhysicalRams()) {
			total_physical_ram_used[physical_ram.getType()] = 0;
			
			// Also defined the available physical ram count for each type 
			// based on the current number of logic blocks used for logic
			// Requiring more physical ram of any type above it's limit 
			// causes LB count to increase and thus incurs more area than just the ram itself
			available_physical_ram[physical_ram.getType()] = floor(circuit.getLogicBlockNum()/physical_ram.getRatio());
			physical_ram_ratio[physical_ram.getType()] = physical_ram.getRatio();
		}
		
			
		for (auto logical_ram: circuit.getLogicalRams()) {
			if (logical_ram.getSolved()) {
				// Skip if solved
				continue;
			}
			
			// Find solution (as long as legal) for every physical ram ...
			vector<MappedRam> possible_sol_across_phy_ram;
				
			for (auto physical_ram: architecture.getPhysicalRams()) {
				
				// Find solution (as long as legal) for every possible width for a given physical ram ...
				vector<MappedRam> possible_sol_given_phy_ram;
				
				/// Legality checks
				// If TrueDualPort mode, and current physical ram is LUTRAM, we skip
				if (logical_ram.getMode() == TrueDualPort && physical_ram.getType() == LUTRAM){
					continue;
				}
				
				for (auto current_width: physical_ram.getPossibleWidths()) { // Assuming there is at least one solution where S <= 16
					
					/// Legality checks
					// If TrueDualPort mode, and current width is max, we skip
					if (logical_ram.getMode() == TrueDualPort && current_width == physical_ram.getMaxWidth()){
						continue;
					}
					
					/// Computations
					unsigned int add_decoder_luts = 0;
					unsigned int add_mux_luts = 0;
					
					// Initialize accordingly
					unsigned int logical_ram_id = logical_ram.getID();
					unsigned int logical_ram_width = logical_ram.getWidth();
					unsigned int logical_ram_depth = logical_ram.getDepth();
					unsigned int id = counter++;
					unsigned int S = 1;
					unsigned int P = 1;
					P_RamType type = physical_ram.getType();
					unsigned int W = current_width;
					unsigned int D = physical_ram.getWidthToDepths()[W];
					L_RamMode mode = logical_ram.getMode();
					
					unsigned int add_luts = 0;
					double area = 0.0;
					
					double cost = 0.0;
					
					// First we check for width
					if (logical_ram.getWidth() > W) {	
						// If physical ram width is not enough, need to go parallel first
						
						// First find out how many in parallel
						P = (unsigned int)ceil((double)logical_ram.getWidth()/(double)W);
					}
						
					// Check if depth is enough
					if (logical_ram.getDepth() > D) {
						// If depth is not enough, we go series
						S = (unsigned int)ceil((double)logical_ram.getDepth()/(double)D);
						if (S > 16) {
							// Skip if S > 16
							continue;
						}
						// If some in series, additional luts needed
						if (S > 1) {
							// Additional decoder LUTs calculation
							if (S == 2) {
								add_decoder_luts = 1;
							} else {
								// Since log2(R):R decoder = R log2(R)-LUTs
								// With R <= 16, log2(R) <= 4 
								// FPGA LUTs are 6-LUTs
								add_decoder_luts = S;
							}
							
							// Additional mux LUTs calculation
							// One 6-LUTs makes a 4:1 mux at maximum
							if (S > 1 && S <= 4) {
								// One 4:1 mux takes 4 inputs
								add_mux_luts = 1;
							} else if (S > 4 && S <= 7) {
								// Two 4:1 mux, first one takes 4 inputs, second one takes 3 more
								add_mux_luts = 2;
							} else if (S > 7 && S <= 10) {
								// Three 4:1 mux, first two takes 4 inputs each, second one takes 2 more
								add_mux_luts = 3;
							} else if (S > 10 && S <= 13) {
								// Four 4:1 mux, first three takes 4 inputs each, second one takes 1 more
								add_mux_luts = 4;
							} else if (S > 13 && S <= 16) {
								// Five 4:1 mux, first four takes 4 inputs each, second one takes none
								add_mux_luts = 5;
							} else {
								// Exceeding limit of S < 16
								cout << "Mapper.cpp: S exceeds limit of 16." << endl;
							}
							// Make sure to account for width
							add_mux_luts *= (W*P);
						}
					}
						
					add_luts = add_decoder_luts + add_mux_luts;
					// If TrueDualPort mode we need to double additional LUTs
					if (logical_ram.getMode() == TrueDualPort) {
						add_luts *= 2;
					}
					
					// Area calculation
					area = ((double)S * (double)P * physical_ram.getArea()) + (ceil((double)add_luts / (double)architecture.getN()) * architecture.getLogicBlockArea());
					
					// Cost calculations
					// Based on what physical ram, how many has been used, how many to be used, we compute a cost 
					// If using this ram does not cause more LB to be needed, cost is just area
					cost = area;
					if ((total_physical_ram_used[physical_ram.getType()] + S*P) > available_physical_ram[physical_ram.getType()]) {
						// If using this ram causes more LB to be needed, add that to cost
						if (physical_ram.getType() == LUTRAM) {
							// physical_ram.getArea() here since LUTRAM blocks are more expensive
							cost += ((double)S*(double)P) * physical_ram.getArea();
							// physical_ram.getRatio() - 1 since a portion is accounted for above
							cost += ((double)S*(double)P) * (physical_ram_ratio[physical_ram.getType()] - 1) * architecture.getLogicBlockArea();
						} else {
							// Assuming all required BRAM blocks are going to require new LB/tiles
							// Assuming all new LBs are just normal ones to simplify calculation
							cost += ((double)S*(double)P) * physical_ram_ratio[physical_ram.getType()] * architecture.getLogicBlockArea();
						}
					}
					
					MappedRam new_mapped_ram(logical_ram_id, logical_ram_width, logical_ram_depth, id, S, P, type, W, D, mode, add_luts, area, cost);
					possible_sol_given_phy_ram.push_back(new_mapped_ram);
				} /* End for (auto current_width: physical_ram.getPossibleWidths()) */
				
				// At this point we have all possible solutions for current physical ram
				// Sort and take least area solution
				if (!possible_sol_given_phy_ram.empty()) {
					sort(possible_sol_given_phy_ram.begin(), possible_sol_given_phy_ram.end());
					possible_sol_across_phy_ram.push_back(possible_sol_given_phy_ram[0]);
				}
				
			} /* End for (auto physical_ram: architecture.getPhysicalRams()) */
			
			// At this point we have all best solutions for each type of physical ram
			// Sort and take smallest solution
			if (!possible_sol_across_phy_ram.empty()) {
				sort(possible_sol_across_phy_ram.begin(), possible_sol_across_phy_ram.end());
				circuit.addMappedRam(possible_sol_across_phy_ram[0]);
				logical_ram.setSolved();
				
				// Update total physical ram used for cost function
				P_RamType physical_ram_used_type = possible_sol_across_phy_ram[0].getType();
				total_physical_ram_used[physical_ram_used_type] += possible_sol_across_phy_ram[0].getS() * possible_sol_across_phy_ram[0].getP();
				
				unsigned int extra_physical_ram_required = total_physical_ram_used[physical_ram_used_type] - available_physical_ram[physical_ram_used_type];
				if (extra_physical_ram_required > 0) {
					// Reach here if solution's ram needs exceed currently available
					// Update available since more LBs are now available with this new ram requirement					
					unsigned int new_total_lb_count = (double)total_physical_ram_used[physical_ram_used_type] * physical_ram_ratio[physical_ram_used_type];
								
					for (auto physical_ram: architecture.getPhysicalRams()) {
						available_physical_ram[physical_ram.getType()] = floor(new_total_lb_count/physical_ram.getRatio());
					}
				}
			}
		} /* End for (auto logical_ram: circuit.getLogicalRams()) */
		
		// At this point we have all solutions for current circuit
		// Carrying on with next circuit ...
		
	} /* End for (auto circuit: circuits) */
}

void Mapper::mapAll_SingleType_ModifiedCostFunctionWithSharing() {
	for (auto& circuit: circuits) {
		unsigned int counter = 0;	
		map<unsigned int, vector<pair<unsigned int, unsigned int>>>& width_logical_ram_map = circuit.getWidthLogicalRamMap();	
		
		// Used for cost function
		map<P_RamType, unsigned int> total_physical_ram_used;
		map<P_RamType, unsigned int> available_physical_ram;
		map<P_RamType, double> physical_ram_ratio;
		for (auto physical_ram: architecture.getPhysicalRams()) {
			total_physical_ram_used[physical_ram.getType()] = 0;
			
			// Also defined the available physical ram count for each type 
			// based on the current number of logic blocks used for logic
			// Requiring more physical ram of any type above it's limit 
			// causes LB count to increase and thus incurs more area than just the ram itself
			available_physical_ram[physical_ram.getType()] = floor(circuit.getLogicBlockNum()/physical_ram.getRatio());
			physical_ram_ratio[physical_ram.getType()] = physical_ram.getRatio();
		}
		
			
		for (auto& logical_ram: circuit.getLogicalRams()) {
			if (logical_ram.getSolved()) {
				// Skip if solved
				continue;
			}
			
			// Find solution (as long as legal) for every physical ram ...
			vector<MappedRam> possible_sol_across_phy_ram;
			// Map for <MappedRam ID, LogicalRam ID that's to share physical ram>
			map<unsigned int, unsigned int> possible_ram_share;
				
			for (auto physical_ram: architecture.getPhysicalRams()) {
				
				// Find solution (as long as legal) for every possible width for a given physical ram ...
				vector<MappedRam> possible_sol_given_phy_ram;
				
				/// Legality checks
				// If TrueDualPort mode, and current physical ram is LUTRAM, we skip
				if (logical_ram.getMode() == TrueDualPort && physical_ram.getType() == LUTRAM){
					continue;
				}
				
				for (auto current_width: physical_ram.getPossibleWidths()) { // Assuming there is at least one solution where S <= 16
					
					/// Legality checks
					// If TrueDualPort mode, and current width is max, we skip
					if (logical_ram.getMode() == TrueDualPort && current_width == physical_ram.getMaxWidth()){
						continue;
					}
					
					/// Computations
					unsigned int add_decoder_luts = 0;
					unsigned int add_mux_luts = 0;
					
					// Initialize accordingly
					unsigned int logical_ram_id = logical_ram.getID();
					unsigned int logical_ram_width = logical_ram.getWidth();
					unsigned int logical_ram_depth = logical_ram.getDepth();
					unsigned int id = counter++;
					unsigned int S = 1;
					unsigned int P = 1;
					P_RamType type = physical_ram.getType();
					unsigned int W = current_width;
					unsigned int D = physical_ram.getWidthToDepths()[W];
					L_RamMode mode = logical_ram.getMode();
					
					unsigned int add_luts = 0;
					double area = 0.0;
					
					double cost = 0.0;
					
					// First we check for width
					if (logical_ram.getWidth() > W) {	
						// If physical ram width is not enough, need to go parallel first
						
						// First find out how many in parallel
						P = (unsigned int)ceil((double)logical_ram.getWidth()/(double)W);
					}
						
					// Check if depth is enough
					if (logical_ram.getDepth() > D) {
						// If depth is not enough, we go series
						S = (unsigned int)ceil((double)logical_ram.getDepth()/(double)D);
						if (S > 16) {
							// Skip if S > 16
							continue;
						}
						// If some in series, additional luts needed
						if (S > 1) {
							// Additional decoder LUTs calculation
							if (S == 2) {
								add_decoder_luts = 1;
							} else {
								// Since log2(R):R decoder = R log2(R)-LUTs
								// With R <= 16, log2(R) <= 4 
								// FPGA LUTs are 6-LUTs
								add_decoder_luts = S;
							}
							
							// Additional mux LUTs calculation
							// One 6-LUTs makes a 4:1 mux at maximum
							if (S > 1 && S <= 4) {
								// One 4:1 mux takes 4 inputs
								add_mux_luts = 1;
							} else if (S > 4 && S <= 7) {
								// Two 4:1 mux, first one takes 4 inputs, second one takes 3 more
								add_mux_luts = 2;
							} else if (S > 7 && S <= 10) {
								// Three 4:1 mux, first two takes 4 inputs each, second one takes 2 more
								add_mux_luts = 3;
							} else if (S > 10 && S <= 13) {
								// Four 4:1 mux, first three takes 4 inputs each, second one takes 1 more
								add_mux_luts = 4;
							} else if (S > 13 && S <= 16) {
								// Five 4:1 mux, first four takes 4 inputs each, second one takes none
								add_mux_luts = 5;
							} else {
								// Exceeding limit of S < 16
								cout << "Mapper.cpp: S exceeds limit of 16." << endl;
							}
							// Make sure to account for width
							add_mux_luts *= (W*P);
						}
					}
						
					add_luts = add_decoder_luts + add_mux_luts;
					// If TrueDualPort mode we need to double additional LUTs
					if (logical_ram.getMode() == TrueDualPort) {
						add_luts *= 2;
					}
					
					// Area calculation
					area = ((double)S * (double)P * physical_ram.getArea()) + (ceil((double)add_luts / (double)architecture.getN()) * architecture.getLogicBlockArea());
					
					// Cost calculations
					// Based on what physical ram, how many has been used, how many to be used, we compute a cost 
					// If using this ram does not cause more LB to be needed, cost is just area
					cost = area;
					if ((total_physical_ram_used[physical_ram.getType()] + S*P) > available_physical_ram[physical_ram.getType()]) {
						// If using this ram causes more LB to be needed, add that to cost
						if (physical_ram.getType() == LUTRAM) {
							// physical_ram.getArea() here since LUTRAM blocks are more expensive
							cost += ((double)S*(double)P) * physical_ram.getArea();
							// physical_ram.getRatio() - 1 since a portion is accounted for above
							cost += ((double)S*(double)P) * (physical_ram_ratio[physical_ram.getType()] - 1) * architecture.getLogicBlockArea();
						} else {
							// Assuming all required BRAM blocks are going to require new LB/tiles
							// Assuming all new LBs are just normal ones to simplify calculation
							cost += ((double)S*(double)P) * physical_ram_ratio[physical_ram.getType()] * architecture.getLogicBlockArea();
						}
					}
					
					// To optimize mapping, we want to see if we can fit in another logical ram in this
					// if this is not LUTRAM and not already in TrueDualPort mode and not operating at the max width
					if (physical_ram.getType() != LUTRAM && logical_ram.getMode() != SimpleDualPort && logical_ram.getMode() != TrueDualPort && current_width != physical_ram.getMaxWidth()){
						
						for (unsigned int width_i = current_width; current_width >= 1; current_width /= 2) {
						
							// Look for other logical rams that has the same (current) width 
							vector<pair<unsigned int, unsigned int>> potential_matches = width_logical_ram_map[width_i];
							
							unsigned int remaining_depth = D*S - logical_ram_depth;
							unsigned int remaining_space = S*P*W*D - logical_ram_depth*logical_ram_width;
							for (auto potential_match: potential_matches) {
								// For each potential match, we check if the depth can fit
								// Note that potential_matches is in sorted order: descending depth size
								
								if (potential_match.first == logical_ram_id) {
									// Skip if found self
									continue;
								}
								
								// Not enough space/depth left, skip
								if (potential_match.second > remaining_depth) {
									continue;
								}
								if (width_i*potential_match.second > remaining_space) {
									continue;
								}
								
								// Assuming there's a possible match
								// Retrieve from circuit's logical_ram vector based on id
								if (circuit.getLogicalRams()[potential_match.first].getSolved()){
									// If solved, move on
									continue;
								}
								
								// At this point we have a logical ram that can share physical ram
								// We record this possible ram share in our map
								// Map is used for look up later after all widths are explored
								possible_ram_share[id] = potential_match.first;
								
								// Update cost value
								cost *= ((double)logical_ram_depth/((double)logical_ram_depth+(double)potential_match.second));
								
								// If choose to share, mode changes to either TrueDualPort or ROM
								mode = TrueDualPort;
								
								// Only can share ram with one other logical ram
								// Since potential_matches is already sorted, 
								// the first one we find is the largest possible depth that fits
								goto EndOfLoop;
							}
							// Note that if no potential match is found, we leave the loop and there is no difference compared
							// to the simple single ram mapping function above
						}
					}
					
					EndOfLoop:
					MappedRam new_mapped_ram(logical_ram_id, logical_ram_width, logical_ram_depth, id, S, P, type, W, D, mode, add_luts, area, cost);
					possible_sol_given_phy_ram.push_back(new_mapped_ram);
				} /* End for (auto current_width: physical_ram.getPossibleWidths()) */
				
				// At this point we have all possible solutions for current physical ram
				// Sort and take least area solution
				if (!possible_sol_given_phy_ram.empty()) {
					sort(possible_sol_given_phy_ram.begin(), possible_sol_given_phy_ram.end());
					possible_sol_across_phy_ram.push_back(possible_sol_given_phy_ram[0]);
				}
				
			} /* End for (auto physical_ram: architecture.getPhysicalRams()) */
			
			// At this point we have all best solutions for each type of physical ram
			// Sort and take smallest solution
			if (!possible_sol_across_phy_ram.empty()) {
				sort(possible_sol_across_phy_ram.begin(), possible_sol_across_phy_ram.end());
				
				// Check if chosen solution does any ram sharing
				if (possible_ram_share.find(possible_sol_across_phy_ram[0].getID()) != possible_ram_share.end()) {
					// If ram sharing
					unsigned int other_logical_ram_id = possible_ram_share[possible_sol_across_phy_ram[0].getID()];
					LogicalRam other_logical_ram = circuit.getLogicalRams()[other_logical_ram_id];
					
					MappedRam shared_mapped_ram(other_logical_ram_id, other_logical_ram.getWidth(), other_logical_ram.getDepth(), possible_sol_across_phy_ram[0].getID(), possible_sol_across_phy_ram[0].getS(), possible_sol_across_phy_ram[0].getP(), possible_sol_across_phy_ram[0].getType(), possible_sol_across_phy_ram[0].getW(), possible_sol_across_phy_ram[0].getD(), possible_sol_across_phy_ram[0].getMode(), possible_sol_across_phy_ram[0].getAddLuts(), possible_sol_across_phy_ram[0].getArea(), possible_sol_across_phy_ram[0].getArea()-possible_sol_across_phy_ram[0].getCost());
					circuit.addMappedRam(shared_mapped_ram);
							
					circuit.getLogicalRams()[other_logical_ram_id].setSolved();
				}
				
				circuit.addMappedRam(possible_sol_across_phy_ram[0]);
				logical_ram.setSolved();
				
				// Update total physical ram used for cost function
				P_RamType physical_ram_used_type = possible_sol_across_phy_ram[0].getType();
				total_physical_ram_used[physical_ram_used_type] += possible_sol_across_phy_ram[0].getS() * possible_sol_across_phy_ram[0].getP();
				
				unsigned int extra_physical_ram_required = total_physical_ram_used[physical_ram_used_type] - available_physical_ram[physical_ram_used_type];
				if (extra_physical_ram_required > 0) {
					// Reach here if solution's ram needs exceed currently available
					// Update available since more LBs are now available with this new ram requirement					
					unsigned int new_total_lb_count = (double)total_physical_ram_used[physical_ram_used_type] * physical_ram_ratio[physical_ram_used_type];
								
					for (auto physical_ram: architecture.getPhysicalRams()) {
						available_physical_ram[physical_ram.getType()] = floor(new_total_lb_count/physical_ram.getRatio());
					}
				}
			}
		} /* End for (auto logical_ram: circuit.getLogicalRams()) */
		
		// At this point we have all solutions for current circuit
		// Carrying on with next circuit ...
		
	} /* End for (auto circuit: circuits) */
}

void Mapper::computeCircuitAreas() {
	for (auto& i: circuits) {
		i.computeArea(architecture.getPhysicalRams(), architecture.getN(), architecture.getLogicBlockArea());
	}
}

// Print functions
void Mapper::printCircuits() {
	for (auto i: circuits) {
		i.printInfo_LogicalRam();
	}
}

void Mapper::printCircuitSolutions() {
	for (auto i: circuits) {
		i.printInfo_MappedRam();
	}
}

void Mapper::printCircuitWidthLogicalRamMap() {
	for (auto i: circuits) {
		i.printInfo_WidthLogicalRamMap();
	}
}
void Mapper::printCircuitAreas() {
	vector<double> data;
	for (auto i: circuits) {
		i.printInfo_Area();
		data.push_back(i.getArea());
	}
	cout << "Geomean: " << geomean(data) << endl;
}

void Mapper::printCircuits_raw() {
	for (auto i: circuits) {
		cout << i.getID() << " " << i.getLogicBlockNum() << endl;
	}
}

void Mapper::printLogicalRam_raw() {
	for (auto i: circuits) {
		for (auto j: i.getLogicalRams()) {
			cout << i.getID() << " " << j.getID() << " " << getLRAMMode_S(j.getMode()) << " " << j.getDepth() << " " << j.getWidth() << endl;
		}
	}
}

void Mapper::printCircuitSolutions_raw() {
	for (auto i: circuits) {
		// Assuming stuff comes out in order atm
		for (auto j: i.getMappedRams()) {
			cout << i.getID() << " ";
			cout << j.getLogicalRamID() << " ";
			cout << j.getAddLuts() << " ";
			cout << "LW " << j.getLogicalRamWidth() << " ";
			cout << "LD " << j.getLogicalRamDepth() << " ";
			cout << "ID " << j.getID() << " ";
			cout << "S " << j.getS() << " ";
			cout << "P " << j.getP() << " ";
			cout << "Type " << j.getType_output() << " ";
			cout << "Mode " << getLRAMMode_S(j.getMode()) << " ";
			cout << "W " << j.getW() << " ";
			cout << "D " << j.getD();
			cout << endl;
		}
	}
}
