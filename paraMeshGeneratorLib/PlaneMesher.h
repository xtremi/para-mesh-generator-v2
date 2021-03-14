#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class PlaneMesher : private Mesher
{
public:
	static void writeNodesPlane(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec2&	size,
		const glm::ivec2&	nnodes,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesPlaneQ(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec2&	dp,
		const glm::ivec2&	nnodes,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesPlaneXZq(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& dxz, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXYq(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& dxy, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneYZq(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& dyz, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXZ(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXY(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneYZ(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElementsPlane(
		FEAwriter*	writer,
		glm::ivec2	nnodes,
		bool		closedLoop);


};

class PlaneMesherRef : private Mesher {
public:
	static void writeNodesPlane_ref(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec2&	size,
		int					nNodesEdge,
		int					nRefinements,
		bool				startWithOffset,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesPlaneXY_ref(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements, bool startWithOffset, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneXZ_ref(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements, bool startWithOffset, glm::dmat3x3* csys = nullptr);
	static void writeNodesPlaneYZ_ref(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements, bool startWithOffset, glm::dmat3x3* csys = nullptr);

	static void writeElementsPlane_ref(
		FEAwriter*	writer,
		int			nNodesY,
		int			nRefinements,
		bool		closedLoop);

};

