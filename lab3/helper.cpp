#include <cmath>

#include "helper.h"

string getLRAMMode_S(L_RamMode mode) {
	switch (mode) {
		case ROM:
			return "ROM";
		case SinglePort:
			return "SinglePort";
		case SimpleDualPort:
			return "SimpleDualPort";
		case TrueDualPort:
			return "TrueDualPort";
	}
	return "";
}

string getPRAMType_S(P_RamType type) {
	switch (type) {
		case LUTRAM:
			return "LUTRAM";
		case BRAM8192:
			return "BRAM8192";
		case BRAM128K:
			return "BRAM128K";
		case BRAM_CUSTOM_1:
			return "BRAM_CUSTOM_1";
		case BRAM_CUSTOM_2:
			return "BRAM_CUSTOM_2";
	}
	return "";
}

double geomean(vector<double>& data) {
	double sum = 0.0;
	double size = data.size();
	for (auto i: data) {
		sum += log(i);
	}
	sum /= size;
	return exp(sum);
}