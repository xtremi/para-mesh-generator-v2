#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecTubeMesher: public Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&			pos,
		MeshCsys&					csys,
		const MeshDensity2DrecTube& meshDens,
		const RecTubeSize&			size,
		plane						pln);

	static void writeNodes2(
		const glm::dvec3&			pos,
		MeshCsys&					csys,
		const MeshDensity2DrecTube& meshDens,
		const RecTubeSize&			size,
		double						height,
		plane						pln);

	static void writeElements(const MeshDensity2DrecTube& meshDens);
	static void writeElements2(const MeshDensity2DrecTube& meshDens);
};

class RecTubeMesherRef : public Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const glm::ivec2&		nNodesWidthHeight,
		int						nRefs,
		const glm::dvec2&		recSizeInner,
		plane					pln);

	static void writeNodes2(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity2Dref&	meshDens,
		const RecTubeSize&		size,
		double					height,
		plane					pln);

	static void writeNodes2(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity2Dref&	meshDens,
		const glm::ivec2&		nNodesWidthHeight,
		const RecTubeSize&		size,
		double					height,
		plane					pln);

	static void writeElements1(const glm::ivec2& nNodesWidthHeight, int	nRefs);
	static void writeElements2(const MeshDensity2Dref&	meshDens);

private:
	struct RefShapeData {
		MeshCsys*	csys;
		plane		pln;
	};
	struct RefLayerData {
		glm::dvec3 curPos;
		glm::dvec2 curRecSize;
		glm::dvec2 curElSizeWH;
		glm::ivec2 curNodesWH;
	};
	struct RefShapeData2 {
		MeshCsys*				csys;
		const MeshDensity2Dref* meshDens;
		const RecTubeSize*		size;
		plane					pln;
		direction				upAxis;
		double					height;
		double					length;
	};
	struct RefLayerData2 {
		glm::dvec3				curPos;
		double					curLength;
		glm::dvec2				curSize;
		glm::dvec2				curElSize;
	};

	static void incrementStep(const RefShapeData& rsData, RefLayerData& rlData);
	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);

	static void incrementStep2(const RefShapeData2& rsData, RefLayerData2& rlData);
	static void writeNodes2_layerB(const RefShapeData2& rsData, RefLayerData2& rlData, int refLayer);
	static void writeNodes2_layerM(const RefShapeData2& rsData, RefLayerData2& rlData, int refLayer);
	static void writeNodes2_layerT(const RefShapeData2& rsData, RefLayerData2& rlData, int refLayer);

	static void writeElements_layerB(const glm::ivec2& nNodesWidthHeight, int& nid1, int curRefLayer);
	static void writeElements_layersMT(const glm::ivec2& nNodesWidthHeight, int& nid1, int curRefLayer);
};