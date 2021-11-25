#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <vector>

#include "LogicalRam.h"
#include "PhysicalRam.h"

string getLRAMMode_S(L_RamMode mode);
string getPRAMType_S(P_RamType type);

double geomean(vector<double>& data);

#endif /* HELPER_H_ */