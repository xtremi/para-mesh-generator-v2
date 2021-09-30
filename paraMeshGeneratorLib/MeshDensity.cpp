#include "MeshDensity.h"
#include "Shapes.h"

int MeshDensity2D::cornerNode(int cornerID) {
	switch (cornerID)
	{
	case 0: return 0;						break;
	case 1: return dir1();					break;
	case 2: return (dir2() - 1) * dir1();	break;
	case 3: return nNodes();				break;
	default:
		throw("Invalid corner ID in MeshDensity2D::cornerNode");
		break;
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
