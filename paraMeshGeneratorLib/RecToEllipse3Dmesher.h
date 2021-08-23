#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecToEllipse3Dmesher : public Mesher
{
public:
	static void writeNodes(
		MeshCsys&			spos,
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

class RecToEllipse3DmesherRef : public Mesher
{
public:
	static void writeNodes(
		MeshCsys&			spos,
		const MeshDensity3Dref&	meshDens,
		const EllipseRadius&	radiusStart,
		const EllipseRadius&	radiusEnd,
		const glm::dvec2&		recSizeStart,
		const glm::dvec2&		recSizeEnd,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);

	static void writeElements(const MeshDensity3Dref& meshDens);


private:
	struct RefShapeData {
		const MeshDensity3Dref* meshDens;
		const EllipseRadius*	radiusStart;
		const EllipseRadius*	radiusEnd;
		const ArcAngles*		angle;
		const glm::dvec2*		recSizeStart;
		const glm::dvec2*		recSizeEnd;
		direction				rotAxis;
		double					height;
	};

	struct RefLayerData {
		MeshCsys				curPos;
	};

	static void incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData);
	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM1(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM2(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM3(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
};

