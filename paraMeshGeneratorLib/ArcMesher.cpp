#include "ArcMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"

void ArcMesher::writeNodesCircularX(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircular(feaWriter, centerPos, radius, startAng, endAng, nnodes, direction::x, csys);
}
void ArcMesher::writeNodesCircularY(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircular(feaWriter, centerPos, radius, startAng, endAng, nnodes, direction::y, csys);
}
void ArcMesher::writeNodesCircularZ(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircular(feaWriter, centerPos, radius, startAng, endAng, nnodes, direction::z, csys);
}
void ArcMesher::writeNodesCircularXq(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircularQ(feaWriter, centerPos, radius, startAng, dAng, nnodes, direction::x, csys);
}
void ArcMesher::writeNodesCircularYq(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircularQ(feaWriter, centerPos, radius, startAng, dAng, nnodes, direction::y, csys);
}
void ArcMesher::writeNodesCircularZq(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, glm::dmat3x3* csys)
{
	writeNodesCircularQ(feaWriter, centerPos, radius, startAng, dAng, nnodes, direction::z, csys);
}

void ArcMesher::writeNodesCircular(
	FEAwriter*			feaWriter,
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
	writeNodesCircularQ(feaWriter, centerPos, radius, startAng, dang, nnodes, rotAxis, csys);
}

void ArcMesher::writeNodesCircularQ(
	FEAwriter*			feaWriter,
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
	nodeID1 = feaWriter->getNextNodeID();

	for (int i = 0; i < nnodes; i++) {
		feaWriter->writeNode(coordsOnCircle(ang, radius, rotAxis), centerPos, csys);
		ang += dAng;
	}
}

/*
	Same as writeNodeCircularQ but will skip every "skipNth" node

	OBS: this skips also the first node, which is probably required when used by the refinement meshing funcitons,
	but is confusing.
*/
void ArcMesher::writeNodesCircularQ_nth(
	FEAwriter*			feaWriter,
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
	nodeID1 = feaWriter->getNextNodeID();

	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth) {
			feaWriter->writeNode(coordsOnCircle(ang, radius, rotAxis), centerPos, csys);
		}
		ang += dAng;
	}
}

void ArcMesher::writeNodesCircular_nth(
	FEAwriter*			feaWriter,
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
	writeNodesCircularQ_nth(feaWriter, centerPos, radius, startAng, dang, nnodes, skipNth, rotAxis, csys);
}



void ArcMesher::writeElementsLine(
	FEAwriter*	writer,
	int			nnodes,
	bool		closedLoop)
{
	return LineMesher::writeElementsLine(writer, nnodes, closedLoop);
}