#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class ConeMesher : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	centerPos,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				endAng,
		double				height,
		const glm::ivec2&	nnodes,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesX(FEAwriter* writer, const glm::dvec3& centerPos, 
		double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesY(FEAwriter* writer, const glm::dvec3& centerPos, 
		double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesZ(FEAwriter* writer, const glm::dvec3& centerPos, 
		double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElements(FEAwriter* writer, glm::ivec2 nnodes, bool closedLoop);


	static void writeNodes_nthLine(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				dang,
		double				height,
		int					skipNth,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodes_nthArc(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				dang,
		double				height,
		int					skipNth,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);
};

class ConeMesherRef : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	centerPos,
		int					nNodesEdge,
		int					nRefinements,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				endAng,
		double				height,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(FEAwriter* writer, int nNodesY, int nRefinements, bool closedLoop);
};