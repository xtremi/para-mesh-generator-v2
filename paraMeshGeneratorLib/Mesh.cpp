#include "Mesh.h"
#include "PlaneMesher.h"
#include "ConeMesher.h"
#include "CuboidMesher.h"
#include "Cone3Dmesher.h"

Mesh2D_planeExtrusion::Mesh2D_planeExtrusion(int nElementsY, double lengthY)
	: MeshPrimitive2D(){
	setStartEdgeY(nElementsY, lengthY);
}
Mesh2D_planeExtrusion::Mesh2D_planeExtrusion() : MeshPrimitive2D() {}

Mesh3D_volumeExtrusion::Mesh3D_volumeExtrusion(const MeshDensity2D& meshDensYZface, const glm::dvec2& sizeYZ)
	: MeshPrimitive3D() {
	setStartFaceYZ(meshDensYZface, sizeYZ);
}
Mesh3D_volumeExtrusion::Mesh3D_volumeExtrusion() : MeshPrimitive3D() {}

void Mesh2D_planeExtrusion::setStartEdgeY(int nElementsY, double _lengthY) {
	lengthY = _lengthY;
	meshDensity.setDir2(nElementsY + 1);
}
void Mesh3D_volumeExtrusion::setStartFaceYZ(const MeshDensity2D& _meshDensYZface, const glm::dvec2& _sizeYZ) {
	sizeYZ = _sizeYZ;
	meshDensity.setDir2(_meshDensYZface.dir1()); //y-dir
	meshDensity.setDir3(_meshDensYZface.dir2()); //z-dir
}

void Mesh2D_planeExtrusion::extrudeYedgeArc(double endAng, double radius, int nElements) {
	MeshEdgeExtrusion* prevExtrusion = extrusionsXdir.size() > 0 ? &extrusionsXdir[extrusionsXdir.size() - 1] : nullptr;
	extrusionsXdir.push_back(MeshEdgeExtrusion(radius, endAng , nElements, meshDensity.dir2(), nNodes, prevExtrusion));
	calculateNumberOfNodes();
	calculateNumberOfElements();
}
void Mesh3D_volumeExtrusion::extrudeYZedgeArc(double endAng, double radiusInner, int nElements) {
	MeshFaceExtrusion* prevExtrusion = extrusionsXdir.size() > 0 ? &extrusionsXdir[extrusionsXdir.size() - 1] : nullptr;
	extrusionsXdir.push_back(MeshFaceExtrusion(radiusInner, endAng, nElements, 
		MeshDensity2D(meshDensity.axis(), meshDensity.norm()), nNodes, prevExtrusion));
	calculateNumberOfNodes();
	calculateNumberOfElements();
}


void Mesh2D_planeExtrusion::extrudeYedge(double length, int nElements) {
	MeshEdgeExtrusion* prevExtrusion = extrusionsXdir.size() > 0 ? &extrusionsXdir[extrusionsXdir.size() - 1] : nullptr;
	extrusionsXdir.push_back(MeshEdgeExtrusion(length, nElements, meshDensity.dir2(), nNodes, prevExtrusion));
	calculateNumberOfNodes();
	calculateNumberOfElements();
}

void Mesh3D_volumeExtrusion::extrudeYZface(double length, int nElements) {
	MeshFaceExtrusion* prevExtrusion = extrusionsXdir.size() > 0 ? &extrusionsXdir[extrusionsXdir.size() - 1] : nullptr;
	extrusionsXdir.push_back(MeshFaceExtrusion(length, nElements, meshDensity.meshDensD23(), nNodes, prevExtrusion));
	calculateNumberOfNodes();
	calculateNumberOfElements();
}

MeshEdge Mesh2D_planeExtrusion::getEdge(int section, int edgeIndex) {
	if (section < extrusionsXdir.size() && edgeIndex < 4) {
		return extrusionsXdir[section].edges[edgeIndex];
	}
	return MeshEdge();
}
MeshFace Mesh3D_volumeExtrusion::getFace(int section, int faceIndex) {
	if (section < extrusionsXdir.size() && faceIndex < 6) {
		return extrusionsXdir[section].faces[faceIndex];
	}
	return MeshFace();
}

/*

		nElements
	|---|---|---|---|
		---Edg1-->
	 x---x---x---x---x
 |	 x---x---x---x---x  |
Edg0 x---x---x---x---x Edg2
 |	 x---x---x---x---x  |
 v	 x---x---x---x---x  v
		---Edg3-->

	<-----length----->
	----> Extrusion


*/
MeshEdge_ext Mesh2D_planeExtrusion::getExtrudedEdge(int edgeIndex) {
	std::vector<NodeIterator1D> iterators;
	
	if (edgeIndex == 1 || edgeIndex == 3) {
		for (MeshEdgeExtrusion& edgeExtr : extrusionsXdir) {
			iterators.push_back(edgeExtr.edges[edgeIndex].nodeIter);
		}
	}
	else if (edgeIndex == 0) {
		iterators.push_back(extrusionsXdir[0].edges[0].nodeIter);
	}
	else if (edgeIndex == 2) {
		iterators.push_back(extrusionsXdir[extrusionsXdir.size() - 1].edges[2].nodeIter);
	}
	
	return MeshEdge_ext(NodeIterator1Dm(iterators, true));
}

/*!
	Updates the node iterators for this mesh writing:
*/
void Mesh2D_planeExtrusion::setNodeOffsetOnMeshEdgeExtrusions(int nodeIDoffset) {
	for (int i = 0; i < extrusionsXdir.size(); i++) {
		extrusionsXdir[i].setNodeOffset(nodeIDoffset);
	}
}
void Mesh3D_volumeExtrusion::setNodeOffsetOnMeshEdgeExtrusions(int nodeIDoffset) {
	for (int i = 0; i < extrusionsXdir.size(); i++) {
		extrusionsXdir[i].setNodeOffset(nodeIDoffset);
	}
}

/*

 x1 x2 x3 x4    x13  x14  x15        x22         x2
 x5 x6 x7 x8    x16  x17  x18        x24         x25
 x9 x10x11x12   x19  x20  x21        x26         x27
*/

/*
	 (default)
		1st   2nd     3rd      (extrusions)
     |-----|<---------|<---|  
 y	 x x x x   x   x   x x x
 ^	 x x x x   x   x   x x x
 |	 x x x x   x   x   x x x
 |	 x x x x   x   x   x x x
 |     3el      3el      2el
       4no      3no      2no
  --------->x


*/
void Mesh2D_planeExtrusion::writeNodes() {
	bool   firstExtrusion = true;		
	double spacingY = lengthY / (double)meshDensity.nElDir2();
	
	setNodeOffsetOnMeshEdgeExtrusions(Mesher::getWriter()->getNextNodeID());
	csys.update();

	glm::dmat3x3 rotM(1.0);
	ExtrudeStepData curExtrData;
	curExtrData.csys = MeshCsys(&csys, glm::dvec3(0.), &rotM);
	curExtrData.pos = glm::dvec3(0.0);
	curExtrData.arcAngle= 0.0;
	

	for (MeshExtrusion& extr : extrusionsXdir) {

		curExtrData.dxy = glm::dvec2(extr.spacing(), spacingY);
		curExtrData.startSpace	= 0.0;
		
		if (!firstExtrusion)
			curExtrData.startSpace = extr.spacing();

		if(extr.extrusionType == ExtrusionType::line){
			writeNodesExtrudeLine(curExtrData, extr);
		}
		else if (extr.extrusionType == ExtrusionType::arc){
			writeNodesExtrudeArc(curExtrData, extr);
		}
		curExtrData.csys.update();
		firstExtrusion = false;
	}
	
}
void Mesh3D_volumeExtrusion::writeNodes() {
	bool   firstExtrusion = true;
	glm::dvec2 spacingYZ(sizeYZ[0] / (double)meshDensity.nElDir2(), sizeYZ[1] / (double)meshDensity.nElDir3());

	setNodeOffsetOnMeshEdgeExtrusions(Mesher::getWriter()->getNextNodeID()); //to rename function
	csys.update();

	glm::dmat3x3 rotM(1.0);
	ExtrudeStepData curExtrData;
	curExtrData.csys = MeshCsys(&csys, glm::dvec3(0.), &rotM);
	curExtrData.pos = glm::dvec3(0.0);
	curExtrData.arcAngle = 0.0;


	for (MeshExtrusion& extr : extrusionsXdir) {

		curExtrData.dxyz = glm::dvec3(extr.spacing(), spacingYZ);
		curExtrData.startSpace = 0.0;
		curExtrData.nNodesEdgeX = extr.nNodes();

		if (!firstExtrusion)
			curExtrData.startSpace = extr.spacing();

		if (extr.extrusionType == ExtrusionType::line) {
			writeNodesExtrudeLine(curExtrData, extr);
		}
		else if (extr.extrusionType == ExtrusionType::arc) {
			writeNodesExtrudeArc(curExtrData, extr);
		}
		curExtrData.csys.update();
		firstExtrusion = false;
	}



}


void Mesh2D_planeExtrusion::writeNodesExtrudeLine(ExtrudeStepData& curExtrData, MeshExtrusion& curExtr) {
	curExtrData.pos = glm::dvec3(curExtrData.startSpace, 0., 0.);
	PlaneMesher::writeNodesXYq(
		curExtrData.pos, 
		curExtrData.csys, 
		((MeshEdgeExtrusion*)&curExtr)->meshDens,
		curExtrData.dxy);

	curExtrData.csys.moveInLocalCsys(glm::dvec3(curExtr.length, 0.,0.));
}
void Mesh3D_volumeExtrusion::writeNodesExtrudeLine(ExtrudeStepData& curExtrData, MeshExtrusion& curExtr) {
	curExtrData.pos = glm::dvec3(curExtrData.startSpace, 0., 0.);
	CuboidMesher::writeNodesQ(
		curExtrData.pos, 
		curExtrData.csys,
		((MeshFaceExtrusion*)&curExtr)->meshDens,
		curExtrData.dxyz,
		plane::xy);

	curExtrData.csys.moveInLocalCsys(glm::dvec3(curExtr.length, 0., 0.));
}

void Mesh2D_planeExtrusion::writeNodesExtrudeArc(ExtrudeStepData& curExtrData, MeshExtrusion& curExtr) {

	curExtrData.pos = glm::dvec3(0., 0., curExtr.radius);

	ArcAngles ang;
	ang.setStart(-curExtrData.startSpace - GLMPI);
	ang.setEnd(-(GLMPI + curExtr.endAngle));

	ConeMesher::writeNodesY(
		curExtrData.pos, 
		curExtrData.csys, 
		((MeshEdgeExtrusion*)&curExtr)->meshDens,
		Cone2Dradius(curExtr.radius, curExtr.radius),
		ang, lengthY);

	curExtrData.arcAngle += curExtr.endAngle;
	curExtrData.csys.moveInLocalCsys(coordsOnCircle(ang.end, curExtr.radius, direction::y) + glm::dvec3(0, 0, curExtr.radius));
	(*curExtrData.csys.csys) = makeCsysMatrix(Y_DIR, curExtrData.arcAngle);
}
void Mesh3D_volumeExtrusion::writeNodesExtrudeArc(ExtrudeStepData& curExtrData, MeshExtrusion& curExtr) {
	
	MeshFaceExtrusion* curFextr = (MeshFaceExtrusion*)&curExtr;
	
	curExtrData.pos = glm::dvec3(0., 0., curExtr.radius);

	ArcAngles ang;
	ang.setStart(-curExtrData.startSpace - GLMPI);
	ang.setEnd(-(GLMPI + curExtr.endAngle));

	Cone3Dmesher::writeNodes(
		curExtrData.pos, 
		curExtrData.csys, 
		((MeshFaceExtrusion*)&curExtr)->meshDens,
		Pipe3Dradius(
			curExtr.radius, curExtr.radius - sizeYZ[1],//curFextr->radiusOuter, 
			curExtr.radius, curExtr.radius - sizeYZ[1]),//curFextr->radiusOuter),
		ang, sizeYZ[0], direction::y);

	curExtrData.arcAngle += curExtr.endAngle;
	curExtrData.csys.moveInLocalCsys(coordsOnCircle(ang.end, curExtr.radius, direction::y) + glm::dvec3(0, 0, curExtr.radius));
	(*curExtrData.csys.csys) = makeCsysMatrix(Y_DIR, curExtrData.arcAngle);

}

/*

  (default)
  	1st   2nd     3rd      (extrusions)
  |-----|<---------|<---|
  x x x x   x   x   x x x
  x x x x   x   x   x x x
  x x x x   x   x   x x x
  x x x x   x   x   x x x
    3el      3el      2el
    4no      3no      2no

 x1 x2 x3 x4    x13  x14  x15        x22         x23
 x5 x6 x7 x8    x16  x17  x18        x24         x25
 x9 x10x11x12   x19  x20  x21        x26         x27

 1st
 nodeID1_side1 = x4
 nodeID1_side2 = x13

 2nd
 nodeID1_side1 = x15
 nodeID1_side2 = x22

*/
void Mesh2D_planeExtrusion::writeElements() {

	MeshEdgeExtrusion* extr = nullptr;
	
	for (int i = 0; i < extrusionsXdir.size(); i++) {
		extr = &extrusionsXdir[i];
		
		if (!extr->isStart()){
			RowMesher2D::writeElements(&extr->edges[0].nodeIter, &extr->edges[4].nodeIter);
		}
		Mesher::setNodeID1(extr->edges[4].nodeIter.first());
		PlaneMesher::writeElements(extr->meshDens);
	}
	
}

void Mesh3D_volumeExtrusion::writeElements() {

	MeshFaceExtrusion* extr = nullptr;

	for (int i = 0; i < extrusionsXdir.size(); i++) {
		extr = &extrusionsXdir[i];

		if (!extr->isStart()) {
			RowMesher3D::writeElements(&extr->faces[0].nodeIter, &extr->faces[6].nodeIter);
		}
		Mesher::setNodeID1(extr->faces[6].nodeIter.first());
		CuboidMesher::writeElements(extr->meshDens);
	}

}

/*
	Reduce 1 for each iteration to remove duplication node
	Each non start section reuses one node from the previous section

	Start with nNodesX because first section has correct number of nodes.
*/
void Mesh2D_planeExtrusion::calculateNumberOfNodesX() {
	int nNodesX = 0;
	for (MeshExtrusion& meshExtrusion : extrusionsXdir) {
		nNodesX += (meshExtrusion.nNodes());
	}
	meshDensity.setDir1(nNodesX);
}
//same as Mesh2D_planeExtrusion::calculateNumberOfNodesX
void Mesh3D_volumeExtrusion::calculateNumberOfNodesX() {
	int nNodesX = 0;
	for (MeshExtrusion& meshExtrusion : extrusionsXdir) {
		nNodesX += (meshExtrusion.nNodes());
	}
	meshDensity.setDir1(nNodesX);
}

void Mesh2D_planeExtrusion::calculateNumberOfNodes() {
	calculateNumberOfNodesX();
	nNodes = meshDensity.nNodes();
}
void Mesh3D_volumeExtrusion::calculateNumberOfNodes() {
	calculateNumberOfNodesX();
	nNodes = meshDensity.nNodes();
}

void Mesh2D_planeExtrusion::calculateNumberOfElements() {
	nElements = meshDensity.nElements();
}
void Mesh3D_volumeExtrusion::calculateNumberOfElements() {}
