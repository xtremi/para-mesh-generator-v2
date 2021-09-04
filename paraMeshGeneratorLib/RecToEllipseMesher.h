#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecToEllipseMesher : public Mesher
{
public:
	static void writeNodes(
		MeshCsys&				spos,
		const MeshDensity2D&	meshDens,
		const EllipseRadius&	radius,
		const glm::dvec2&		recSize,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);
	
	static void writeElements(const MeshDensity2D& meshDens);

	static void writeNodes(
		MeshCsys&			       spos,
		const MeshDensity2D&		   meshDens,
		const std::vector<glm::dvec2>& startCoords,
		const std::vector<glm::dvec3>& directions,
		const std::vector<double>&	   distances,
		double						   height,
		direction					   rotaxis);

	static void writeNodesPerimeter_nth(
		MeshCsys&					   spos,
		int							   nnodes,
		const std::vector<glm::dvec2>& startCoords,
		const std::vector<glm::dvec3>& directions,
		const std::vector<double>&	   distances,
		double						   distanceFactor,
		int							   skipNth,
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

class RecToEllipseMesherRef : private Mesher
{
public:
	static void writeNodes(
		MeshCsys&			spos,
		const MeshDensity2Dref&	meshDens,
		const EllipseRadius&	radius,
		const glm::dvec2&		recSize,
		const ArcAngles&		angle,
		double					height,
		bool					startWithOffset,
		direction				rotaxis);

	static void writeElements(const MeshDensity2Dref& meshDens);

private:

	struct RefShapeData {
		const MeshDensity2Dref* meshDens;
		const EllipseRadius*	radius;
		glm::dvec2				recSize;
		const ArcAngles*		angle;
		direction				rotAxis;
		double					height;
		double					maxLength;
		std::vector<glm::dvec2>* coordsSquare;
		std::vector<glm::dvec3>* recToEllipseDirs;
		std::vector<double>*	 recToEllipseDist;
	};
	struct RefLayerData {
		MeshCsys				curPos;
		double					curLength;
		double					curFactor;
		EllipseRadius			curRadius;
		glm::dvec2				curElSize;
	};

	static void incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData);
	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
};