#include "general_utilities.h"


//template <typename T>
int stdVecSum(const VecI& vec) {
	int sum = 0;
	for (int el : vec) sum += el;
	return sum;
}