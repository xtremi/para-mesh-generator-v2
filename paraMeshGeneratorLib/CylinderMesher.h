#pragma once
#include "Mesher.h"
#include "FeaWrite.h"



class CylinderMesher :	private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		double				 radius,
		const ArcAngles&	 angle,
		double				 height,
		direction			 rotaxis);

	static void writeNodesX(const MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height);
	static void writeNodesY(const MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height);
	static void writeNodesZ(const MeshCsys& spos, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height);
	
	static void writeElements(const MeshDensity2D& meshDens);
};

class CylinderMesherRef : private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity2Dref&	meshDens,
		double					radius,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);

	static void writeElements(const MeshDensity2Dref& meshDens);
};