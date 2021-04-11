#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class SquareToCircleMesher : public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity2D&	meshDens,
		const EllipseRadius&		radius,
		const glm::dvec2&		recSize,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);
	
	static void writeElements(const MeshDensity2D& meshDens);


};

