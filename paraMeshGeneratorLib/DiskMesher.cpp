#include "DiskMesher.h"
#include "Conemesher.h"

void DiskMesher::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity2D&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	direction				rotaxis)
{
	ConeMesher::writeNodes(spos, meshDens, radius, angle, 0.0, rotaxis);
}

void DiskMesher::writeNodesX(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle){
	ConeMesher::writeNodesX(spos, meshDens, radius, angle, 0.0);
}
void DiskMesher::writeNodesY(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle){
	ConeMesher::writeNodesY(spos, meshDens, radius, angle, 0.0);
}
void DiskMesher::writeNodesZ(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle){
	ConeMesher::writeNodesZ(spos, meshDens, radius, angle, 0.0);
}

void DiskMesher::writeElements(const MeshDensity2D& meshDens){
	ConeMesher::writeElements(meshDens);
}


void DiskMesherRef::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	bool					startWithOffset,
	direction				rotaxis)
{
	ConeMesherRef::writeNodes(spos, meshDens, radius, angle, 0.0, startWithOffset, rotaxis);
}

void DiskMesherRef::writeElements(const MeshDensity2Dref& meshDens)
{
	ConeMesherRef::writeElements(meshDens);
}