#include "CylinderMesher.h"
#include "ConeMesher.h"

void CylinderMesher::writeNodes(
	MeshCsys&		 spos,
	const MeshDensity2D& meshDens,
	double				 radius,
	const ArcAngles&	 angle,
	double				 height,
	direction			 rotaxis)
{
	ConeMesher::writeNodes(spos, meshDens, Cone2Dradius(radius, radius), angle, height, rotaxis);
}

void CylinderMesher::writeNodesX(MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesX(spos, meshDens, Cone2Dradius(radius, radius), angle, height);
}
void CylinderMesher::writeNodesY(MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesY(spos, meshDens, Cone2Dradius(radius, radius), angle, height);
}
void CylinderMesher::writeNodesZ(MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height){
	ConeMesher::writeNodesY(spos, meshDens, Cone2Dradius(radius, radius), angle, height);
}

void CylinderMesher::writeElements(const MeshDensity2D& meshDens){
	ConeMesher::writeElements(meshDens);
}


void CylinderMesherRef::writeNodes(
	MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	double					radius,
	const ArcAngles&		angle,
	double					height,
	bool					startWithOffset,
	direction				rotaxis)
{
	ConeMesherRef::writeNodes(spos, meshDens, Cone2Dradius(radius, radius), angle, height, startWithOffset, rotaxis);
}

void CylinderMesherRef::writeElements(const MeshDensity2Dref& meshDens){
	ConeMesherRef::writeElements(meshDens);
}