#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecToEllipseMesher : public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity2D&	meshDens,
		const EllipseRadius&	radius,
		const glm::dvec2&		recSize,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);
	
	static void writeElements(const MeshDensity2D& meshDens);

	static void writeNodes(
		const MeshCsys&			       spos,
		const MeshDensity2D&		   meshDens,
		const std::vector<glm::dvec2>& startCoords,
		const std::vector<glm::dvec3>& directions,
		const std::vector<double>&	   distances,
		double						   height,
		direction					   rotaxis);

	static void calculateRecToEllipseDirections(
		const MeshDensity2D&			meshDens,
		const glm::dvec2&				recSize,
		double							height,
		const std::vector<glm::dvec2>&	recCoords,
		const std::vector<glm::dvec2>&	ellipseCoords,
		std::vector<glm::dvec3>&		recToEllipseDirs,
		std::vector<double>&			recToEllipseDist);

};

