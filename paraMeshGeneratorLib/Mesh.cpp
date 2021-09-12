#include "Mesh.h"
#include "PlaneMesher.h"
#include "ConeMesher.h"

void MeshRec2D_ext::initRectangle(const glm::dvec2& _sizeXY, const glm::ivec2& _nNodesXY) {
	lengthY = _sizeXY.y;
	nNodesY = _nNodesXY.y;
	extrusionsXdir.push_back(MeshEdgeExtrusion(_sizeXY.x, _nNodesXY.x - 1, nNodesY, nNodes));

	calculateNumberOfNodes();
	calculateNumberOfElements();
}

void MeshRec2D_ext::extrudeYedgeArc(double endAng, double radius, int nElements) {
	extrusionsXdir.push_back(MeshEdgeExtrusion(radius, endAng , nElements, nNodesY, nNodes, &extrusionsXdir[extrusionsXdir.size() - 1]));
	calculateNumberOfNodes();
	calculateNumberOfElements();

}

void MeshRec2D_ext::extrudeYedge(double length, int nElements) {
	extrusionsXdir.push_back(MeshEdgeExtrusion(length, nElements, nNodesY, nNodes, &extrusionsXdir[extrusionsXdir.size() - 1]));
	calculateNumberOfNodes();
	calculateNumberOfElements();
}

MeshEdge MeshRec2D_ext::getEdge(int section, int edgeIndex) {
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
MeshEdge_ext MeshRec2D_ext::getExtrudedEdge(int edgeIndex) {
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
	
	return MeshEdge_ext(iterators);
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
void MeshRec2D_ext::writeNodes() {
	bool firstExtrusion = true;		
	double spacingY = lengthY / (double)(nNodesY - 1);
	
	//Updates the node iterators for this mesh writing:
	for (int i = 0; i < extrusionsXdir.size(); i++) {
		extrusionsXdir[i].setNodeOffset(Mesher::getWriter()->getNextNodeID());
	}

	glm::dvec3	currentPos(0.0);
	for (MeshExtrusion& extr : extrusionsXdir) {

		glm::dvec2	dxy(extr.spacing(), spacingY);
		double		startSpace	= 0.0;
		int			nNodesEdgeX = extr.nNodes();
		
		if (!firstExtrusion)
			startSpace = extr.spacing();

		if(extr.extrusionType == ExtrusionType::line){
			currentPos.x += startSpace;
			
			PlaneMesher::writeNodesXYq(currentPos, csys, MeshDensity2D(nNodesEdgeX, nNodesY), dxy);
			currentPos.x += extr.length;
		}
		else if (extr.extrusionType == ExtrusionType::arc){
			glm::dvec3 cpos = currentPos - glm::dvec3(0.0, 0.0, extr.radius);
			//ConeMesher::writeNodesY(cpos, extr.radius, extr.radius, startSpace, extr.endAngle, lengthY, nodesPerEdge);
			//ConeMesher::writeNodesY(MeshCsys(cpos), extr.radius, extr.radius, startSpace, extr.endAngle, lengthY, nodesPerEdge);
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
void MeshRec2D_ext::writeElements() {

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
void MeshRec2D_ext::calculateNumberOfNodesX() {
	nNodesX = 0;
	for (MeshExtrusion& meshExtrusion : extrusionsXdir) {
		nNodesX += (meshExtrusion.nNodes());
	}
}

void MeshRec2D_ext::calculateNumberOfNodes() {
	calculateNumberOfNodesX();
	nNodes = nNodesX * nNodesY;
}
void MeshRec2D_ext::calculateNumberOfElements() {
	nElements = (nNodesX - 1) * (nNodesY - 1);
}

