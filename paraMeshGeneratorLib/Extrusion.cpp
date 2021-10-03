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


MeshFaceExtrusion::MeshFaceExtrusion(
	double				 _length,
	int					 _nElements,
	const MeshDensity2D& face0nodes,
	int					 firstNodeID,
	MeshExtrusion*		 previousExtrusion)
	: MeshExtrusion(_length, _nElements, previousExtrusion)
{
	initFaces(face0nodes, firstNodeID, previousExtrusion);
}

MeshFaceExtrusion::MeshFaceExtrusion(
	double				 _radiusInner,
	double				 _endAngle,
	int					 _nElements,
	const MeshDensity2D& face0nodes,
	int					 firstNodeID,
	MeshExtrusion*		 previousExtrusion)
	: MeshExtrusion(_radiusInner, _endAngle, _nElements, previousExtrusion)
{
	initFaces(face0nodes, firstNodeID, previousExtrusion);
}

/*!
	This offset allows to reuse the extrusion with different nodeIDs
	The offset should normally be equal to the firstNodeID of its Mesh
*/
void MeshEdgeExtrusion::setNodeOffset(int nOffs) {
	for (MeshEdge& edg : edges)
		edg.nodeIter.nodeIDoffset = nOffs;
}
void MeshFaceExtrusion::setNodeOffset(int nOffs) {
	for (MeshFace& face : faces)
		face.nodeIter.nodeIDoffset = nOffs;
}

void MeshEdgeExtrusion::addToFirstNodeID(int n) {
	for (MeshEdge& edg : edges)
		edg.nodeIter.firstNodeID += n;
}
void MeshFaceExtrusion::addToFirstNodeID(int n) {
	for (MeshFace& face : faces)
		face.nodeIter.firstNodeID += n;
	for (NodeIterator1D& nIt : endEdgeIterators)
		nIt.firstNodeID += n;
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
x5  x6  x7  x8
x1  x2  x3  x4


 ------------------

normal extrusion:

nElements = 3
nNodes    = 3
-   x5  x6  x7
-   x1  x2  x3



  initial  second         third
  extr.    extrusion      extrusion
|--------|<------------|<-----------------|
|        |             |                  |
         
x--x--x--o2-----x------o2-----x---x---x---o2
|  |  |  |      |      |      |   |   |   |
x--x--x--x------x------x------x---x---x---x
|  |  |  |      |      |      |   |   |   |
x--x--x--o1-----x------o1-----x---x---x---o1

o1/o2 - preNode1 / preNode2 


*/
void MeshEdgeExtrusion::initEdges(int nnodeEdge1, int firstNodeID, MeshExtrusion* previousExtrusion) {
	int nnodesExtr = nNodes();
	meshDens = MeshDensity2D(nnodesExtr, nnodeEdge1);

	//the end nodes of the previous extrusion
	int preNodeEdge1 = 0, preNodeEdge2 = 0;
	if (!isStart()) {
		preNodeEdge1 = ((MeshEdgeExtrusion*)previousExtrusion)->endCornerNode1();
		preNodeEdge2 = ((MeshEdgeExtrusion*)previousExtrusion)->endCornerNode2();
	}
		
	edges[1] = MeshEdge(meshDens.edgeNodeIterator(1, firstNodeID, preNodeEdge1));
	edges[2] = MeshEdge(meshDens.edgeNodeIterator(2, firstNodeID));
	edges[3] = MeshEdge(meshDens.edgeNodeIterator(3, firstNodeID, preNodeEdge2));
	edges[4] = MeshEdge(meshDens.edgeNodeIterator(0, firstNodeID));
	if (isStart()) {
		edges[0] = edges[4];
	}
	else {
		edges[0] = ((MeshEdgeExtrusion*)previousExtrusion)->edges[2];
	}
}

/*
   ^ Z-dir
   |
   x-----2----x
   |		  |
   3		  1
   |          |
   x-----0----x ----> Y-dir
        
		x X-dir

*/
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator0(){
	return endEdgeIterators[0];
}
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator1() {
	return endEdgeIterators[1];
}
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator2(){
	return endEdgeIterators[2];
}
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator3(){
	return endEdgeIterators[3];
}


void MeshFaceExtrusion::initFaces(const MeshDensity2D& face0nodes, int firstNodeID, MeshExtrusion* prevExtr) {
	int nExtr = nNodes();

	meshDens = MeshDensity3D(nExtr, face0nodes.dir1(), face0nodes.dir2());

	NodeIterator1D prevIterEdg0, prevIterEdg1, prevIterEdg2, prevIterEdg3;
	if (prevExtr) {
		prevIterEdg0 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator0();
		prevIterEdg1 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator1();
		prevIterEdg2 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator2();
		prevIterEdg3 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator3();
	}


	faces[6] = MeshFace(meshDens.faceNodeIterator(0, firstNodeID));
	faces[1] = MeshFace(meshDens.faceNodeIterator(1, firstNodeID, prevIterEdg1));
	faces[2] = MeshFace(meshDens.faceNodeIterator(2, firstNodeID));
	faces[3] = MeshFace(meshDens.faceNodeIterator(3, firstNodeID, prevIterEdg3));
	faces[4] = MeshFace(meshDens.faceNodeIterator(4, firstNodeID, prevIterEdg0));
	faces[5] = MeshFace(meshDens.faceNodeIterator(5, firstNodeID, prevIterEdg2));
	
	if (isStart()) {
		faces[0] = faces[6];
	}
	else {
		faces[0] = ((MeshFaceExtrusion*)prevExtr)->faces[2];
	}

	int nD1 = face0nodes.dir1();
	int nD2 = face0nodes.dir2();
	int nFace4 = nExtr * nD1;
	endEdgeIterators[0] = NodeIterator1D(firstNodeID + nExtr - 1, nD1, nExtr);
	endEdgeIterators[1] = NodeIterator1D(firstNodeID + nExtr - 1, nD2, nFace4);
	endEdgeIterators[2] = NodeIterator1D(endEdgeIterators[1].last(), nD1, nExtr);
	endEdgeIterators[3] = NodeIterator1D(endEdgeIterators[0].last(), nD2, nFace4);

}
