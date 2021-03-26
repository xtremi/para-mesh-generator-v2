#include "TestDef.h"
#include "Mesh.h"
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

int extruded2Drecs(const std::string& fileName);
int extrude2Darc(const std::string& filename);
int extrude2DarcMulti(const std::string& filename);

std::vector<TestDef> testFunctions({

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
	
	TestDef(120, "cuboidMesher",	"basic meshers 3D", (testFunction)cuboidMesher),
	TestDef(121, "cuboidMesherRef",	"basic meshers 3D", (testFunction)cuboidMesherRef),
	TestDef(122, "cone3Dmesher",	"basic meshers 3D", (testFunction)cone3Dmesher),
	TestDef(123, "cone3DmesherRef",	"basic meshers 3D", (testFunction)cone3DmesherRef),

	TestDef(200, "extruded2Drecs",		"extrusion", (testFunction)extruded2Drecs),
	TestDef(270, "extrude2Darc",		"extrusion", (testFunction)extrude2Darc),	
	TestDef(290, "extrude2DarcMulti",	"extrusion", (testFunction)extrude2DarcMulti),
});


int main(int argc, char* argv[]) {


	for (TestDef& testDef : testFunctions) {
		testDef.run();
	}
	return 0;
}

int lineMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	//Mesher::setFEAwriter(&writer);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	int nnodes = 20;
	LineMesher::writeNodesLine(pos, pos + glm::dvec3(10.0, 4.0, 3.0), nnodes);
	LineMesher::writeElementsLine(nnodes);

	LineMesher::writeNodesLineX(pos, 10.0, nnodes);
	LineMesher::writeElementsLine(nnodes);

	LineMesher::writeNodesLineY(pos, 10.0, nnodes);
	LineMesher::writeElementsLine(nnodes);

	LineMesher::writeNodesLineZ(pos, 10.0, nnodes);
	LineMesher::writeElementsLine(nnodes);

	pos.x += 20.0;
	nnodes = 10;
	LineMesher::writeNodesLineQ(pos, glm::dvec3(0.8, 0.3, 0.3), nnodes);
	LineMesher::writeElementsLine(nnodes);

	LineMesher::writeNodesLineXq(pos, 0.7, nnodes);
	LineMesher::writeElementsLine(nnodes);

	LineMesher::writeNodesLineYq(pos, 0.7, nnodes);
	LineMesher::writeElementsLine(nnodes);

	LineMesher::writeNodesLineZq(pos, 0.7, nnodes);
	LineMesher::writeElementsLine(nnodes);

	writer.close();
	return 0;
}


int arcMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	int nnodes = 11;
	double radius = 5.0;

	ArcMesher::writeNodesCircular(pos, radius, 0.0, glm::pi<double>(), nnodes, direction::x);
	ArcMesher::writeElementsLine(nnodes);

	ArcMesher::writeNodesCircular(pos, radius, 0.0, glm::pi<double>()*0.75, nnodes, direction::y);
	ArcMesher::writeElementsLine(nnodes);

	ArcMesher::writeNodesCircular(pos, radius, 0.0, glm::pi<double>()*0.50, nnodes, direction::z);
	ArcMesher::writeElementsLine(nnodes);

	pos.x += 12.0;
	ArcMesher::writeNodesCircularX(pos, radius, 0.0, glm::pi<double>(), nnodes);
	ArcMesher::writeElementsLine(nnodes);

	ArcMesher::writeNodesCircularY(pos, radius, 0.0, glm::pi<double>()*0.75, nnodes);
	ArcMesher::writeElementsLine(nnodes);
	
	ArcMesher::writeNodesCircularZ(pos, radius, 0.0, glm::pi<double>()*0.50, nnodes);
	ArcMesher::writeElementsLine(nnodes);

	pos.x += 12.0;
	double dang = glm::pi<double>() / 20.0;
	ArcMesher::writeNodesCircularQ(pos, radius, glm::pi<double>() / 3.0, dang, nnodes, direction::x);
	ArcMesher::writeElementsLine(nnodes);
																		    
	ArcMesher::writeNodesCircularQ(pos, radius, glm::pi<double>() / 3.0, dang, nnodes, direction::y);
	ArcMesher::writeElementsLine(nnodes);
																		    
	ArcMesher::writeNodesCircularQ(pos, radius, glm::pi<double>() / 3.0, dang, nnodes, direction::z);
	ArcMesher::writeElementsLine(nnodes);

	pos.x += 12.0;
	ArcMesher::writeNodesCircularXq(pos, radius, glm::pi<double>(), dang, nnodes);
	ArcMesher::writeElementsLine(nnodes);

	ArcMesher::writeNodesCircularYq(pos, radius, glm::pi<double>(), dang, nnodes);
	ArcMesher::writeElementsLine(nnodes);

	ArcMesher::writeNodesCircularZq(pos, radius, glm::pi<double>(), dang, nnodes);
	ArcMesher::writeElementsLine(nnodes);


	pos.x += 12.0;
	ArcMesher::writeNodesCircular_nth(pos, radius, glm::pi<double>(), glm::pi<double>()*2.0, nnodes, 5, direction::x);
	ArcMesher::writeElementsLine(nnodes - 2-1);

	pos.x += 2.0;
	ArcMesher::writeNodesCircular_nth(pos, radius, glm::pi<double>(), glm::pi<double>()*2.0, nnodes, 3, direction::x);
	ArcMesher::writeElementsLine(nnodes - 3-1);

	pos.x += 2.0;
	ArcMesher::writeNodesCircular_nth(pos, radius, glm::pi<double>(), glm::pi<double>()*2.0, nnodes, 7, direction::x);
	ArcMesher::writeElementsLine(nnodes - 1-1);
	
	writer.close();
	return 0;
}

int planeMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	glm::ivec2 nnodes(10, 13);
	glm::dvec2 size(20.0, 17.0);
	
	PlaneMesher::writeNodesPlane(pos, size, nnodes, plane::xy);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlane(pos, size, nnodes, plane::xz);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlane(pos, size, nnodes, plane::yz);
	PlaneMesher::writeElementsPlane(nnodes, false);
	
	pos.x += 25.0;
	PlaneMesher::writeNodesPlaneXY(pos, size, nnodes);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlaneXZ(pos, size, nnodes);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlaneYZ(pos, size, nnodes);
	PlaneMesher::writeElementsPlane(nnodes, false);
	
	pos.x += 25.0;
	glm::dvec2 elsize(1.0, 1.3);
	PlaneMesher::writeNodesPlaneQ(pos, elsize, nnodes, plane::xy);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlaneQ(pos, elsize, nnodes, plane::xz);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlaneQ(pos, elsize, nnodes, plane::yz);
	PlaneMesher::writeElementsPlane(nnodes, false);
	
	pos.x += 25.0;
	PlaneMesher::writeNodesPlaneXYq(pos, elsize, nnodes);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlaneXZq(pos, elsize, nnodes);
	PlaneMesher::writeElementsPlane(nnodes, false);
	PlaneMesher::writeNodesPlaneYZq(pos, elsize, nnodes);
	PlaneMesher::writeElementsPlane(nnodes, false);
	writer.close();
	return 0;
}


int planeMesherRef(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);	
	glm::dvec2 size(20.0, 17.0);
	int nRef = 3;
	int nNodesY0 = std::pow(2, nRef + 1) + 1;

	PlaneMesherRef::writeNodesPlane_ref(pos, size, nNodesY0, nRef, true, plane::xy);
	PlaneMesherRef::writeElementsPlane_ref(nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlane_ref(pos, size, nNodesY0, nRef, true, plane::xz);
	PlaneMesherRef::writeElementsPlane_ref(nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlane_ref(pos, size, nNodesY0, nRef, true, plane::yz);
	PlaneMesherRef::writeElementsPlane_ref(nNodesY0, nRef, false);

	nRef = 5;
	pos.x += 24.0;
	nNodesY0 = std::pow(2, nRef + 1) + 1;
	size = glm::dvec2(15.0, 7.5);
	PlaneMesherRef::writeNodesPlaneXY_ref(pos, size, nNodesY0, nRef, false);
	PlaneMesherRef::writeElementsPlane_ref(nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlaneXZ_ref(pos, size, nNodesY0, nRef, false);
	PlaneMesherRef::writeElementsPlane_ref(nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlaneYZ_ref(pos, size, nNodesY0, nRef, false);
	PlaneMesherRef::writeElementsPlane_ref(nNodesY0, nRef, false);

	writer.close();
	return 0;
}
int coneMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	glm::ivec2 nnodes(10, 13);
	double height	= 4.0;
	double radiusStart = 4.0;
	double radiusEnd = 2.0;


	//Quarter cone:	
	ConeMesher::writeNodes(pos, radiusStart, radiusEnd, 0.0, 1.0*glm::pi<double>() / 2.0, height, nnodes, direction::x);
	ConeMesher::writeElements(nnodes, false);
	pos.x += 8.0;
	//Almost full cone:
	ConeMesher::writeNodes(pos, radiusStart, radiusEnd, 0.0, 1.99*glm::pi<double>() / 2.0, height, nnodes, direction::y);
	ConeMesher::writeElements(nnodes, false);
	pos.x += 8.0;
	//Full cone:
	ConeMesher::writeNodes(pos, radiusStart, radiusEnd, -1, -1, height, nnodes, direction::z);
	ConeMesher::writeElements(nnodes, true);

	pos.y += 8.0;
	pos.x = 0.0;
	nnodes.x *= 2;
	nnodes.y /= 2;
	//Almost full disk:
	ConeMesher::writeNodes(pos, radiusStart, radiusEnd, 0.0, 1.8*glm::pi<double>(), 0.0, nnodes, direction::x);
	ConeMesher::writeElements(nnodes, false);
	pos.x += 8.0;

	//Full disk:
	ConeMesher::writeNodes(pos, radiusEnd, radiusStart, -1, -1, 0.0, nnodes, direction::z);
	ConeMesher::writeElements(nnodes, true);

	pos.y += 8.0;
	pos.x = 0.0;

	//Full Cylinder:
	ConeMesher::writeNodes(pos, radiusStart, radiusStart, -1, -1, radiusStart*2.0, nnodes, direction::x);
	ConeMesher::writeElements(nnodes, true);
	pos.x += 18.0;

	//Half cylinder:
	ConeMesher::writeNodes(pos, radiusStart, radiusStart, 1.2*glm::pi<double>(), 0.0, radiusStart*2.0, nnodes, direction::z);
	ConeMesher::writeElements(nnodes, false);

	writer.close();
	return 0;
}
int coneMesherRef(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	double height = 4.0;
	double radiusStart = 4.0;
	double radiusEnd = 2.0;
	
	int nRef = 4;
	int nNodesY0 = std::pow(2, nRef + 3) + 1;

	//Quarter cone:	
	ConeMesherRef::writeNodes(pos, nNodesY0, nRef, radiusStart, radiusEnd, 0.0, 1.0*glm::pi<double>() / 2.0, height, direction::x);
	ConeMesherRef::writeElements(nNodesY0, nRef, false);
	pos.x += 8.0;
	//Almost full cone:
	ConeMesherRef::writeNodes(pos, nNodesY0, nRef, radiusStart, radiusEnd, 0.0, 1.99*glm::pi<double>() / 2.0, height, direction::y);
	ConeMesherRef::writeElements(nNodesY0, nRef, false);
	pos.x += 8.0;
	//Full cone:
	ConeMesherRef::writeNodes(pos, nNodesY0 - 1, nRef, radiusStart, radiusEnd, -1, -1, height, direction::z);
	ConeMesherRef::writeElements(nNodesY0 - 1, nRef, true);

	pos.y += 8.0;
	pos.x = 0.0;

	//Almost full disk:
	ConeMesherRef::writeNodes(pos, nNodesY0, nRef, radiusStart, radiusEnd, 0.0, 1.8*glm::pi<double>(), 0.0, direction::x);
	ConeMesherRef::writeElements(nNodesY0, nRef, false);
	pos.x += 8.0;

	//Full disk:
	ConeMesherRef::writeNodes(pos, nNodesY0 - 1, nRef, radiusEnd, radiusStart, -1, -1, 0.0, direction::z);
	ConeMesherRef::writeElements(nNodesY0 - 1, nRef, true);

	pos.y += 8.0;
	pos.x = 0.0;

	//Full Cylinder:
	ConeMesherRef::writeNodes(pos, nNodesY0 - 1, nRef, radiusStart, radiusStart, -1, -1, radiusStart*2.0,  direction::x);
	ConeMesherRef::writeElements(nNodesY0 - 1, nRef, true);
	pos.x += 18.0;

	//Half cylinder:
	ConeMesherRef::writeNodes(pos, nNodesY0, nRef, radiusStart, radiusStart, 1.2*glm::pi<double>(), 0.0, radiusStart*2.0, direction::z);
	ConeMesherRef::writeElements(nNodesY0, nRef, false);

	writer.close();
	return 0;

}

int diskMesher(const std::string& fileName){
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);
	glm::ivec2 nnodes(13, 6);

	double radStart = 2.0;
	double radEnd	= 4.0;

	DiskMesher::writeNodes(pos, radStart, radEnd, 0.0, 1.9*glm::pi<double>(), nnodes, direction::x);
	DiskMesher::writeElements(nnodes, false);

	pos.x += radEnd * 2.1;
	DiskMesher::writeNodesX(pos, radStart, radEnd, 0.0, 1.0*glm::pi<double>() / 2.0, nnodes);
	DiskMesher::writeElements(nnodes, false);

	pos.x += radEnd * 2.1;
	DiskMesher::writeNodesY(pos, radStart, radEnd, 0.0, 1.0*glm::pi<double>() / 4.0, nnodes);
	DiskMesher::writeElements(nnodes, false);

	pos.x += radEnd * 2.1;
	DiskMesher::writeNodesY(pos, radStart, radEnd, -1, -1, nnodes);
	DiskMesher::writeElements(nnodes, true);

	return 0;
}
int diskMesherRef(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);
	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;

	double radStart = 2.0;
	double radEnd = 4.0;

	DiskMesherRef::writeNodes(pos, nNodesEdge, nRef, radStart, radEnd, 0.0, 1.9*glm::pi<double>(), direction::x);
	DiskMesherRef::writeElements(nNodesEdge, nRef, false);

	pos.x += radEnd * 2.1;
	DiskMesherRef::writeNodes(pos, nNodesEdge, nRef, radStart, radEnd, 0.0, 1.0*glm::pi<double>() / 2.0, direction::y);
	DiskMesherRef::writeElements(nNodesEdge, nRef, false);

	pos.x += radEnd * 2.1;
	DiskMesherRef::writeNodes(pos, nNodesEdge, nRef, radStart, radEnd, 0.0, 1.0*glm::pi<double>() / 4.0, direction::z);
	DiskMesherRef::writeElements(nNodesEdge, nRef, false);

	nNodesEdge = std::pow(2, nRef + 5);
	pos.x += radEnd * 2.1;
	DiskMesherRef::writeNodes(pos, nNodesEdge, nRef, radStart, radEnd, -1, -1, direction::z);
	DiskMesherRef::writeElements(nNodesEdge, nRef, true);

	pos.x += radEnd * 2.1;
	DiskMesherRef::writeNodes(pos, nNodesEdge, nRef, radEnd, radStart, -1, -1, direction::z);
	DiskMesherRef::writeElements(nNodesEdge, nRef, true);
	return 0;
}


int cylinderMesher(const std::string& fileName)
{
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);
	glm::ivec2 nnodes(13, 6);

	double radius = 2.0;
	double height = 8.0;

	CylinderMesher::writeNodes(pos, radius, height, 0.0, 1.9*glm::pi<double>(), nnodes, direction::x);
	CylinderMesher::writeElements(nnodes, false);

	pos.x += height * 2.1;
	CylinderMesher::writeNodesX(pos, radius, height, 0.0, 1.0*glm::pi<double>() / 2.0, nnodes);
	CylinderMesher::writeElements(nnodes, false);

	pos.x += height * 2.1;
	CylinderMesher::writeNodesY(pos, radius, height, 0.0, 1.0*glm::pi<double>() / 4.0, nnodes);
	CylinderMesher::writeElements(nnodes, false);

	pos.x += height * 2.1;
	CylinderMesher::writeNodesY(pos, radius, height, -1, -1, nnodes);
	CylinderMesher::writeElements(nnodes, true);

	return 0;
}

int cylinderMesherRef(const std::string& fileName) 
{
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);
	int nRef = 4;
	int nNodesEdge = std::pow(2, nRef + 3) + 1;

	double radius = 2.0;
	double height = 4.0;

	CylinderMesherRef::writeNodes(pos, nNodesEdge, nRef, radius, height, 0.0, 1.9*glm::pi<double>(), direction::x);
	CylinderMesherRef::writeElements(nNodesEdge, nRef, false);

	pos.x += height * 1.1;
	CylinderMesherRef::writeNodes(pos, nNodesEdge, nRef, radius, height, 0.0, 1.0*glm::pi<double>() / 2.0, direction::y);
	CylinderMesherRef::writeElements(nNodesEdge, nRef, false);

	pos.x += height * 1.1;
	CylinderMesherRef::writeNodes(pos, nNodesEdge, nRef, radius, height, 0.0, 1.0*glm::pi<double>() / 4.0, direction::z);
	CylinderMesherRef::writeElements(nNodesEdge, nRef, false);

	nNodesEdge = std::pow(2, nRef + 5);
	pos.x += height * 1.1;
	CylinderMesherRef::writeNodes(pos, nNodesEdge, nRef, radius, height, -1, -1,  direction::z);
	CylinderMesherRef::writeElements(nNodesEdge, nRef, true);

	pos.x += height * 1.1;
	CylinderMesherRef::writeNodes(pos, nNodesEdge, nRef, radius, height, -1, -1, direction::z);
	CylinderMesherRef::writeElements(nNodesEdge, nRef, true);
	return 0;

}

int cuboidMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	glm::dvec3 size(20.0, 30.0, 100.0);
	glm::ivec3 nnodes(5, 5, 10);

	CuboidMesher::writeNodes(pos, size, nnodes, plane::xy);
	CuboidMesher::writeElements(nnodes, false);

	pos.x += size.x*1.1;
	CuboidMesher::writeNodes(pos, size, nnodes, plane::xz);
	CuboidMesher::writeElements(nnodes, false);

	pos.x += size.x*1.1;
	CuboidMesher::writeNodes(pos, size, nnodes, plane::yz);
	CuboidMesher::writeElements(nnodes, false);
	

	pos.x = 0.0;
	pos.y += size.y*1.1;
	CuboidMesher::writeNodesXYZ(pos, size, nnodes);
	CuboidMesher::writeElements(nnodes, false);


	writer.close();
	return 0;
}

int cuboidMesherRef(const std::string& fileName) {

	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	glm::dvec3 size(20.0, 30.0, 50.0);
	
	int nRef = 3;
	
	int nNodesX0 = std::pow(2, nRef + 2) + 1;	
	int nNodesY0 = nNodesX0;
	glm::ivec2 nnodesXY(nNodesX0, nNodesY0);

	CuboidMesherRef::writeNodes(pos, size, nnodesXY, nRef, false, plane::xy);
	CuboidMesherRef::writeElements(nnodesXY, nRef, false);
	
	pos.x += size.x*1.6;
	nnodesXY.y = std::pow(2, nRef + 1) + 1;
	CuboidMesherRef::writeNodes(pos, size, nnodesXY, nRef, false, plane::xy);
	CuboidMesherRef::writeElements(nnodesXY, nRef, false);

	if (true) {
		int lastNode = writer.getNextNodeID();
		int n[2] = { 1, 0 };
		for (int i = 2; i < lastNode; i++) {
			n[1] = i;
			writer.write2nodedBeam(i, n);
		}
	}

	writer.close();
	return 0;
}

int cone3Dmesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);

	glm::dvec3	  pos(0.0);
	MeshDensity3D meshDens(15, 4, 16);
	Pipe3Dradius  radius(5.0, 9.0, 3.0, 5.0);
	ArcAngles	  angle(0.0, glm::pi<double>()*1.95);
	double		  height = 32.0;

	Cone3Dmesher::writeNodes(pos, meshDens, radius, angle, height, direction::z);
	Cone3Dmesher::writeElements(meshDens, false);
	
	pos.x += height;
	radius.end.inner = radius.start.inner;
	radius.end.outer = radius.start.outer;
	angle.setFullCircle();

	Cone3Dmesher::writeNodes(pos, meshDens, radius, angle, height, direction::x);
	Cone3Dmesher::writeElements(meshDens, true);

	writer.close();
	return 0;
}

int cone3DmesherRef(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	glm::dvec3 pos(0.0);

	int nRef = 2;
	int nNodesAround = std::pow(2, nRef + 4) + 1;
	glm::ivec2 nnodes12(nNodesAround, nNodesAround);

	double radStartInner = 5.0;
	double radStartOuter = 9.0;
	double radEndInner = 1.5;
	double radEndOuter = 5.5;
	double height = 8.;

	Cone3DmesherRef::writeNodes(pos, nnodes12, nRef, radStartOuter, radEndOuter, radStartInner, radEndInner, 
		0.0, glm::pi<double>(), height,	direction::z);
	Cone3DmesherRef::writeElements(nnodes12, nRef, false);
	
	pos.x += radStartOuter*2.0;
	nnodes12.x--;
	nnodes12.y = std::pow(2, nRef + 2) + 1;
	Cone3DmesherRef::writeNodes(pos, nnodes12, nRef, radStartOuter, radEndOuter, radStartInner, radEndInner,
		-1, -1, height, direction::z);
	Cone3DmesherRef::writeElements(nnodes12, nRef, true);

	pos.x += radStartOuter * 2.0;
	Cone3DmesherRef::writeNodes(pos, nnodes12, nRef, radStartOuter, radEndOuter, radStartInner, radEndInner,
		-1, -1, height, direction::x);
	Cone3DmesherRef::writeElements(nnodes12, nRef, true);

	pos.x += radStartOuter * 2.0;
	Cone3DmesherRef::writeNodes(pos, nnodes12, nRef, radStartOuter, radStartOuter, radStartInner, radEndInner,
		-1, -1, height, direction::y);
	Cone3DmesherRef::writeElements(nnodes12, nRef, true);

	if(false){
		int lastNode = writer.getNextNodeID();
		int n[2] = { 1, 0 };
		for (int i = 2; i < lastNode; i++) {
			n[1] = i;
			writer.write2nodedBeam(i, n);
		}
	}

	writer.close();
	return 0;
}

int extruded2Drecs(const std::string& fileName)
{
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

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

	//file.close();
	return 0;
}

int extrude2Darc(const std::string& fileName)
{
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	glm::dmat3x3 csys1 = makeCsysMatrix(glm::dvec3(1.0, 0.0, 0.0), glm::dvec3(0.0, 1.0, 0.0));

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
	MeshRec2D mesh2D;
	mesh2D.initRectangle(glm::dvec2(4.0, 4.0), glm::ivec2(4, 6));
	mesh2D.extrudeYedgeArc(glm::pi<double>() / 1.0, 2.0, 22);
	mesh2D.writeNodes(&writer);
	mesh2D.writeElements(&writer);
	return 0;
}

int extrude2DarcMulti(const std::string& filename) 
{
	std::ofstream file;
	file.open(filename);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	Mesher::setFEAwriter(&writer);
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
	return 0;
}