#include "TestDef.h"
#include <vector>
#include <set>
#include "pmgPath.h"
#include "pmgMeshWriter.h"
#include "pmgMesher.h"

int test_pmgPath01(const std::string& filepath);
int test_pmgMeshWriter01(const std::string& filepath);
int test_pmgMesherPath01(const std::string& filepath);
int test_pmgMeshCsys01(const std::string& filepath);
int test_pmgMeshToMesh1D(const std::string& filepath);
int test_pmgSurface_pathToPath01(const std::string& filepath);


std::vector<TestDef> testFunctions({
	TestDef(1000, "test_pmgPath01", "pmgPath", (testFunction)test_pmgPath01),
	TestDef(1100, "test_pmgMeshWriter01", "pmgMeshWriter", (testFunction)test_pmgMeshWriter01),
	TestDef(1110, "test_pmgMeshToMesh1D", "pmgMeshWriter", (testFunction)test_pmgMeshToMesh1D),
	TestDef(1200, "test_pmgMesherPath01", "pmgMesherPath", (testFunction)test_pmgMesherPath01),
	TestDef(1250, "test_pmgMeshCsys01", "pmgMeshCsys", (testFunction)test_pmgMeshCsys01),
	TestDef(1300, "test_pmgSurface_pathToPath01", "pmgMesherSurface", (testFunction)test_pmgSurface_pathToPath01),
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
	
	mesh.meshDensity = pmg::MeshDensity1D(20, pmg::node_skip::none);
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

int test_pmgMeshToMesh1D(const std::string& filepath) {
	pmg::NastranWriter nasWriter(filepath);
	if (!nasWriter.open()) return 1;

	glm::dvec3 xdir(10., 0., 0.);
	pmg::PathLinear pathXdir(xdir);
	pmg::MeshCsys csys = pmg::MeshCsys();

	pmg::Mesh1D mesh1;
	mesh1.meshDensity = pmg::MeshDensity1D(20, pmg::node_skip::none);
	mesh1.path = &pathXdir;
	mesh1.csys = &csys;

	pmg::Mesher mesher;
	mesher.setFEAwriter(&nasWriter);
	mesher.write(mesh1);	//write a line

	pmg::Mesh1D mesh2 = mesh1;
	csys.pos.x += 11.;
	csys.update();
	mesher.write(mesh2); //write another line

	mesher.write(mesh1, mesh2); //write connecting element

	nasWriter.close();
	return 0;
}


int test_pmgMeshCsys01(const std::string& filepath) {
	pmg::NastranWriter nasWriter(filepath);
	if (!nasWriter.open()) return 1;
	
	glm::dvec3 xdir(10., 0., 0.);
	pmg::PathLinear pathXdir(xdir);
	pmg::MeshCsys csys = pmg::MeshCsys();

	pmg::Mesh1D mesh;
	mesh.meshDensity = pmg::MeshDensity1D(20, pmg::node_skip::none);
	mesh.path = &pathXdir;
	mesh.csys = &csys;

	pmg::Mesher mesher;
	mesher.setFEAwriter(&nasWriter);
	mesher.write(mesh);	//write a line

	csys.pos.x += 10.;
	csys.update();
	mesher.write(mesh); //write another line

	pmg::MeshCsys csys2 = pmg::MeshCsys(&csys, glm::dvec3(10., 0., 0.), pmg::makeRotatationMatrix(glm::dvec3(0., 1., 0.), 0.2));
	mesh.csys = &csys2;
	mesher.write(mesh);

	pmg::MeshCsys csys3 = pmg::MeshCsys(&csys2, glm::dvec3(10., 2., 0.), pmg::makeRotatationMatrix(glm::dvec3(0., 1., 0.), 0.2));
	mesh.csys = &csys3;
	mesher.write(mesh);

	nasWriter.close();
	return 0;
}



int test_pmgSurface_pathToPath01(const std::string& filepath) {
	pmg::NastranWriter nasWriter(filepath);
	if (!nasWriter.open()) return 1;

	pmg::Mesher mesher;
	mesher.setFEAwriter(&nasWriter);

	pmg::MeshCsys csys = pmg::MeshCsys();
	pmg::Mesh2D mesh;
	mesh.meshDensity = pmg::MeshDensity2D(20, 10);
	mesh.csys = &csys;
	

	//A bounded surface between two paths (linearly interpolated between paths)
	pmg::BoundedSurface surface;
	surface.inner = std::make_shared<pmg::PathLinear>(glm::dvec3(10., 0., 0.));
	surface.outer = std::make_shared<pmg::PathLinear>(glm::dvec3(10., 5., 0.));
	surface.outerTranslation = glm::dvec3(0., 0., 5.);;
	
	mesh.surface = &surface;
	mesher.write(mesh);	//write a surface
	
	//Another surface, but modifying one of the paths to a Sine curve:
	surface.outer = std::make_shared<pmg::PathSine>(pmg::X_DIR, pmg::Z_DIR, 10., 0.5, 2.5);
	csys.pos.y += 5.0;
	csys.update();
	mesher.write(mesh);	//write a surface

	//3 more similar surfaces but increasing the mesh density:
	for(int i = 0; i < 3; i++){
		mesh.meshDensity.x *= 2;
		csys.pos.y += 2.0;
		csys.update();
		mesher.write(mesh);	//write a surface
	}

	//Another bounded surface with closed loop - changing paths to circles so this becomes a cone:
	surface.inner = std::make_shared<pmg::PathArc>(2.5, pmg::Z_DIR, pmg::X_DIR);
	surface.outer = std::make_shared<pmg::PathArc>(1., pmg::Z_DIR, pmg::X_DIR);
	mesh.meshDensity.closedLoop = true;
	mesh.meshDensity.x = 10;
	mesh.meshDensity.y = 5;
	mesh.meshDensity.nodeSkipX = pmg::node_skip::every_2;
	csys.pos.y += 5.0;
	csys.update();
	mesher.write(mesh);	//write a surface

	//Changing the surface type to Mathematical surface (here SinCosWavesSurface):
	pmg::SinCosWavesSurface sinCosSurface1(0.1, 0.2, 1., 1.5, 10., 10.);
	mesh.surface = &sinCosSurface1;
	mesh.meshDensity.x = 40;
	mesh.meshDensity.y = 40;
	mesh.meshDensity.closedLoop = false;

	csys.pos.y += 10.0;
	csys.update();
	mesher.write(mesh);	//write a surface

	//Another SinCosWaveSurface but larger:
	pmg::SinCosWavesSurface sinCosSurface2(2.0, 12., 10., 60., 80., 80.);
	mesh.surface = &sinCosSurface2;
	mesh.meshDensity.x = 100;
	mesh.meshDensity.y = 100;
	mesh.meshDensity.closedLoop = false;

	csys.pos = glm::dvec3(0., 0., -20.);
	csys.update();
	mesher.write(mesh);	//write a surface


	nasWriter.close();
	return 0;
}
