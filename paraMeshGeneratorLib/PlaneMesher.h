#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class PlaneMesher : private Mesher
{
public:
	static void writeNodes(
		MeshCsys&			 spos,
		const MeshDensity2D& meshDens,
		const glm::dvec2&	 size,		
		plane				 pln);
	
	static void writeNodesQ(
		MeshCsys&				csys,
		const MeshDensity2D&	meshDens,
		const glm::dvec2&		dsize,
		plane					pln,
		const glm::dvec3&		pos = glm::dvec3(0.0));
	
	static void writeNodesQ_nth(
		MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		const glm::dvec2&	 dsize,
		plane				 pln,
		int					 skipNth,
		bool				 skipAlongDir1);

	static void writeNodesXZq(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxz);
	static void writeNodesXYq(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxy);
	static void writeNodesYZq(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dyz);
	static void writeNodesXZ( MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size);
	static void writeNodesXY( MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size);
	static void writeNodesYZ( MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size);

	static void writeElements(const MeshDensity2D& meshDens);
};

class PlaneMesherRef : private Mesher {
public:
	static void writeNodes(
		MeshCsys&			spos,
		const MeshDensity2Dref& meshDens,
		const glm::dvec2&		size,
		bool					startWithOffset,
		plane					pln);

	static void writeNodesXY(MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesXZ(MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesYZ(MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);

	static void writeElements(const MeshDensity2Dref& meshDens);

private:

	struct RefShapeData {
		const MeshDensity2Dref* meshDens;
		direction				refDir;
		direction				edgeDir;
	};
	struct RefLayerData {
		MeshCsys				curPos;
		glm::dvec2				curElSize;
	};

	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);

	static void writeElements_layerB(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer);
	static void writeElements_layersMT(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer);
};

