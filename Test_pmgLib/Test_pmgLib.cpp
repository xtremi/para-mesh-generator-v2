#include "TestDef.h"
#include <vector>
#include <set>
#include "pmgPath.h"
#include "pmgMeshWriter.h"

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


class MeshDensity {
public:
	MeshDensity(bool isClosedLoop) : closedLoop{isClosedLoop}{}
	bool closedLoop = false;
};
class MeshDensity1D : public MeshDensity {
public:
	MeshDensity1D() : MeshDensity(false) {}
	MeshDensity1D(int nx, bool isClosedLoop = false) : MeshDensity(isClosedLoop) {
		x = nx; 
	}
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

static const pmg::MeshCsys globalCsys;
static const pmg::MeshTransformer noTransformer;

class Mesh {
public:
	int		  firstNodeID;
	int		  firstElementID;
	const pmg::MeshCsys* csys = &globalCsys;
	const pmg::MeshTransformer* transformer = &noTransformer;
};

class Mesh1D : public Mesh {
public:
	pmg::MeshDensity1D meshDensity;
	pmg::Path*		   path;
};
class Mesh2D : public Mesh {
public:
	pmg::MeshDensity2D meshDensity;
	pmg::Surface*	   surface;
};
class Mesh3D : public Mesh {
public:
	pmg::MeshDensity3D	  meshDensity;
	pmg::ExtrudedSurface* volume;
};


class Mesher {
public:
	void setFEAwriter(pmg::MeshWriter* _writer) {
		writer = _writer;
	}

	void write(Mesh1D& mesh);
	void write(Mesh2D& mesh);
	void write(Mesh3D& mesh);

	void write(const Mesh1D& mesh1, const Mesh1D mesh2);
	void write(const Mesh2D& mesh1, const Mesh2D mesh2, int edge1, int edge2);
	void write(const Mesh3D& mesh1, const Mesh3D mesh2, int face1, int face2);

private:
	MeshWriter* writer;
};


void Mesher::write(Mesh1D& mesh) {
	mesh.firstNodeID = writer->nextNodeID();
	mesh.firstElementID = writer->nextElementID();

	for (int i = 0; i < mesh.meshDensity.x; i++) {
		glm::dvec3 pos = mesh.path->positionI(i, mesh.meshDensity.x, mesh.meshDensity.closedLoop);
		writer->writeNode(pos, *mesh.csys, *mesh.transformer);
	}
}


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
	pmg::PathLinear pathLinear(glm::dvec3(10.));
	pmg::PathArc pathArc(10., glm::dvec3(1., 1., 0.), glm::dvec3(1.,0.,0.));
	
	mesh.meshDensity = pmg::MeshDensity1D(20, false);
	mesh.path = &pathLinear;

	pmg::Mesher mesher;
	mesher.setFEAwriter(&nasWriter);
	mesher.write(mesh);	//write a line
	mesher.write(mesh); //write same line
	mesh.path = &pathArc;
	mesh.meshDensity.closedLoop = true;
	mesher.write(mesh); //write circle (full arc)

	nasWriter.close();
	return 0;
}