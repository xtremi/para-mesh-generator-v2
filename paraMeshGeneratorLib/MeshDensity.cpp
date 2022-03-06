#include "MeshDensity.h"
#include "Shapes.h"
#include "RefinementCalculations.h"
#include "general_utilities.h"

/*

     nnodes[0]  nnodes[1] nnodes[2]
	x---x---x---x---x---x----x----x
	|-----------|-------|---------|
	       
*/
MeshDensity1DlineStrip::MeshDensity1DlineStrip(const std::vector<int>& nodeDistr, bool _closedLoop)
	: MeshDensity1D(0, _closedLoop)
{
	nodesDistribution = nodeDistr;
	nnodes = stdVecSum(nodesDistribution) - (nodesDistribution.size() - 1);
}


int MeshDensity2D::cornerNode(int cornerID) {
	switch (cornerID)
	{
	case 0: return 0;						break;
	case 1: return dir1() - 1;				break;
	case 2: return (dir2() * dir1() - 1);		break;
	case 3: return (dir2() - 1) * dir1();	break;
	default:
		throw("Invalid corner ID in MeshDensity2D::cornerNode");
		break;
	}
}

NodeIterator1D MeshDensity2D::edgeNodeIterator(int edgeID, int firstNodeID, int preNode)
{
	if (edgeID == 0) {
		return NodeIterator1D(firstNodeID, dir2(), dir1(), preNode);
	}
	else if (edgeID == 1) {
		return NodeIterator1D(firstNodeID, dir1(), 1, preNode);
	}
	else if (edgeID == 2) {
		return NodeIterator1D(firstNodeID + cornerNode(1), dir2(), dir1(), preNode);
	}
	else if (edgeID == 3) {
		return NodeIterator1D(firstNodeID + cornerNode(3), dir1(), 1, preNode);
	}
	else {
		throw("Invalid edge ID in MeshDensity2D::edgeNodeIterator");
	}
}

MeshDensity1DlineStrip MeshDensityQuadStrip::getMesh1DlineStrip(bool closedLoop) const {
	return MeshDensity1DlineStrip(dir1nodes, closedLoop);
}
int MeshDensityQuadStrip::nQuads() const {
	return dir1nodes.size();
}

MeshDensity2DquadStrip::MeshDensity2DquadStrip(const std::vector<int>& d1, int d2, bool closedLoop)
	: MeshDensity2D(0, d2, closedLoop)
{
	dir1nodes = d1;
	setDir1(stdVecSum(dir1nodes) - (dir1nodes.size() - 1));
	if (closedLoop) {
		setDir1(dir1() - 1);
	}
}
MeshDensity1DlineStrip MeshDensity2DquadStrip::getMeshDensDir1() const {
	return getMesh1DlineStrip(closedLoop);
}


MeshDensity3DquadStrip::MeshDensity3DquadStrip(
	const std::vector<int>& d1,
	int d2, int d3, bool closedLoop)
	: MeshDensity3D(0, d2, d3, closedLoop)
{
	dir1nodes = d1;
	setDir1(stdVecSum(dir1nodes) - (dir1nodes.size() - 1));
	if (closedLoop) {
		setDir1(dir1() - 1);
	}
}
MeshDensity1DlineStrip MeshDensity3DquadStrip::getMeshDensDir1() const {
	return getMesh1DlineStrip(closedLoop);
}

int MeshDensity3D::cornerNode(int cornerID) {
	switch (cornerID)
	{
	case 0: return 0;									break;
	case 1: return dir1() * dir2() * (dir3() - 1);		break;
	case 2: return dir1() * dir2() * dir3() - dir1();	break;
	case 3: return dir1() * (dir2() - 1);	break;
	case 4: return cornerNode(0) + dir1() - 1;	break;
	case 5: return cornerNode(1) + dir1() - 1;	break;
	case 6: return cornerNode(2) + dir1() - 1;	break;
	case 7: return cornerNode(3) + dir1() - 1;	break;
	default:
		throw("Invalid corner ID in MeshDensity3D::cornerNode");
		break;
	}

}

/*

Example:
	d1 x d2 x d3 = X x Y x Z = 3x4x3 nodes

 ________________________________________________________________
| Face	| start	  | nodesX	| nodesY	| incrX		| incrY		|
|---------------------------------------------------------------|
|	0	| 	0	  |	d2(y)	|	d3(x)	|	d1(y)	| d1d2(xy)  |
|	1	| 	0	  |	d1(x)	|	d3(z)	|	1   	| d1d2(xy)	|
|	2	| corner4 |	d2(y)	|	d3(x)	|	d1(y)	| d1d2(xy)  |
|	3	| corner3 |	d1(x)	|	d3(z)	|	1   	| d1d2(xy)	|
|	4	| 	0	  |	d1(x)	|	d2(y)	|	1   	| d1(x)	    |
|	5	| corner1 |	d1(x)	|	d2(y)	|	1   	| d1(x)	    |
|----------------------------------------------------------------


*/
NodeIterator2D MeshDensity3D::faceNodeIterator(int faceID, int n1, const NodeIterator1D& preNodes) {
	
	if (faceID == 0) {
		return NodeIterator2D(n1, dir2(), dir3(), dir1(), dir1()*dir2(), preNodes);
	}
	else if (faceID == 1) {
		return NodeIterator2D(n1, dir1(), dir3(), 1, dir1()*dir2(), preNodes);
	}
	else if (faceID == 2) {
		return NodeIterator2D(n1 + cornerNode(4), dir2(), dir3(), dir1(), dir1()*dir2(), preNodes);
	}
	else if (faceID == 3) {
		return NodeIterator2D(n1 + cornerNode(3), dir1(), dir3(), 1, dir1()*dir2(), preNodes);
	}
	else if (faceID == 4) {
		return NodeIterator2D(n1, dir1(), dir2(), 1, dir1(), preNodes);
	}
	else if (faceID == 5) {
		return NodeIterator2D(n1 + cornerNode(1), dir1(), dir2(), 1, dir1(), preNodes);
	}
	else {
		throw("Invalid face ID in MeshDensity3D::faceNodeIterator");
	}
}

int MeshDensity2Dref::cornerNode(int cornerID) {
	switch (cornerID)
	{
	case 0: return 0;												 break;
	case 1: return cornerNode(2) - nNodesRowT(nRefs() - 1) + 1;		 break;
	case 2: return refinement::nNodesTot_2d(nRefs(), nElDir2()) - 1; break;
	case 3: return dir2() - 1;										 break;
	default:
		throw("Invalid corner ID in MeshDensity2Dref::cornerNode");
		break;
	}
}


/*!number of nodes in dir2 after refinement (nodes on edge 2)*/
int MeshDensity2Dref::dir2ref(){ 
	return refinement::nNodesLayerT_2d(nRefs() - 1, dir2()); 
}

/*!number of elements in dir2 after refinement (elements on edge 2)*/
int MeshDensity2Dref::nElDir2ref() { 
	return closedLoop ? dir2ref() : dir2ref() - 1; 
}

NodeIterator1Dref MeshDensity2Dref::edgeNodeIteratorRefDir(int edgeID, int firstNodeID, int preNode) {
	if (edgeID == 1) {
		return NodeIterator1Dref(firstNodeID, nRefs(), nElDir2(), NodeIterator1Dref::Type::edge1, preNode);
	}
	else if (edgeID == 3) {
		return NodeIterator1Dref(firstNodeID + cornerNode(3), nRefs(), nElDir2(), NodeIterator1Dref::Type::edge3, preNode);
	}
	else {
		throw("Invalid edge ID in MeshDensity2Dref::edgeNodeIteratorRefDir");
	}
}

NodeIterator1D MeshDensity2Dref::edgeNodeIterator(int edgeID, int firstNodeID, int preNode) {
	if (edgeID == 0) {
		return NodeIterator1D(firstNodeID, dir2(), 1, preNode);
	}
	else if (edgeID == 2) {
		return NodeIterator1D(firstNodeID + cornerNode(1), dir2ref(), 1, preNode);
	}
	else {
		throw("Invalid edge ID in MeshDensity2Dref::edgeNodeIterator");
	}
}

void MeshDensity2DrecTube::setNodesInner(int nNodePerimeterInner, const glm::dvec2& sizeInner) {
	setCirc(nNodePerimeterInner);
	Rectangle rec(sizeInner);
	rec.elementsPerSides(nNodePerimeterInner, nNodesW, nNodesH);
}
void MeshDensity2DrecTube::setNodesInner(int nNodesWidth, int nNodesHeight) {
	nNodesW = nNodesWidth;
	nNodesH = nNodesHeight;
	setCirc(2 * (nNodesWidth + nNodesHeight));
}
void MeshDensity2DrecTube::setNodesLayer(int nLayers) {
	setNorm(nLayers);
}

int MeshDensity2DrecTube::nNodePerimeter(int layer) const {
	return circ() + 8 * layer; 
}

int MeshDensity2DrecTube::nNodesWidth(int layer) const {
	return nNodesW + 2 * layer;
}
int MeshDensity2DrecTube::nNodesHeight(int layer) const {
	return nNodesH + 2 * layer;
}
void MeshDensity2DrecTube::cornerNodes(int n[4], int layer) const {
	n[0] = 0;
	n[1] = nNodesWidth(layer);
	n[2] = n[1] + nNodesHeight(layer);
	n[3] = n[2] + nNodesWidth(layer);
}


int MeshDensity2Dref::nElRowB(int refLayer) const {
	return refinement::nElementsLayerB_2d(refLayer, nElDir2());
}
int MeshDensity2Dref::nElRowT(int refLayer) const {
	return refinement::nElementsLayerT_2d(refLayer, nElDir2());
}

int MeshDensity2Dref::nNodesRowB(int refLayer) const {
	return closedLoop ? nElRowB(refLayer) : nElRowB(refLayer) + 1;
}
int MeshDensity2Dref::nNodesRowM(int refLayer) const {	
	return 3 * nElRowB(refLayer) / 4;
}
int MeshDensity2Dref::nNodesRowT(int refLayer) const {
	return nNodesRowB(refLayer + 1);
}
int MeshDensity2Dref::nNodes() const {
	return refinement::nNodesTot_2d(nRefs(), nElDir2());
}
int MeshDensity2Dref::nElements() const {
	throw("MeshDensity2Dref::nElements() - is not implemented");
}

int MeshDensity3Dref::dir1ref(){
	return refinement::nNodesLayerT_2d(nRefs() - 1, dir1());
}
int MeshDensity3Dref::dir3ref() {
	return refinement::nNodesLayerT_2d(nRefs() - 1, dir3());
}
int MeshDensity3Dref::nElDir2ref() {
	return closedLoop ? dir1ref() : dir1ref() - 1;
}
int MeshDensity3Dref::nElDir3ref() {
	return dir3ref() - 1;
}

int MeshDensity3Dref::nNodes() const {
	return refinement::nNodesTot_3d(nRefs(), nElDir1(), nElDir3());
}
int MeshDensity3Dref::nElements() const {
	throw("MeshDensity3Dref::nElements() - is not implemented");
}

int MeshDensity3Dref::cornerNode(int cornerID) {
	switch (cornerID)
	{

	case 0: return -1;			break;
	case 1: return dir1();		break;
	case 2: return -1;	break;
	case 3: return -1;	break;
	case 4: return (cornerNode(6) - refinement::nNodesLayerT_3d(nRefs() - 1, nElDir1(), nElDir3()) + 1);	break;
	case 5: return -1;	break;
	case 6: return (refinement::nNodesTot_3d(nRefs(), nElDir1(), nElDir3()) - 1);	break;
	case 7: return -1;	break;
	default:
		throw("Invalid corner ID in MeshDensity3D::cornerNode");
		break;
	}
}


NodeIterator2Dref MeshDensity3Dref::faceNodeIteratorRefDir(
	int faceID, 
	int firstNodeID, 
	const NodeIterator1D& preNodes) 
{
	if (faceID == 1) {
		return NodeIterator2Dref(firstNodeID, dir1(), dir3(), nRefs(), NodeIterator2Dref::Type::face1, preNodes);
	}
	else if (faceID == 3) {
		return NodeIterator2Dref(firstNodeID, dir1(), dir3(), nRefs(), NodeIterator2Dref::Type::face3, preNodes);
	}
	else if (faceID == 4) {
		return NodeIterator2Dref(firstNodeID, dir1(), dir3(), nRefs(), NodeIterator2Dref::Type::face4, preNodes);
	}
	else if (faceID == 5) {
		return NodeIterator2Dref(firstNodeID, dir1(), dir3(), nRefs(), NodeIterator2Dref::Type::face5, preNodes);
	}
	else {
		throw("Invalid edge ID in MeshDensity3Dref::faceNodeIteratorRefDir");
	}
}
NodeIterator2D MeshDensity3Dref::faceNodeIterator(int faceID, int firstNodeID, const NodeIterator1D& preNodes) {
	if (faceID == 0) {
		return NodeIterator2D(firstNodeID, dir1(), dir3(), 1, dir1(), preNodes);
	}
	else if (faceID == 2) {
		return NodeIterator2D(firstNodeID + cornerNode(4), dir1ref(), dir3ref(), 1, dir1ref(), preNodes);
	}
	else {
		throw("Invalid edge ID in MeshDensity3Dref::faceNodeIterator");
	}
}
