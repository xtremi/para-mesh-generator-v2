#include "pmgUtilities.h"
using namespace pmg;

int pmg::stdVecSum(const VecI& vec) {
	int sum = 0;
	for (int el : vec) sum += el;
	return sum;
}