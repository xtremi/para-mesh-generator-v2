#include "TestDef.h"
#include <vector>
#include <set>
#include "pmgPath.h"

int test_pmgPath01(const std::string& filepath);


std::vector<TestDef> testFunctions({
	TestDef(10, "test_pmgPath01", "pmgPath", (testFunction)test_pmgPath01),
});


int main(int argc, char* argv[]) {

	std::set<int> testsToRun;
	int testSmallerThan = -1;
	int testLargerThan = -1;	

	if (argc > 2 && !strcmp(argv[1], "-only")) {
		for (int i = 2; i < argc; i++) {
			if (argv[i][0] == '<') {
				testSmallerThan = std::stoi(argv[i] + 1);
			}
			else if (argv[i][0] == '>') {
				testLargerThan = std::stoi(argv[i] + 1);
			}
			else {
				testsToRun.insert(std::stoi(argv[i]));
			}
			
		}
	}
	
	for (TestDef& testDef : testFunctions) {

		bool runTest = false;
		if (testsToRun.find(testDef.id) != testsToRun.end()){
			runTest = true;
		}
		else if(testLargerThan > -1 || testSmallerThan > -1){
			if ((testDef.id >= testLargerThan || testLargerThan == -1) && (testDef.id <= testSmallerThan || testSmallerThan == -1)) {
				runTest = true;
			}
		}
		else if (testsToRun.empty() && testSmallerThan == -1 && testLargerThan == -1){
			runTest = true;
		}

		if (runTest)
		{
			testDef.run();
		}
	}
	return 0;
}

namespace pmg{
class Surface {
private:
};

/*!
	Defines a surface either:
	1) Between two non-closed paths
		- Each end points of the paths are then connected by straight lines
	2) Between two closed loops
*/
class BoundedSurface : public Surface {
private:
	std::shared_ptr<pmg::Path> outer, inner;
	bool closedLoop = false;
};

class ExtrudedSurface {
private:
	BoundedSurface surface;
	std::shared_ptr<pmg::Path> extrudePath;
};

class MeshDensity1D{
public:
	int x;
};
class MeshDensity2D : public MeshDensity1D {
public:
	int y;
};
class MeshDensity3D : public MeshDensity2D {
public:
	int z;
};

class FEAwriter {};
class NastranWriter : public FEAwriter {};

class Csys {};

class Mesher {
public:
	void write(const pmg::Path&    path, const pmg::MeshDensity1D& meshDensity, const pmg::Csys& csys);
	void write(const pmg::Surface& path, const pmg::MeshDensity2D& meshDensity, const pmg::Csys& csys);
	void write(const pmg::Path&    path, const pmg::MeshDensity3D& meshDensity, const pmg::Csys& csys);
};

}

int test_pmgPath01(const std::string& filepath) {

	pmg::PathSine sinePath(glm::dvec3(1., 1., 0.), glm::dvec3(0., 0.2, 1.), 5., 2., 2.5);
	std::shared_ptr<pmg::Path> sinePathP = std::shared_ptr<pmg::Path>(&sinePath);

	int N = 40;
	for (int i = 0; i < N; i++) {
		glm::dvec3 p = sinePathP->position(i, N);
		std::cout << p.x << ", " << p.y << ", " << p.z << std::endl;
	}

	


	return 1;
}
