#include "TestDef.h"
//#include "Mesh.h"
#include <vector>

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

#include "math_utilities.h"

int lineMesher(const std::string& fileName);
int arcMesher(const std::string& fileName);
int recEdgeMesher(const std::string& fileName);
int ellipseMesher(const std::string& fileName);

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

int cuboidMesher(const std::string& fileName);
int cuboidMesherRef(const std::string& fileName);
int cone3Dmesher(const std::string& fileName);
int cone3DmesherRef(const std::string& fileName);
int recToEllipse3Dmesher(const std::string& fileName);


int refinement2dHeight(const std::string& fileName);
int refinement3dHeight(const std::string& fileName);
int refinementCone2dHeight(const std::string& fileName);
int refinementCone3dHeight(const std::string& fileName);


#ifdef TO_FIX

int extruded2Drecs(const std::string& fileName);
int extrude2Darc(const std::string& filename);
int extrude2DarcMulti(const std::string& filename);
#endif

void writeDebugBeamElements(FEAwriter* w, int firstNode, int lastNode);


std::vector<TestDef> testFunctions({
#ifndef SKIP
	TestDef(101, "lineMesher",			"basic meshers 1D", (testFunction)lineMesher),	
	TestDef(102, "arcMesher",			"basic meshers 1D", (testFunction)arcMesher),
	TestDef(103, "recEdgeMesher",		"basic meshers 1D", (testFunction)recEdgeMesher),
	TestDef(104, "ellipseMesher",		"basic meshers 1D", (testFunction)ellipseMesher),

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
	TestDef(211, "recToEllipseMesherRef",	"basic meshers 2D", (testFunction)recToEllipseMesherRef),
	TestDef(211, "recTubeMesher",			"basic meshers 2D", (testFunction)recTubeMesher),

	TestDef(320, "cuboidMesher",	    "basic meshers 3D", (testFunction)cuboidMesher),
	TestDef(321, "cuboidMesherRef",	    "basic meshers 3D", (testFunction)cuboidMesherRef),
	TestDef(322, "cone3Dmesher",	    "basic meshers 3D", (testFunction)cone3Dmesher),	
	TestDef(323, "cone3DmesherRef",	    "basic meshers 3D", (testFunction)cone3DmesherRef),
	TestDef(324, "recToEllipse3Dmesher","basic meshers 3D", (testFunction)recToEllipse3Dmesher),

	TestDef(430, "refinement2dHeight",		"basic meshers 2D", (testFunction)refinement2dHeight),
	TestDef(431, "refinement3dHeight",		"basic meshers 3D", (testFunction)refinement3dHeight),
	TestDef(432, "refinementCone2dHeight",	"basic meshers 2D", (testFunction)refinementCone2dHeight),
	TestDef(433, "refinementCone3dHeight",	"basic meshers 3D", (testFunction)refinementCone3dHeight),


#ifdef TO_FIX

	TestDef(500, "extruded2Drecs",		"extrusion", (testFunction)extruded2Drecs),
	TestDef(570, "extrude2Darc",		"extrusion", (testFunction)extrude2Darc),	
	TestDef(590, "extrude2DarcMulti",	"extrusion", (testFunction)extrude2DarcMulti),
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

int recEdgeMesher(const std::string& fileName) {
	TEST_START

	int nnodes = 14;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(5.0, 5.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.z += 2.0;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(5.0, 10.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.z += 2.0;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.z += 2.0;
	nnodes = 18;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);
	
	pos.pos.z += 2.0;
	nnodes = 20;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.z += 2.0;
	nnodes = 22;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.z += 2.0;
	nnodes = 4;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(2.0, 12.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.x = 20.0;
	pos.pos.z = 0.0;
	nnodes = 48;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(10.0, 7.0), plane::xy);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.z = 0.1;
	pos.pos.y -= 3.5;	
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(10.0, 7.0), plane::xz);
	RecEdgeMesher::writeElements(nnodes);

	pos.pos.y += 7.0;
	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(10.0, 7.0), plane::xz);
	RecEdgeMesher::writeElements(nnodes);

	RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(7.0, 20.0), plane::yz);
	RecEdgeMesher::writeElements(nnodes);
	
	
	pos.pos = glm::dvec3(0.0);
	double elSize = 0.0;
	double recWidthStart = 10.0;
	double recWidth		 = recWidthStart;
	for(int i = 1; i < 6; i++){		
		pos.pos.y += 40.0;
		nnodes = i*4*8;
		elSize = recWidth / (double)(nnodes / 4);
		
		//L1/L2/L3 | L1/L2/L3 | L1/L2/L3 | 

		int nRefs = 3;
		for (int refs = 0; refs < nRefs; refs++) {
			//L1
			RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
			RecEdgeMesher::writeElements(nnodes);
			nnodes += 8;				//corner nodes
			recWidth += 1.8*elSize;		//increase rec size

			//L2
			RecEdgeMesher::writeNodes_nth(pos, nnodes, glm::dvec2(recWidth, recWidth), 4, plane::xy);
			RecEdgeMesher::writeElements(3 * nnodes / 4);
			nnodes += 8;								//corner nodes
			recWidth += 1.8*elSize;						//increase rec size
			nnodes /= 2;								//fewer elements			
			elSize = recWidth / (double)(nnodes / 4);	//larger elements

			//L3
			RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
			RecEdgeMesher::writeElements(nnodes);
			nnodes += 8;								//corner nodes
			recWidth += 1.8*elSize;						//increase rec size
			//elSize = recWidth / (double)(nnodes / 4);

		}
		recWidth = recWidthStart;
		/*
		//Layer 1: normal
		RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
		RecEdgeMesher::writeElements(nnodes);
		nnodes += 8;
		recWidth += 2.0*elSize;

		//Layer 2: nth
		RecEdgeMesher::writeNodes_nth(pos, nnodes, glm::dvec2(recWidth, recWidth), 4, plane::xy);
		RecEdgeMesher::writeElements(3* nnodes / 4);
		nnodes += 8;
		nnodes /= 2;
		recWidth += 2.0*elSize;
		elSize = recWidth / (double)(nnodes / 4);

		//Layer 3: normal
		RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
		RecEdgeMesher::writeElements(nnodes);
		nnodes += 8;
		recWidth += 2.0*elSize;
		elSize = recWidth / (double)(nnodes / 4);

		//Layer 4: normal
		RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
		RecEdgeMesher::writeElements(nnodes);
		nnodes += 8;
		recWidth += 2.0*elSize;

		//Layer 5: nth
		RecEdgeMesher::writeNodes_nth(pos, nnodes, glm::dvec2(recWidth, recWidth), 4, plane::xy);
		RecEdgeMesher::writeElements(3 * nnodes / 4);
		nnodes += 8;
		nnodes /= 2;
		recWidth += 2.0*elSize;
		elSize = recWidth / (double)(nnodes / 4);

		//Layer 6: normal
		RecEdgeMesher::writeNodes(pos, nnodes, glm::dvec2(recWidth, recWidth), plane::xy);
		RecEdgeMesher::writeElements(nnodes);


		recWidth = recWidthStart;*/
	}

	nnodes = 58;
	std::vector<glm::dvec2> xycoords(nnodes);
	RecEdgeMesher::getLocalCoords(xycoords, nnodes, glm::dvec2(60.0, 35.0));
	LineMesher::writeNodes(xycoords, 10.0);
	LineMesher::writeElementsLine(nnodes, true);

	TEST_END
}

int ellipseMesher(const std::string& fileName) {
	TEST_START
	EllipseMesher::writeNodes(pos, 24, EllipseRadius(10.0, 5.0), ArcAngles(), direction::z);
	EllipseMesher::writeElements(24, true);
	EllipseMesher::writeNodes(pos, 24, EllipseRadius(10.0, 5.0), ArcAngles(), direction::x);
	EllipseMesher::writeElements(24, true);
	EllipseMesher::writeNodes(pos, 24, EllipseRadius(10.0, 5.0), ArcAngles(), direction::y);
	EllipseMesher::writeElements(24, true);

	pos.pos.x = 50.0;
	int nEllipse = 10;
	for(int i = 0; i < nEllipse; i++){
		EllipseMesher::writeNodes(pos, 98, EllipseRadius(1.0 + 15.0*(double)i/(double)nEllipse, 5.0), ArcAngles(), direction::y);
		EllipseMesher::writeElements(98, true);
		pos.pos.y += 1.0;
	}

	pos.pos.x = 100.0;
	std::vector<glm::dvec2> localCoords(1500);
	EllipseMesher::getLocalCoords(localCoords, 1500, EllipseRadius(100.0, 50.0), ArcAngles());
	LineMesher::writeNodes(localCoords, 4.0);
	LineMesher::writeElementsLine(1500, true);

	TEST_END
}

int planeMesher(const std::string& fileName) {
	TEST_START

	MeshDensity2D meshDens(10, 12);
	glm::dvec2 size(20.0, 17.0);
	
	PlaneMesher::writeNodes(pos, meshDens, size, plane::xy);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodes(pos, meshDens, size, plane::xz);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodes(pos, meshDens, size, plane::yz);
	PlaneMesher::writeElements(meshDens);
	
	pos.pos.x += 1.2*size.x;
	PlaneMesher::writeNodesXY(pos, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesXZ(pos, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesYZ(pos, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	
	pos.pos.x += 1.2*size.x;
	glm::dvec2 elsize(size.x / (double)meshDens.nElDir1(), size.y / (double)meshDens.nElDir2());
	PlaneMesher::writeNodesQ(pos, meshDens, elsize, plane::xy);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesQ(pos, meshDens, elsize, plane::xz);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesQ(pos, meshDens, elsize, plane::yz);
	PlaneMesher::writeElements(meshDens);
	
	pos.pos.x += 1.2*size.x;
	PlaneMesher::writeNodesXYq(pos, meshDens, elsize);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesXZq(pos, meshDens, elsize);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesYZq(pos, meshDens, elsize);
	PlaneMesher::writeElements(meshDens);
	TEST_END
}


int planeMesherRef(const std::string& fileName) {
	TEST_START

	glm::dvec2 size(20.0, 17.0);
	int nRef = 3;
	int nNodesDir2 = std::pow(2, nRef + 1) + 1;
	MeshDensity2Dref meshDens(nRef, nNodesDir2, false);

	PlaneMesherRef::writeNodes(pos, meshDens, size, true, plane::xy);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodes(pos, meshDens, size, true, plane::xz);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodes(pos, meshDens, size, true, plane::yz);
	PlaneMesherRef::writeElements(meshDens);

	pos.pos.x += 1.2*size.x;

	nRef = 5;
	meshDens.setNrefs(nRef);
	meshDens.setDir2(std::pow(2, nRef + 1) + 1);
	size = glm::dvec2(15.0, 7.5);
	PlaneMesherRef::writeNodesXY(pos, meshDens, size, false);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodesXZ(pos, meshDens, size, false);
	PlaneMesherRef::writeElements(meshDens);
	PlaneMesherRef::writeNodesYZ(pos, meshDens, size, false);
	PlaneMesherRef::writeElements(meshDens);

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
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::x);
	ConeMesherRef::writeElements(meshDens);
	
	//Almost full cone:
	pos.pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLMPI);
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::y);
	ConeMesherRef::writeElements(meshDens);
	
	//Full cone:
	pos.pos.x += 2.1*radius.start();
	angle.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge - 1);
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Full disk:
	pos.pos.y += 2.1*radius.start();
	pos.pos.x = 0.0;
	height = 0.0;
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);

	//Almost full disk:
	pos.pos.x += 2.1*radius.start();
	angle.setEnd(0.99*GLM2PI);
	meshDens.setClosedLoop(false);
	meshDens.setCirc(nNodesEdge);
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Full Cylinder:
	pos.pos.y += 2.1*radius.start();
	pos.pos.x = 0.0;
	angle.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge - 1);
	radius.setEnd(radius.start());
	height = 10.0;
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::z);
	ConeMesherRef::writeElements(meshDens);
	
	//Half cylinder:
	pos.pos.x = 2.1*radius.start();
	angle.setEnd(GLMPI);
	meshDens.setClosedLoop(false);
	meshDens.setCirc(nNodesEdge);
	height = 6.0;
	ConeMesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::z);
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

	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, false, direction::x);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x += radius.outer() * 2.1;
	angles.setEnd(GLMPI);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, false, direction::y);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x += radius.outer() * 2.1;
	angles.setEnd(GLMPI/4.0);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, false, direction::z);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x = 0.0;
	pos.pos.y += radius.outer() * 2.1;
	nNodesEdge = std::pow(2, nRef + 5);
	angles.setFullCircle();
	meshDens.setClosedLoop();
	meshDens.setCirc(nNodesEdge);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, false, direction::z);
	DiskMesherRef::writeElements(meshDens);

	pos.pos.x += radius.outer() * 2.1;
	radius.setOuter(radius.inner() / 2.0);
	DiskMesherRef::writeNodes(pos, meshDens, radius, angles, false, direction::z);
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

	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, false, direction::x);
	CylinderMesherRef::writeElements(meshDens);

	pos.pos.x += height * 1.1;
	angles.setEnd(GLMPI / 2.0);
	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, false, direction::y);
	CylinderMesherRef::writeElements(meshDens);

	pos.pos.x += height * 1.1;
	angles.setEnd(GLMPI / 4.0);
	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, false, direction::z);
	CylinderMesherRef::writeElements(meshDens);

	
	pos.pos.x += height * 1.1;
	nNodesEdge = std::pow(2, nRef + 5);
	meshDens.setCirc(nNodesEdge);
	meshDens.setClosedLoop();
	angles.setFullCircle();
	CylinderMesherRef::writeNodes(pos, meshDens, radius, angles, height, false, direction::z);
	CylinderMesherRef::writeElements(meshDens);


	TEST_END
}

int recToEllipseMesher(const std::string& fileName) {
	TEST_START

	MeshDensity2D meshDens(128, 18, true);
	
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(36., 36.), glm::dvec2(18., 18.), ArcAngles(), 10.0, direction::x);
	RecToEllipseMesher::writeElements(meshDens);
	

	pos.pos.x += 85.0;
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(36., 18.), glm::dvec2(18., 9.), ArcAngles(), 10.0, direction::y);
	RecToEllipseMesher::writeElements(meshDens);

	pos.pos.x += 85.0;
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(36., 36.), glm::dvec2(18., 18.), ArcAngles(), 0., direction::y);
	RecToEllipseMesher::writeElements(meshDens);

	pos.pos.x += 85.0;
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(36., 18.), glm::dvec2(18., 9.), ArcAngles(), 0., direction::x);
	RecToEllipseMesher::writeElements(meshDens);

	pos.pos.x += 85.0;
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(18., 18.), glm::dvec2(72., 72.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(meshDens);
	DiskMesherRef::writeNodes(pos, MeshDensity2Dref(3, 128, true), Disk2Dradius(17.8, 9.), ArcAngles(), false, direction::z);
	DiskMesherRef::writeElements(MeshDensity2Dref(3, 128, true));

	pos.pos.y += 85.0;
	RecToEllipseMesher::writeNodes(pos, MeshDensity2D(32, 8, true), EllipseRadius(18., 18.), glm::dvec2(72., 72.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(MeshDensity2D(32, 8, true));
	DiskMesherRef::writeNodes(pos, MeshDensity2Dref(2, 128, true), Disk2Dradius(9., 17.8), ArcAngles(), false, direction::z);
	DiskMesherRef::writeElements(MeshDensity2Dref(2, 128, true));
	RecToEllipseMesher::writeNodes(pos, MeshDensity2D(128, 12, true), EllipseRadius(8.9, 8.9), glm::dvec2(2., 2.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(MeshDensity2D(128, 12, true));
	pos.pos.y -= 85.0;
	return 0;

	pos.pos.x += 85.0;
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(18., 9.), glm::dvec2(72., 36.), ArcAngles(), 0., direction::z);
	RecToEllipseMesher::writeElements(meshDens);

	pos.pos.x += 64.0;
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(24.0, 5.0), glm::dvec2(24.0, 5.0), ArcAngles(), 10.0, direction::z);
	RecToEllipseMesher::writeElements(meshDens);

	pos.pos.x += 64.0;
	RecToEllipseMesher::writeNodes(pos, meshDens, EllipseRadius(32.0, 32.0), glm::dvec2(32.0, 32.0), ArcAngles(), 10.0, direction::z);
	RecToEllipseMesher::writeElements(meshDens);


	TEST_END
}

int recToEllipseMesherRef (const std::string& fileName) {
	TEST_START

	MeshDensity2Dref meshDens(5, 64, true);
	
	RecToEllipseMesherRef::writeNodes(pos, meshDens, EllipseRadius(36., 36.), glm::dvec2(18., 18.), ArcAngles(), 10.0, false, direction::z);
	RecToEllipseMesherRef::writeElements(meshDens);


	TEST_END
}

int recTubeMesher(const std::string& fileName) {
	TEST_START

	int nNodesEdge = 8;
	int nnodesInner = nNodesEdge * 4 * 1;
	int nLayers = 4;
	glm::dvec2 sizeInner(10.0, 10.0);
	RecTubeSize size(sizeInner, sizeInner + 2.*(double)(nLayers - 1)*sizeInner /(double)nNodesEdge); 
	//glm::dvec2(10. + (double)(nLayers - 1)*2.*10. / 8.));

	RecTubeMesher::writeNodes(pos, nnodesInner, nLayers, size, plane::xy);
	writeDebugBeamElements(&writer, 1, writer.getNextNodeID());

	nLayers = 5;
	pos.pos.x += 25.0;
	RecTubeMesher::writeNodes2(pos, nnodesInner, nLayers, size, plane::xy);
	PlaneMesher::writeElements(MeshDensity2D(nnodesInner, nLayers, true));

	nLayers = 14;
	pos.pos.x += 25.0;
	RecTubeMesher::writeNodes2(pos, nnodesInner*3, nLayers, RecTubeSize(glm::dvec2(5.0, 5.0), glm::dvec2(15.0, 15.0)), plane::xy);
	PlaneMesher::writeElements(MeshDensity2D(nnodesInner * 3, nLayers, true));

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
	ArcAngles	  angle(0.0, 0.95*GLM2PI);
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
	int nNodesAround = std::pow(2, nRef + 3) + 1;
	int nNodesEdge   = std::pow(2, nRef + 2) + 1;
	MeshDensity3Dref meshDens(nRef, nNodesAround, nNodesEdge);
	Pipe3Dradius  radius(5.0, 9.0, 1.5, 5.5);
	ArcAngles	  angle(0.0, GLMPI);
	double		  height = 8.0;

	Cone3DmesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::z);
	Cone3DmesherRef::writeElements(meshDens);
	
	pos.pos.x += radius.start.outer() * 2.0;
	meshDens.setCirc(meshDens.dir1() - 1);
	meshDens.setNorm(std::pow(2, nRef + 2) + 1);
	meshDens.setClosedLoop();
	angle.setFullCircle();
	Cone3DmesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::z);
	Cone3DmesherRef::writeElements(meshDens);
	
	pos.pos.x += radius.start.outer() * 2.0;
	Cone3DmesherRef::writeNodes(pos, meshDens, radius, angle, height, false, direction::x);
	Cone3DmesherRef::writeElements(meshDens);

	//cylinder
	pos.pos.x += radius.start.outer() * 2.0;
	Cone3DmesherRef::writeNodes(pos, meshDens, Pipe3Dradius(5.0, 9.0, 5.0, 9.0), angle, height, false, direction::x);
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

int recToEllipse3Dmesher(const std::string& fileName) {
TEST_START
	
	MeshDensity3D meshDens(64, 18, 4, true);
	
	EllipseRadius radS(36., 36.);
	EllipseRadius radE(36., 36.);
	glm::dvec2 recS(18., 18.);
	glm::dvec2 recE(18., 18.);
	ArcAngles ang;

	pos.pos.x = -400.;
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radS, EllipseRadius(radS.rad1, radS.rad2*2.0), recS, recS*2.0, ang, 50.0, direction::y);
	RecToEllipse3Dmesher::writeElements(meshDens);
	
	pos.pos.x += 100.;
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radS, radE, recS, recE, ang, 10.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.pos.x += 100.;
	radE = EllipseRadius(48., 48.);
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radS, radE, recS, recE, ang, 10.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.pos.x += 100.;
	pos.pos.z = -25.0;
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radS, radS, recS, recE, ang, 50.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.pos.z = 25.0 + 1.0;
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radS, radE, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.pos.z = -25.0 - 1.0 - 20.0;
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radE, radS, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.pos.x += 100.;
	recE /= 2.0;
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radS, radS, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

	pos.pos.x += 100.;
	recE.x = recS.x;
	meshDens.setAxis(18);
	RecToEllipse3Dmesher::writeNodes(pos, meshDens, radS, radS, recS, recE, ang, 20.0, direction::z);
	RecToEllipse3Dmesher::writeElements(meshDens);

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
		PlaneMesherRef::writeNodesXY(pos, meshDensC1, sizeC1, skipFirstRow[i]);
		PlaneMesherRef::writeElements(meshDensC1);

		//Lower limit Plane1:
		LineMesher::writeNodesLine(pos, 2, lengthLineC1, direction::y);
		LineMesher::writeElementsLine(2);
		//Upper limit Plane1:
		LineMesher::writeNodesLine(MeshCsys(pos.pos + glm::dvec3(heightC1, 0., 0.)), 2, lengthLineC1, direction::y);
		LineMesher::writeElementsLine(2);

		pos.pos.x += 1.2*sizeC1.x;
		//Plane 2:
		PlaneMesherRef::writeNodesXY(pos, meshDensC2, sizeC2, skipFirstRow[i]);
		PlaneMesherRef::writeElements(meshDensC2);

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
		PlaneMesher::writeNodesXY(pos, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cube1:
		PlaneMesher::writeNodesXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC1)), meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);

		pos.pos.x += 1.2*sizeC1.x;
		//Cube 2:
		CuboidMesherRef::writeNodes(pos, meshDensC2, sizeC2, skipFirstRow[i], plane::xy);
		CuboidMesherRef::writeElements(meshDensC2);
		
		//Lower limit Cube1:
		PlaneMesher::writeNodesXY(pos, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cube1:
		PlaneMesher::writeNodesXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC2)), meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);


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
		ConeMesherRef::writeNodes(pos, meshDensC1, radiusC1, ArcAngles(), heightC1, skipFirstRow[i], direction::z);
		ConeMesherRef::writeElements(meshDensC1);

		//Lower limit Cone 1:
		PlaneMesher::writeNodesXY(pos, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 1:
		PlaneMesher::writeNodesXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC1)), meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);

		pos.pos.x += 1.2*radiusC2.start()*2.0;
		//Cone 2:
		ConeMesherRef::writeNodes(pos, meshDensC2, radiusC2, ArcAngles(), heightC2, skipFirstRow[i], direction::z);
		ConeMesherRef::writeElements(meshDensC2);

		//Lower limit Cone 2:
		PlaneMesher::writeNodesXY(pos, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 2:
		PlaneMesher::writeNodesXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC2)), meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);


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
		Cone3DmesherRef::writeNodes(pos, meshDensC1, radiusC1, ArcAngles(), heightC1, skipFirstRow[i], direction::z);
		Cone3DmesherRef::writeElements(meshDensC1);

		//Lower limit Cone 1:
		PlaneMesher::writeNodesXY(pos, meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 1:
		PlaneMesher::writeNodesXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC1)), meshDensPlate, sizePlateC1);
		PlaneMesher::writeElements(meshDensPlate);

		pos.pos.x += 1.2*radiusC2.start.outer()*2.0;
		//Cone 2:
		Cone3DmesherRef::writeNodes(pos, meshDensC2, radiusC2, ArcAngles(), heightC2, skipFirstRow[i], direction::z);
		Cone3DmesherRef::writeElements(meshDensC2);

		//Lower limit Cone 2:
		PlaneMesher::writeNodesXY(pos, meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);
		//Upper limit Cone 2:
		PlaneMesher::writeNodesXY(MeshCsys(pos.pos + glm::dvec3(0., 0., heightC2)), meshDensPlate, sizePlateC2);
		PlaneMesher::writeElements(meshDensPlate);


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




void writeDebugBeamElements(FEAwriter* w, int firstNode, int lastNode) {
	int n[2] = { firstNode, 0 };
	for (int i = 2; i < lastNode; i++) {
		n[1] = i;
		w->write2nodedBeam(i, n);
	}
}