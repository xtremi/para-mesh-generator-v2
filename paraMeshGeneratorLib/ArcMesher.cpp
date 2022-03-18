#include "ArcMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"

void ArcMesher::writeNodesCircularX(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, const ArcAngles&  arcAngles){
	writeNodesCircular(pos, csys, nnodes, radius, arcAngles, direction::x);
}
void ArcMesher::writeNodesCircularY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, const ArcAngles&  arcAngles){
	writeNodesCircular(pos, csys, nnodes, radius, arcAngles, direction::y);
}
void ArcMesher::writeNodesCircularZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, const ArcAngles&  arcAngles){
	writeNodesCircular(pos, csys, nnodes, radius, arcAngles, direction::z);
}
void ArcMesher::writeNodesCircularXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, double startAng, double dAng){
	writeNodesCircularQ(pos, csys, nnodes, radius, startAng, dAng, direction::x);
}
void ArcMesher::writeNodesCircularYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, double startAng, double dAng){
	writeNodesCircularQ(pos, csys, nnodes, radius, startAng, dAng, direction::y);
}
void ArcMesher::writeNodesCircularZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, double startAng, double dAng){
	writeNodesCircularQ(pos, csys, nnodes, radius, startAng, dAng, direction::z);
}

void ArcMesher::writeNodesCircular(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	double				radius,
	const ArcAngles& 	arcAngles,
	direction			rotAxis)
{
	double dang = arcAngles.angStep(nnodes);	
	writeNodesCircularQ(pos, csys, nnodes, radius, arcAngles.start, dang,  rotAxis);
}

void ArcMesher::writeNodesCircularQ(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	double				radius,
	double				startAng,
	double				dAng,	
	direction			rotAxis)
{
	Mesher::nodeID1 = writer->getNextNodeID();
	double ang = startAng;

	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coordsOnCircle(ang, radius, rotAxis), pos, nullptr, &csys);
		ang += dAng;
	}
}

/*
	Same as writeNodeCircularQ but will skip every "skipNth" node

	OBS: this skips also the first node, which is probably required when used by the refinement meshing funcitons,
	but is confusing.
*/
void ArcMesher::writeNodesCircularQ_nth(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	double				radius,
	double				startAng,
	double				dAng,
	int					skipNth,
	direction			rotAxis)
{
	double ang = startAng;
	Mesher::nodeID1 = writer->getNextNodeID();

	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth) {
			writer->writeNode(coordsOnCircle(ang, radius, rotAxis), pos, nullptr, &csys);
		}
		ang += dAng;
	}
}

void ArcMesher::writeNodesCircular_nth(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	double				radius,
	const ArcAngles& 	arcAngles,
	int					skipNth,
	direction			rotAxis)
{
	double dang = arcAngles.angStep(nnodes);
	writeNodesCircularQ_nth(pos, csys, nnodes, radius, arcAngles.start, dang, skipNth, rotAxis);
}

void ArcMesher::writeNodes(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	double				radius,
	const ArcAngles&	angle,
	const glm::dvec3&	xdir,
	const glm::dvec3&	ydir,
	node_skip			nskip)
{
	MESHER_NODE_WRITE_START
	double dAng = angle.angStep(nnodes);
	double ang = angle.start;
	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coordsOnCircleQ(ang, radius, xdir, ydir), pos, nullptr, &csys);
		ang += dAng;
	}
	MESHER_NODE_WRITE_END
}

void ArcMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	int				  nnodes,
	double			  radius,
	const glm::dvec3& p1,
	const glm::dvec3& p2,
	const glm::dvec3& normal,
	node_skip		  nskip)
{
	//MESHER_NODE_WRITE_START
	glm::dvec3 center = circleCenter(p1, p2, normal, radius);
	glm::dvec3 dirX = glm::normalize(p1 - center);
	glm::dvec3 dirY = glm::normalize(glm::cross(normal, dirX));
	ArcAngles angles(angleOfPointOnCircle(p1, center), angleOfPointOnCircle(p2, center));
	ArcMesher::writeNodes(pos, csys, nnodes, radius, angles, dirX, dirY, nskip);


	//MESHER_NODE_WRITE_END
}

void ArcMesher::writeElements(
	int			nnodes,
	bool		closedLoop)
{
	return LineMesher::writeElements(nnodes, closedLoop);
}