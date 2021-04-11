#include "RecEdgeMesher.h"
#include "LineMesher.h"

std::vector<glm::dvec2> RecEdgeMesher::default_empty_coord_vec = std::vector<glm::dvec2>();

/*

   x---x---x---x
   |           |
   x           x
H  |           |
   x           x
   |           |
   x---x---x---x
         W

    1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16      elements
  X---x---x---x:::x:::x:::x:::x:::x---x---x---x:::x:::x:::x:::x:::X
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16        nodes

*/

void RecEdgeMesher::writeNodes(
	const MeshCsys&		spos,
	int					nnodes,
	const glm::dvec2&	size,
	plane				pln)
{
	getOrWriteCoords(MesherMode::write, spos, default_empty_coord_vec, nnodes, size, pln);
}

void RecEdgeMesher::getLocalCoords(
	std::vector<glm::dvec2>& coords,
	int						 nnodes,
	const glm::dvec2&		 size)
{
	getOrWriteCoords(MesherMode::get, MeshCsys(), coords, nnodes, size, plane::xy);
}

void RecEdgeMesher::getOrWriteCoords(
	MesherMode				 mode,
	const MeshCsys&			 spos,
	std::vector<glm::dvec2>& coords,
	int						 nnodes,
	const glm::dvec2&		 size,
	plane					 pln)
{
	int firstNode = writer->getNextNodeID();

	double recPerimeter = 2.0 * size.x + 2.0 * size.y;
	double recWfactor = size.x / recPerimeter;

	int nElW = (int)(recWfactor * (double)nnodes);
	int nElH = (nnodes - 2 * nElW) / 2;
	if ((2 * nElW + 2 * nElH) != nnodes) {
		int notMatching = 1;
	}

	double elSizeRecW = size.x / (double)nElW;
	double elSizeRecH = size.y / (double)nElH;

	int nNodesEdges[4] = { nElW, nElH, nElW, nElH };
	glm::dvec2 curCornerCoord;
	glm::dvec2 curStep;
	glm::dvec2 curCoordLocal;
	glm::dvec3 curCoord(0.0);

	double     curElSize;

	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	Rectangle rec(size);
	int nindex = 0;
	for (int edge = 0; edge < 4; edge++) {
		curCornerCoord = glm::dvec2(rec.getCornerCoord(edge + 1));
		curElSize = edge == 0 || edge == 2 ? elSizeRecW : elSizeRecH;
		curStep = Rectangle::recDirs[edge] * curElSize;
		for (int i = 0; i < nNodesEdges[edge]; i++) {
			curCoordLocal = curCornerCoord + (double)i * curStep;
			if(mode == MesherMode::write){
				curCoord[(size_t)dir1] = curCoordLocal.x;
				curCoord[(size_t)dir2] = curCoordLocal.y;
				writer->writeNode(curCoord, spos.pos, spos.csys);
			}
			else {
				coords[nindex++] = curCoordLocal;
			}
		}
	}

	Mesher::nodeID1 = firstNode;
}


void RecEdgeMesher::writeElements(int nnodes, bool closedLoop) {
	LineMesher::writeElementsLine(nnodes, closedLoop);
}
