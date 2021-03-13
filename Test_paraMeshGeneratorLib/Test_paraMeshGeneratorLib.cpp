#include "TestDef.h"
#include "Mesh.h"
#include <vector>

#include "LineMesher.h"
#include "ArcMesher.h"
#include "PlaneMesher.h"
#include "ConeMesher.h"

#include "CuboidMesher.h"

int lineMesher(const std::string& fileName);
int arcMesher(const std::string& fileName);
int planeMesher(const std::string& fileName);
int planeMesherRef(const std::string& fileName);
int coneMesher(const std::string& fileName);
int coneMesherRef(const std::string& fileName);

int cuboidMesher(const std::string& fileName);
int cuboidMesherRef(const std::string& fileName);

int extruded2Drecs(const std::string& fileName);
int extrude2Darc(const std::string& filename);
int extrude2DarcMulti(const std::string& filename);

std::vector<TestDef> testFunctions({

	TestDef(101, "lineMesher",		"basic meshers 2D", (testFunction)lineMesher),
	TestDef(102, "arcMesher",		"basic meshers 2D", (testFunction)arcMesher),
	TestDef(103, "planeMesher",		"basic meshers 2D", (testFunction)planeMesher),
	TestDef(104, "planeMesherRef",	"basic meshers 2D", (testFunction)planeMesherRef),
	TestDef(105, "coneMesher",		"basic meshers 2D", (testFunction)coneMesher),
	TestDef(106, "coneMesherRef",	"basic meshers 2D", (testFunction)coneMesherRef),

	TestDef(120, "cuboidMesher",	"basic meshers 3D", (testFunction)cuboidMesher),
	TestDef(121, "cuboidMesherRef",	"basic meshers 3D", (testFunction)cuboidMesherRef),

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
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);

	int nnodes = 20;
	LineMesher::writeNodesLine(w, pos, pos + glm::dvec3(10.0, 4.0, 3.0), nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	LineMesher::writeNodesLineX(w, pos, 10.0, nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	LineMesher::writeNodesLineY(w, pos, 10.0, nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	LineMesher::writeNodesLineZ(w, pos, 10.0, nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	pos.x += 20.0;
	nnodes = 10;
	LineMesher::writeNodesLineQ(w, pos, glm::dvec3(0.8, 0.3, 0.3), nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	LineMesher::writeNodesLineXq(w, pos, 0.7, nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	LineMesher::writeNodesLineYq(w, pos, 0.7, nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	LineMesher::writeNodesLineZq(w, pos, 0.7, nnodes);
	LineMesher::writeElementsLine(w, nnodes);

	w->close();
	return 0;
}


int arcMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);

	int nnodes = 11;
	double radius = 5.0;

	ArcMesher::writeNodesCircular(w, pos, radius, 0.0, glm::pi<double>(), nnodes, direction::x);
	ArcMesher::writeElementsLine(w, nnodes);

	ArcMesher::writeNodesCircular(w, pos, radius, 0.0, glm::pi<double>()*0.75, nnodes, direction::y);
	ArcMesher::writeElementsLine(w, nnodes);

	ArcMesher::writeNodesCircular(w, pos, radius, 0.0, glm::pi<double>()*0.50, nnodes, direction::z);
	ArcMesher::writeElementsLine(w, nnodes);

	pos.x += 12.0;
	ArcMesher::writeNodesCircularX(w, pos, radius, 0.0, glm::pi<double>(), nnodes);
	ArcMesher::writeElementsLine(w, nnodes);

	ArcMesher::writeNodesCircularY(w, pos, radius, 0.0, glm::pi<double>()*0.75, nnodes);
	ArcMesher::writeElementsLine(w, nnodes);
	
	ArcMesher::writeNodesCircularZ(w, pos, radius, 0.0, glm::pi<double>()*0.50, nnodes);
	ArcMesher::writeElementsLine(w, nnodes);

	pos.x += 12.0;
	double dang = glm::pi<double>() / 20.0;
	ArcMesher::writeNodesCircularQ(w, pos, radius, glm::pi<double>() / 3.0, dang, nnodes, direction::x);
	ArcMesher::writeElementsLine(w, nnodes);
																		    
	ArcMesher::writeNodesCircularQ(w, pos, radius, glm::pi<double>() / 3.0, dang, nnodes, direction::y);
	ArcMesher::writeElementsLine(w, nnodes);
																		    
	ArcMesher::writeNodesCircularQ(w, pos, radius, glm::pi<double>() / 3.0, dang, nnodes, direction::z);
	ArcMesher::writeElementsLine(w, nnodes);

	pos.x += 12.0;
	ArcMesher::writeNodesCircularXq(w, pos, radius, glm::pi<double>(), dang, nnodes);
	ArcMesher::writeElementsLine(w, nnodes);

	ArcMesher::writeNodesCircularYq(w, pos, radius, glm::pi<double>(), dang, nnodes);
	ArcMesher::writeElementsLine(w, nnodes);

	ArcMesher::writeNodesCircularZq(w, pos, radius, glm::pi<double>(), dang, nnodes);
	ArcMesher::writeElementsLine(w, nnodes);


	pos.x += 12.0;
	ArcMesher::writeNodesCircular_nth(w, pos, radius, glm::pi<double>(), glm::pi<double>()*2.0, nnodes, 5, direction::x);
	ArcMesher::writeElementsLine(w, nnodes - 2-1);

	pos.x += 2.0;
	ArcMesher::writeNodesCircular_nth(w, pos, radius, glm::pi<double>(), glm::pi<double>()*2.0, nnodes, 3, direction::x);
	ArcMesher::writeElementsLine(w, nnodes - 3-1);

	pos.x += 2.0;
	ArcMesher::writeNodesCircular_nth(w, pos, radius, glm::pi<double>(), glm::pi<double>()*2.0, nnodes, 7, direction::x);
	ArcMesher::writeElementsLine(w, nnodes - 1-1);
	
	w->close();
	return 0;
}

int planeMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);

	glm::ivec2 nnodes(10, 13);
	glm::dvec2 size(20.0, 17.0);
	
	PlaneMesher::writeNodesPlane(w, pos, size, nnodes, plane::xy);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlane(w, pos, size, nnodes, plane::xz);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlane(w, pos, size, nnodes, plane::yz);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	
	pos.x += 25.0;
	PlaneMesher::writeNodesPlaneXY(w, pos, size, nnodes);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlaneXZ(w, pos, size, nnodes);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlaneYZ(w, pos, size, nnodes);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	
	pos.x += 25.0;
	glm::dvec2 elsize(1.0, 1.3);
	PlaneMesher::writeNodesPlaneQ(w, pos, elsize, nnodes, plane::xy);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlaneQ(w, pos, elsize, nnodes, plane::xz);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlaneQ(w, pos, elsize, nnodes, plane::yz);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	
	pos.x += 25.0;
	PlaneMesher::writeNodesPlaneXYq(w, pos, elsize, nnodes);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlaneXZq(w, pos, elsize, nnodes);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	PlaneMesher::writeNodesPlaneYZq(w, pos, elsize, nnodes);
	PlaneMesher::writeElementsPlane(w, nnodes, false);
	w->close();
	return 0;
}


int planeMesherRef(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);	
	glm::dvec2 size(20.0, 17.0);
	int nRef = 3;
	int nNodesY0 = std::pow(2, nRef + 1) + 1;

	PlaneMesherRef::writeNodesPlane_ref(w, pos, size, nNodesY0, nRef, true, plane::xy);
	PlaneMesherRef::writeElementsPlane_ref(w, nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlane_ref(w, pos, size, nNodesY0, nRef, true, plane::xz);
	PlaneMesherRef::writeElementsPlane_ref(w, nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlane_ref(w, pos, size, nNodesY0, nRef, true, plane::yz);
	PlaneMesherRef::writeElementsPlane_ref(w, nNodesY0, nRef, false);

	nRef = 5;
	pos.x += 24.0;
	nNodesY0 = std::pow(2, nRef + 1) + 1;
	size = glm::dvec2(15.0, 7.5);
	PlaneMesherRef::writeNodesPlaneXY_ref(w, pos, size, nNodesY0, nRef, false);
	PlaneMesherRef::writeElementsPlane_ref(w, nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlaneXZ_ref(w, pos, size, nNodesY0, nRef, false);
	PlaneMesherRef::writeElementsPlane_ref(w, nNodesY0, nRef, false);
	PlaneMesherRef::writeNodesPlaneYZ_ref(w, pos, size, nNodesY0, nRef, false);
	PlaneMesherRef::writeElementsPlane_ref(w, nNodesY0, nRef, false);

	w->close();
	return 0;
}
int coneMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);

	glm::ivec2 nnodes(10, 13);
	double height	= 4.0;
	double radiusStart = 4.0;
	double radiusEnd = 2.0;


	//Quarter cone:	
	ConeMesher::writeNodesCone(w, pos, radiusStart, radiusEnd, 0.0, 1.0*glm::pi<double>() / 2.0, height, nnodes, direction::x);
	ConeMesher::writeElementsCone(w, nnodes, false);
	pos.x += 8.0;
	//Almost full cone:
	ConeMesher::writeNodesCone(w, pos, radiusStart, radiusEnd, 0.0, 1.99*glm::pi<double>() / 2.0, height, nnodes, direction::y);
	ConeMesher::writeElementsCone(w, nnodes, false);
	pos.x += 8.0;
	//Full cone:
	ConeMesher::writeNodesCone(w, pos, radiusStart, radiusEnd, -1, -1, height, nnodes, direction::z);
	ConeMesher::writeElementsCone(w, nnodes, true);

	pos.y += 8.0;
	pos.x = 0.0;
	nnodes.x *= 2;
	nnodes.y /= 2;
	//Almost full disk:
	ConeMesher::writeNodesCone(w, pos, radiusStart, radiusEnd, 0.0, 1.8*glm::pi<double>(), 0.0, nnodes, direction::x);
	ConeMesher::writeElementsCone(w, nnodes, false);
	pos.x += 8.0;

	//Full disk:
	ConeMesher::writeNodesCone(w, pos, radiusEnd, radiusStart, -1, -1, 0.0, nnodes, direction::z);
	ConeMesher::writeElementsCone(w, nnodes, true);

	pos.y += 8.0;
	pos.x = 0.0;

	//Full Cylinder:
	ConeMesher::writeNodesCone(w, pos, radiusStart, radiusStart, -1, -1, radiusStart*2.0, nnodes, direction::x);
	ConeMesher::writeElementsCone(w, nnodes, true);
	pos.x += 18.0;

	//Half cylinder:
	ConeMesher::writeNodesCone(w, pos, radiusStart, radiusStart, 1.2*glm::pi<double>(), 0.0, radiusStart*2.0, nnodes, direction::z);
	ConeMesher::writeElementsCone(w, nnodes, false);

	w->close();
	return 0;
}
int coneMesherRef(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);

	double height = 4.0;
	double radiusStart = 4.0;
	double radiusEnd = 2.0;
	
	int nRef = 4;
	int nNodesY0 = std::pow(2, nRef + 3) + 1;

	//Quarter cone:	
	ConeMesherRef::writeNodesCone_ref(w, pos, nNodesY0, nRef, radiusStart, radiusEnd, 0.0, 1.0*glm::pi<double>() / 2.0, height, direction::x);
	ConeMesherRef::writeElementsCone_ref(w, nNodesY0, nRef, false);
	pos.x += 8.0;
	//Almost full cone:
	ConeMesherRef::writeNodesCone_ref(w, pos, nNodesY0, nRef, radiusStart, radiusEnd, 0.0, 1.99*glm::pi<double>() / 2.0, height, direction::y);
	ConeMesherRef::writeElementsCone_ref(w, nNodesY0, nRef, false);
	pos.x += 8.0;
	//Full cone:
	ConeMesherRef::writeNodesCone_ref(w, pos, nNodesY0 - 1, nRef, radiusStart, radiusEnd, -1, -1, height, direction::z);
	ConeMesherRef::writeElementsCone_ref(w, nNodesY0 - 1, nRef, true);

	pos.y += 8.0;
	pos.x = 0.0;

	//Almost full disk:
	ConeMesherRef::writeNodesCone_ref(w, pos, nNodesY0, nRef, radiusStart, radiusEnd, 0.0, 1.8*glm::pi<double>(), 0.0, direction::x);
	ConeMesherRef::writeElementsCone_ref(w, nNodesY0, nRef, false);
	pos.x += 8.0;

	//Full disk:
	ConeMesherRef::writeNodesCone_ref(w, pos, nNodesY0 - 1, nRef, radiusEnd, radiusStart, -1, -1, 0.0, direction::z);
	ConeMesherRef::writeElementsCone_ref(w, nNodesY0 - 1, nRef, true);

	pos.y += 8.0;
	pos.x = 0.0;

	//Full Cylinder:
	ConeMesherRef::writeNodesCone_ref(w, pos, nNodesY0 - 1, nRef, radiusStart, radiusStart, -1, -1, radiusStart*2.0,  direction::x);
	ConeMesherRef::writeElementsCone_ref(w, nNodesY0 - 1, nRef, true);
	pos.x += 18.0;

	//Half cylinder:
	ConeMesherRef::writeNodesCone_ref(w, pos, nNodesY0, nRef, radiusStart, radiusStart, 1.2*glm::pi<double>(), 0.0, radiusStart*2.0, direction::z);
	ConeMesherRef::writeElementsCone_ref(w, nNodesY0, nRef, false);

	w->close();
	return 0;

}


int cuboidMesher(const std::string& fileName) {
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);

	glm::dvec3 size(20.0, 30.0, 100.0);
	glm::ivec3 nnodes(5, 5, 10);

	CuboidMesher::writeNodes(w, pos, size, nnodes, plane::xy);
	CuboidMesher::writeElements(w, nnodes, false);

	pos.x += size.x*1.1;
	CuboidMesher::writeNodes(w, pos, size, nnodes, plane::xz);
	CuboidMesher::writeElements(w, nnodes, false);

	pos.x += size.x*1.1;
	CuboidMesher::writeNodes(w, pos, size, nnodes, plane::yz);
	CuboidMesher::writeElements(w, nnodes, false);
	

	pos.x = 0.0;
	pos.y += size.y*1.1;
	CuboidMesher::writeNodesXYZ(w, pos, size, nnodes);
	CuboidMesher::writeElements(w, nnodes, false);


	w->close();
	return 0;
}

int cuboidMesherRef(const std::string& fileName) {

	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) return 1;

	NastranFEAwriter writer(&file);
	NastranFEAwriter* w = &writer;
	glm::dvec3 pos(0.0);

	glm::dvec3 size(20.0, 30.0, 50.0);
	
	int nRef = 4;
	int nNodesY0 = std::pow(2, nRef+1) + 1;
	glm::ivec2 nnodesXY(nNodesY0, nNodesY0);

	CuboidMesher::writeNodesRef(w, pos, size, nnodesXY, nRef, false, plane::xy);
	CuboidMesher::writeElementsRef(w, nnodesXY, nRef, false);

	int elID = 1; int n[2] = { 1,0 };
	for (int nodeID = 2; nodeID < w->getNextNodeID(); nodeID++) {
		n[1] = nodeID;
		writer.write2nodedBeam(elID++, n);
	}

	w->close();
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