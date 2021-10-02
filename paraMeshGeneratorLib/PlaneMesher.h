#pragma once
#include "Mesher.h"
#include "FeaWrite.h"
#include "MeshDensity.h"

/*
	dir2 (default = y)

	^
[c3]|                   [c2]
	x13-x14-x15-x16-x17-x18
	|   |   |   |   |   |
	x7--x8--x9--x10-x11-x12
	|   |   |   |   |   |
	x1--x2--x3--x4--x5--x6  --->dir1 (default = x)
[c0]				    [c1]

*/


class PlaneMesher : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&	 pos,
		MeshCsys&			 csys,
		const MeshDensity2D& meshDens,
		const glm::dvec2&	 size,		
		plane				 pln);
	
	static void writeNodesQ(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity2D&	meshDens,
		const glm::dvec2&		dsize,
		plane					pln);
	
	static void writeNodesQ_nth(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity2D&	meshDens,
		const glm::dvec2&		dsize,
		plane					pln,
		int						skipNth,
		bool					skipAlongDir1);

	static void writeNodesXZq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& dxz);
	static void writeNodesXYq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& dxy);
	static void writeNodesYZq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& dyz);
	static void writeNodesXZ( const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& size);
	static void writeNodesXY( const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& size);
	static void writeNodesYZ( const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& size);

	static void writeElements(const MeshDensity2D& meshDens);
};

class PlaneMesherRef : private Mesher {
public:
	static void writeNodes(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity2Dref& meshDens,
		const glm::dvec2&		size,
		bool					startWithOffset,
		plane					pln);

	static void writeNodesXY(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesXZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesYZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);

	static void writeElements(const MeshDensity2Dref& meshDens);

private:

	struct RefShapeData {
		const MeshDensity2Dref* meshDens;
		direction				refDir;
		direction				edgeDir;
		MeshCsys*				csys;
	};
	struct RefLayerData {
		glm::dvec3				curPos;
		glm::dvec2				curElSize;
	};

	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);

	static void writeElements_layerB(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer);
	static void writeElements_layersMT(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer);
};

