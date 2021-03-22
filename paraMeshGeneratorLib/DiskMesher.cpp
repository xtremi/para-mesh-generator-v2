#include "DiskMesher.h"
#include "Conemesher.h"

void DiskMesher::writeNodes(
	const glm::dvec3&	centerPos,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				endAng,
	const glm::ivec2&	nnodes,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	ConeMesher::writeNodes(centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, rotaxis, csys);
}

void DiskMesher::writeNodesX(const glm::dvec3& centerPos,
	double radiusStart, double radiusEnd, double startAng, double endAng,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesX(centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, csys);
}
void DiskMesher::writeNodesY(const glm::dvec3& centerPos,
	double radiusStart, double radiusEnd, double startAng, double endAng,
	const glm::ivec2& nnodes, glm::dmat3x3* csys) 
{
	ConeMesher::writeNodesY(centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, csys);
}
void DiskMesher::writeNodesZ(const glm::dvec3& centerPos,
	double radiusStart, double radiusEnd, double startAng, double endAng,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesZ(centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, csys);
}

void DiskMesher::writeElements(glm::ivec2 nnodes, bool closedLoop) 
{
	ConeMesher::writeElements(nnodes, closedLoop);
}


void DiskMesherRef::writeNodes(
	const glm::dvec3&	centerPos,
	int					nNodesEdge,
	int					nRefinements,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				endAng,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	ConeMesherRef::writeNodes(centerPos, nNodesEdge, nRefinements, radiusStart, radiusEnd, startAng, endAng, 0.0,
		rotaxis, csys);
}

void DiskMesherRef::writeElements(int nNodesY, int nRefinements, bool closedLoop)
{
	ConeMesherRef::writeElements(nNodesY, nRefinements, closedLoop);
}