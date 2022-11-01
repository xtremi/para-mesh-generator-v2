#include "TestDef.h"
#include <vector>
#include <set>
#include "pmgPath.h"
#include "pmgMeshWriter.h"
#include "pmgMesher.h"

int test_pmgPath01(const std::string& filepath);
int test_pmgMeshWriter01(const std::string& filepath);
int test_pmgMesherPath01(const std::string& filepath);


std::vector<TestDef> testFunctions({
	TestDef(1000, "test_pmgPath01", "pmgPath", (testFunction)test_pmgPath01),
	TestDef(1100, "test_pmgMeshWriter01", "pmgMeshWriter", (testFunction)test_pmgMeshWriter01),
	TestDef(1200, "test_pmgMesherPath01", "pmgMesherPath", (testFunction)test_pmgMesherPath01),
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



int test_pmgPath01(const std::string& filepath) {

	pmg::PathSine sinePath(glm::dvec3(1., 1., 0.), glm::dvec3(0., 0.2, 1.), 5., 2., 2.5);
	std::ofstream file;
	file.open(filepath);
	if (!file.is_open()) return 1;

	int N = 40;
	for (int i = 0; i < N; i++) {
		glm::dvec3 p = sinePath.positionI(i, N);
		file << p.x << ", " << p.y << ", " << p.z << std::endl;
	}
	file.close();
	return 0;
}



int test_pmgMeshWriter01(const std::string& filepath) {

	pmg::NastranWriter nasWriter(filepath);
	if (!nasWriter.open()) return 1;
	pmg::MeshWriter* writer = &nasWriter;

	writer->writeNode(glm::dvec3(1., 0., 0.), pmg::globalCsys, pmg::noTransformer);
	writer->writeNode(glm::dvec3(1., 0., 1.12341123e-2), pmg::globalCsys, pmg::noTransformer);
	writer->close();
	return 0;
}

int test_pmgMesherPath01(const std::string& filepath) {
	pmg::NastranWriter nasWriter(filepath);
	if (!nasWriter.open()) return 1;

	pmg::Mesh1D mesh;
	glm::dvec3 normal(10., 5., 2.);
	glm::dvec3 xdir(10., 0., 0.);
	pmg::PathLinear pathNormal(normal);
	pmg::PathLinear pathXdir(xdir);
	pmg::PathArc pathArc(10., normal, xdir);
	
	mesh.meshDensity = pmg::MeshDensity1D(20, false);
	mesh.path = &pathNormal;

	pmg::Mesher mesher;
	mesher.setFEAwriter(&nasWriter);
	mesher.write(mesh);	//write a line
	mesh.path = &pathXdir;
	mesher.write(mesh); //write another line
	mesh.path = &pathArc;
	mesh.meshDensity.closedLoop = true;
	mesher.write(mesh); //write circle (full arc)

	nasWriter.close();
	return 0;
}