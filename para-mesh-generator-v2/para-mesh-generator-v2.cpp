#include "paraMeshGeneratorDll.h"


int main(int argc, char* argv[]) {

	int32_t res = test1(1, 2);
	if (res != 3) return 1;
	res = test2(1, 2);
	if (res != -1) return 1;

	return 0;
}