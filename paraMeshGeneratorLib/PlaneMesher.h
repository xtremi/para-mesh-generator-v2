#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class PlaneMesher : private Mesher
{
public:
	static void writeNodesPlane(
		const glm::dvec3&	spos,
		const glm::dvec2&	size,
		const glm::ivec2&	nnodes,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesPlaneQ(
		const glm::dvec3&	spos,
		const glm::dvec2&	dp,
		const glm::ivec2&	nnodes,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesPlaneXZq(const glm::dvec3& spos, const glm::dvec2& dxz, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXYq(const glm::dvec3& spos, const glm::dvec2& dxy, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneYZq(const glm::dvec3& spos, const glm::dvec2& dyz, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXZ(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXY(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneYZ(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElementsPlane(
		glm::ivec2	nnodes,
		bool		closedLoop);


};

class PlaneMesherRef : private Mesher {
public:
	static void writeNodesPlane_ref(
		const glm::dvec3&	spos,
		const glm::dvec2&	size,
		int					nNodesEdge,
		int					nRefinements,
		bool				startWithOffset,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesPlaneXY_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements, bool startWithOffset, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXZ_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements, bool startWithOffset, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneYZ_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements, bool startWithOffset, glm::dmat3x3* csys = nullptr);

	static void writeElementsPlane_ref(
		int			nNodesY,
		int			nRefinements,
		bool		closedLoop);

};

