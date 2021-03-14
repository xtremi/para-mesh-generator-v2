#include "CylinderMesher.h"
#include "ConeMesher.h"

void CylinderMesher::writeNodes(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	double				radius,
	double				height,
	double				startAng,
	double				endAng,	
	const glm::ivec2&	nnodes,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	ConeMesher::writeNodes(writer, spos, radius, radius, startAng, endAng, height, nnodes, rotaxis, csys);
}

void CylinderMesher::writeNodesX(FEAwriter* writer, const glm::dvec3& spos, double radius, double height,
	double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesX(writer, spos, radius, radius, startAng, endAng, height, nnodes, csys);
}
void CylinderMesher::writeNodesY(FEAwriter* writer, const glm::dvec3& spos, double radius, double height,
	double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesY(writer, spos, radius, radius, startAng, endAng, height, nnodes, csys);
}
void CylinderMesher::writeNodesZ(FEAwriter* writer, const glm::dvec3& spos, double radius, double height,
	double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	ConeMesher::writeNodesY(writer, spos, radius, radius, startAng, endAng, height, nnodes, csys);
}

void CylinderMesher::writeElements(FEAwriter* writer, glm::ivec2 nnodes, bool closedLoop)
{
	ConeMesher::writeElements(writer, nnodes, closedLoop);
}


void CylinderMesherRef::writeNodes(
	FEAwriter*			writer,
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
	ConeMesherRef::writeNodes(writer, spos, nNodesEdge, nRefinements, radius, radius, startAng, endAng, height, rotaxis, csys);
}

void CylinderMesherRef::writeElements(FEAwriter* writer, int nNodesY, int nRefinements, bool closedLoop)
{
	ConeMesherRef::writeElements(writer, nNodesY, nRefinements, closedLoop);
}