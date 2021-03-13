#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class ConeMesher : private Mesher
{
public:
	static void writeNodesCone(
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

	static void writeNodesConeX(FEAwriter* writer, const glm::dvec3& centerPos, 
		double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesConeY(FEAwriter* writer, const glm::dvec3& centerPos, 
		double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesConeZ(FEAwriter* writer, const glm::dvec3& centerPos, 
		double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElementsCone(FEAwriter* writer, glm::ivec2 nnodes, bool closedLoop);

};

class ConeMesherRef : private Mesher
{
public:
	static void writeNodesCone_ref(
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

	static void writeElementsCone_ref(FEAwriter* writer, int nNodesY, int nRefinements, bool closedLoop);
};