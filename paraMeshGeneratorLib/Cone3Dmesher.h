#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class Cone3Dmesher : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		double				radiusStartOuter,
		double				radiusEndOuter,
		double				radiusStartInner,
		double				radiusEndInner,
		double				startAng,
		double				endAng,
		double				height,
		const glm::ivec3&	nnodes,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);
#ifdef TEMP_DISABLE
	static void writeNodes_ref(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes12,
		int					nRefinements,
		double				radiusStartOuter,
		double				radiusEndOuter,
		double				radiusStartInner,
		double				radiusEndInner,
		double				startAng,
		double				endAng,
		double				height,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);
#endif
	static void writeElements(
		FEAwriter*	writer,
		glm::ivec3	nnodes,
		bool		closedLoop);
#ifdef TEMP_DISABLE
	static void writeElements_ref(
		FEAwriter*	writer,
		glm::ivec2	nnodes12,
		int			nRefinements,
		bool		closedLoop);
#endif
};
