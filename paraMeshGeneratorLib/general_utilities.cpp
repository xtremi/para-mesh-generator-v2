#include "general_utilities.h"


//template <typename T>
int stdVecSum(const std::vector<int>& vec) {
	int sum = 0;
	for (int el : vec) sum += el;
	return sum;
}