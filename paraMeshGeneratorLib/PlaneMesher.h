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
	static void writeNodesPlane_ref(
		const MeshCsys&			spos,
		const MeshDensity2Dref& meshDens,
		const glm::dvec2&		size,
		bool					startWithOffset,
		plane					pln);

	static void writeNodesPlaneXY_ref(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesPlaneXZ_ref(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);
	static void writeNodesPlaneYZ_ref(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset);

	static void writeElementsPlane_ref(const MeshDensity2Dref& meshDens);

};

