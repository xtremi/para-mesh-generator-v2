#include "CylinderMesher.h"
#include "ConeMesher.h"

void CylinderMesher::writeNodes(
	const MeshCsys&		 spos,
	const MeshDensity2D& meshDens,
	double				 radius,
	const ArcAngles&	 angle,
	double				 height,
	direction			 rotaxis)
{
	ConeMesher::writeNodes(spos, meshDens, Cone2Dradius(radius, radius), angle, height, rotaxis);
}

void CylinderMesher::writeNodesX(const MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesX(spos, meshDens, Cone2Dradius(radius, radius), angle, height);
}
void CylinderMesher::writeNodesY(const MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesY(spos, meshDens, Cone2Dradius(radius, radius), angle, height);
}
void CylinderMesher::writeNodesZ(const MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesY(spos, meshDens, Cone2Dradius(radius, radius), angle, height);
}

void CylinderMesher::writeElements(const MeshDensity2D& meshDens){
	ConeMesher::writeElements(meshDens);
}


void CylinderMesherRef::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	double					radius,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
	ConeMesherRef::writeNodes(spos, meshDens, Cone2Dradius(radius, radius), angle, height, rotaxis);
}

void CylinderMesherRef::writeElements(const MeshDensity2Dref& meshDens){
	ConeMesherRef::writeElements(meshDens);
}