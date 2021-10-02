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
/*

			 F3 (back)

	 [c2]_________e10_______[c6]
		|\                  |\
		| \                 | \
		|  \e2   F5(top)    |  \e6
		|   \             e7|   \
	  e3|    \              |    \
		| [c1]\_______e9__________\[c5]
F0(F6)->|     |             |     |  <-F2
	[c3]| _ __| _ _ e11 _ _ |[c7] |	    -----> dir1
		 \    |              \    |e5
	 ^	  \   |e1  F1(front)  \e4 |
  ^  |	 e0\  |                \  |
   \ |dir3	\ |                 \ |
dir2\|	     \|__________________\|
		  [c0]	  ^   e8          [c4]
				  |
				 F4 (bottom)

*/
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
