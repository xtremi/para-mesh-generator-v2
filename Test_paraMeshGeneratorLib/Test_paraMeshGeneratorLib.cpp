#include "TestDef.h"
//#include "Mesh.h"
#include <vector>
#include <set>

//1d elements
#include "LineMesher.h"
#include "ArcMesher.h"
#include "RecEdgeMesher.h"
#include "EllipseMesher.h"

//2d elements
#include "PlaneMesher.h"
#include "ConeMesher.h"
#include "DiskMesher.h"
#include "CylinderMesher.h"
#include "RecToEllipseMesher.h"
#include "RecTubeMesher.h"

//3d elements
#include "CuboidMesher.h"
#include "Cone3Dmesher.h"
#include "RecToEllipse3Dmesher.h"

//Mesh extruder and meshes : to be fixed (2021.08.14)
#include "Mesh.h"

#include "math_utilities.h"


int speedTestAddition(const std::string& fileName);
int speedTestMultiplication(const std::string& fileName);
int speedTestVec3addition(const std::string& fileName);
int speedTestMat3Vec3multiplication(const std::string& fileName);
int speedTestWriteCubes(const std::string& fileName);
int speedTestWriteRotatedCubes(const std::string& fileName);
int speedTestWriteNodesAndElements(const std::string& fileName);

int nodeIterator1D(const std::string& fileName);
int nodeIterator1Dm(const std::string& fileName);
int nodeIterator2D(const std::string& fileName);
int nodeIterator2D_4(const std::string& fileName);
int nodeIterator2Dm(const std::string& fileName);
int meshEdgeExtrusion(const std::string& fileName);
int meshFaceExtrusion(const std::string& fileName);

int lineMesher(const std::string& fileName);
int arcMesher(const std::string& fileName);
int recEdgeMesher(const std::string& fileName);
int ellipseMesher(const std::string& fileName);

int meshCsys1(const std::string& fileName);
int meshCsys2(const std::string& fileName);
int meshCsys3(const std::string& fileName);

int planeMesher(const std::string& fileName);
int planeMesherRef(const std::string& fileName);
int coneMesher(const std::string& fileName);
int coneMesherRef(const std::string& fileName);
int diskMesher(const std::string& fileName);
int diskMesherRef(const std::string& fileName);
int cylinderMesher(const std::string& fileName);
int cylinderMesherRef(const std::string& fileName);
int recToEllipseMesher(const std::string& fileName);
int recToEllipseMesherRef(const std::string& fileName);
int recTubeMesher(const std::string& fileName);
int recTubeMesherRef(const std::string& fileName);

int cuboidMesher(const std::string& fileName);
int cuboidMesherRef(const std::string& fileName);
int cone3Dmesher(const std::string& fileName);
int cone3DmesherRef(const std::string& fileName);
int recToEllipse3Dmesher(const std::string& fileName);


int refinement2dHeight(const std::string& fileName);
int refinement3dHeight(const std::string& fileName);
int refinementCone2dHeight(const std::string& fileName);
int refinementCone3dHeight(const std::string& fileName);


int extrude2DedgeLine(const std::string& fileName);
int extrude2DedgeArc(const std::string& filename);
int extrude2DedgeArcAndLine(const std::string& filename);
int extrude3DfaceLine(const std::string& fileName);
int extrude3DfaceArc(const std::string& fileName);
int extrude3DfaceArcAndLine(const std::string& fileName);
#ifdef TO_FIX
int extrude2DarcMulti(const std::string& filename);
#endif

void writeDebugBeamElements(FEAwriter* w, int firstNode, int lastNode);

std::vector<TestDef> testFunctions({
#ifndef SKIP
	TestDef(10, "speedTestAddition",				"speed test", (testFunction)speedTestAddition),
	TestDef(11, "speedTestMultiplication",			"speed test", (testFunction)speedTestMultiplication),
	TestDef(12, "speedTestVec3addition",			"speed test", (testFunction)speedTestVec3addition),
	TestDef(13, "speedTestMat3Vec3multiplication",	"speed test", (testFunction)speedTestMat3Vec3multiplication),
	TestDef(14, "speedTestWriteCubes",				"speed test", (testFunction)speedTestWriteCubes),
	TestDef(15, "speedTestWriteRotatedCubes",		"speed test", (testFunction)speedTestWriteRotatedCubes),
	TestDef(16, "speedTestWriteNodesAndElements",	"speed test", (testFunction)speedTestWriteNodesAndElements),
	
	TestDef(30, "nodeIterator1D",			"utilities", (testFunction)nodeIterator1D),
	TestDef(31, "nodeIterator1Dm",			"utilities", (testFunction)nodeIterator1Dm),
	TestDef(32, "nodeIterator2D",			"utilities", (testFunction)nodeIterator2D),	
	TestDef(33, "nodeIterator2Dm",			"utilities", (testFunction)nodeIterator2Dm),
	TestDef(34, "nodeIterator2D_4",			"utilities", (testFunction)nodeIterator2D_4),
	TestDef(40, "meshEdgeExtrusion",	"utilities", (testFunction)meshEdgeExtrusion),
	TestDef(50, "meshFaceExtrusion",	"utilities", (testFunction)meshFaceExtrusion),
	

	TestDef(101, "lineMesher",			"basic meshers 1D", (testFunction)lineMesher),	
	TestDef(102, "arcMesher",			"basic meshers 1D", (testFunction)arcMesher),
	TestDef(103, "recEdgeMesher",		"basic meshers 1D", (testFunction)recEdgeMesher),
	TestDef(104, "ellipseMesher",		"basic meshers 1D", (testFunction)ellipseMesher),

	TestDef(150, "meshCsys1",			"transformations",  (testFunction)meshCsys1),
	TestDef(151, "meshCsys2",			"transformations",  (testFunction)meshCsys2),
	TestDef(152, "meshCsys3",			"transformations",  (testFunction)meshCsys3),

	TestDef(203, "planeMesher",			"basic meshers 2D", (testFunction)planeMesher),
	TestDef(204, "planeMesherRef",		"basic meshers 2D", (testFunction)planeMesherRef),
	TestDef(205, "coneMesher",			"basic meshers 2D", (testFunction)coneMesher),
	TestDef(206, "coneMesherRef",		"basic meshers 2D", (testFunction)coneMesherRef),
	TestDef(207, "diskMesher",			"basic meshers 2D", (testFunction)diskMesher),
	TestDef(208, "diskMesherRef",		"basic meshers 2D", (testFunction)diskMesherRef),
	TestDef(209, "cylinderMesher",		"basic meshers 2D", (testFunction)cylinderMesher),
	TestDef(210, "cylinderMesherRef",	"basic meshers 2D", (testFunction)cylinderMesherRef),
#endif
	TestDef(211, "recToEllipseMesher",		"basic meshers 2D", (testFunction)recToEllipseMesher),
	TestDef(212, "recToEllipseMesherRef",	"basic meshers 2D", (testFunction)recToEllipseMesherRef),
	TestDef(213, "recTubeMesher",			"basic meshers 2D", (testFunction)recTubeMesher),
	TestDef(214, "recTubeMesherRef",		"basic meshers 2D", (testFunction)recTubeMesherRef),
	
	TestDef(320, "cuboidMesher",	    "basic meshers 3D", (testFunction)cuboidMesher),
	TestDef(321, "cuboidMesherRef",	    "basic meshers 3D", (testFunction)cuboidMesherRef),
	TestDef(322, "cone3Dmesher",	    "basic meshers 3D", (testFunction)cone3Dmesher),	
	TestDef(323, "cone3DmesherRef",	    "basic meshers 3D", (testFunction)cone3DmesherRef),
	TestDef(324, "recToEllipse3Dmesher","basic meshers 3D", (testFunction)recToEllipse3Dmesher),

	TestDef(430, "refinement2dHeight",		"basic meshers 2D", (testFunction)refinement2dHeight),
	TestDef(431, "refinement3dHeight",		"basic meshers 3D", (testFunction)refinement3dHeight),
	TestDef(432, "refinementCone2dHeight",	"basic meshers 2D", (testFunction)refinementCone2dHeight),
	TestDef(433, "refinementCone3dHeight",	"basic meshers 3D", (testFunction)refinementCone3dHeight),

	TestDef(500, "extrude2DedgeLine",		"extrusion", (testFunction)extrude2DedgeLine),
	TestDef(520, "extrude2DedgeArc",		"extrusion", (testFunction)extrude2DedgeArc),
	TestDef(540, "extrude2DedgeArcAndLine",		"extrusion", (testFunction)extrude2DedgeArcAndLine),

	TestDef(600, "extrude3DfaceLine",		"extrusion", (testFunction)extrude3DfaceLine),
	TestDef(620, "extrude3DfaceArc",		"extrusion", (testFunction)extrude3DfaceArc),
	TestDef(640, "extrude3DfaceArcAndLine",		"extrusion", (testFunction)extrude3DfaceArcAndLine),

#ifdef TO_FIX
	TestDef(590, "extrude2DarcMulti",	"extrusion", (testFunction)extrude2DarcMulti),
#endif

	
});


int main(int argc, char* argv[]) {

	std::set<int> testsToRun;
	if (argc > 2 && !strcmp(argv[1], "-only")) {
		for (int i = 2; i < argc; i++) {
			testsToRun.insert(std::stoi(argv[i]));
		}
	}

	for (TestDef& testDef : testFunctions) {
		if (testsToRun.empty() || (testsToRun.find(testDef.id) != testsToRun.end())) {
			testDef.run();
		}
	}
	return 0;
}
 
/*
	Tests should be defined in some class and these functions should
	be defined there.
*/
/*************************************************************************/
#define TEST_START std::ofstream file; \
				   file.open(fileName); \
				   if (!file.is_open()) return 1; \
				   NastranFEAwriter writer(&file); \
				   Mesher::setFEAwriter(&writer); \
				   MeshCsys pos(glm::dvec3(0.0));

#define TEST_START2 std::ofstream file; \
				   file.open(fileName); \
				   if (!file.is_open()) return 1; \
				   NastranFEAwriter writer(&file); \
				   Mesher::setFEAwriter(&writer); \
				   MeshCsys glCsys(glm::dvec3(0.0)); \
				   glm::dvec3 pos(NULL_POS);

#define TEST_END writer.close(); \
				 return 0;
/*************************************************************************/
#ifdef _DEBUG
#define SPEED_TEST_AMOUNT 1e5
#else
#define SPEED_TEST_AMOUNT 1e8
#endif

int speedTestAddition(const std::string& fileName) {
	double numbers[10] = { 1, 2., 232.,2342., 1231., 434., 345345., 234., 454., 4. };

	int index = 0;
	double res = 0.;
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = numbers[index] + numbers[index + 1];
		index++;
	}
	return 0;
}
int speedTestMultiplication(const std::string& fileName) {
	double numbers[10] = { 1, 2., 232.,2342., 1231., 434., 345345., 234., 454., 4. };

	int index = 0;
	double res = 0.;
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = numbers[index] * numbers[index + 1];
		index++;
	}
	return 0;
}
int speedTestVec3addition(const std::string& fileName) {
	glm::dvec3 vecs[10] = { 
		glm::dvec3(223.,311.,345.), glm::dvec3(24.,3343.,523423.), glm::dvec3(2334.,2343.,576.), 
		glm::dvec3(2133.,3.,5434.), glm::dvec3(2342.,3776.,56.),	glm::dvec3(2.,3.,5.),
		glm::dvec3(25.,38.,58888.),	glm::dvec3(1112.,322.,511.), glm::dvec3(22342.,376.,5777.), 
		glm::dvec3(23.,33.,35.)
	};

	int index = 0;
	glm::dvec3 res(0.0);
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = vecs[index] + vecs[index + 1];
		index++;
	}
	return 0;
}
int speedTestMat3Vec3multiplication(const std::string& fileName) {
	glm::dvec3 vecs[10] = {
		glm::dvec3(223.,311.,345.), glm::dvec3(24.,3343.,523423.), glm::dvec3(2334.,2343.,576.),
		glm::dvec3(2133.,3.,5434.), glm::dvec3(2342.,3776.,56.),	glm::dvec3(2.,3.,5.),
		glm::dvec3(25.,38.,58888.),	glm::dvec3(1112.,322.,511.), glm::dvec3(22342.,376.,5777.),
		glm::dvec3(23.,33.,35.)
	};
	glm::dmat3x3 mats[10] = {
		makeCsysMatrix(vecs[1], GLM2PI*0.1), makeCsysMatrix(vecs[7], GLM2PI*0.1),
		makeCsysMatrix(vecs[0], GLM2PI*3.1), makeCsysMatrix(vecs[8], GLM2PI*2.1),
		makeCsysMatrix(vecs[4], GLM2PI*56.1), makeCsysMatrix(vecs[3], GLM2PI*0.21),
		makeCsysMatrix(vecs[2], GLM2PI*2.1), makeCsysMatrix(vecs[5], GLM2PI*0.11),
		makeCsysMatrix(vecs[0], GLM2PI*1.1), makeCsysMatrix(vecs[9], GLM2PI*0.91)
	};

	int index = 0;
	glm::dvec3 res(0.0);
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = mats[index + 1] * vecs[index];
		index++;
	}
	return 0;

}

void writeXYZlines(MeshCsys& csys, double length, int nNodes) {
	LineMesher::writeNodesLineX(NULL_POS, csys, nNodes, length);
	LineMesher::writeElementsLine(nNodes);
	LineMesher::writeNodesLineY(NULL_POS, csys, nNodes, length);
	LineMesher::writeElementsLine(nNodes);
	LineMesher::writeNodesLineZ(NULL_POS, csys, nNodes, length);
	LineMesher::writeElementsLine(nNodes);
}

void writeXYZplanes(MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	PlaneMesher::writeNodesXY(NULL_POS, csys, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesXZ(NULL_POS, csys, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodes(NULL_POS, csys, meshDens, size, plane::yz);
	PlaneMesher::writeElements(meshDens);
}

static const int nCubesSpeedTest = 100;
static const int nNodesCubeSpeedTest = 50;
int speedTestWriteCubes(const std::string& fileName) {
	TEST_START

	int n = nNodesCubeSpeedTest;

	for (int i = 0; i < nCubesSpeedTest; i++) {
		CuboidMesher::writeNodes(NULL_POS, pos, MeshDensity3D(n,n,n), glm::dvec3(0.5, 0.5, 2.0), plane::xy);
		CuboidMesher::writeElements(MeshDensity3D(n, n, n));
	}

	TEST_END
}
int speedTestWriteRotatedCubes(const std::string& fileName) {
	TEST_START

	double R = 5.;
	glm::dvec3 v(1., 1., 1.);
	v = glm::normalize(v);

	glm::dmat3x3 rotMF2 = makeCsysMatrix(v, 0.);

	MeshCsys csysF0(10.*Z_DIR);
	MeshCsys csysF1(R*Z_DIR, &rotMF2);
	MeshCsys csysF2(-R * Z_DIR);
	csysF1.setParentCsys(&csysF0);
	csysF2.setParentCsys(&csysF1);

	writeXYZlines(csysF0, 2.0, 5);
	writeXYZlines(csysF1, 2.0, 5);
	writeXYZlines(csysF2, 2.0, 5);

	int n = nNodesCubeSpeedTest;

	for (int i = 0; i < nCubesSpeedTest; i++) {
		double ang = GLMPI * (double)i / 20.;
		rotMF2 = makeCsysMatrix(v, ang);
		csysF1.pos += 1.45*v;
		csysF2.updateParents();

		CuboidMesher::writeNodes(NULL_POS, pos, MeshDensity3D(n, n, n), glm::dvec3(0.5, 0.5, 2.0), plane::xy);
		CuboidMesher::writeElements(MeshDensity3D(n, n, n));
	}

	TEST_END
}

int speedTestWriteNodesAndElements(const std::string& fileName) {
	TEST_START
	int nNodes = nNodesCubeSpeedTest * nNodesCubeSpeedTest * nNodesCubeSpeedTest * nCubesSpeedTest;
	int nElements = (nNodesCubeSpeedTest -1) * (nNodesCubeSpeedTest - 1)* (nNodesCubeSpeedTest - 1)* nCubesSpeedTest;
	
	for(int i = 0; i < nNodes; i++){
		writer.writeNode(i + 1, glm::dvec3(12.23, 23.2, 6.));
	}
	int nodeIDs[4] = { 1,2,3,4 };
	for (int i = 0; i < nElements; i++) {
		writer.write4nodedShell(i + 1, nodeIDs);
	}
	TEST_END
}

int meshCsys1(const std::string& fileName) {
	TEST_START
	MeshCsys globalCsys;
	MeshCsys csysa1(4.0*X_DIR);
	MeshCsys csysa2(4.0*Y_DIR);
	MeshCsys csysa3(4.0*Z_DIR);

	csysa1.setParentCsys(&globalCsys);	
	csysa2.setParentCsys(&csysa1);
	csysa3.setParentCsys(&csysa2);
	//csys in (4.0,4.0,4.0)
	writeXYZlines(csysa3, 4.0, 11);

	glm::dmat3x3 rotMb = makeCsysMatrix(Z_DIR, GLMPI / 4.0);
	MeshCsys csysb1(NULL_POS, &rotMb);
	csysb1.setParentCsys(&csysa1);
	//csys in (4.0,0.0,0.0) rotated 45deg along Z-axis
	writeXYZlines(csysb1, 2.0, 5);

	MeshCsys csysb2(4.0*X_DIR);
	csysb2.setParentCsys(&csysb1);
	//csys in (4.0,0.0,0.0) + 2.0 in X-dir in 45deg rotated csys
	writeXYZlines(csysb2, 2.0, 5);

	MeshCsys csysb3(4.0*Y_DIR);
	csysb3.setParentCsys(&csysb1);
	//csys in (4.0,0.0,0.0) + 2.0 in Y-dir in 45deg rotated csys
	writeXYZlines(csysb3, 2.0, 5);

	glm::dmat3x3 rotMc = makeCsysMatrix(Z_DIR, GLMPI / 4.0);
	MeshCsys csysc1(NULL_POS, &rotMc);
	//10 x cys 
	for (int i = 0; i < 10; i++) {
		rotMc = makeCsysMatrix(glm::dvec3(1.0, 1.0, 0.0), GLMPI * (double)i/10.0);
		csysc1.pos.x = 3.0 * (double)i / 10.0;
		csysc1.setParentCsys(&csysb3);
		writeXYZlines(csysc1, 1.0, 8);
	}

	MeshCsys csysD1;
	writeXYZplanes(csysD1, MeshDensity2D(6,6), glm::dvec2(1.0, 2.0));
	glm::dmat3x3 rotMd2 = makeCsysMatrix(Z_DIR, -GLMPI / 2.0);
	MeshCsys csysD2(3.1 * X_DIR, &rotMd2);
	writeXYZplanes(csysD2, MeshDensity2D(6, 6), glm::dvec2(1.0, 2.0));

	int nnodes = 64;
	MeshCsys csysE1(-10.*Y_DIR);
	ArcMesher::writeNodesCircular(NULL_POS, csysE1, nnodes, 4.0, ArcAngles(), direction::z);
	ArcMesher::writeElementsLine(nnodes, true);

	glm::dmat3x3 rotME2;
	MeshCsys csysE2(10.0*X_DIR, &rotME2);
	MeshCsys csysE3(-10.0*X_DIR);
	csysE2.setParentCsys(&csysE1);
	csysE3.setParentCsys(&csysE2);

	for (int i = 0; i < 50; i++) {
		double ang = GLMPI * (double)i / 50.;
		rotME2 = makeCsysMatrix(Y_DIR, ang);
		csysE1.pos.y += 0.25;
		csysE3.updateParents();
		ArcMesher::writeNodesCircular(NULL_POS, csysE3, nnodes, 4.0, ArcAngles(), direction::z);
		ArcMesher::writeElementsLine(nnodes, true);

		DiskMesher::writeNodes(NULL_POS, csysE3, MeshDensity2D(24, 6), Cone2Dradius(1.0, 3.25), ArcAngles(), direction::z);
		DiskMesher::writeElements(MeshDensity2D(24, 6, true));

	}

	double R = 5.;
	glm::dvec3 v(1., 1., 1.);
	v = glm::normalize(v);

	glm::dmat3x3 rotMF2 = makeCsysMatrix(v, 0.);

	MeshCsys csysF0(10.*Z_DIR);
	MeshCsys csysF1(R*Z_DIR, &rotMF2);
	MeshCsys csysF2(-R*Z_DIR);
	csysF1.setParentCsys(&csysF0);
	csysF2.setParentCsys(&csysF1);
	
	writeXYZlines(csysF0, 2.0, 5);
	writeXYZlines(csysF1, 2.0, 5);
	writeXYZlines(csysF2, 2.0, 5);


	for (int i = 0; i < 40; i++) {
		double ang = GLMPI * (double)i / 20.;
		rotMF2 = makeCsysMatrix(v, ang);
		csysF1.pos += 1.45*v;
		csysF2.updateParents();

		writeXYZlines(csysF1, 2.0, 5);
		writeXYZlines(csysF2, 2.0, 5);

		CuboidMesher::writeNodes(NULL_POS, csysF2, MeshDensity3D(4, 4, 4), glm::dvec3(0.5, 0.5, 2.0), plane::xy);
		CuboidMesher::writeElements(MeshDensity3D(4, 4, 4));
	}


	TEST_END
}

int meshCsys2(const std::string& fileName) {
	TEST_START

	std::vector<glm::dvec3> dirs({
		glm::normalize(glm::dvec3(-1,-1,0)),
		glm::normalize(glm::dvec3(1,-1,0)),
		glm::normalize(glm::dvec3(1,1,0)),
		glm::normalize(glm::dvec3(-1,1,0))
	});


	MeshDensity2D meshDens2D(20, 5);
	MeshDensity3Dref meshDens3Dcuboid(2, 8 * 3 + 1, 8 * 3 + 1);
	Cone2Dradius radius(1.0, 0.2);

	MeshCsys glcsys;
	CuboidMesherRef::writeNodes(NULL_POS, glcsys, meshDens3Dcuboid, glm::dvec3(1.5,1.5,3.0), false, plane::xy);
	CuboidMesherRef::writeElements(meshDens3Dcuboid);

	std::vector<glm::dmat3x3> rotMs(4);

	for (int i = 0; i < 4; i++) {

		MeshCsys csys0(4.0 * dirs[i]);
		MeshCsys csys1(&csys0, 20.*dirs[i], &rotMs[i]);
		MeshCsys csys2(&csys1, -20.*dirs[i]);
		
		MeshCsys csys2b(&csys2, 3.*Z_DIR);
		MeshCsys csys2c(&csys2b, 3.*Z_DIR);

		for (int j = 0; j < 8; j++) {

			rotMs[i] = makeCsysMatrix(glm::cross(dirs[i], Z_DIR), GLM2PI * (double)j / 10.);
			csys2c.updateParents();

			ConeMesher::writeNodes(NULL_POS, csys2, meshDens2D, radius, ArcAngles(), 3.0, direction::z);
			ConeMesher::writeElements(meshDens2D);

			CylinderMesher::writeNodes(NULL_POS, csys2b, meshDens2D, 0.2, ArcAngles(), 2.0, direction::z);
			CylinderMesher::writeElements(meshDens2D);

			CuboidMesherRef::writeNodes(NULL_POS, csys2c, meshDens3Dcuboid, glm::dvec3(1.5, 1.5, 3.0), false, plane::xy);
			CuboidMesherRef::writeElements(meshDens3Dcuboid);
		}
	}

	TEST_END
}

int meshCsys3(const std::string& fileName) {
	TEST_START

	
	glm::dmat3x3 rotM0;
	glm::dmat3x3 rotM1;
	MeshCsys csysGlobal;
	MeshCsys csys0(&csysGlobal, glm::dvec3(0.), &rotM0);
	MeshCsys csys1(&csys0, glm::dvec3(5.,.0,.0));
	MeshCsys csys2(&csys1, glm::dvec3(0.), &rotM1);

	MeshDensity2D meshDens(5, 5);
	PlaneMesher::writeNodesQ(NULL_POS, csysGlobal, meshDens, glm::dvec2(.2), plane::xy);
	PlaneMesher::writeElements(meshDens);


	for(int i = 0; i < 10; i++){

		rotM0 = makeCsysMatrix(glm::dvec3(0.0, 0.0, 1.0), GLMPI * (double)i / 10.);

		for(int j = 0; j < 10; j++){
			rotM1 = makeCsysMatrix(glm::dvec3(1.0, 0.0, 0.0), GLMPI * (double)j/10.);
			csys2.updateParents();
			PlaneMesher::writeNodesQ(glm::dvec3((double)i / 1., 0.0, 0.0), csys2, meshDens, glm::dvec2(.2), plane::xy);
			PlaneMesher::writeElements(meshDens);
		}

	}

	TEST_END
}

int lineMesher(const std::string& fileName) {
	TEST_START2

	int		nnodes = 20;
	double  length = 10.0;

	LineMesher::writeNodesLine(pos, glCsys, nnodes, length, direction::x);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLine(pos, glCsys, nnodes, length, direction::y);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLine(pos, glCsys, nnodes, length, direction::z);
	LineMesher::writeElementsLine(nnodes);

	pos.x += 1.2*length;
	LineMesher::writeNodesLineX(pos, glCsys, nnodes, length);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineY(pos, glCsys, nnodes, length);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineZ(pos, glCsys, nnodes, length);
	LineMesher::writeElementsLine(nnodes);
	
	nnodes = 10;	
	glm::dvec3 dsvec = glm::dvec3(1.0, 0.35, 0.15);

	pos.y += 1.2*length;
	LineMesher::writeNodesLineQ(pos, glCsys, nnodes, dsvec);
	LineMesher::writeElementsLine(nnodes);
	
	double ds = 0.746;

	pos.x = 0.0;
	pos.y += 1.2*length;
	LineMesher::writeNodesLineXq(pos, glCsys, nnodes, ds);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineYq(pos, glCsys, nnodes, ds);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineZq(pos, glCsys, nnodes, ds);
	LineMesher::writeElementsLine(nnodes);

	
	pos.y += 1.2*length;
	double rad1 = 10.0, rad2 = 15.0;
	double ang = 0.0;
	int nLines = 8;
	double dang = GLM2PI / (double)nLines;
	glm::dvec3 posStart, posEnd;
	
	for (int i = 0; i < 8; i++) {
		posStart = coordsOnCircle(ang, rad1, direction::z) + pos;
		posEnd = coordsOnCircle(ang, rad2, direction::z) + pos + glm::dvec3(0, 0, 1.0);
		
		LineMesher::writeNodesLine(posStart, glCsys, nnodes, posEnd);
		LineMesher::writeElementsLine(nnodes);
		ang += dang;
	}


	TEST_END
}


int arcMesher(const std::string& fileName) {
	TEST_START2

	int nnodes = 11;
	double radius = 5.0;

	ArcMesher::writeNodesCircular(pos, glCsys, nnodes, radius, ArcAngles(0.0, GLMPI), direction::x);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircular(pos, glCsys, nnodes, radius, ArcAngles(GLMPI, 0.0), direction::y);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircular(pos, glCsys, nnodes, radius, ArcAngles(), direction::z);
	ArcMesher::writeElementsLine(nnodes, true);

	pos.x += radius*2.5;
	ArcMesher::writeNodesCircularX(pos, glCsys, nnodes, radius, ArcAngles(0.0, 3.0*GLM2PI/4.0));
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularY(pos, glCsys, nnodes, radius, ArcAngles(0.0, GLM2PI/3.0));
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularZ(pos, glCsys, nnodes, radius, ArcAngles(0.0, GLM2PI*0.95));
	ArcMesher::writeElementsLine(nnodes);

	pos.x += radius * 2.5;
	double dang = 0.7*GLM2PI / nnodes;
	ArcMesher::writeNodesCircularQ(pos, glCsys, nnodes, radius, 0.0, dang, direction::x);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularQ(pos, glCsys, nnodes, radius, 0.0, -dang, direction::y);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularQ(pos, glCsys, nnodes, radius, GLMPI, 0.6*dang, direction::z);
	ArcMesher::writeElementsLine(nnodes);

	pos.x = 0.0;
	pos.y += radius * 2.5;
	ArcMesher::writeNodesCircularXq(pos, glCsys, nnodes, radius, GLMPI/2.0, dang);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularYq(pos, glCsys, nnodes, radius, GLMPI, dang);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularZq(pos, glCsys, nnodes, radius, GLMPI, -dang);
	ArcMesher::writeElementsLine(nnodes);


	pos.x += radius * 2.5;
	ArcMesher::writeNodesCircular_nth(pos, glCsys, nnodes, radius, ArcAngles(GLMPI, GLM2PI), 5, direction::x);
	ArcMesher::writeElementsLine(nnodes - 2-1);

	pos.x += radius * 2.5;
	ArcMesher::writeNodesCircular_nth(pos, glCsys, nnodes, radius, ArcAngles(GLMPI, GLM2PI), 3, direction::x);
	ArcMesher::writeElementsLine(nnodes - 3-1);

	pos.x += radius * 2.5;
	ArcMesher::writeNodesCircular_nth(pos, glCsys, nnodes, radius, ArcAngles(GLMPI, GLM2PI), 7, direction::x);
	ArcMesher::writeElementsLine(nnodes - 1-1);
	
	TEST_END
}

int recEdgeMesher(const std::string& fileName) {
	TEST_START2

	int nnodes = 14;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(5.0, 5.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);
	
	pos.z += 2.0;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(5.0, 10.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.z += 2.0;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.z += 2.0;
	nnodes = 18;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);
	
	pos.z += 2.0;
	nnodes = 20;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.z += 2.0;
	nnodes = 22;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.z += 2.0;
	nnodes = 4;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.x = 20.0;
	pos.z = 0.0;
	nnodes = 48;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(10.0, 7.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.z = 0.1;
	pos.y -= 3.5;	
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(10.0, 7.0), plane::xz);
	RecEdgeMesher::writeElements(nnodes);

	pos.y += 7.0;
	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(10.0, 7.0), plane::xz);
	RecEdgeMesher::writeElements(nnodes);

	RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(7.0, 20.0), plane::yz);
	RecEdgeMesher::writeElements(nnodes);
	
	
	
	pos = glm::dvec3(0.0);
	pos.y += 40.0;
	double elSize = 0.0;
	double recWidthStart = 10.0;
	double recWidth		 = recWidthStart;
	for(int i = 2; i < 6; i++){		
		
		nnodes = i*4*8;
		elSize = recWidth / (double)(nnodes / 4);
		
		//L1/L2/L3 | L1/L2/L3 | L1/L2/L3 | 

		int nRefs = 3;
		for (int refs = 0; refs < nRefs; refs++) {
			//L1
			RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
			RecEdgeMesher::writeElements(nnodes);
			nnodes += 8;				//corner nodes
			recWidth += 1.8*elSize;		//increase rec size

			//L2
			RecEdgeMesher::writeNodes_nth(pos, glCsys, nnodes, glm::dvec2(recWidth, recWidth), 4, plane::xy);
			RecEdgeMesher::writeElements(3 * nnodes / 4);
			nnodes += 8;								//corner nodes
			recWidth += 1.8*elSize;						//increase rec size
			nnodes /= 2;								//fewer elements			
			elSize = recWidth / (double)(nnodes / 4);	//larger elements

			//L3
			RecEdgeMesher::writeNodes(pos, glCsys, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
			RecEdgeMesher::writeElements(nnodes);
			nnodes += 8;								//corner nodes
			recWidth += 1.8*elSize;						//increase rec size
			//elSize = recWidth / (double)(nnodes / 4);

		}
		recWidth = recWidthStart;
		pos.x += 40.0;
	}

	nnodes = 58;
	std::vector<glm::dvec2> xycoords(nnodes);
	RecEdgeMesher::getLocalCoords(xycoords, nnodes, glm::dvec2(60.0, 35.0));
	LineMesher::writeNodes(xycoords, 10.0);
	LineMesher::writeElementsLine(nnodes, true);

	TEST_END
}

int ellipseMesher(const std::string& fileName) {
	TEST_START2
	EllipseMesher::writeNodes(pos, glCsys, 24, EllipseRadius(10.0, 5.0), ArcAngles(), direction::z);
	EllipseMesher::writeElements(24, true);
	EllipseMesher::writeNodes(pos, glCsys, 24, EllipseRadius(10.0, 5.0), ArcAngles(), direction::x);
	EllipseMesher::writeElements(24, true);
	EllipseMesher::writeNodes(pos, glCsys, 24, EllipseRadius(10.0, 5.0), ArcAngles(), direction::y);
	EllipseMesher::writeElements(24, true);

	pos.x = 50.0;
	int nEllipse = 10;
	for(int i = 0; i < nEllipse; i++){
		EllipseMesher::writeNodes(pos, glCsys, 98, EllipseRadius(1.0 + 15.0*(double)i/(double)nEllipse, 5.0), ArcAngles(), direction::y);
		EllipseMesher::writeElements(98, true);
		pos.y += 1.0;
	}

	pos.x = 100.0;
	std::vector<glm::dvec2> localCoords(1500);
	EllipseMesher::getLocalCoords(localCoords, 1500, EllipseRadius(100.0, 50.0), ArcAngles());
	LineMesher::writeNodes(localCoords, 4.0);
	LineMesher::writeElementsLine(1500, true);

	TEST_END
}

int planeMesher(const std::string& fileName) {
	TEST_START2

	MeshDensity2D meshDens(10, 12);
	glm::dvec2 size(20.0, 17.0);
	
	PlaneMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodes(pos, glCsys, meshDens, size, plane::xz);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodes(pos, glCsys, meshDens, size, plane::yz);
	PlaneMesher::writeElements(meshDens);
	
	pos.x += 1.2*size.x;
	PlaneMesher::writeNodesXY(pos, glCsys, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesXZ(pos, glCsys, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesYZ(pos, glCsys, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	

	pos.x += 1.2*size.x;
	glm::dvec2 elsize(size.x / (double)meshDens.nElDir1(), size.y / (double)meshDens.nElDir2());
	PlaneMesher::writeNodesQ(pos, glCsys, meshDens, elsize, plane::xy);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesQ(pos, glCsys, meshDens, elsize, plane::xz);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesQ(pos, glCsys, meshDens, elsize, plane::yz);
	PlaneMesher::writeElements(meshDens);
	
	pos.x += 1.2*size.x;
	PlaneMesher::writeNodesXYq(pos, glCsys, meshDens, elsize);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesXZq(pos, glCsys, meshDens, elsize);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesYZq(pos, glCsys, meshDens, elsize);
	PlaneMesher::writeElements(meshDens);
	TEST_END
}


int planeMesherRef(const std::string& fileName) {
	TEST_START2

	glm::dvec2 size(20.0, 17.0);
	int nRef = 3;
	int nNodesDir2 = std::pow(2, nRef + 1) + 1;
	MeshDensity2Dref meshDens(nRef, nNodesDir2, false);

	PlaneMesherRef::writeNodes(pos, glCsys, meshDens, size, true, plane::xy);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodes(pos, glCsys, meshDens, size, true, plane::xz);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodes(pos, glCsys, meshDens, size, true, plane::yz);
	PlaneMesherRef::writeElements(meshDens);

	pos.x += 1.2*size.x;

	nRef = 5;
	meshDens.setNrefs(nRef);
	meshDens.setDir2(std::pow(2, nRef + 1) + 1);
	size = glm::dvec2(15.0, 7.5);
	PlaneMesherRef::writeNodesXY(pos, glCsys, meshDens, size, false);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodesXZ(pos, glCsys, meshDens, size, false);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodesYZ(pos, glCsys, meshDens, size, false);
	PlaneMesherRef::writeElements(meshDens);

	TEST_END
}


int coneMesher(const std::string& fileName) {
	TEST_START2

	double height = 4.0;
	MeshDensity2D meshDens(10, 13); 
	Cone2Dradius radius(4.0, 2.0);
	ArcAngles angle(0.0, GLMPI);

	//Half cone:	
	ConeMesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::x);
	ConeMesher::writeElements(meshDens);
	
	//Almost full cone:
	pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLM2PI);
	ConeMesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::y);
	ConeMesher::writeElements(meshDens);
		
	//Full cone:
	pos.x += 2.1*radius.start();
	angle.setFullCircle();
	meshDens.closedLoop = true;
	ConeMesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::z);
	ConeMesher::writeElements(meshDens);

	
	height = 0.0;	
	//Almost full disk:
	pos.y += 2.1*radius.start();
	pos.x = 0.0;
	meshDens.nodes().x *= 2;
	meshDens.nodes().y /= 2;
	angle.setEnd(0.99*GLM2PI);
	ConeMesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::z);
	ConeMesher::writeElements(meshDens);

	//Full disk:
	pos.x += 2.1*radius.start();
	angle.setFullCircle();
	meshDens.setClosedLoop();
	ConeMesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::x);
	ConeMesher::writeElements(meshDens);

	pos.x += 2.1*radius.start();
	height = radius.start();
	radius.setStart(radius.end());	
	//Full Cylinder:
	ConeMesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::y);
	ConeMesher::writeElements(meshDens);
		
	//Half cylinder:
	pos.x += 2.5*radius.start();
	angle.setStart(GLMPI);
	angle.setEnd(GLM2PI);
	meshDens.setClosedLoop(false);
	ConeMesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::z);
	ConeMesher::writeElements(meshDens);

	TEST_END
}


int coneMesherRef(const std::string& fileName) {
	TEST_START2
			
	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;	
	MeshDensity2Dref meshDens(nRef, nNodesEdge, false);
	double height = 4.0;
	ArcAngles angle(0.0, GLMPI / 2.0);
	Cone2Dradius radius(4.0, 2.0);
	double radiusStart = 4.0;
	double radiusEnd = 2.0;

	//Quarter cone:	
	ConeMesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::x);
	ConeMesherRef::writeElements(meshDens);
	
	//Almost full cone:
	pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLM2PI);
	ConeMesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::y);
	ConeMesherRef::writeElements(meshDens);
	
	//Full cone:
	pos.x += 2.1*radius.start();
	angle.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge - 1);
	ConeMesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Full disk:
	pos.y += 2.1*radius.start();
	pos.x = 0.0;
	height = 0.0;
	ConeMesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);

	//Almost full disk:
	pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLM2PI);
	meshDens.setClosedLoop(false);
	meshDens.setCirc(nNodesEdge);
	ConeMesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Full Cylinder:
	pos.y += 2.1*radius.start();
	pos.x = 0.0;
	angle.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge - 1);
	radius.setEnd(radius.start());
	height = 10.0;
	ConeMesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Half cylinder:
	pos.x = 2.1*radius.start();
	angle.setEnd(GLMPI);
	meshDens.setClosedLoop(false);
	meshDens.setCirc(nNodesEdge);
	height = 6.0;
	ConeMesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);

	TEST_END

}

int diskMesher(const std::string& fileName){
	TEST_START2
	MeshDensity2D meshDens(13,6);
	Disk2Dradius radius(2.0, 4.0);
	ArcAngles angles(0.0, 0.99*GLM2PI);

	//Almost full disk:
	DiskMesher::writeNodes(pos, glCsys, meshDens, radius, angles, direction::x);
	DiskMesher::writeElements(meshDens);

	pos.x += 2.1*radius.outer();
	//Half disk:
	angles.setEnd(GLMPI);
	DiskMesher::writeNodesX(pos, glCsys, meshDens, radius, angles);
	DiskMesher::writeElements(meshDens);

	
	pos.x += 2.1*radius.outer();
	//Quarter disk:
	angles.setEnd(GLMPI/2.0);
	DiskMesher::writeNodesY(pos, glCsys, meshDens, radius, angles);
	DiskMesher::writeElements(meshDens);

	pos.x += 2.1*radius.outer();
	//Full disk:
	angles.setFullCircle();
	meshDens.setClosedLoop();
	DiskMesher::writeNodesZ(pos, glCsys, meshDens, radius, angles);
	DiskMesher::writeElements(meshDens);

	TEST_END
}


int diskMesherRef(const std::string& fileName) {
	TEST_START2
	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;

	MeshDensity2Dref meshDens(nRef, nNodesEdge, false);
	Disk2Dradius radius(2.0, 4.0);
	ArcAngles angles(0.0, 0.99*GLM2PI);

	//Almost full disk:
	DiskMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, false, direction::x);
	DiskMesherRef::writeElements(meshDens);

	pos.x += radius.outer() * 2.1;
	//Half disk:
	angles.setEnd(GLMPI);
	DiskMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, false, direction::y);
	DiskMesherRef::writeElements(meshDens);

	pos.x += radius.outer() * 2.1;
	//Half quarter disk:
	angles.setEnd(GLMPI/4.0);
	DiskMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, false, direction::z);
	DiskMesherRef::writeElements(meshDens);

	pos.x = 0.0;
	pos.y += radius.outer() * 2.1;
	//Full disk:
	nNodesEdge = std::pow(2, nRef + 5);
	angles.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge);
	DiskMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, false, direction::z);
	DiskMesherRef::writeElements(meshDens);

	pos.x += radius.outer() * 2.1;
	radius.setOuter(radius.inner() / 2.0);
	//Full disk, ref outwards:
	DiskMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, false, direction::z);
	DiskMesherRef::writeElements(meshDens);

	TEST_END
}


int cylinderMesher(const std::string& fileName)
{
	TEST_START2

	MeshDensity2D meshDens(13, 6);
	ArcAngles angles(0.0, 0.99*GLM2PI);
	double radius = 2.0;
	double height = 8.0;

	//Almost full cylinder:
	CylinderMesher::writeNodes(pos, glCsys, meshDens, radius, angles, height, direction::x);
	CylinderMesher::writeElements(meshDens);

	pos.x += height * 2.1;
	//Quarter cylinder:
	angles.setEnd(GLMPI / 2.0);
	CylinderMesher::writeNodesX(pos, glCsys, meshDens, radius, angles, height);
	CylinderMesher::writeElements(meshDens);

	pos.x += height * 2.1;
	angles.setEnd(GLMPI / 4.0);
	//Half quarter cylinder;
	CylinderMesher::writeNodesY(pos, glCsys, meshDens, radius, angles, height);
	CylinderMesher::writeElements(meshDens);

	pos.x += height * 2.1;
	//Full cylinder:
	meshDens.setClosedLoop();
	angles.setFullCircle();
	CylinderMesher::writeNodesY(pos, glCsys, meshDens, radius, angles, height);
	CylinderMesher::writeElements(meshDens);

	TEST_END
}


int cylinderMesherRef(const std::string& fileName) 
{
	TEST_START2

	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;
	MeshDensity2Dref meshDens(nRef, nNodesEdge, false);
	ArcAngles angles(0.0, 0.99*GLM2PI);
	double radius = 2.0;
	double height = 8.0;

	//Almost full cylinder:
	CylinderMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, height, false, direction::x);
	CylinderMesherRef::writeElements(meshDens);

	pos.x += height * 1.1;
	//Quarter cylinder:
	angles.setEnd(GLMPI / 2.0);
	CylinderMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, height, false, direction::y);
	CylinderMesherRef::writeElements(meshDens);

	pos.x += height * 1.1;
	//Half quarter cylinder:
	angles.setEnd(GLMPI / 4.0);
	CylinderMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, height, false, direction::z);
	CylinderMesherRef::writeElements(meshDens);

	
	pos.x += height * 1.1;
	//Full cylinder:
	nNodesEdge = std::pow(2, nRef + 5);
	meshDens.setCirc(nNodesEdge);
	meshDens.setClosedLoop();
	angles.setFullCircle();
	CylinderMesherRef::writeNodes(pos, glCsys, meshDens, radius, angles, height, false, direction::z);
	CylinderMesherRef::writeElements(meshDens);

	TEST_END
}

int recToEllipseMesher(const std::string& fileName) {
	TEST_START2

	MeshDensity2D meshDens(128, 18, true);
	
	//Cone circle shape, square inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(36., 36.), glm::dvec2(18., 18.), ArcAngles(), 10.0, direction::x);
	RecToEllipseMesher::writeElements(meshDens);
	
	pos.x += 85.0;
	//Cone ellipse shape, square inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(36., 18.), glm::dvec2(18., 9.), ArcAngles(), 10.0, direction::y);
	RecToEllipseMesher::writeElements(meshDens);

	pos.x += 85.0;
	//Circle shape, square inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(36., 36.), glm::dvec2(18., 18.), ArcAngles(), 0., direction::y);
	RecToEllipseMesher::writeElements(meshDens);

	pos.x += 85.0;
	//Ellipse shape, square inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(36., 18.), glm::dvec2(18., 9.), ArcAngles(), 0., direction::x);
	RecToEllipseMesher::writeElements(meshDens);

	pos.x += 85.0;
	//Circle shape, square outside + ref disk inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(18., 18.), glm::dvec2(72., 72.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(meshDens);
	DiskMesherRef::writeNodes(pos, glCsys, MeshDensity2Dref(3, 128, true), Disk2Dradius(17.8, 9.), ArcAngles(), false, direction::z);
	DiskMesherRef::writeElements(MeshDensity2Dref(3, 128, true));

	pos.y += 85.0;
	//Circle shape, square outside + ref disk inside + Circle shape, square inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, MeshDensity2D(32, 8, true), EllipseRadius(18., 18.), glm::dvec2(72., 72.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(MeshDensity2D(32, 8, true));
	DiskMesherRef::writeNodes(pos, glCsys, MeshDensity2Dref(2, 128, true), Disk2Dradius(9., 17.8), ArcAngles(), false, direction::z);
	DiskMesherRef::writeElements(MeshDensity2Dref(2, 128, true));
	RecToEllipseMesher::writeNodes(pos, glCsys, MeshDensity2D(128, 12, true), EllipseRadius(8.9, 8.9), glm::dvec2(2., 2.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(MeshDensity2D(128, 12, true));
	pos.y -= 85.0;
	

	pos.x += 85.0;
	//Ellipse shape, square outside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(18., 9.), glm::dvec2(72., 36.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(meshDens);

	pos.x += 64.0;
	//Cone ellipse shape, square inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(24.0, 5.0), glm::dvec2(24.0, 5.0), ArcAngles(), 10.0, direction::z);
	RecToEllipseMesher::writeElements(meshDens);

	pos.x += 64.0;
	//Cone ellipse shape, square inside:
	RecToEllipseMesher::writeNodes(pos, glCsys, meshDens, EllipseRadius(32.0, 32.0), glm::dvec2(32.0, 32.0), ArcAngles(), 10.0, direction::z);
	RecToEllipseMesher::writeElements(meshDens);
	TEST_END
}

/*
	
	Not working correctly yet
	TODO

*/
int recToEllipseMesherRef (const std::string& fileName) {
	TEST_START2

	MeshDensity2Dref meshDens(5, 64, true);
	
	RecToEllipseMesherRef::writeNodes(pos, glCsys, meshDens, EllipseRadius(36., 36.), glm::dvec2(18., 18.), ArcAngles(), 10.0, false, direction::z);
	//RecToEllipseMesherRef::writeElements(meshDens);
	writeDebugBeamElements(&writer, 1, writer.getNextNodeID());

	TEST_END
}

int recTubeMesher(const std::string& fileName) {
	TEST_START2

	//RecTubes with increasing nodes per layer, for rec structure:

	//Quad tube, type1 (1):
	int nNodesEdge = 8;
	int nLayers = 4;
	glm::dvec2 sizeInner(10.0, 10.0);
	RecTubeSize size(sizeInner);
	size.setOuterSize(nNodesEdge, nLayers);
	MeshDensity2DrecTube meshDens(nNodesEdge * 4, nLayers, sizeInner);
	RecTubeMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	RecTubeMesher::writeElements(meshDens);

	
	pos.x += 25.0;
	//Quad tube, type1 (2):
	nNodesEdge = 16;
	nLayers    = 6;
	size.setOuterSize(nNodesEdge, nLayers);
	meshDens.setNodesInner(nNodesEdge * 4, size.inner);
	meshDens.setNodesLayer(nLayers);	
	RecTubeMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	RecTubeMesher::writeElements(meshDens);

	
	pos.x += 25.0;		
	//Quad tube, type1 (3):
	size.inner = glm::dvec2(4.0);
	nNodesEdge = 16;
	nLayers = 22;
	size.setOuterSize(nNodesEdge, nLayers);
	meshDens.setNodesInner(nNodesEdge * 4, size.inner);
	meshDens.setNodesLayer(nLayers);	
	RecTubeMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	RecTubeMesher::writeElements(meshDens);

	pos.x += 25.0;
	//Quad tube, type1 (4):
	size.inner = glm::dvec2(5.0);
	nNodesEdge = 4;
	nLayers = 8;
	size.setOuterSize(nNodesEdge, nLayers);
	meshDens.setNodesInner(nNodesEdge * 4, size.inner);
	meshDens.setNodesLayer(nLayers);
	RecTubeMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	RecTubeMesher::writeElements(meshDens);

	pos.x = 0.0;
	pos.y += 25.0;
	//Rectangle tube, type1 (1):
	size.inner = glm::dvec2(5.0, 2.5);	
	meshDens.setNodesInner(64, size.inner);
	nLayers = 8;	
	size.setOuterSize(meshDens.nNodesWidth(0), meshDens.nNodesHeight(0), nLayers);	
	RecTubeMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	RecTubeMesher::writeElements(meshDens);

	pos.x += 25.0;
	//Rectangle tube, type1 (2):
	size.inner = glm::dvec2(5.0,  2.5);
	size.outer = glm::dvec2(8.23, 7.1);
	meshDens.setNodesInner(40, size.inner);
	nLayers = 8;
	RecTubeMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	RecTubeMesher::writeElements(meshDens);


	//RecTubes with same node density per layer:
	
	pos.x = 0.0;
	pos.y += 25.0;
	//Rectangle tube, type2 (1):
	nLayers = 5;
	size.inner = glm::dvec2(5.0, 5.0);
	size.outer = 2.*size.inner;
	meshDens.setNodesInner(24, size.inner);
	meshDens.setNorm(nLayers);
	RecTubeMesher::writeNodes2(pos, glCsys, meshDens, size, 0.0, plane::xy);
	RecTubeMesher::writeElements2(meshDens);

	pos.x += 25.0;
	//Rectangle tube, type2 (2):
	nLayers = 14;
	meshDens.setNodesInner(40, size.inner);
	meshDens.setNorm(nLayers);
	size.inner = glm::dvec2(10.0, 6.2);
	size.outer = 2.*size.inner;
	RecTubeMesher::writeNodes2(pos, glCsys, meshDens, size, 0.0, plane::xy);
	RecTubeMesher::writeElements2(meshDens);

	pos.x += 25.0;
	//Rectangle tube, type2 - with height (1):
	RecTubeMesher::writeNodes2(pos, glCsys, meshDens, size, 10.0, plane::xy);
	RecTubeMesher::writeElements2(meshDens);

	pos.x += 25.0;
	//Rectangle tube, type2 - with height (2):
	size.inner = glm::dvec2(10.0, 3.2);
	size.outer = size.inner;
	RecTubeMesher::writeNodes2(pos, glCsys, meshDens, size, 10.0, plane::xy);
	RecTubeMesher::writeElements2(meshDens);

	
	
	pos.x = 0.0;
	pos.y += 25.0;
	//Mix: RecTube type2 as outer | RecTube type1 as middle | RecTube type2 as inner
	nLayers = 4;
	nNodesEdge = 8;
	size.inner = glm::dvec2(10.0);
	size.setOuterSize(nNodesEdge, nLayers);
	meshDens.setNodesInner(nNodesEdge * 4, size.inner);
	meshDens.setNodesLayer(nLayers);
	RecTubeMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	RecTubeMesher::writeElements(meshDens);

	//Outer QUAD
	size.inner = size.outer;
	size.outer = 2.*size.inner;
	meshDens.setNodesInner(meshDens.nNodePerimeter(3), size.inner);
	meshDens.setNorm(8);
	RecTubeMesher::writeNodes2(pos, glCsys, meshDens, size, 0.0, plane::xy);
	RecTubeMesher::writeElements2(meshDens);
	
	//Inner QUAD
	size.outer = glm::dvec2(10.0);
	size.inner = size.outer*0.75;
	meshDens.setNodesInner(meshDens.nNodePerimeter(0), size.inner);
	meshDens.setNorm(3);	
	RecTubeMesher::writeNodes2(pos, glCsys, meshDens, size, 0.0, plane::xy);
	RecTubeMesher::writeElements2(meshDens);

	TEST_END
}

int recTubeMesherRef(const std::string& fileName) {
	TEST_START2

	int nNodesEdge = 8 * 3;
	int nNodesEdge2 = 8 * 4;
	int nnodesInner = nNodesEdge * 4;
	int nRefs = 3;
	glm::dvec2 sizeInner(10.0, 10.0);
	glm::dvec2 sizeOuter(15.0, 15.0);	

	//Rectangle tube, type1 (1):
	RecTubeMesherRef::writeNodes(pos, glCsys, glm::ivec2(nNodesEdge, nNodesEdge), nRefs, sizeInner, plane::xy);
	RecTubeMesherRef::writeElements1(glm::ivec2(nNodesEdge, nNodesEdge), nRefs);
	
	pos.x += 25.0;
	//Rectangle tube, type1 (2):
	RecTubeMesherRef::writeNodes(pos, glCsys, glm::ivec2(nNodesEdge2, nNodesEdge), 2, glm::dvec2(8.0, 5.0), plane::xy);
	RecTubeMesherRef::writeElements1(glm::ivec2(nNodesEdge2, nNodesEdge), 2);
	
	pos.y += 25.0;
	pos.x = 0.;
	//Rectangle tube, type2 (1):
	nnodesInner = nNodesEdge2 * 4;
	MeshDensity2Dref meshDens(nRefs, nnodesInner, true);
	RecTubeMesherRef::writeNodes2(pos, glCsys, meshDens, RecTubeSize(sizeInner, sizeOuter), 0.0, plane::xy);
	RecTubeMesherRef::writeElements2(meshDens);
	
	pos.x += 25.0;
	//Rectangle tube, type2 - with height (1):
	RecTubeMesherRef::writeNodes2(pos, glCsys, meshDens, RecTubeSize(sizeInner, sizeOuter), 10.0, plane::xy);
	RecTubeMesherRef::writeElements2(meshDens);

	pos.x += 25.0;
	//Rectangle tube, type2 - with height (2):
	RecTubeMesherRef::writeNodes2(pos, glCsys, meshDens, RecTubeSize(sizeInner, sizeInner), 10.0, plane::xy);
	RecTubeMesherRef::writeElements2(meshDens);


	TEST_END
}

int cuboidMesher(const std::string& fileName) {
	TEST_START2

	MeshDensity3D meshDens(5, 5, 10);
	glm::dvec3 size(20.0, 30.0, 100.0);

	//Cuboid 1:
	CuboidMesher::writeNodes(pos, glCsys, meshDens, size, plane::xy);
	CuboidMesher::writeElements(meshDens);

	pos.x += 1.1*size.x;
	//Cuboid 2:
	CuboidMesher::writeNodes(pos, glCsys, meshDens, size, plane::xz);
	CuboidMesher::writeElements(meshDens);

	pos.x += 1.1*size.x;
	//Cuboid 3:
	CuboidMesher::writeNodes(pos, glCsys, meshDens, size, plane::yz);
	CuboidMesher::writeElements(meshDens);
	
	pos.x = 0.0;
	pos.y += 1.1*size.y;
	//Cuboid 4:
	CuboidMesher::writeNodesXYZ(pos, glCsys, meshDens, size);
	CuboidMesher::writeElements(meshDens);

	TEST_END
}


int cuboidMesherRef(const std::string& fileName) {
	TEST_START2		

	int nRef = 2;
	
	//Cuboid 1:
	int nNodesX0 = std::pow(2, nRef + 1) + 1;	
	int nNodesY0 = nNodesX0;
	MeshDensity3Dref meshDens(nRef, nNodesX0, nNodesY0);
	glm::dvec3 size(20.0, 30.0, 50.0);

	CuboidMesherRef::writeNodes(pos, glCsys, meshDens, size, false, plane::xy);
	CuboidMesherRef::writeElements(meshDens);

	pos.x += 1.2*size.x;
	//Cuboid 2:
	nRef = 4;
	meshDens.setDir1(std::pow(2, nRef + 1) + 1);
	meshDens.setDir2(std::pow(2, nRef + 2) + 1);
	CuboidMesherRef::writeNodes(pos, glCsys, meshDens, size, false, plane::xy);
	CuboidMesherRef::writeElements(meshDens);

	TEST_END
}

int cone3Dmesher(const std::string& fileName) {
	TEST_START2

	MeshDensity3D meshDens(15, 4, 16);
	Pipe3Dradius  radius(5.0, 9.0, 3.0, 5.0);
	ArcAngles	  angle(0.0, 0.95*GLM2PI);
	double		  height = 32.0;

	//Almost full cone:
	Cone3Dmesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::z);
	Cone3Dmesher::writeElements(meshDens);
	
	pos.x += height;
	//Full cylinder:
	radius.end.setInner(radius.start.inner());
	radius.end.setOuter(radius.start.outer());
	angle.setFullCircle();
	meshDens.setClosedLoop();
	Cone3Dmesher::writeNodes(pos, glCsys, meshDens, radius, angle, height, direction::x);
	Cone3Dmesher::writeElements(meshDens);

	TEST_END
}


int cone3DmesherRef(const std::string& fileName) {
	TEST_START2

	int nRef = 2;
	int nNodesAround = std::pow(2, nRef + 3) + 1;
	int nNodesEdge   = std::pow(2, nRef + 2) + 1;
	MeshDensity3Dref meshDens(nRef, nNodesAround, nNodesEdge);
	Pipe3Dradius  radius(5.0, 9.0, 1.5, 5.5);
	ArcAngles	  angle(0.0, GLMPI);
	double		  height = 8.0;

	//Half cone Z-axis:
	Cone3DmesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::z);
	Cone3DmesherRef::writeElements(meshDens);
	
	pos.x += radius.start.outer() * 2.0;
	//Full cone Z-axis:
	meshDens.setCirc(meshDens.dir1() - 1);
	meshDens.setNorm(std::pow(2, nRef + 2) + 1);
	meshDens.setClosedLoop();
	angle.setFullCircle();
	Cone3DmesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::z);
	Cone3DmesherRef::writeElements(meshDens);
	
	pos.x += radius.start.outer() * 2.0;
	//Full cone X-axis:
	Cone3DmesherRef::writeNodes(pos, glCsys, meshDens, radius, angle, height, false, direction::x);
	Cone3DmesherRef::writeElements(meshDens);

	//Full cylinder X-axis:
	pos.x += radius.start.outer() * 2.0;
	Cone3DmesherRef::writeNodes(pos, glCsys, meshDens, Pipe3Dradius(5.0, 9.0, 5.0, 9.0), angle, height, false, direction::x);
	Cone3DmesherRef::writeElements(meshDens);

	TEST_END
}

int recToEllipse3Dmesher(const std::string& fileName) {
	TEST_START2
	
	MeshDensity3D meshDens(64, 18, 4, true);
	
	EllipseRadius radS(36., 36.);
	EllipseRadius radE(36., 36.);
	glm::dvec2 recS(18., 18.);
	glm::dvec2 recE(18., 18.);
	ArcAngles ang;

	pos.x = -400.;
	//Circle to square to Ellipse to square Y-axis:
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radS, 
		EllipseRadius(radS.rad1, radS.rad2*2.0), recS, recS*2.0, ang, 50.0, direction::y);
	RecToEllipse3Dmesher::writeElements(meshDens);
	
	pos.x += 100.;
	//Cylinder to square Z-axis:
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radS, radE, recS, recE, ang, 10.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.x += 100.;
	//Cone to square Z-axis:
	radE = EllipseRadius(48., 48.);
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radS, radE, recS, recE, ang, 10.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.x += 100.;
	//Stacked: Cone to square + cylinder to square + Cone to square Z-axis:
	pos.z = -25.0;
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radS, radS, recS, recE, ang, 50.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.z = 25.0 + 1.0;
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radS, radE, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.z = -25.0 - 1.0 - 20.0;
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radE, radS, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.x += 100.;
	recE /= 2.0;
	//Cylinder to decreasing square Z-axis:
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radS, radS, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.x += 100.;
	//Cylinder to decreasing square Z-axis (TODO: this one looks buggy):
	/*
		Probably because number of nodes per edge in inner square is changing as ratio of rectangle
		edges are changing.
	*/
	recE.x = recS.x;
	meshDens.setAxis(18);
	RecToEllipse3Dmesher::writeNodes(pos, glCsys, meshDens, radS, radS, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

TEST_END
}


int refinement2dHeight(const std::string& fileName) {
	TEST_START2

	double heightC1 = 18.05;
	double heightC2 = 23.4;
	int nRefC1 = 2;
	int nRefC2 = 4;
	int nNodesY0C1 = std::pow(2, nRefC1 + 2) + 1;
	int nNodesY0C2 = std::pow(2, nRefC2 + 1) + 1;

	MeshDensity2Dref meshDensC1(nRefC1, nNodesY0C1);
	MeshDensity2Dref meshDensC2(nRefC2, nNodesY0C2);
	glm::dvec2 sizeC1(heightC1, 20.0);
	glm::dvec2 sizeC2(heightC2, 10.0);

	double lengthLineC1 = 1.2*sizeC1.y;
	double lengthLineC2 = 1.2*sizeC2.y;

	bool skipFirstRow[2] = { false, true };

	for (int i = 0; i < 2; i++) {

		//Plane 1:
		PlaneMesherRef::writeNodesXY(pos, glCsys, meshDensC1, sizeC1, skipFirstRow[i]);
		PlaneMesherRef::writeElements(meshDensC1);

		//Lower limit Plane1:
		glm::dvec3 linePos = pos;
		LineMesher::writeNodesLine(pos, glCsys, 2, lengthLineC1, direction::y);
		LineMesher::writeElementsLine(2);
		//Upper limit Plane1:
		linePos = pos + glm::dvec3(heightC1, 0., 0.);
		LineMesher::writeNodesLine(linePos, glCsys, 2, lengthLineC1, direction::y);
		LineMesher::writeElementsLine(2);

		pos.x += 1.2*sizeC1.x;
		//Plane 2:
		PlaneMesherRef::writeNodesXY(pos, glCsys, meshDensC2, sizeC2, skipFirstRow[i]);
		PlaneMesherRef::writeElements(meshDensC2);

		//Lower limit Plane1:
		linePos = pos;
		LineMesher::writeNodesLine(linePos, glCsys, 2, lengthLineC2, direction::y);
		LineMesher::writeElementsLine(2);
		//Upper limit Plane1:
		linePos = pos + glm::dvec3(heightC2, 0., 0.);
		LineMesher::writeNodesLine(linePos, glCsys, 2, lengthLineC2, direction::y);
		LineMesher::writeElementsLine(2);

		pos.z += 10.0;
		pos.x = 0.0;
	}

	TEST_END
}


int refinement3dHeight(const std::string& fileName) {
	TEST_START2

	double heightC1 = 10.0;
	double heightC2 = 13.4;
	int nRefC1 = 2;
	int nRefC2 = 4;
	int nNodesX0C1 = std::pow(2, nRefC1 + 1) + 1;
	int nNodesY0C1 = nNodesX0C1;
	int nNodesX0C2 = std::pow(2, nRefC2 + 2) + 1;
	int nNodesY0C2 = std::pow(2, nRefC2 + 1) + 1;

	MeshDensity3Dref meshDensC1(nRefC1, nNodesX0C1, nNodesY0C1);
	MeshDensity3Dref meshDensC2(nRefC2, nNodesX0C2, nNodesY0C2);
	glm::dvec3 sizeC1(20.0, 20.0, heightC1);
	glm::dvec3 sizeC2(20.0, 10.0, heightC2);

	MeshDensity2D meshDensPlate(2, 2);
	glm::dvec2 sizePlateC1 = 1.2*glm::dvec2(sizeC1.x, sizeC1.y);
	glm::dvec2 sizePlateC2 = 1.2*glm::dvec2(sizeC2.x, sizeC2.y);


	bool skipFirstRow[2] = { false, true };

	for (int i = 0; i < 2; i++) {

		//Cube 1:
		CuboidMesherRef::writeNodes(pos, glCsys, meshDensC1, sizeC1, skipFirstRow[i], plane::xy);
		CuboidMesherRef::writeElements(meshDensC1);

		//Lower limit Cube1:
		glm::dvec3 planePos = pos;
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cube1:
		planePos = pos + glm::dvec3(0., 0., heightC1);
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);

		pos.x += 1.2*sizeC1.x;
		//Cube 2:
		CuboidMesherRef::writeNodes(pos, glCsys, meshDensC2, sizeC2, skipFirstRow[i], plane::xy);
		CuboidMesherRef::writeElements(meshDensC2);
		
		//Lower limit Cube1:
		planePos = pos;
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cube1:
		planePos = pos + glm::dvec3(0., 0., heightC2);
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);


		pos.y += sizePlateC1.y*2.0;
		pos.x = 0.0;
	}
	TEST_END
}

/*
	TODO: if cone has cone shape (different top and bottom radius),
		  the height is not correct (slightly different) if skip first row is on vs off
		  end radius is also not correct
*/
int refinementCone2dHeight(const std::string& fileName)
{
	TEST_START2

	double heightC1 = 10.0;
	double heightC2 = 13.4;
	int nRefC1 = 2;
	int nRefC2 = 3;
	int nNodesC1 = std::pow(2, nRefC1 + 3);
	int nNodesC2 = std::pow(2, nRefC2 + 4);

	MeshDensity2Dref meshDensC1(nRefC1, nNodesC1, true);
	MeshDensity2Dref meshDensC2(nRefC2, nNodesC2, true);
	Cone2Dradius radiusC1(5.00, 0.25);
	Cone2Dradius radiusC2(5.00, 5.00);
	
	MeshDensity2D meshDensPlate(2, 2);
	glm::dvec2 sizePlateC1 = 3.2*glm::dvec2(radiusC1.end());
	glm::dvec2 sizePlateC2 = 1.2*glm::dvec2(radiusC2.end());


	bool skipFirstRow[2] = { false, true };

	for (int i = 0; i < 2; i++) {

		//Cone 1:
		ConeMesherRef::writeNodes(pos, glCsys, meshDensC1, radiusC1, ArcAngles(), heightC1, skipFirstRow[i], direction::z);
		ConeMesherRef::writeElements(meshDensC1);

		//Lower limit Cone 1:
		glm::dvec3 planePos = pos;
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 1:
		planePos = pos + glm::dvec3(0., 0., heightC1);
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);

		pos.x += 1.2*radiusC2.start()*2.0;
		//Cone 2:
		ConeMesherRef::writeNodes(pos, glCsys, meshDensC2, radiusC2, ArcAngles(), heightC2, skipFirstRow[i], direction::z);
		ConeMesherRef::writeElements(meshDensC2);

		//Lower limit Cone 2:
		planePos = pos;
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 2:
		planePos = pos + glm::dvec3(0., 0., heightC2);
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);


		pos.y += 1.2*radiusC2.start()*2.0;
		pos.x = 0.0;
	}
	TEST_END

}

/*
	TODO: if cone has cone shape (different top and bottom radius),
		  the height is not correct (slightly different) if skip first row is on vs off
*/
int refinementCone3dHeight(const std::string& fileName)
{
	TEST_START2

	double heightC1 = 4.0;
	double heightC2 = 6.4;
	int nRefC1 = 2;
	int nRefC2 = 3;
	int nNodesC1		= std::pow(2, nRefC1 + 4);
	int nNodesC1norm	= std::pow(2, nRefC1 + 2) + 1;
	int nNodesC2		= std::pow(2, nRefC2 + 5);
	int nNodesC2norm	= std::pow(2, nRefC2 + 2) + 1;

	MeshDensity3Dref meshDensC1(nRefC1, nNodesC1, nNodesC1norm, true);
	MeshDensity3Dref meshDensC2(nRefC2, nNodesC2, nNodesC2norm, true);
	Pipe3Dradius radiusC1(5.00, 8.25, 3.0, 5.0);
	Pipe3Dradius radiusC2(5.00, 8.00, 5.00, 8.00);
	
	MeshDensity2D meshDensPlate(2, 2);
	glm::dvec2 sizePlateC1 = 1.2*glm::dvec2(radiusC1.start.outer()*2.0);
	glm::dvec2 sizePlateC2 = 1.2*glm::dvec2(radiusC2.start.outer()*2.0);


	bool skipFirstRow[2] = { false, true };

	for (int i = 0; i < 2; i++) {

		//Cone 1:
		Cone3DmesherRef::writeNodes(pos, glCsys, meshDensC1, radiusC1, ArcAngles(), heightC1, skipFirstRow[i], direction::z);
		Cone3DmesherRef::writeElements(meshDensC1);

		//Lower limit Cone 1:
		glm::dvec3 planePos = pos;
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 1:
		planePos = pos + glm::dvec3(0., 0., heightC1);
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);

		pos.x += 1.2*radiusC2.start.outer()*2.0;
		//Cone 2:
		Cone3DmesherRef::writeNodes(pos, glCsys, meshDensC2, radiusC2, ArcAngles(), heightC2, skipFirstRow[i], direction::z);
		Cone3DmesherRef::writeElements(meshDensC2);

		//Lower limit Cone 2:
		planePos = pos;
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 2:
		planePos = pos + glm::dvec3(0., 0., heightC2);
		PlaneMesher::writeNodesXY(planePos, glCsys, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);


		pos.y += sizePlateC1.y*2.0;
		pos.x = 0.0;
}
	TEST_END
}

template<typename T>
bool equalVectors(const std::vector<T>& vec1, const std::vector<T>& vec2) {
	if (vec1.size() != vec2.size()) return false;
	for (int i = 0; i < vec1.size(); i++) {
		if (vec1[i] != vec2[i]) return false;
	}
	return true;
}

template<typename T>
bool equalVecVectors(const std::vector<std::vector<T>>& vec1, std::vector<std::vector<T>>& vec2) {
	if (vec1.size() != vec2.size()) {
		return false;
	}
	for (int i = 0; i < vec1.size(); i++) {
		if (!equalVectors(vec1[i], vec2[i])) {
			return false;
		}
	}
	return true;
}

std::vector<int> getNodeIteratorResult(NodeIterator& nit) {
	std::vector<int> res;
	for (int nid = nit.first(); nid; nid = nit.next()) {
		res.push_back(nid);
	}
	return res;
}

std::vector<std::vector<int>> getNodeIteratorResult_4(NodeIterator2D& nit) {
	std::vector<std::vector<int>> res;
	int n1, n2, n3, n4;
	for (int b = nit.first4(n1, n2, n3, n4); b; b = nit.next4(n1, n2, n3, n4)) {
		res.push_back(std::vector<int>({ n1,n2,n3,n4 }));
	}
	return res;
}

int nodeIterator1D(const std::string& fileName) {

	std::vector<int> result;
	std::vector<int> expectedResult;
	
	NodeIterator1D it1(1, 4, 1);
	result = getNodeIteratorResult(it1);
	expectedResult = {1,2,3,4};
	if (!equalVectors(result, expectedResult)) return 1;

	NodeIterator1D it2(1, 4, 1, 9);
	result = getNodeIteratorResult(it2);
	expectedResult = { 9, 1, 2, 3, 4 };
	if (!equalVectors(result, expectedResult)) return 1;

	NodeIterator1D it3(2, 4, 2);
	result = getNodeIteratorResult(it3);
	expectedResult = { 2, 4, 6, 8 };
	if (!equalVectors(result, expectedResult)) return 1;

	NodeIterator1D it4(99, 3, 100, 1000);
	result = getNodeIteratorResult(it4);
	expectedResult = { 1000, 99, 199, 299 };
	if (!equalVectors(result, expectedResult)) return 1;
	
	return 0;
}



int nodeIterator1Dm(const std::string& fileName) {

	std::vector<int> result;
	std::vector<int> expectedResult;

	
	NodeIterator1D it1(1, 4, 1);		//1,2,3,4	
	NodeIterator1D it2(8, 2, 2, 9);		//9,8,10	
	NodeIterator1D it3(100, 3, 3, 900);	//900,100,103,106

	expectedResult = { 1,2,3,4, 9,8,10, 900,100,103,106 };
	NodeIterator1Dm itm1(std::vector<NodeIterator1D>({ it1,it2,it3 }), false);
	result = getNodeIteratorResult(itm1);
	if (!equalVectors(result, expectedResult)) return 1;

	it1 = NodeIterator1D(10, 4, 1);			//10,11,12,13	
	it2 = NodeIterator1D(8, 2, 2, 13);		//13,8,10	
	it3 = NodeIterator1D(100, 3, 3, 10);	//10,100,103,106


	expectedResult = { 10,11,12,13,		8,10,	100,103,106 };
	itm1 = NodeIterator1Dm(std::vector<NodeIterator1D>({ it1,it2,it3 }), true);
	result = getNodeIteratorResult(itm1);
	if (!equalVectors(result, expectedResult)) return 1;

	return 0;
}

int nodeIterator2D(const std::string& fileName) {

	std::vector<int> result;
	std::vector<int> expectedResult;

	//From NodeIterator2D example 1
	NodeIterator2D it1(1, 5, 3, 25, 5);
	result = getNodeIteratorResult(it1);
	expectedResult = { 1,26,51,76,101,6,31,56,81,106,11,36,61,86,111 };
	if (!equalVectors(result, expectedResult)) return 1;

	//From NodeIterator2D example 2
	NodeIterator1D preNodesIt(101, 3, 1);
	NodeIterator2D it2(1, 3, 3, 1, 12, preNodesIt);
	result = getNodeIteratorResult(it2);
	expectedResult = { 101,1,2,3, 102,13,14,15, 103,25,26,27 };
	if (!equalVectors(result, expectedResult)) return 1;

	return 0;
}

int nodeIterator2D_4(const std::string& fileName) {

	std::vector<std::vector<int>> result;
	std::vector<std::vector<int>> expectedResult;

	/*
	::::::::::::::::::::::::::::::::::::::::::::
	From NodeIterator2D example 1
	::::::::::::::::::::::::::::::::::::::::::::
		nNodesX = 5, nNodesY = 3
			   	  		  
	incrY = 5
	  ^	  11x   36x   61x   86x  111x
	  |       
	dirY   6x   31x   56x   81x  106x
	  |	   
	  |    1x   26x   51x   76x  101x

		----> dirX incrX = 25
	
	
	*/
	NodeIterator2D it1(1, 5, 3, 25, 5);
	result = getNodeIteratorResult_4(it1);
	expectedResult = { 
		{1, 26, 31, 6}, {26, 51, 56, 31}, {51, 76, 81, 56}, {76, 101, 106, 81}, 
		{6, 31, 36, 11}, {31, 56, 61, 36}, {56, 81, 86, 61}, {81, 106, 111, 86}
	};

	if (!equalVecVectors(result, expectedResult)) return 1;

	//From NodeIterator2D example 2
	//NodeIterator1D preNodesIt(101, 3, 1);
	//NodeIterator2D it2(1, 3, 3, 1, 12, preNodesIt);
	//result = getNodeIteratorResult(it2);
	//expectedResult = { 101,1,2,3, 102,13,14,15, 103,25,26,27 };
	//if (!equalVectors(result, expectedResult)) return 1;

	return 0;
}

int nodeIterator2Dm(const std::string& fileName) {
	return 1;
}

int meshEdgeExtrusion(const std::string& fileName) {

	double length = 10.0;

	/*
	   1-->                                                2
	1---2---3---4--------13--------14--19--20--21          |
 	|   |   |   |        |          |   |   |   |          V
0	5---6---7---8--------15--------16--22--23--24
|	|   |   |   |        |          |   |   |   |
v	9--10--11--12--------17--------18--25--26--27  
       3-->

	*/
	std::vector<std::vector<int>> expectedEdges;
	std::vector<std::vector<int>> resultEdges;

	//First extrusion:
	MeshEdgeExtrusion edgeExtr1(length, 3, 3, 1);
	expectedEdges = std::vector<std::vector<int>>({
		{1,5,9},
		{1,2,3,4},
		{4,8,12},
		{9,10,11,12},
		{1,5,9}
		});

	resultEdges = std::vector<std::vector<int>>({
		getNodeIteratorResult(edgeExtr1.edges[0].nodeIter),
		getNodeIteratorResult(edgeExtr1.edges[1].nodeIter),
		getNodeIteratorResult(edgeExtr1.edges[2].nodeIter),
		getNodeIteratorResult(edgeExtr1.edges[3].nodeIter),
		getNodeIteratorResult(edgeExtr1.edges[4].nodeIter)
		});

	for (int i = 0; i < 5; i++) {
		if (!equalVectors(expectedEdges[i], resultEdges[i])) return 1;
	}

	//Second extrusion:
	MeshEdgeExtrusion edgeExtr2(length, 2, 3, 13, &edgeExtr1);
	expectedEdges = std::vector<std::vector<int>>({
		{4, 8, 12},
		{4, 13, 14},
		{14,16,18},
		{12, 17, 18},
		{13, 15, 17}
		});

	resultEdges = std::vector<std::vector<int>>({
		getNodeIteratorResult(edgeExtr2.edges[0].nodeIter),
		getNodeIteratorResult(edgeExtr2.edges[1].nodeIter),
		getNodeIteratorResult(edgeExtr2.edges[2].nodeIter),
		getNodeIteratorResult(edgeExtr2.edges[3].nodeIter),
		getNodeIteratorResult(edgeExtr2.edges[4].nodeIter)
		});

	for (int i = 0; i < 5; i++) {
		if (!equalVectors(expectedEdges[i], resultEdges[i])) return 1;
	}

	//Third extrusion:
	MeshEdgeExtrusion edgeExtr3(length, 3, 3, 19, &edgeExtr2);
	expectedEdges = std::vector<std::vector<int>>({
		{14, 16, 18},
		{14, 19, 20, 21},
		{21, 24, 27},
		{18, 25, 26, 27},
		{19, 22, 25}
		});

	resultEdges = std::vector<std::vector<int>>({
		getNodeIteratorResult(edgeExtr3.edges[0].nodeIter),
		getNodeIteratorResult(edgeExtr3.edges[1].nodeIter),
		getNodeIteratorResult(edgeExtr3.edges[2].nodeIter),
		getNodeIteratorResult(edgeExtr3.edges[3].nodeIter),
		getNodeIteratorResult(edgeExtr3.edges[4].nodeIter),
		});

	for (int i = 0; i < 5; i++) {
		if (!equalVectors(expectedEdges[i], resultEdges[i])) return 1;
	}



	return 0;
}



int meshFaceExtrusion(const std::string& fileName) {

	double length = 10.0;

	TEST_START2
	MeshDensity3D meshDens(3, 4, 3);
	MeshDensity2D meshDensFace(meshDens.dir2(), meshDens.dir3());
	CuboidMesher::writeNodes(NULL_POS, glCsys, meshDens, glm::dvec3(5.0, 7.0, 5.0), plane::xy);
	CuboidMesher::writeElements(meshDens);
	
	std::vector<std::vector<int>> expectedFaces;
	std::vector<std::vector<int>> resultFaces;

	//First extrusion:
	MeshFaceExtrusion faceExtr1(length, 2, meshDensFace, 1);
	
	expectedFaces = std::vector<std::vector<int>>({
		{1,4,7,10,	13,16,19,22, 25,28,31,34},
		{1,2,3, 13,14,15, 25,26,27},
		{3,6,9,12, 15,18,21,24, 27,30,33,36},
		{10,11,12, 22,23,24, 34,35,36},
		{1,2,3, 4,5,6, 7,8,9, 10,11,12},
		{25,26,27, 28,29,30, 31,32,33, 34,35,36}
		});
	resultFaces.clear();
	for (int i = 0; i < 6; i++) {
		resultFaces.push_back(getNodeIteratorResult(faceExtr1.faces[i].nodeIter));
	}

	for (int i = 0; i < 6; i++) {
		if (!equalVectors(expectedFaces[i], resultFaces[i])) return 1;
	}

	//Second extrusion:	
	MeshFaceExtrusion faceExtr2(length, 2, meshDensFace, 37, &faceExtr1);
	expectedFaces = std::vector<std::vector<int>>({
		{3,6,9,12, 15,18,21,24, 27,30,33,36},		
		{3,37,38, 15,45,46, 27,53,54},
		{38,40,42,44, 46,48,50,52, 54,56,58,60},
		{12,43,44, 24,51,52, 36,59,60},
		{3,37,38, 6,39,40, 9,41,42, 12,43,44},
		{27,53,54, 30,55,56, 33,57,58, 36,59,60},

		{37,39,41,43, 45,47,49,51, 53,55,57,59},
		});
	resultFaces.clear();
	for (int i = 0; i < 7; i++) {
		resultFaces.push_back(getNodeIteratorResult(faceExtr2.faces[i].nodeIter));
	}

	for (int i = 0; i < 7; i++) {
		if (!equalVectors(expectedFaces[i], resultFaces[i])) return 1;
	}

	//third extrusion:
	MeshFaceExtrusion faceExtr3(length, 2, meshDensFace, 61, &faceExtr2);
	expectedFaces = std::vector<std::vector<int>>({
		{38,40,42,44, 46,48,50,52, 54,56,58,60},
		{38,61,62, 46,69,70, 54,77,78},
		{62,64,66,68, 70,72,74,76, 78,80,82,84},
		{44,67,68, 52,75,76, 60,83,84},
		{38,61,62, 40,63,64, 42,65,66, 44,67,68},
		{54,77,78, 56,79,80, 58,81,82, 60,83,84},

		{61,63,65,67, 69,71,73,75, 77,79,81,83},
		});
	resultFaces.clear();
	for (int i = 0; i < 7; i++) {
		resultFaces.push_back(getNodeIteratorResult(faceExtr3.faces[i].nodeIter));
	}

	for (int i = 0; i < 7; i++) {
		if (!equalVectors(expectedFaces[i], resultFaces[i])) return 1;
	}
	
	return 0;
}

int extrude2DedgeLine(const std::string& fileName)
{
	TEST_START2
	glm::dmat3x3 rotM1 = makeCsysMatrix(X_DIR, Y_DIR);
	glm::dmat3x3 rotM2 = makeCsysMatrix(Y_DIR, X_DIR);
	glm::dmat3x3 rotM3 = makeCsysMatrix(Z_DIR, X_DIR);
	glm::dmat3x3 rotM4 = makeCsysMatrix(glm::dvec3(0.8, 1.0, 0.0), glm::dvec3(1.0, 1.0, 0.5));

	MeshCsys csys1(glm::dvec3(0.0, 0.0, 0.0), &rotM1);
	MeshCsys csys2(glm::dvec3(0.0, 0.0, 2.0), &rotM2);
	MeshCsys csys3(glm::dvec3(0.0, 0.0, 5.0), &rotM3);
	MeshCsys csys4(glm::dvec3(0.0, 0.0, 8.0), &rotM4);

	std::vector<MeshCsys> csyss({ csys1, csys2, csys3, csys4 });

	Mesh2D_planeExtrusion mesh2D(8, 4.0);
	mesh2D.extrudeYedge(1.0, 2);	//0.0 - 1.0
	mesh2D.extrudeYedge(10.0, 3);	//1.0 - 11.0
	mesh2D.extrudeYedge(1.0, 3);	//11.0 - 12.0
	mesh2D.extrudeYedge(10.0, 2);	//12.0 - 22.0


	for (int i = 0; i < 4; i++) {
		mesh2D.setCsys(csyss[i]);
		mesh2D.writeNodes();
		mesh2D.writeElements();
	}

	TEST_END
}

int extrude2DedgeArc(const std::string& fileName)
{
	TEST_START
	glm::dmat3x3 rotM1 = makeCsysMatrix(X_DIR, Y_DIR);
	MeshCsys csys1(glm::dvec3(0.0, 0.0, 0.0), &rotM1);
	
	Mesh2D_planeExtrusion mesh2D(8, 4.0);
	mesh2D.extrudeYedgeArc(GLMPI/3.0, 10.0, 10);	//0.0 - 1.0
	mesh2D.extrudeYedgeArc(GLMPI/3.0, 2.0, 5);	//0.0 - 1.0
	mesh2D.extrudeYedgeArc(GLMPI/1.0, 1.0, 10);	//0.0 - 1.0
	mesh2D.extrudeYedgeArc(-GLMPI/1.0, -1.0, 8);	//0.0 - 1.0
	mesh2D.setCsys(csys1);
	mesh2D.writeNodes();
	mesh2D.writeElements();

	
	rotM1 = makeCsysMatrix(Z_DIR, GLMPI * 0.7);
	mesh2D.setCsys(csys1);

	mesh2D.writeNodes();
	mesh2D.writeElements();

	TEST_END
}

int extrude2DedgeArcAndLine(const std::string& fileName)
{
	TEST_START
	glm::dmat3x3 rotM1 = makeCsysMatrix(X_DIR, Y_DIR);
	MeshCsys csys1(glm::dvec3(0.0, 0.0, 0.0), &rotM1);

	Mesh2D_planeExtrusion mesh2D(8, 4.0);
	mesh2D.extrudeYedgeArc(GLMPI / 3.0, 10.0, 10);	//0.0 - 1.0
	mesh2D.extrudeYedgeArc(GLMPI / 3.0, 2.0, 5);	//0.0 - 1.0
	mesh2D.extrudeYedge(5.0, 5);	
	mesh2D.extrudeYedgeArc(GLMPI, 0.5, 8);	//0.0 - 1.0
	mesh2D.extrudeYedge(5.0, 5);
	mesh2D.extrudeYedgeArc(-GLMPI / 3.0, -1.5, 5);	//0.0 - 1.0
	mesh2D.extrudeYedgeArc(-GLMPI / 3.0, -8.0, 10);	//0.0 - 1.0

	mesh2D.setCsys(csys1);
	mesh2D.writeNodes();
	mesh2D.writeElements();
	
	rotM1 = makeCsysMatrix(Z_DIR, GLMPI * 0.7);
	mesh2D.setCsys(csys1);

	mesh2D.writeNodes();
	mesh2D.writeElements();

	TEST_END
}


int extrude3DfaceLine(const std::string& fileName){

	TEST_START2
	Mesh3D_volumeExtrusion mesh3D(MeshDensity2D(8,3), glm::dvec2(5.0, 2.0));
	mesh3D.extrudeYZface(10., 2);
	mesh3D.extrudeYZface(10., 3);
	mesh3D.extrudeYZface(5.0, 4);
	mesh3D.extrudeYZface(5.0, 34);
	mesh3D.writeNodes();
	mesh3D.writeElements();
	TEST_END
}
int extrude3DfaceArc(const std::string& fileName) {

	TEST_START2
	Mesh3D_volumeExtrusion mesh3D(MeshDensity2D(7, 10), glm::dvec2(5.0, 2.0));
	mesh3D.extrudeYZedgeArc(-GLMPI / 10.0, -100.0, -96.0, 30);
	mesh3D.extrudeYZedgeArc(GLMPI / 1.0, 4.0, 8., 20);	//0.0 - 1.0
	mesh3D.extrudeYZedgeArc(GLMPI / 3.0, 10.0, 14.0, 10);	//0.0 - 1.0
	mesh3D.extrudeYZedgeArc(-GLMPI / 1.0, -6, -2, 18);
	mesh3D.writeNodes();
	mesh3D.writeElements();
	TEST_END

	return false;
}
int extrude3DfaceArcAndLine(const std::string& fileName) {
	return false;
}

#ifdef TO_FIX
int extrude2DarcMulti(const std::string& filename) 
{
	TEST_START

	MeshRec2D mesh2D;
	mesh2D.initRectangle(glm::dvec2(4.0, 4.0), glm::ivec2(4, 8));
	mesh2D.extrudeYedgeArc(glm::pi<double>() / 2.0, 2.0, 8);
	mesh2D.extrudeYedgeArc(glm::pi<double>() / 1.2, 3.0, 16);
	mesh2D.extrudeYedge(3.0, 5);

	glm::dvec3 pos(0.0);
	int nodeID = 1;
	int elementID = 1;
	for(int i = 0; i < 4; i++){
		mesh2D.setFirstNodeID(nodeID);
		mesh2D.setFirstElementID(elementID);

		mesh2D.setPosition(pos);
		mesh2D.writeNodes(&writer);
		mesh2D.writeElements(&writer);
		pos += glm::dvec3(1.0, 0.0, 0.5);

		nodeID    += mesh2D.numberOfNodes();
		elementID += mesh2D.numberOfElements();
	}
	TEST_END
}
#endif




void writeDebugBeamElements(FEAwriter* w, int firstNode, int lastNode) {
	int n[2] = { firstNode, 0 };
	for (int i = 2; i < lastNode; i++) {
		n[1] = i;
		w->write2nodedBeam(i, n);
	}
}