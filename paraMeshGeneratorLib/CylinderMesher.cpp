#include "CylinderMesher.h"
#include "ConeMesher.h"

void CylinderMesher::writeNodes(
	const glm::dvec3&	spos,
	double				radius,
	double				height,
	double				startAng,
	double				endAng,	
	const glm::ivec2&	nnodes,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	ConeMesher::writeNodes(spos, radius, radius, startAng, endAng, height, nnodes, rotaxis, csys);
}

void CylinderMesher::writeNodesX(const glm::dvec3& spos, double radius, double height,
	double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesX(spos, radius, radius, startAng, endAng, height, nnodes, csys);
}
void CylinderMesher::writeNodesY(const glm::dvec3& spos, double radius, double height,
	double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesY(spos, radius, radius, startAng, endAng, height, nnodes, csys);
}
void CylinderMesher::writeNodesZ(const glm::dvec3& spos, double radius, double height,
	double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesY(spos, radius, radius, startAng, endAng, height, nnodes, csys);
}

void CylinderMesher::writeElements(const glm::ivec2& nnodes, bool closedLoop)
{
	ConeMesher::writeElements(nnodes, closedLoop);
}


void CylinderMesherRef::writeNodes(
	const glm::dvec3&	spos,
	int					nNodesEdge,
	int					nRefinements,
	double				radius,
	double				height,
	double				startAng,
	double				endAng,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	ConeMesherRef::writeNodes(spos, nNodesEdge, nRefinements, radius, radius, startAng, endAng, height, rotaxis, csys);
}

void CylinderMesherRef::writeElements(int nNodesY, int nRefinements, bool closedLoop)
{
	ConeMesherRef::writeElements(nNodesY, nRefinements, closedLoop);
}