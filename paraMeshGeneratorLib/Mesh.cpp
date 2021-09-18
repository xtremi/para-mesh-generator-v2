#include "Mesh.h"
#include "PlaneMesher.h"
#include "ConeMesher.h"

Mesh2D_planeExtrusion::Mesh2D_planeExtrusion(int nElementsY, double lengthY)
	: MeshPrimitive2D(){
	setStartEdgeY(nElementsY, lengthY);
}
Mesh2D_planeExtrusion::Mesh2D_planeExtrusion() : MeshPrimitive2D() {}

void Mesh2D_planeExtrusion::setStartEdgeY(int nElementsY, double _lengthY) {
	lengthY = _lengthY;
	nNodesY = nElementsY + 1;
}

void Mesh2D_planeExtrusion::extrudeYedgeArc(double endAng, double radius, int nElements) {
	MeshEdgeExtrusion* prevExtrusion = extrusionsXdir.size() > 0 ? &extrusionsXdir[extrusionsXdir.size() - 1] : nullptr;

	extrusionsXdir.push_back(MeshEdgeExtrusion(radius, endAng , nElements, nNodesY, nNodes, prevExtrusion));
	calculateNumberOfNodes();
	calculateNumberOfElements();

}

void Mesh2D_planeExtrusion::extrudeYedge(double length, int nElements) {
	
	MeshEdgeExtrusion* prevExtrusion = extrusionsXdir.size() > 0 ? &extrusionsXdir[extrusionsXdir.size() - 1] : nullptr;
	extrusionsXdir.push_back(MeshEdgeExtrusion(length, nElements, nNodesY, nNodes, prevExtrusion));

	calculateNumberOfNodes();
	calculateNumberOfElements();
}

MeshEdge Mesh2D_planeExtrusion::getEdge(int section, int edgeIndex) {
	if (section < extrusionsXdir.size() && edgeIndex < 4) {
		return extrusionsXdir[section].edges[edgeIndex];
	}
	return MeshEdge();
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
	double spacingY = lengthY / (double)(nNodesY - 1);
	
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
		curExtrData.nNodesEdgeX = extr.nNodes();
		
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

void Mesh2D_planeExtrusion::writeNodesExtrudeLine(ExtrudeStepData& curExtrData, MeshExtrusion& curExtr) {
	curExtrData.pos = glm::dvec3(curExtrData.startSpace, 0., 0.);
	PlaneMesher::writeNodesXYq(curExtrData.pos, curExtrData.csys, MeshDensity2D(curExtrData.nNodesEdgeX, nNodesY), curExtrData.dxy);
	curExtrData.csys.moveInLocalCsys(glm::dvec3(curExtr.length, 0.,0.));
}
void Mesh2D_planeExtrusion::writeNodesExtrudeArc(ExtrudeStepData& curExtrData, MeshExtrusion& curExtr) {

	curExtrData.pos = glm::dvec3(0., 0., curExtr.radius);

	ArcAngles ang;
	ang.setStart(-curExtrData.startSpace - GLMPI);
	ang.setEnd(-(GLMPI + curExtr.endAngle));

	ConeMesher::writeNodesY(curExtrData.pos, curExtrData.csys, MeshDensity2D(curExtrData.nNodesEdgeX, nNodesY),
		Cone2Dradius(curExtr.radius, curExtr.radius), ang, lengthY);

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

	MeshEdgeExtrusion* extr = nullptr;// , *prevExtr = nullptr;
	MeshDensity2D meshDens(0, nNodesY);
	
	for (int i = 0; i < extrusionsXdir.size(); i++) {
		extr = &extrusionsXdir[i];
		
		if (!extr->isStart()){
			RowMesher2D::writeElements(&extr->edges[0].nodeIter, &extr->edges[4].nodeIter);
		}
		Mesher::setNodeID1(extr->edges[4].nodeIter.first());
		meshDens.setDir1(extr->nNodes());
		PlaneMesher::writeElements(meshDens);
		
	}
	
}

/*
	Reduce 1 for each iteration to remove duplication node
	Each non start section reuses one node from the previous section

	Start with nNodesX because first section has correct number of nodes.
*/
void Mesh2D_planeExtrusion::calculateNumberOfNodesX() {
	nNodesX = 0;
	for (MeshExtrusion& meshExtrusion : extrusionsXdir) {
		nNodesX += (meshExtrusion.nNodes());
	}
}

void Mesh2D_planeExtrusion::calculateNumberOfNodes() {
	calculateNumberOfNodesX();
	nNodes = nNodesX * nNodesY;
}
void Mesh2D_planeExtrusion::calculateNumberOfElements() {
	nElements = (nNodesX - 1) * (nNodesY - 1);
}

