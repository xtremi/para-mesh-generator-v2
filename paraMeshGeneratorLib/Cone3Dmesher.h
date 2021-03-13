#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class Cone3Dmesher : public Mesher
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


	static void writeElements(
		FEAwriter*	writer,
		glm::ivec3	nnodes,
		bool		closedLoop);
};
