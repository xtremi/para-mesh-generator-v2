#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecTubeMesher: public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&				spos,
		const MeshDensity2DrecTube& meshDens,
		const RecTubeSize&			size,
		plane						pln);

	static void writeNodes2(
		const MeshCsys&				spos,
		const MeshDensity2DrecTube& meshDens,
		const RecTubeSize&			size,
		plane						pln);

	//static void writeNodes2(
	//	const MeshCsys&		spos,
	//	int					nNodesWidth,
	//	int					nNodesHeight,
	//	int					nLayers,
	//	const RecTubeSize&	size,
	//	plane				pln);

	static void writeElements(const MeshDensity2DrecTube& meshDens);
	static void writeElements2(const MeshDensity2DrecTube& meshDens);
};

class RecTubeMesherRef : public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&		spos,
		const MeshDensity2Dref&	meshDens,
		int					nRefs,
		const RecTubeSize&	size,
		plane				pln);

	static void writeNodes2(
		const MeshCsys&		spos,
		const MeshDensity2Dref&	meshDens,
		const RecTubeSize&	size,
		plane				pln);

	static void writeNodes2(
		const MeshCsys&		spos,
		const MeshDensity2Dref&	meshDens,
		int					nNodesWidth,
		int					nNodesHeight,
		const RecTubeSize&	size,
		plane				pln);

	static void writeElements();

private:
	struct RefShapeData {
		const MeshDensity2Dref* meshDens;
		const RecTubeSize*		size;
		plane					pln;
		double					height;
		double					length;
	};
	struct RefLayerData {
		MeshCsys				curPos;
		double					curLength;
		glm::dvec2				curSize;
		glm::dvec2				curElSize;
	};

	static void incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData);
	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);

};