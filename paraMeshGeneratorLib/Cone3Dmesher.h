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



	static void writeElements(
		FEAwriter*	writer,
		glm::ivec3	nnodes,
		bool		closedLoop);



};

class Cone3DmesherRef : private Mesher
{
public:
	static void writeNodes(
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

	static void writeElements(
		FEAwriter*	writer,
		glm::ivec2	nnodes12,
		int			nRefinements,
		bool		closedLoop);

private:
	static void writeNodes_refLayer_1(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes12,
		double				radiusInner,
		double				radiusOuter,
		double				startAng,
		double				dang,
		int					skipNth,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodes_refLayer_2(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes12,
		double				radiusInner,
		double				radiusOuter,
		double				startAng,
		double				dang,
		int					skipNth,
		bool				closesLoop,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);
};