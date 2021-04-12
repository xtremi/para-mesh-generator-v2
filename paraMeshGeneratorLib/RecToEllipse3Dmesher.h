#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecToEllipse3Dmesher : public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity3D&	meshDens,
		const EllipseRadius&	radiusStart,
		const EllipseRadius&	radiusEnd,
		const glm::dvec2&		recSizeStart,
		const glm::dvec2&		recSizeEnd,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);

	static void writeElements(const MeshDensity3D& meshDens);
};

