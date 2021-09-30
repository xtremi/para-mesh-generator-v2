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
	double				 _radiusOuter,
	double				 _endAngle,
	int					 _nElements,
	const MeshDensity2D& face0nodes,
	int					 firstNodeID,
	MeshExtrusion*		 previousExtrusion)
	: MeshExtrusion(_radiusInner, _endAngle, _nElements, previousExtrusion)
{
	initFaces(face0nodes, firstNodeID, previousExtrusion);
	radiusOuter = _radiusOuter;
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
	meshDens = MeshDensity2D(nElements + 1, nnodesExtr);

	//the end nodes of the previous extrusion
	int preNodeEdge1 = 0, preNodeEdge2 = 0;
	if (!isStart()) {
		preNodeEdge1 = ((MeshEdgeExtrusion*)previousExtrusion)->endCornerNode1();
		preNodeEdge2 = ((MeshEdgeExtrusion*)previousExtrusion)->endCornerNode2();
		preNodeEdge1 = ((MeshEdgeExtrusion*)previousExtrusion)->meshDens.cornerNode(2);
		preNodeEdge1 = ((MeshEdgeExtrusion*)previousExtrusion)->meshDens.cornerNode(3);
	}

	/*								firstNodeID							nNodes      nodeIncr		preNode*/
	edges[4] = MeshEdge(NodeIterator1D(0,								nnodeEdge1, nnodesExtr));
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

/*

Example: 
	Face YZ = 4x3 nodes 
	Extrude = 3 nodes

	D1(y)   = 4		start3 = (D1 - 1)*extr
	D2(z)   = 3		start5 = (D2 - 1)*faceXY
	extr(x) = 3	

	faceF4 = extr * D1

 ________________________________________________________________
| Face	| start	| nodesX	| nodesY	| incrX		| incrY		|
|---------------------------------------------------------------|
|	0	|	1	|	D1		|	D2		|	extr	|	faceF4	|
|	1	|	1	|	extr	|	D2		|	1   	|	faceF4	|	
|	2	|extr(3)|	D1		|	D2		|	extr	|	faceF4	|	
|	3	|S3(10) |	extr	|	D2		|	1   	|	faceF4	|	
|	4	|	1	|	extr	|	D1		|	1   	|	extr	|	
|	5	|S5(25) |	extr	|	D1		|	1   	|	extr	|	
|----------------------------------------------------------------


*/
void MeshFaceExtrusion::initFaces(const MeshDensity2D& face0nodes, int firstNodeID, MeshExtrusion* prevExtr) {
	int nExtr = nNodes();


	int nD1    = face0nodes.dir1();
	int nD2    = face0nodes.dir2();
	int nFace4 = nExtr * nD1;
	int S3	   = (nD1 - 1)*nExtr;
	int S5	   = (nD2 - 1)*nFace4;
	
	NodeIterator1D prevIterEdg0, prevIterEdg1, prevIterEdg2, prevIterEdg3;
	if(prevExtr){
		prevIterEdg0 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator0();
		prevIterEdg1 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator1();
		prevIterEdg2 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator2();
		prevIterEdg3 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator3();
	}
		
	/*						firstNodeID			nNodesX	nNodesZ	nodeIncrX	nodeIncrZ	preNode*/
	faces[6] = MeshFace(NodeIterator2D(0,		nD1,	nD2,	nExtr,		nFace4));	
	faces[1] = MeshFace(NodeIterator2D(0,		nExtr,	nD2,	1,			nFace4,		prevIterEdg1));
	faces[2] = MeshFace(NodeIterator2D(nExtr-1,	nD1,	nD2,	nExtr,		nFace4));
	faces[3] = MeshFace(NodeIterator2D(S3,		nExtr,	nD2,	1,			nFace4,		prevIterEdg3));
	faces[4] = MeshFace(NodeIterator2D(0,		nExtr,	nD1,	1,			nExtr,		prevIterEdg0));
	faces[5] = MeshFace(NodeIterator2D(S5,		nExtr,	nD1,	1,			nExtr,		prevIterEdg2));
	
	if (isStart()) { faces[0] = faces[6]; }
		
	endEdgeIterators[0] = NodeIterator1D(nExtr - 1,					 nD1,	nExtr);
	endEdgeIterators[1] = NodeIterator1D(nExtr - 1,					 nD2,	nFace4);
	endEdgeIterators[2] = NodeIterator1D(endEdgeIterators[1].last(), nD1,	nExtr);
	endEdgeIterators[3] = NodeIterator1D(endEdgeIterators[0].last(), nD2,	nFace4);
	
	addToFirstNodeID(firstNodeID);

	//Should not have firstNodeID added on this edge:
	if (!isStart()) {
		faces[0] = ((MeshFaceExtrusion*)prevExtr)->faces[2];
	}
}
