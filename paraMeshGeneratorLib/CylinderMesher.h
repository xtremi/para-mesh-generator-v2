#pragma once
#include "Mesher.h"
#include "FeaWrite.h"



class CylinderMesher :	private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&	 pos,
		MeshCsys&			 csys,
		const MeshDensity2D& meshDens,
		double				 radius,
		const ArcAngles&	 angle,
		double				 height,
		direction			 rotaxis);

	static void writeNodesX(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height);
	static void writeNodesY(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height);
	static void writeNodesZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, double radius, const ArcAngles& angle, double height);
	
	static void writeElements(const MeshDensity2D& meshDens);
};

class CylinderMesherRef : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity2Dref&	meshDens,
		double					radius,
		const ArcAngles&		angle,
		double					height,
		bool					startWithOffset,
		direction				rotaxis);

	static void writeElements(const MeshDensity2Dref& meshDens);
};