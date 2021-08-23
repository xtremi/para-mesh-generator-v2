#include "ArcMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"

void ArcMesher::writeNodesCircularX(MeshCsys& spos, int nnodes, double radius, const ArcAngles&  arcAngles){
	writeNodesCircular(spos, nnodes, radius, arcAngles, direction::x);
}
void ArcMesher::writeNodesCircularY(MeshCsys& spos, int nnodes, double radius, const ArcAngles&  arcAngles){
	writeNodesCircular(spos, nnodes, radius, arcAngles, direction::y);
}
void ArcMesher::writeNodesCircularZ(MeshCsys& spos, int nnodes, double radius, const ArcAngles&  arcAngles){
	writeNodesCircular(spos, nnodes, radius, arcAngles, direction::z);
}
void ArcMesher::writeNodesCircularXq(MeshCsys& spos, int nnodes, double radius, double startAng, double dAng){
	writeNodesCircularQ(spos, nnodes, radius, startAng, dAng, direction::x);
}
void ArcMesher::writeNodesCircularYq(MeshCsys& spos, int nnodes, double radius, double startAng, double dAng){
	writeNodesCircularQ(spos, nnodes, radius, startAng, dAng, direction::y);
}
void ArcMesher::writeNodesCircularZq(MeshCsys& spos, int nnodes, double radius, double startAng, double dAng){
	writeNodesCircularQ(spos, nnodes, radius, startAng, dAng, direction::z);
}

void ArcMesher::writeNodesCircular(
	MeshCsys&		spos,
	int					nnodes,
	double				radius,
	const ArcAngles& 	arcAngles,
	direction			rotAxis)
{
	double dang = arcAngles.angStep(nnodes);	
	writeNodesCircularQ(spos, nnodes, radius, arcAngles.start, dang,  rotAxis);
}

void ArcMesher::writeNodesCircularQ(
	MeshCsys&		spos,
	int					nnodes,
	double				radius,
	double				startAng,
	double				dAng,	
	direction			rotAxis)
{
	glm::dvec3 coords;
	double ang = startAng;
	nodeID1 = writer->getNextNodeID();

	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coordsOnCircle(ang, radius, rotAxis), glm::dvec3(0.), nullptr, &spos);
		ang += dAng;
	}
}

/*
	Same as writeNodeCircularQ but will skip every "skipNth" node

	OBS: this skips also the first node, which is probably required when used by the refinement meshing funcitons,
	but is confusing.
*/
void ArcMesher::writeNodesCircularQ_nth(
	MeshCsys&		spos,
	int					nnodes,
	double				radius,
	double				startAng,
	double				dAng,
	int					skipNth,
	direction			rotAxis)
{
	glm::dvec3 coords;
	double ang = startAng;
	nodeID1 = writer->getNextNodeID();

	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth) {
			writer->writeNode(coordsOnCircle(ang, radius, rotAxis), spos.pos, spos.csys);
		}
		ang += dAng;
	}
}

void ArcMesher::writeNodesCircular_nth(
	MeshCsys&		spos,
	int					nnodes,
	double				radius,
	const ArcAngles& 	arcAngles,
	int					skipNth,
	direction			rotAxis)
{
	double dang = arcAngles.angStep(nnodes);
	writeNodesCircularQ_nth(spos, nnodes, radius, arcAngles.start, dang, skipNth, rotAxis);
}



void ArcMesher::writeElementsLine(
	int			nnodes,
	bool		closedLoop)
{
	return LineMesher::writeElementsLine(nnodes, closedLoop);
}