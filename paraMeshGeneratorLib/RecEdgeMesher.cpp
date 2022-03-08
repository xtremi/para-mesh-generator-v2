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
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	const glm::dvec2&	size,
	plane				pln)
{
	getOrWriteCoords_nth(MesherMode::write, pos, csys, default_empty_coord_vec, nnodes, size, 0, false, pln);
}

void RecEdgeMesher::writeNodes(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nNodesWidth,
	int					nNodesHeight,
	const glm::dvec2&	size,
	plane				pln)
{
	getOrWriteCoords_nth(MesherMode::write, pos, csys, default_empty_coord_vec, nNodesWidth, nNodesHeight, size, 0, false, pln);

}

void RecEdgeMesher::writeNodes_nth(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nNodesWidth,
	int					nNodesHeight,
	const glm::dvec2&	size,
	int					skipNth,
	plane				pln)
{
	getOrWriteCoords_nth(MesherMode::write, pos, csys, default_empty_coord_vec, nNodesWidth, nNodesHeight, size, skipNth, true, pln);
}

void RecEdgeMesher::writeNodes_nth(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	const glm::dvec2&	size,
	int					skipNth,
	plane				pln)
{
	getOrWriteCoords_nth(MesherMode::write, pos, csys, default_empty_coord_vec, nnodes, size, skipNth, true, pln);
}

void RecEdgeMesher::getLocalCoords(
	std::vector<glm::dvec2>& coords,
	int						 nnodes,
	const glm::dvec2&		 size)
{
	MeshCsys csys;
	getOrWriteCoords_nth(MesherMode::get, glm::dvec3(0.), csys, coords, nnodes, size, 0, false, plane::xy);
}



void RecEdgeMesher::getOrWriteCoords_nth(
	MesherMode					mode,
	const glm::dvec3&			pos,
	MeshCsys&					csys,
	std::vector<glm::dvec2>&	coords,
	int							nnodes,
	const glm::dvec2&			size,
	int							skipNth,
	bool						skipCorners,
	plane						pln)
{
	Rectangle rec(size);
	int nElW, nElH;
	rec.elementsPerSides(nnodes, nElW, nElH);
	if ((2 * nElW + 2 * nElH) != nnodes) {
		int notMatching = 1;
	}

	getOrWriteCoords_nth(mode, pos, csys, coords, nElW, nElH, size, skipNth, skipCorners, pln);
}

void RecEdgeMesher::getOrWriteCoords_nth(
	MesherMode					mode,
	const glm::dvec3&			pos,
	MeshCsys&					csys,
	std::vector<glm::dvec2>&	coords,
	int							nNodesWidth,
	int							nNodesHeight,
	const glm::dvec2&			size,
	int							skipNth,
	bool						skipCorners,
	plane						pln)
{
	int firstNode = writer->getNextNodeID();

	double elSizeRecW = size.x / (double)nNodesWidth;
	double elSizeRecH = size.y / (double)nNodesHeight;

	int nNodesEdges[4] = { nNodesHeight, nNodesWidth, nNodesHeight, nNodesWidth };
	glm::dvec2 curCornerCoord;
	glm::dvec2 curStep;
	glm::dvec2 curCoordLocal;
	glm::dvec3 curCoord(0.0);
	double     curElSize;

	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	int nindex = 0;
	int nEdgeIndex = 0; //skips corners
	
	Rectangle rec(size);
	for (int edge = 0; edge < 4; edge++) {
		curCornerCoord = glm::dvec2(rec.getCornerCoord(edge));
		curElSize = edge == 0 || edge == 2 ? elSizeRecH : elSizeRecW;
		curStep = Rectangle::recDirs[edge] * curElSize;


		for (int i = 0; i < nNodesEdges[edge]; i++) {
			curCoordLocal = curCornerCoord + (double)i * curStep;
			if (!(skipCorners && i == 0)) {
				if (skipNth == 0 || nEdgeIndex % skipNth) {
					if (mode == MesherMode::write) {
						curCoord[(size_t)dir1] = curCoordLocal.x;
						curCoord[(size_t)dir2] = curCoordLocal.y;
						writer->writeNode(curCoord, pos, nullptr, &csys);
					}
					else {
						coords[nindex] = curCoordLocal;
					}
				}
				nEdgeIndex++;
			}
			nindex++;
		}
		nEdgeIndex = 0;
	}

	Mesher::nodeID1 = firstNode;
}

void RecEdgeMesher::writeElements(int nnodes, bool closedLoop) {
	LineMesher::writeElementsLine(nnodes, closedLoop);
}
