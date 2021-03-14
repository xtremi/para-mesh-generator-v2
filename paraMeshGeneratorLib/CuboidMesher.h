#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class CuboidMesher : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec3&	size,
		const glm::ivec3&	nnodes,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesQ(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec3&	dp,
		const glm::ivec3&	nnodes,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesXYZq(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec3& dxz,  const glm::ivec3& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesXYZ(FEAwriter* writer,  const glm::dvec3& spos, const glm::dvec3& size, const glm::ivec3& nnodes, glm::dmat3x3* csys = nullptr);	

	static void writeElements(
		FEAwriter*	writer,
		glm::ivec3	nnodes,
		bool		closedLoop);


};


class CuboidMesherRef : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec3&	size,
		const glm::ivec2&	nNodesFace,
		int					nRefinements,
		bool				startWithOffset,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(
		FEAwriter*	writer,
		glm::ivec2	nNodesFace,
		int			nRefinements,
		bool		closedLoop);

private:
	static void writeElements_rows_bm1m2(
		FEAwriter*			writer,
		const glm::ivec2&	currentNodes12,
		glm::ivec2&			nextNodes12,
		int&				firstNodeBrow,
		int&				firstNodeM1row,
		int&				firstNodeM2row,
		bool				closedLoop);

	static void writeElements_rows_m2m3t(
		FEAwriter*			writer,
		const glm::ivec2&	currentNodes12,
		glm::ivec2&			nextNodes12,
		int&				firstNodeM2row,
		int&				firstNodeM3row,
		int&				firstNodeTrow,
		bool				closedLoop);
};