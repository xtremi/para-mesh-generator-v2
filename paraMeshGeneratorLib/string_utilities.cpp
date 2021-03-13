#include "string_utilities.h"

bool isReal(const std::string& str) {
	try { 
		std::stod(str); 
		return true;
	}
	catch (...) {
		return false;
	}
}

double toReal(const std::string& str, double defaultValue) {
	double r = defaultValue;
	try {
		r = std::stod(str);
	}
	catch(...){}
	return r;
}


std::string vecToString(const std::vector<int>& vec) {
	std::string str;
	for (int i : vec) str += (std::to_string(i) + ", ");
	str.pop_back(); str.pop_back();
	return str;
}

std::string arrayToString(int* arr, int size) {
	std::string str;
	for (int i = 0; i < size; i++)
		str += (std::to_string(arr[i]) + ", ");
	return str;
}