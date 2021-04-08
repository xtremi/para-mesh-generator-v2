#include "Mesh.h"
#include "PlaneMesher.h"
#include "ConeMesher.h"

void MeshRec2D::initRectangle(const glm::dvec2& _sizeXY, const glm::ivec2& _nNodesXY) {
	lengthY = _sizeXY.y;
	nNodesY = _nNodesXY.y;
	extrusionsXdir.push_back(MeshEdgeExtrusion(_sizeXY.x, _nNodesXY.x - 1, nNodesY, nNodes, true));

	calculateNumberOfNodes();
	calculateNumberOfElements();
}

void MeshRec2D::extrudeYedgeArc(double endAng, double radius, int nElements) {
	extrusionsXdir.push_back(MeshEdgeExtrusion(radius, endAng , nElements, nNodesY, nNodes));
	calculateNumberOfNodes();
	calculateNumberOfElements();

}

void MeshRec2D::extrudeYedge(double length, int nElements) {
	extrusionsXdir.push_back(MeshEdgeExtrusion(length, nElements, nNodesY, nNodes));
	calculateNumberOfNodes();
	calculateNumberOfElements();
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
void MeshRec2D::writeNodes(FEAwriter* writer) {

	int nodeID    = nodeID1;
	glm::dvec3 currentPos = pos;

	double spacingY = lengthY / (double)(nNodesY - 1);

	bool firstExtrusion = true;
	for (MeshExtrusion& extr : extrusionsXdir) {

		glm::dvec2 dxy(extr.spacing(), spacingY);

		double startSpace = 0.0;

		int nNodesEdgeX = extr.nNodes();
		if (!firstExtrusion)
			startSpace = extr.spacing();

		glm::ivec2 nodesPerEdge(nNodesEdgeX, nNodesY);
		
		if(extr.extrusionType == ExtrusionType::line){
			glm::dvec3 cpos = currentPos + glm::dvec3(startSpace, 0.0, 0.0);
			PlaneMesher::writeNodesXYq(cpos, dxy, nodesPerEdge, csys);
			//nodeID = writer->writeNodesXY(currentPos + glm::dvec3(startSpace, 0.0, 0.0), dxy, nodesPerEdge, nodeID, csys);
			currentPos.x += (extr.length);
		}
		else if (extr.extrusionType == ExtrusionType::arc){
			glm::dvec3 cpos = currentPos - glm::dvec3(0.0, 0.0, extr.radius);
			ConeMesher::writeNodesY(cpos, extr.radius, extr.radius, startSpace, extr.endAngle, lengthY, nodesPerEdge);
			//nodeID = writer->writeNodesConeY(cpos, extr.radius, extr.radius, startSpace, extr.endAngle, lengthY, nodesPerEdge, nodeID);
			double dx = extr.radius * glm::sin(extr.endAngle);
			double dy = extr.radius * glm::cos(extr.endAngle);
			currentPos = glm::dvec3(extr.radius * glm::sin(extr.endAngle), 0.0, extr.radius * glm::cos(extr.endAngle)) + cpos;
		}

		firstExtrusion = false;
	}
	
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
void MeshRec2D::writeElements(FEAwriter* writer) {

	int elID = elementID1;
	int nodeID = nodeID1;

	MeshEdgeExtrusion* extr = nullptr, *prevExtr = nullptr;

	for (int i = 0; i < extrusionsXdir.size(); i++) {
		extr = &extrusionsXdir[i];
		extr->setNodeOffset(nodeID1);
		
		if (!extr->isStart){
			elID = writer->writeElementRow(&prevExtr->edges[2].nodeIter, &extr->edges[0].nodeIter, elID);

		}
		nodeID = extr->edges[0].nodeIter.first();
		PlaneMesher::writeElements(glm::ivec2(extr->nNodes(), nNodesY), nodeID);
		elID = writer->getNextElementID();
		//elID = writer->writeElements(glm::ivec2(extr->nNodes(), nNodesY), nodeID, elID);
		
		prevExtr = extr;
	}
	
}

void MeshRec2D::calculateNumberOfNodesX() {
	nNodesX = 0;
	for (MeshExtrusion& meshExtrusion : extrusionsXdir) {
		nNodesX += meshExtrusion.nNodes();
	}
}

void MeshRec2D::calculateNumberOfNodes() {
	calculateNumberOfNodesX();
	nNodes = nNodesX * nNodesY;
}
void MeshRec2D::calculateNumberOfElements() {
	nElements = (nNodesX - 1) * (nNodesY - 1);
}

