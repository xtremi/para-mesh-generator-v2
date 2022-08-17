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

#define pmgptr std::shared_ptr

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
	pmgptr<pmg::Path> outer, inner;
	bool closedLoop = false;
};

class ExtrudedSurface {
private:
	BoundedSurface surface;
	pmgptr<pmg::Path> extrudePath;
};

class NodeIterator1D{};
class NodeIterator2D{};


class MeshDensity {};
class MeshDensity1D : public MeshDensity {
public:
	int x;
	NodeIterator1D getNodeIter();
};
class MeshDensity2D : public MeshDensity {
public:
	int x, y;
	NodeIterator1D getNodeIter(int edge);
};
class MeshDensity3D : public MeshDensity {
public:
	int x,y,z;
	NodeIterator2D getNodeIter(int face);
};



class Csys {};


class Mesh {
public:
	int		  firstNodeID;
	int		  firstElementID;
	pmg::Csys csys;
};

class Mesh1D : public Mesh {
public:
	pmg::MeshDensity1D	meshDensity;
};
class Mesh2D : public Mesh {
public:
	pmg::MeshDensity2D	meshDensity;
};
class Mesh3D : public Mesh {
public:
	pmg::MeshDensity3D	meshDensity;
};



class Mesher {
public:
	void setFEAwriter(pmgptr<MeshWriter> _writer);

	void write(Mesh1D& mesh);
	void write(Mesh2D& mesh);
	void write(Mesh3D& mesh);

	void write(const Mesh1D& mesh1, const Mesh1D mesh2);
	void write(const Mesh2D& mesh1, const Mesh2D mesh2, int edge1, int edge2);
	void write(const Mesh3D& mesh1, const Mesh3D mesh2, int face1, int face2);

private:
	pmgptr<MeshWriter> writer;
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
