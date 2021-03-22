#include "ArcMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"

void ArcMesher::writeNodesCircularX(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircular(centerPos, radius, startAng, endAng, nnodes, direction::x, csys);
}
void ArcMesher::writeNodesCircularY(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircular(centerPos, radius, startAng, endAng, nnodes, direction::y, csys);
}
void ArcMesher::writeNodesCircularZ(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircular(centerPos, radius, startAng, endAng, nnodes, direction::z, csys);
}
void ArcMesher::writeNodesCircularXq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircularQ(centerPos, radius, startAng, dAng, nnodes, direction::x, csys);
}
void ArcMesher::writeNodesCircularYq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircularQ(centerPos, radius, startAng, dAng, nnodes, direction::y, csys);
}
void ArcMesher::writeNodesCircularZq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircularQ(centerPos, radius, startAng, dAng, nnodes, direction::z, csys);
}

void ArcMesher::writeNodesCircular(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				endAng,
	int					nnodes,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	double dang = 0.0;
	limitArcAngles(startAng, endAng, dang, nnodes);
	writeNodesCircularQ(centerPos, radius, startAng, dang, nnodes, rotAxis, csys);
}

void ArcMesher::writeNodesCircularQ(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				dAng,
	int					nnodes,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords;
	double ang = startAng;
	nodeID1 = writer->getNextNodeID();

	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coordsOnCircle(ang, radius, rotAxis), centerPos, csys);
		ang += dAng;
	}
}

/*
	Same as writeNodeCircularQ but will skip every "skipNth" node

	OBS: this skips also the first node, which is probably required when used by the refinement meshing funcitons,
	but is confusing.
*/
void ArcMesher::writeNodesCircularQ_nth(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				dAng,
	int					nnodes,
	int					skipNth,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords;
	double ang = startAng;
	nodeID1 = writer->getNextNodeID();

	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth) {
			writer->writeNode(coordsOnCircle(ang, radius, rotAxis), centerPos, csys);
		}
		ang += dAng;
	}
}

void ArcMesher::writeNodesCircular_nth(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				endAng,
	int					nnodes,
	int					skipNth,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	double dang = 0.0;
	limitArcAngles(startAng, endAng, dang, nnodes);
	writeNodesCircularQ_nth(centerPos, radius, startAng, dang, nnodes, skipNth, rotAxis, csys);
}



void ArcMesher::writeElementsLine(
	int			nnodes,
	bool		closedLoop)
{
	return LineMesher::writeElementsLine(nnodes, closedLoop);
}