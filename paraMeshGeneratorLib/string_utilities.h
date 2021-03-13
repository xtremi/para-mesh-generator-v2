#pragma once
#include <string>
#include <vector>

std::string vecToString(const std::vector<int>& vec);

bool isReal(const std::string& str);
double toReal(const std::string& str, double defaultValue = 0.0);

std::string arrayToString(int* arr, int size);