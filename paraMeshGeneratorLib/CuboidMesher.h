#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class CuboidMesher : private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity3D&	meshDens,
		const glm::dvec3&		size,		
		plane					pln);

	static void writeNodesQ(
		const MeshCsys&			spos,
		const MeshDensity3D&	meshDens,
		const glm::dvec3&		dp,	
		plane					pln);

	static void writeNodesXYZq(const MeshCsys& spos, const MeshDensity3D& meshDens,const glm::dvec3& dxyz);
	static void writeNodesXYZ(const MeshCsys& spos, const MeshDensity3D& meshDens, const glm::dvec3& size);

	static void writeElements(const MeshDensity3D& meshDens);


};


class CuboidMesherRef : private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity3Dref&	meshDens,
		const glm::dvec3&		size,		
		bool					startWithOffset,
		plane					pln);

	static void writeElements(const MeshDensity3Dref& meshDens);

private:

	struct RefShapeData {
		const MeshDensity3Dref* meshDens;
		plane					pln;
		direction				refDir;		
		direction				edgeDir;
	};
	struct RefLayerData {
		MeshCsys				curPos;
		glm::dvec2				curElSize;
		double					curElSizeRefDir;
	};

	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM1(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM2(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM3(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);

	static void writeElements_layerB(const MeshDensity3Dref& meshDens, int& nid1, int curRefLayer);
	static void writeElements_layerBM1M2(const MeshDensity3Dref& meshDens, int& nid1, int curRefLayer);
	static void writeElements_layerM2M3T(const MeshDensity3Dref& meshDens, int& nid1, int curRefLayer);
};