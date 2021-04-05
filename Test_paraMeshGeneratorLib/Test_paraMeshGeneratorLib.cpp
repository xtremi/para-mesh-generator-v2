#include "TestDef.h"
//#include "Mesh.h"
#include <vector>

//1d elements
#include "LineMesher.h"
#include "ArcMesher.h"

//2d elements
#include "PlaneMesher.h"
#include "ConeMesher.h"
#include "DiskMesher.h"
#include "CylinderMesher.h"

//3d elements
#include "CuboidMesher.h"
#include "Cone3Dmesher.h"

#include "math_utilities.h"

int lineMesher(const std::string& fileName);
int arcMesher(const std::string& fileName);
int planeMesher(const std::string& fileName);
int planeMesherRef(const std::string& fileName);
int coneMesher(const std::string& fileName);
int coneMesherRef(const std::string& fileName);
int diskMesher(const std::string& fileName);
int diskMesherRef(const std::string& fileName);
int cylinderMesher(const std::string& fileName);
int cylinderMesherRef(const std::string& fileName);

int cuboidMesher(const std::string& fileName);
int cuboidMesherRef(const std::string& fileName);
int cone3Dmesher(const std::string& fileName);
int cone3DmesherRef(const std::string& fileName);

int refinement2dHeight(const std::string& fileName);
int refinement3dHeight(const std::string& fileName);
int refinementCone2dHeight(const std::string& fileName);
int refinementCone3dHeight(const std::string& fileName);

#ifdef TO_FIX

int extruded2Drecs(const std::string& fileName);
int extrude2Darc(const std::string& filename);
int extrude2DarcMulti(const std::string& filename);
#endif

std::vector<TestDef> testFunctions({
#ifdef SKIP
	TestDef(101, "lineMesher",			"basic meshers 2D", (testFunction)lineMesher),	
	TestDef(102, "arcMesher",			"basic meshers 2D", (testFunction)arcMesher),
	TestDef(103, "planeMesher",			"basic meshers 2D", (testFunction)planeMesher),
	TestDef(104, "planeMesherRef",		"basic meshers 2D", (testFunction)planeMesherRef),
	TestDef(105, "coneMesher",			"basic meshers 2D", (testFunction)coneMesher),
	TestDef(106, "coneMesherRef",		"basic meshers 2D", (testFunction)coneMesherRef),
	TestDef(107, "diskMesher",			"basic meshers 2D", (testFunction)diskMesher),
	TestDef(108, "diskMesherRef",		"basic meshers 2D", (testFunction)diskMesherRef),
	TestDef(109, "cylinderMesher",		"basic meshers 2D", (testFunction)cylinderMesher),
	TestDef(110, "cylinderMesherRef",	"basic meshers 2D", (testFunction)cylinderMesherRef),

	TestDef(120, "cuboidMesher",	    "basic meshers 3D", (testFunction)cuboidMesher),
	TestDef(121, "cuboidMesherRef",	    "basic meshers 3D", (testFunction)cuboidMesherRef),
	TestDef(122, "cone3Dmesher",	    "basic meshers 3D", (testFunction)cone3Dmesher),	
	TestDef(123, "cone3DmesherRef",	    "basic meshers 3D", (testFunction)cone3DmesherRef),
	TestDef(130, "refinement2dHeight",	"basic meshers 2D", (testFunction)refinement2dHeight),
#endif
	TestDef(131, "refinement3dHeight",	"basic meshers 3D", (testFunction)refinement3dHeight),
	TestDef(132, "refinementCone2dHeight",	"basic meshers 2D", (testFunction)refinementCone2dHeight),
	TestDef(133, "refinementCone3dHeight",	"basic meshers 3D", (testFunction)refinementCone3dHeight),
#ifdef TO_FIX

	TestDef(200, "extruded2Drecs",		"extrusion", (testFunction)extruded2Drecs),
	TestDef(270, "extrude2Darc",		"extrusion", (testFunction)extrude2Darc),	
	TestDef(290, "extrude2DarcMulti",	"extrusion", (testFunction)extrude2DarcMulti),
#endif
});


int main(int argc, char* argv[]) {

	for (TestDef& testDef : testFunctions) {
		testDef.run();
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

#define TEST_END writer.close(); \
				 return 0;
/*************************************************************************/

int lineMesher(const std::string& fileName) {
	TEST_START

	int		nnodes = 20;
	double  length = 10.0;

	LineMesher::writeNodesLine(pos, nnodes, length, direction::x);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLine(pos, nnodes, length, direction::y);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLine(pos, nnodes, length, direction::z);
	LineMesher::writeElementsLine(nnodes);

	pos.pos.x += 1.2*length;
	LineMesher::writeNodesLineX(pos, nnodes, length);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineY(pos, nnodes, length);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineZ(pos, nnodes, length);
	LineMesher::writeElementsLine(nnodes);
	
	nnodes = 10;	
	glm::dvec3 dsvec = glm::dvec3(1.0, 0.35, 0.15);

	pos.pos.y += 1.2*length;
	LineMesher::writeNodesLineQ(pos, nnodes, dsvec);
	LineMesher::writeElementsLine(nnodes);

	double ds = 0.746;

	pos.pos.x = 0.0;
	pos.pos.y += 1.2*length;
	LineMesher::writeNodesLineXq(pos, nnodes, ds);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineYq(pos, nnodes, ds);
	LineMesher::writeElementsLine(nnodes);
	LineMesher::writeNodesLineZq(pos, nnodes, ds);
	LineMesher::writeElementsLine(nnodes);

	
	pos.pos.y += 1.2*length;
	double rad1 = 10.0, rad2 = 15.0;
	double ang = 0.0;
	int nLines = 8;
	double dang = GLM2PI / (double)nLines;
	glm::dvec3 posStart, posEnd;
	for (int i = 0; i < 8; i++) {
		posStart = coordsOnCircle(ang, rad1, direction::z) + pos.pos;
		posEnd = coordsOnCircle(ang, rad2, direction::z) + pos.pos + glm::dvec3(0, 0, 1.0);
		LineMesher::writeNodesLine(MeshCsys(posStart), nnodes, posEnd);
		LineMesher::writeElementsLine(nnodes);
		ang += dang;
	}


	TEST_END
}


int arcMesher(const std::string& fileName) {
	TEST_START

	int nnodes = 11;
	double radius = 5.0;

	ArcMesher::writeNodesCircular(pos, nnodes, radius, ArcAngles(0.0, GLMPI), direction::x);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircular(pos, nnodes, radius, ArcAngles(GLMPI, 0.0), direction::y);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircular(pos, nnodes, radius, ArcAngles(), direction::z);
	ArcMesher::writeElementsLine(nnodes);

	pos.pos.x += radius*2.5;
	ArcMesher::writeNodesCircularX(pos, nnodes, radius, ArcAngles(0.0, 3.0*GLM2PI/4.0));
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularY(pos, nnodes, radius, ArcAngles(0.0, GLM2PI/3.0));
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularZ(pos, nnodes, radius, ArcAngles(0.0, GLM2PI*0.95));
	ArcMesher::writeElementsLine(nnodes);

	pos.pos.x += radius * 2.5;
	double dang = 0.7*GLM2PI / nnodes;
	ArcMesher::writeNodesCircularQ(pos, nnodes, radius, 0.0, dang, direction::x);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularQ(pos, nnodes, radius, 0.0, -dang, direction::y);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularQ(pos, nnodes, radius, GLMPI, 0.6*dang, direction::z);
	ArcMesher::writeElementsLine(nnodes);

	pos.pos.x = 0.0;
	pos.pos.y += radius * 2.5;
	ArcMesher::writeNodesCircularXq(pos, nnodes, radius, GLMPI/2.0, dang);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularYq(pos, nnodes, radius, GLMPI, dang);
	ArcMesher::writeElementsLine(nnodes);
	ArcMesher::writeNodesCircularZq(pos, nnodes, radius, GLMPI, -dang);
	ArcMesher::writeElementsLine(nnodes);


	pos.pos.x += radius * 2.5;
	ArcMesher::writeNodesCircular_nth(pos, nnodes, radius, ArcAngles(GLMPI, GLM2PI), 5, direction::x);
	ArcMesher::writeElementsLine(nnodes - 2-1);

	pos.pos.x += radius * 2.5;
	ArcMesher::writeNodesCircular_nth(pos, nnodes, radius, ArcAngles(GLMPI, GLM2PI), 3, direction::x);
	ArcMesher::writeElementsLine(nnodes - 3-1);

	pos.pos.x += radius * 2.5;
	ArcMesher::writeNodesCircular_nth(pos, nnodes, radius, ArcAngles(GLMPI, GLM2PI), 7, direction::x);
	ArcMesher::writeElementsLine(nnodes - 1-1);
	
	TEST_END
}

int planeMesher(const std::string& fileName) {
	TEST_START

	MeshDensity2D meshDens(10, 12);
	glm::dvec2 size(20.0, 17.0);
	
	PlaneMesher::writeNodesPlane(pos, meshDens, size, plane::xy);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlane(pos, meshDens, size, plane::xz);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlane(pos, meshDens, size, plane::yz);
	PlaneMesher::writeElementsPlane(meshDens);
	
	pos.pos.x += 1.2*size.x;
	PlaneMesher::writeNodesPlaneXY(pos, meshDens, size);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlaneXZ(pos, meshDens, size);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlaneYZ(pos, meshDens, size);
	PlaneMesher::writeElementsPlane(meshDens);
	
	pos.pos.x += 1.2*size.x;
	glm::dvec2 elsize(size.x / (double)meshDens.nElDir1(), size.y / (double)meshDens.nElDir2());
	PlaneMesher::writeNodesPlaneQ(pos, meshDens, elsize, plane::xy);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlaneQ(pos, meshDens, elsize, plane::xz);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlaneQ(pos, meshDens, elsize, plane::yz);
	PlaneMesher::writeElementsPlane(meshDens);
	
	pos.pos.x += 1.2*size.x;
	PlaneMesher::writeNodesPlaneXYq(pos, meshDens, elsize);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlaneXZq(pos, meshDens, elsize);
	PlaneMesher::writeElementsPlane(meshDens);
	PlaneMesher::writeNodesPlaneYZq(pos, meshDens, elsize);
	PlaneMesher::writeElementsPlane(meshDens);
	TEST_END
}


int planeMesherRef(const std::string& fileName) {
	TEST_START

	glm::dvec2 size(20.0, 17.0);
	int nRef = 3;
	int nNodesDir2 = std::pow(2, nRef + 1) + 1;
	MeshDensity2Dref meshDens(nRef, nNodesDir2, false);

	PlaneMesherRef::writeNodesPlane_ref(pos, meshDens, size, true, plane::xy);
	PlaneMesherRef::writeElementsPlane_ref(meshDens);
	PlaneMesherRef::writeNodesPlane_ref(pos, meshDens, size, true, plane::xz);
	PlaneMesherRef::writeElementsPlane_ref(meshDens);
	PlaneMesherRef::writeNodesPlane_ref(pos, meshDens, size, true, plane::yz);
	PlaneMesherRef::writeElementsPlane_ref(meshDens);

	pos.pos.x += 1.2*size.x;

	nRef = 5;
	meshDens.setNrefs(nRef);
	meshDens.setDir2(std::pow(2, nRef + 1) + 1);
	size = glm::dvec2(15.0, 7.5);
	PlaneMesherRef::writeNodesPlaneXY_ref(pos, meshDens, size, false);
	PlaneMesherRef::writeElementsPlane_ref(meshDens);
	PlaneMesherRef::writeNodesPlaneXZ_ref(pos, meshDens, size, false);
	PlaneMesherRef::writeElementsPlane_ref(meshDens);
	PlaneMesherRef::writeNodesPlaneYZ_ref(pos, meshDens, size, false);
	PlaneMesherRef::writeElementsPlane_ref(meshDens);

	TEST_END
}


int coneMesher(const std::string& fileName) {
	TEST_START

	double height = 4.0;
	MeshDensity2D meshDens(10, 13); 
	Cone2Dradius radius(4.0, 2.0);
	ArcAngles angle(0.0, GLMPI);

	//Quarter cone:	
	ConeMesher::writeNodes(pos, meshDens, radius, angle, height, direction::x);
	ConeMesher::writeElements(meshDens);
	
	//Almost full cone:
	pos.pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLM2PI);
	ConeMesher::writeNodes(pos, meshDens, radius, angle, height, direction::y);
	ConeMesher::writeElements(meshDens);
		
	//Full cone:
	pos.pos.x += 2.1*radius.start();
	angle.setFullCircle();
	ConeMesher::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesher::writeElements(meshDens);

	
	height = 0.0;	
	//Almost full disk:
	pos.pos.y += 2.1*radius.start();
	pos.pos.x = 0.0;
	meshDens.nodes().x *= 2;
	meshDens.nodes().y /= 2;
	angle.setEnd(0.99*GLM2PI);
	ConeMesher::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesher::writeElements(meshDens);

	//Full disk:
	pos.pos.x += 2.1*radius.start();
	angle.setFullCircle();
	meshDens.setClosedLoop();
	ConeMesher::writeNodes(pos, meshDens, radius, angle, height, direction::x);
	ConeMesher::writeElements(meshDens);

	pos.pos.x += 2.1*radius.start();
	height = radius.start();
	radius.setStart(radius.end());	
	//Full Cylinder:
	ConeMesher::writeNodes(pos, meshDens, radius, angle, height, direction::y);
	ConeMesher::writeElements(meshDens);
		
	//Half cylinder:
	pos.pos.x += 2.1*radius.start();
	angle.setStart(GLMPI);
	angle.setEnd(GLM2PI);
	meshDens.setClosedLoop(false);
	ConeMesher::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesher::writeElements(meshDens);

	TEST_END
}


int coneMesherRef(const std::string& fileName) {
	TEST_START
			
	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;	
	MeshDensity2Dref meshDens(nRef, nNodesEdge, false);
	double height = 4.0;
	ArcAngles angle(0.0, GLMPI / 2.0);
	Cone2Dradius radius(4.0, 2.0);
	double radiusStart = 4.0;
	double radiusEnd = 2.0;

	//Quarter cone:	
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::x);
	ConeMesherRef::writeElements(meshDens);
	
	//Almost full cone:
	pos.pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLMPI);
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::y);
	ConeMesherRef::writeElements(meshDens);
	
	//Full cone:
	pos.pos.x += 2.1*radius.start();
	angle.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge - 1);
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Full disk:
	pos.pos.y += 2.1*radius.start();
	pos.pos.x = 0.0;
	height = 0.0;
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesherRef::writeElements(meshDens);

	//Almost full disk:
	pos.pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLM2PI);
	meshDens.setClosedLoop(false);
	meshDens.setCirc(nNodesEdge);
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Full Cylinder:
	pos.pos.y += 2.1*radius.start();
	pos.pos.x = 0.0;
	angle.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge - 1);
	radius.setEnd(radius.start());
	height = 10.0;
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Half cylinder:
	pos.pos.x = 2.1*radius.start();
	angle.setEnd(GLMPI);
	meshDens.setClosedLoop(false);
	meshDens.setCirc(nNodesEdge);
	height = 6.0;
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	ConeMesherRef::writeElements(meshDens);

	TEST_END

}

int diskMesher(const std::string& fileName){
	TEST_START
	MeshDensity2D meshDens(13,6);
	Disk2Dradius radius(2.0, 4.0);
	ArcAngles angles(0.0, 0.99*GLM2PI);

	DiskMesher::writeNodes(pos, meshDens, radius, angles, direction::x);
	DiskMesher::writeElements(meshDens);

	pos.pos.x += 2.1*radius.outer();
	angles.setEnd(GLMPI);
	DiskMesher::writeNodesX(pos, meshDens, radius, angles);
	DiskMesher::writeElements(meshDens);

	pos.pos.x += 2.1*radius.outer();
	angles.setEnd(GLMPI/2.0);
	DiskMesher::writeNodesY(pos, meshDens, radius, angles);
	DiskMesher::writeElements(meshDens);

	pos.pos.x += 2.1*radius.outer();
	angles.setFullCircle();
	meshDens.setClosedLoop();
	DiskMesher::writeNodesZ(pos, meshDens, radius, angles);
	DiskMesher::writeElements(meshDens);

	TEST_END
}


int diskMesherRef(const std::string& fileName) {
	TEST_START
	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;

	MeshDensity2Dref meshDens(nRef, nNodesEdge, false);
	Disk2Dradius radius(2.0, 4.0);
	ArcAngles angles(0.0, 0.99*GLM2PI);

	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, direction::x);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x += radius.outer() * 2.1;
	angles.setEnd(GLMPI);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, direction::y);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x += radius.outer() * 2.1;
	angles.setEnd(GLMPI/4.0);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, direction::z);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x = 0.0;
	pos.pos.y += radius.outer() * 2.1;
	nNodesEdge = std::pow(2, nRef + 5);
	angles.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, direction::z);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x += radius.outer() * 2.1;
	radius.setOuter(radius.inner() / 2.0);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, direction::z);
	DiskMesherRef::writeElements(meshDens);

	TEST_END
}


int cylinderMesher(const std::string& fileName)
{
	TEST_START

	MeshDensity2D meshDens(13, 6);
	ArcAngles angles(0.0, 0.99*GLM2PI);
	double radius = 2.0;
	double height = 8.0;

	CylinderMesher::writeNodes(pos, meshDens, radius, angles, height, direction::x);
	CylinderMesher::writeElements(meshDens);

	pos.pos.x += height * 2.1;
	angles.setEnd(GLMPI / 2.0);
	CylinderMesher::writeNodesX(pos, meshDens, radius, angles, height);
	CylinderMesher::writeElements(meshDens);

	pos.pos.x += height * 2.1;
	angles.setEnd(GLMPI / 4.0);
	CylinderMesher::writeNodesY(pos, meshDens, radius, angles, height);
	CylinderMesher::writeElements(meshDens);

	pos.pos.x += height * 2.1;
	meshDens.setClosedLoop();
	angles.setFullCircle();
	CylinderMesher::writeNodesY(pos, meshDens, radius, angles, height);
	CylinderMesher::writeElements(meshDens);

	TEST_END
}


int cylinderMesherRef(const std::string& fileName) 
{
	TEST_START

	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;
	MeshDensity2Dref meshDens(nRef, nNodesEdge, false);
	ArcAngles angles(0.0, 0.99*GLM2PI);
	double radius = 2.0;
	double height = 8.0;

	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, direction::x);
	CylinderMesherRef::writeElements(meshDens);

	pos.pos.x += height * 1.1;
	angles.setEnd(GLMPI / 2.0);
	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, direction::y);
	CylinderMesherRef::writeElements(meshDens);

	pos.pos.x += height * 1.1;
	angles.setEnd(GLMPI / 4.0);
	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, direction::z);
	CylinderMesherRef::writeElements(meshDens);

	
	pos.pos.x += height * 1.1;
	nNodesEdge = std::pow(2, nRef + 5);
	meshDens.setCirc(nNodesEdge);
	meshDens.setClosedLoop();
	angles.setFullCircle();
	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, direction::z);
	CylinderMesherRef::writeElements(meshDens);


	TEST_END

}

int cuboidMesher(const std::string& fileName) {
	TEST_START

	MeshDensity3D meshDens(5, 5, 10);
	glm::dvec3 size(20.0, 30.0, 100.0);

	CuboidMesher::writeNodes(pos, meshDens, size, plane::xy);
	CuboidMesher::writeElements(meshDens);

	pos.pos.x += 1.1*size.x;
	CuboidMesher::writeNodes(pos, meshDens, size, plane::xz);
	CuboidMesher::writeElements(meshDens);

	pos.pos.x += 1.1*size.x;
	CuboidMesher::writeNodes(pos, meshDens, size, plane::yz);
	CuboidMesher::writeElements(meshDens);
	
	pos.pos.x = 0.0;
	pos.pos.y += 1.1*size.y;
	CuboidMesher::writeNodesXYZ(pos, meshDens, size);
	CuboidMesher::writeElements(meshDens);

	TEST_END
}


int cuboidMesherRef(const std::string& fileName) {
	TEST_START			

	int nRef = 2;
	
	int nNodesX0 = std::pow(2, nRef + 1) + 1;	
	int nNodesY0 = nNodesX0;
	MeshDensity3Dref meshDens(nRef, nNodesX0, nNodesY0);
	glm::dvec3 size(20.0, 30.0, 50.0);

	CuboidMesherRef::writeNodes(pos, meshDens, size, false, plane::xy);
	CuboidMesherRef::writeElements(meshDens);
	
	pos.pos.x += 1.2*size.x;
	nRef = 4;
	meshDens.setDir1(std::pow(2, nRef + 1) + 1);
	meshDens.setDir2(std::pow(2, nRef + 2) + 1);
	CuboidMesherRef::writeNodes(pos, meshDens, size, false, plane::xy);
	CuboidMesherRef::writeElements(meshDens);

	if (false) {
		int lastNode = writer.getNextNodeID();
		int n[2] = { 1, 0 };
		for (int i = 2; i < lastNode; i++) {
			n[1] = i;
			writer.write2nodedBeam(i, n);
		}
	}

	TEST_END
}

int cone3Dmesher(const std::string& fileName) {
	TEST_START

	MeshDensity3D meshDens(15, 4, 16);
	Pipe3Dradius  radius(5.0, 9.0, 3.0, 5.0);
	ArcAngles	  angle(0.0, 1.95*GLM2PI);
	double		  height = 32.0;

	Cone3Dmesher::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	Cone3Dmesher::writeElements(meshDens);
	
	pos.pos.x += height;
	radius.end.setInner(radius.start.inner());
	radius.end.setOuter(radius.start.outer());
	angle.setFullCircle();
	meshDens.setClosedLoop();

	Cone3Dmesher::writeNodes(pos, meshDens, radius, angle, height, direction::x);
	Cone3Dmesher::writeElements(meshDens);

	TEST_END
}


int cone3DmesherRef(const std::string& fileName) {
	TEST_START

	int nRef = 2;
	int nNodesAround = std::pow(2, nRef + 4) + 1;
	MeshDensity3Dref meshDens(nRef, nNodesAround, nNodesAround);
	Pipe3Dradius  radius(5.0, 9.0, 1.5, 5.5);
	ArcAngles	  angle(0.0, GLMPI);
	double		  height = 8.0;

	Cone3DmesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	Cone3DmesherRef::writeElements(meshDens);
	
	pos.pos.x += radius.start.outer() * 2.0;
	meshDens.setCirc(meshDens.dir1() - 1);
	meshDens.setNorm(std::pow(2, nRef + 2) + 1);
	meshDens.setClosedLoop();
	angle.setFullCircle();
	Cone3DmesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	Cone3DmesherRef::writeElements(meshDens);
	
	pos.pos.x += radius.start.outer() * 2.0;
	Cone3DmesherRef::writeNodes(pos, meshDens, radius, angle, height, direction::x);
	Cone3DmesherRef::writeElements(meshDens);

	//cylinder
	pos.pos.x += radius.start.outer() * 2.0;
	Cone3DmesherRef::writeNodes(pos, meshDens, Pipe3Dradius(5.0, 9.0, 5.0, 9.0), angle, height, direction::x);
	Cone3DmesherRef::writeElements(meshDens);

	if(false){
		int lastNode = writer.getNextNodeID();
		int n[2] = { 1, 0 };
		for (int i = 2; i < lastNode; i++) {
			n[1] = i;
			writer.write2nodedBeam(i, n);
		}
	}

	TEST_END
}

int refinement2dHeight(const std::string& fileName) {
	TEST_START

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
		PlaneMesherRef::writeNodesPlaneXY_ref(pos, meshDensC1, sizeC1, skipFirstRow[i]);
		PlaneMesherRef::writeElementsPlane_ref(meshDensC1);

		//Lower limit Plane1:
		LineMesher::writeNodesLine(pos, 2, lengthLineC1, direction::y);
		LineMesher::writeElementsLine(2);
		//Upper limit Plane1:
		LineMesher::writeNodesLine(MeshCsys(pos.pos + glm::dvec3(heightC1, 0., 0.)), 2, lengthLineC1, direction::y);
		LineMesher::writeElementsLine(2);

		pos.pos.x += 1.2*sizeC1.x;
		//Plane 2:
		PlaneMesherRef::writeNodesPlaneXY_ref(pos, meshDensC2, sizeC2, skipFirstRow[i]);
		PlaneMesherRef::writeElementsPlane_ref(meshDensC2);

		//Lower limit Plane1:
		LineMesher::writeNodesLine(pos, 2, lengthLineC2, direction::y);
		LineMesher::writeElementsLine(2);
		//Upper limit Plane1:
		LineMesher::writeNodesLine(MeshCsys(pos.pos + glm::dvec3(heightC2, 0., 0.)), 2, lengthLineC2, direction::y);
		LineMesher::writeElementsLine(2);

		pos.pos.z += 10.0;
		pos.pos.x = 0.0;
	}

	TEST_END
}


int refinement3dHeight(const std::string& fileName) {
	TEST_START

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
		CuboidMesherRef::writeNodes(pos, meshDensC1, sizeC1, skipFirstRow[i], plane::xy);
		CuboidMesherRef::writeElements(meshDensC1);

		//Lower limit Cube1:
		PlaneMesher::writeNodesPlaneXY(pos, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElementsPlane(meshDensPlate);
		//Upper limit Cube1:
		PlaneMesher::writeNodesPlaneXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC1)), meshDensPlate, sizePlateC1);
		PlaneMesher::writeElementsPlane(meshDensPlate);

		pos.pos.x += 1.2*sizeC1.x;
		//Cube 2:
		CuboidMesherRef::writeNodes(pos, meshDensC2, sizeC2, skipFirstRow[i], plane::xy);
		CuboidMesherRef::writeElements(meshDensC2);
		
		//Lower limit Cube1:
		PlaneMesher::writeNodesPlaneXY(pos, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElementsPlane(meshDensPlate);
		//Upper limit Cube1:
		PlaneMesher::writeNodesPlaneXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC2)), meshDensPlate, sizePlateC2);
		PlaneMesher::writeElementsPlane(meshDensPlate);


		pos.pos.y += sizePlateC1.y*2.0;
		pos.pos.x = 0.0;
	}
	TEST_END
}


int refinementCone2dHeight(const std::string& fileName)
{
	TEST_START

	double heightC1 = 10.0;
	double heightC2 = 13.4;
	int nRefC1 = 2;
	int nRefC2 = 3;
	int nNodesC1 = std::pow(2, nRefC1 + 3);
	int nNodesC2 = std::pow(2, nRefC2 + 4);

	MeshDensity2Dref meshDensC1(nRefC1, nNodesC1, true);
	MeshDensity2Dref meshDensC2(nRefC2, nNodesC2, true);
	Cone2Dradius radiusC1(5.00, 1.25);
	Cone2Dradius radiusC2(5.00, 5.00);
	
	MeshDensity2D meshDensPlate(2, 2);
	glm::dvec2 sizePlateC1 = 1.2*glm::dvec2(radiusC1.end());
	glm::dvec2 sizePlateC2 = 1.2*glm::dvec2(radiusC2.end());


	bool skipFirstRow[2] = { false, true };

	for (int i = 0; i < 2; i++) {

		//Cone 1:
		ConeMesherRef::writeNodes(pos, meshDensC1, radiusC1, ArcAngles(), heightC1, direction::z);
		ConeMesherRef::writeElements(meshDensC1);

		//Lower limit Cone 1:
		PlaneMesher::writeNodesPlaneXY(pos, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElementsPlane(meshDensPlate);
		//Upper limit Cone 1:
		PlaneMesher::writeNodesPlaneXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC1)), meshDensPlate, sizePlateC1);
		PlaneMesher::writeElementsPlane(meshDensPlate);

		pos.pos.x += 1.2*radiusC2.start()*2.0;
		//Cone 2:
		ConeMesherRef::writeNodes(pos, meshDensC2, radiusC2, ArcAngles(), heightC2, direction::z);
		ConeMesherRef::writeElements(meshDensC2);

		//Lower limit Cone 2:
		PlaneMesher::writeNodesPlaneXY(pos, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElementsPlane(meshDensPlate);
		//Upper limit Cone 2:
		PlaneMesher::writeNodesPlaneXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC2)), meshDensPlate, sizePlateC2);
		PlaneMesher::writeElementsPlane(meshDensPlate);


		pos.pos.y += 1.2*radiusC2.start()*2.0;
		pos.pos.x = 0.0;
	}
	TEST_END

}

int refinementCone3dHeight(const std::string& fileName)
{
	TEST_START

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
		Cone3DmesherRef::writeNodes(pos, meshDensC1, radiusC1, ArcAngles(), heightC1, direction::z);
		Cone3DmesherRef::writeElements(meshDensC1);

		//Lower limit Cone 1:
		PlaneMesher::writeNodesPlaneXY(pos, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElementsPlane(meshDensPlate);
		//Upper limit Cone 1:
		PlaneMesher::writeNodesPlaneXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC1)), meshDensPlate, sizePlateC1);
		PlaneMesher::writeElementsPlane(meshDensPlate);

		pos.pos.x += 1.2*radiusC2.start.outer()*2.0;
		//Cone 2:
		Cone3DmesherRef::writeNodes(pos, meshDensC2, radiusC2, ArcAngles(), heightC2, direction::z);
		Cone3DmesherRef::writeElements(meshDensC2);

		//Lower limit Cone 2:
		PlaneMesher::writeNodesPlaneXY(pos, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElementsPlane(meshDensPlate);
		//Upper limit Cone 2:
		PlaneMesher::writeNodesPlaneXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC2)), meshDensPlate, sizePlateC2);
		PlaneMesher::writeElementsPlane(meshDensPlate);


		pos.pos.y += sizePlateC1.y*2.0;
		pos.pos.x = 0.0;
}
	TEST_END
}

#ifdef TO_FIX
int extruded2Drecs(const std::string& fileName)
{
	TEST_START

	glm::dmat3x3 csys1 = makeCsysMatrix(glm::dvec3(1.0, 0.0, 0.0), glm::dvec3(0.0, 1.0, 0.0));
	glm::dmat3x3 csys2 = makeCsysMatrix(glm::dvec3(0.0, 1.0, 0.0), glm::dvec3(1.0, 0.0, 0.0));
	glm::dmat3x3 csys3 = makeCsysMatrix(glm::dvec3(0.0, 0.0, 1.0), glm::dvec3(1.0, 0.0, 0.0));
	glm::dmat3x3 csys4 = makeCsysMatrix(glm::dvec3(0.8, 1.0, 0.0), glm::dvec3(1.0, 1.0, 0.5));

	std::vector<glm::dmat3x3> csyss({ csys1, csys2, csys3, csys4 });

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	MeshRec2D mesh2D;
	mesh2D.initRectangle(glm::dvec2(4.0, 4.0), glm::ivec2(4, 3));
	mesh2D.extrudeYedge(1.0, 3);
	mesh2D.extrudeYedge(10.0, 2);
	mesh2D.extrudeYedge(1.0, 3);
	mesh2D.extrudeYedge(10.0, 2);

	int firstNodeID = 1, firstElementID = 1;
	for (int i = 0; i < 4; i++) {
		mesh2D.setRotationMatrix(&csyss[i]);
		mesh2D.setFirstElementID(firstElementID);
		mesh2D.setFirstNodeID(firstNodeID);
		mesh2D.writeNodes(&writer);
		mesh2D.writeElements(&writer);

		firstNodeID += mesh2D.numberOfNodes();
		firstElementID += mesh2D.numberOfElements();
	}

	TEST_END
}

int extrude2Darc(const std::string& fileName)
{
	TEST_START

	glm::dmat3x3 csys1 = makeCsysMatrix(glm::dvec3(1.0, 0.0, 0.0), glm::dvec3(0.0, 1.0, 0.0));

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	MeshRec2D mesh2D;
	mesh2D.initRectangle(glm::dvec2(4.0, 4.0), glm::ivec2(4, 6));
	mesh2D.extrudeYedgeArc(glm::pi<double>() / 1.0, 2.0, 22);
	mesh2D.writeNodes(&writer);
	mesh2D.writeElements(&writer);
	TEST_END
}

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