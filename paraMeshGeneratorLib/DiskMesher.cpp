#include "DiskMesher.h"
#include "Conemesher.h"

void DiskMesher::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity2D&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	direction				rotaxis)
{
	ConeMesher::writeNodes(pos, csys, meshDens, radius, angle, 0.0, rotaxis);
}

void DiskMesher::writeNodesX(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle){
	ConeMesher::writeNodesX(pos, csys, meshDens, radius, angle, 0.0);
}
void DiskMesher::writeNodesY(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle){
	ConeMesher::writeNodesY(pos, csys, meshDens, radius, angle, 0.0);
}
void DiskMesher::writeNodesZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle){
	ConeMesher::writeNodesZ(pos, csys, meshDens, radius, angle, 0.0);
}

void DiskMesher::writeElements(const MeshDensity2D& meshDens){
	ConeMesher::writeElements(meshDens);
}


void DiskMesherRef::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity2Dref&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	bool					startWithOffset,
	direction				rotaxis)
{
	ConeMesherRef::writeNodes(pos, csys, meshDens, radius, angle, 0.0, startWithOffset, rotaxis);
}

void DiskMesherRef::writeElements(const MeshDensity2Dref& meshDens)
{
	ConeMesherRef::writeElements(meshDens);
}