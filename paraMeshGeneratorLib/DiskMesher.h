#pragma once
#include "FeaWrite.h"
#include "Mesher.h"

class DiskMesher :	private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity2D&	meshDens,
		const Cone2Dradius&		radius,
		const ArcAngles&		angle,
		direction				rotaxis);

	static void writeNodesX(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle);
	static void writeNodesY(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle);
	static void writeNodesZ(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle);

	static void writeElements(const MeshDensity2D& meshDens);

};

class DiskMesherRef : public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity2Dref&	meshDens,
		const Cone2Dradius&		radius,
		const ArcAngles&		angle,
		direction				rotaxis);

	static void writeElements(const MeshDensity2Dref& meshDens);
};