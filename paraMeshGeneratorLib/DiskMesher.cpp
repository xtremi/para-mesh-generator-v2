#include "DiskMesher.h"
#include "Conemesher.h"

void DiskMesher::writeNodes(
	FEAwriter*			writer,
	const glm::dvec3&	centerPos,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				endAng,
	const glm::ivec2&	nnodes,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	ConeMesher::writeNodes(writer, centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, rotaxis, csys);
}

void DiskMesher::writeNodesX(FEAwriter* writer, const glm::dvec3& centerPos,
	double radiusStart, double radiusEnd, double startAng, double endAng,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesX(writer, centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, csys);
}
void DiskMesher::writeNodesY(FEAwriter* writer, const glm::dvec3& centerPos,
	double radiusStart, double radiusEnd, double startAng, double endAng,
	const glm::ivec2& nnodes, glm::dmat3x3* csys) 
{
	ConeMesher::writeNodesY(writer, centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, csys);
}
void DiskMesher::writeNodesZ(FEAwriter* writer, const glm::dvec3& centerPos,
	double radiusStart, double radiusEnd, double startAng, double endAng,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesZ(writer, centerPos, radiusStart, radiusEnd, startAng, endAng, 0.0, nnodes, csys);
}

void DiskMesher::writeElements(FEAwriter* writer, glm::ivec2 nnodes, bool closedLoop) 
{
	ConeMesher::writeElements(writer, nnodes, closedLoop);
}


void DiskMesherRef::writeNodes(
	FEAwriter*			writer,
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
	ConeMesherRef::writeNodes(writer, centerPos, nNodesEdge, nRefinements, radiusStart, radiusEnd, startAng, endAng, 0.0,
		rotaxis, csys);
}

void DiskMesherRef::writeElements(FEAwriter* writer, int nNodesY, int nRefinements, bool closedLoop)
{
	ConeMesherRef::writeElements(writer, nNodesY, nRefinements, closedLoop);
}