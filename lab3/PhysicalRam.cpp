#include <cmath>

#include "PhysicalRam.h"

using namespace std;

PhysicalRam::PhysicalRam(unsigned int max_bits_n, unsigned int max_depth_n, unsigned int max_width_n, P_RamType type_n, double ratio_n) {
	max_bits = max_bits_n;
	max_depth = max_depth_n;
	max_width = max_width_n;
	type = type_n;
	ratio = ratio_n;
	
	if (type_n == LUTRAM) {
		area = 40000;
	} else {
		area = 9000 + (5 * max_bits_n) + (90 * sqrt(max_bits_n)) + (600 * 2 * max_width_n);
	}
}

// Get functions

// Set functions
void PhysicalRam::addPossibleWidth(unsigned int width_n) {
	possible_widths.push_back(width_n);
}

void PhysicalRam::addToWidthToDepths(unsigned int depth_n, unsigned int width_n) {
	width_to_depths[width_n] = depth_n;
}