#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class CuboidMesher : public Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec3&	size,
		const glm::ivec3&	nnodes,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesRef(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec3&	size,
		const glm::ivec2&	nNodesFace,
		int					nRefinements,
		bool				startWithOffset,
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

	static void writeElementsRef(
		FEAwriter*	writer,
		glm::ivec2	nNodesFace,
		int			nRefinements,
		bool		closedLoop);

};
