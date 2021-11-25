#include <iostream>
#include <string>

#include "LogicalRam.h"
#include "helper.h"

using namespace std;

// Get functions

// Print functions
void LogicalRam::printInfo() {
	cout << "ID: " << id;
	cout << ", Depth: " << depth;
	cout << ", Width: " << width;
	cout << ", Mode: " << getLRAMMode_S(mode);
	cout << endl; 
}