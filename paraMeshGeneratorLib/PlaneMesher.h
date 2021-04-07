#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class PlaneMesher : private Mesher
{
public:
	static void writeNodesPlane(
		const MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		const glm::dvec2&	 size,		
		plane				 pln);

	static void writeNodesPlaneQ_nth(
		const MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		const glm::dvec2&	 dsize,
		plane				 pln,
		int					 skipNth,
		bool				 skipAlongDir1);

	static void writeNodesPlaneQ(
		const MeshCsys&			spos,
		const MeshDensity2D&	meshDens,
		const glm::dvec2&		dsize,		
		plane					pln);

	static void writeNodesPlaneXZq(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxz);
	static void writeNodesPlaneXYq(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxy);
	static void writeNodesPlaneYZq(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dyz);
	static void writeNodesPlaneXZ( const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size);
	static void writeNodesPlaneXY( const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size);
	static void writeNodesPlaneYZ( const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size);

	static void writeElementsPlane(const MeshDensity2D& meshDens);
};

class PlaneMesherRef : private Mesher {
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity2Dref& meshDens,
		const glm::dvec2&		size,
		bool					startWithOffset,
		plane					pln);

	static void writeNodesXY(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesXZ(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesYZ(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);

	static void writeElements(const MeshDensity2Dref& meshDens);

private:
	static void writeNodes_layerB(MeshCsys& curPos, int nodesPerRow, const glm::dvec2& elSize, direction refDir, direction edgeDir);
	static void writeNodes_layerM(MeshCsys& curPos, int nodesPerRow, const glm::dvec2& elSize, direction refDir, direction edgeDir);
	static void writeNodes_layerT(MeshCsys& curPos, int nodesPerRow, glm::dvec2& elSize, direction refDir, direction edgeDir);

	static void writeElements_layerB(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer);
	static void writeElements_layersMT(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer);
};

