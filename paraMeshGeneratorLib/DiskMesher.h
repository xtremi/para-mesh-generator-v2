#pragma once
#include "FeaWrite.h"
#include "Mesher.h"

class DiskMesher :	private Mesher
{
public:
	static void writeNodes(
		MeshCsys&			spos,
		const MeshDensity2D&	meshDens,
		const Cone2Dradius&		radius,
		const ArcAngles&		angle,
		direction				rotaxis);

	static void writeNodesX(MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle);
	static void writeNodesY(MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle);
	static void writeNodesZ(MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle);

	static void writeElements(const MeshDensity2D& meshDens);

};

class DiskMesherRef : public Mesher
{
public:
	static void writeNodes(
		MeshCsys&			spos,
		const MeshDensity2Dref&	meshDens,
		const Cone2Dradius&		radius,
		const ArcAngles&		angle,
		bool					startWithOffset,
		direction				rotaxis);

	static void writeElements(const MeshDensity2Dref& meshDens);
};