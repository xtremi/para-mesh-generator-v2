#include "Extrusion.h"

MeshExtrusion::MeshExtrusion(double _length, int _nElements, MeshExtrusion* previousExtrusion) {
	length = _length;
	nElements = _nElements;
	extrusionType = ExtrusionType::line;
	_isStart = (bool)!previousExtrusion;
}
MeshExtrusion::MeshExtrusion(double _radius, double _endAngle, int _nElements, MeshExtrusion* previousExtrusion) {
	radius = _radius;
	endAngle = _endAngle;
	nElements = _nElements;
	extrusionType = ExtrusionType::arc;
	//length = 2.0*glm::pi<double>()*radius * (endAngle/ 2.0*glm::pi<double>());
	length = radius * endAngle;
	_isStart = (bool)!previousExtrusion;
}


double MeshExtrusion::spacing() {
	if (extrusionType == ExtrusionType::line)
		return length / (double)(nElements);
	else if (extrusionType == ExtrusionType::arc)
		return endAngle / (double)(nElements);
}

//Number of nodes along extrusion
int MeshExtrusion::nNodes() { 
	return isStart() ? (nElements + 1) : nElements; 
}


MeshEdgeExtrusion::MeshEdgeExtrusion(
	double _length,
	int	   _nElements,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshExtrusion* previousExtrusion)
: MeshExtrusion(_length, _nElements, previousExtrusion)
{
	initEdges(nnodeEdge1, firstNodeID, previousExtrusion);
}

MeshEdgeExtrusion::MeshEdgeExtrusion(
	double _radius,
	double _endAngle,
	int	   _nElements,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshExtrusion* previousExtrusion)
	: MeshExtrusion(_radius, _endAngle, _nElements, previousExtrusion)
{
	initEdges(nnodeEdge1, firstNodeID, previousExtrusion);
}

/*!
	This offset allows to reuse the extrusion with different nodeIDs
	The offset should normally be equal to the firstNodeID of its Mesh
*/
void MeshEdgeExtrusion::setNodeOffset(int nOffs) {
	for (MeshEdge& edg : edges)
		edg.nodeIter.nodeIDoffset = nOffs;
}

void MeshEdgeExtrusion::addToFirstNodeID(int n) {
	for (MeshEdge& edg : edges)
		edg.nodeIter.firstNodeID += n;
}

int MeshEdgeExtrusion::endCornerNode1() {
	return edges[2].startNode();
}
int MeshEdgeExtrusion::endCornerNode2() {
	return edges[2].endNode();
}

/*!

isStart extrusion:

nElements = 3
nNodes    = 4
x1  x2  x3  x4
x5  x6  x7  x8

 ------------------

normal extrusion:

nElements = 3
nNodes    = 3
-   x1  x2  x3
-   x5  x6  x7


  initial  second         third
  extr.    extrusion      extrusion
|--------|<------------|<-----------------|
|        |             |                  |
         
x--x--x--o1-----x------o1-----x---x---x---o1
|  |  |  |      |      |      |   |   |   |
x--x--x--x------x------x------x---x---x---x
|  |  |  |      |      |      |   |   |   |
x--x--x--o2-----x------o2-----x---x---x---o2

o1/o2 - preNode1 / preNode2 


*/
void MeshEdgeExtrusion::initEdges(int nnodeEdge1, int firstNodeID, MeshExtrusion* previousExtrusion) {
	int nnodesExtr = nNodes();
	
	//the end nodes of the previous extrusion
	int preNodeEdge1 = 0, preNodeEdge2 = 0;
	if (!isStart()) {
		//Move to function of MeshEdgeExtrusion:
		preNodeEdge1 = ((MeshEdgeExtrusion*)previousExtrusion)->endCornerNode1();
		preNodeEdge2 = ((MeshEdgeExtrusion*)previousExtrusion)->endCornerNode2();
	}

	/*								firstNodeID							nNodes      nodeIncr		preNode*/
	edges[4] =MeshEdge(NodeIterator1D(0,								nnodeEdge1, nnodesExtr));
	if(isStart()){
		edges[0] = edges[4];
	}	
	edges[1] = MeshEdge(NodeIterator1D(0,								nnodesExtr, 1,				preNodeEdge1));
	edges[2] = MeshEdge(NodeIterator1D(nnodesExtr - 1,					nnodeEdge1, nnodesExtr));
	edges[3] = MeshEdge(NodeIterator1D(((nnodeEdge1 - 1) * nnodesExtr),	nnodesExtr, 1,				preNodeEdge2));
	addToFirstNodeID(firstNodeID);
	
	//Should not have firstNodeID added on this edge:
	if (!isStart()) {
		edges[0] = ((MeshEdgeExtrusion*)previousExtrusion)->edges[2];
	}


}
