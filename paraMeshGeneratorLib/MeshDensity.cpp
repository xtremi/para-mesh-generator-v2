#include "MeshDensity.h"
#include "Shapes.h"

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
