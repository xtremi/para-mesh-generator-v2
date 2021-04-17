#include "RecTubeMesher.h"
#include "RecEdgeMesher.h"

void RecTubeMesher::writeNodes(
	const MeshCsys&		spos,
	int					nnodesInner,
	int					nLayers,
	const RecTubeSize&	size,
	plane				pln)
{
	glm::dvec2 curSize = size.inner;
	glm::dvec2 dSize = (size.outer - size.inner) / (double)(nLayers - 1);
	int nnodes = nnodesInner;
	for (int i2 = 0; i2 < nLayers; i2++) {
		RecEdgeMesher::writeNodes(spos, nnodes, curSize, pln);
		nnodes += 8;
		curSize += dSize;
	}

}

void RecTubeMesher::writeNodes2(
	const MeshCsys&		spos,
	int					nNodesPerimeter,
	int					nLayers,
	const RecTubeSize&	size,
	plane				pln) 
{
	Rectangle rec(size.inner);
	int nElW, nElH;
	rec.elementsPerSides(nNodesPerimeter, nElW, nElH);
	if ((2 * nElW + 2 * nElH) != nNodesPerimeter) {
		int notMatching = 1;
	}

	writeNodes2(spos, nElW, nElH, nLayers, size, pln);
}

void RecTubeMesher::writeNodes2(
	const MeshCsys&		spos,
	int					nNodesWidth,
	int					nNodesHeight,
	int					nLayers,
	const RecTubeSize&	size,
	plane				pln) 
{
	int firstNode = writer->getNextNodeID();

	glm::dvec2 curSize = size.inner;
	glm::dvec2 dSize   = (size.outer - size.inner) / (double)(nLayers - 1);
	
	for (int i2 = 0; i2 < nLayers; i2++) {
		RecEdgeMesher::writeNodes(spos, nNodesWidth, nNodesHeight, curSize, pln);
		curSize += dSize;
	}

	Mesher::nodeID1 = firstNode;
}

void RecTubeMesher::writeElements(){}

void RecTubeMesherRef::writeNodes2(
	const MeshCsys&		spos,
	int					nNodesPerimeter,
	int					nRefs,
	const RecTubeSize&	size,
	plane				pln)
{
	Rectangle rec(size.inner);
	int nElW, nElH;
	rec.elementsPerSides(nNodesPerimeter, nElW, nElH);
	if ((2 * nElW + 2 * nElH) != nNodesPerimeter) {
		int notMatching = 1;
	}

	writeNodes2(spos, nElW, nElH, nRefs, size, pln);
}

void RecTubeMesherRef::writeNodes2(
	const MeshCsys&		spos,
	int					nNodesWidth,
	int					nNodesHeight,
	int					nRefs,
	const RecTubeSize&	size,
	plane				pln)
{
	int firstNode = writer->getNextNodeID();
	Mesher::nodeID1 = firstNode;

}