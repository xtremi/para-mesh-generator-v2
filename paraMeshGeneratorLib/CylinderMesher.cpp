#include "CylinderMesher.h"
#include "ConeMesher.h"

void CylinderMesher::writeNodes(
	const glm::dvec3&	 pos,
	MeshCsys&			 csys,
	const MeshDensity2D& meshDens,
	double				 radius,
	const ArcAngles&	 angle,
	double				 height,
	direction			 rotaxis)
{
	ConeMesher::writeNodes(pos, csys, meshDens, Cone2Dradius(radius, radius), angle, height, rotaxis);
}

void CylinderMesher::writeNodesX(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesX(pos, csys, meshDens, Cone2Dradius(radius, radius), angle, height);
}
void CylinderMesher::writeNodesY(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesY(pos, csys, meshDens, Cone2Dradius(radius, radius), angle, height);
}
void CylinderMesher::writeNodesZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesY(pos, csys, meshDens, Cone2Dradius(radius, radius), angle, height);
}

void CylinderMesher::writeElements(const MeshDensity2D& meshDens){
	ConeMesher::writeElements(meshDens);
}


void CylinderMesherRef::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity2Dref&	meshDens,
	double					radius,
	const ArcAngles&		angle,
	double					height,
	bool					startWithOffset,
	direction				rotaxis)
{
	ConeMesherRef::writeNodes(pos, csys, meshDens, Cone2Dradius(radius, radius), angle, height, startWithOffset, rotaxis);
}

void CylinderMesherRef::writeElements(const MeshDensity2Dref& meshDens){
	ConeMesherRef::writeElements(meshDens);
}