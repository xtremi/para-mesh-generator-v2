#pragma once
#include "FeaWrite.h"
#include "Mesher.h"

class DiskMesher :	private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	centerPos,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				endAng,		
		const glm::ivec2&	nnodes,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesX(FEAwriter* writer, const glm::dvec3& centerPos,
		double radiusStart, double radiusEnd, double startAng, double endAng,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesY(FEAwriter* writer, const glm::dvec3& centerPos,
		double radiusStart, double radiusEnd, double startAng, double endAng, 
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesZ(FEAwriter* writer, const glm::dvec3& centerPos,
		double radiusStart, double radiusEnd, double startAng, double endAng,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElements(FEAwriter* writer, glm::ivec2 nnodes, bool closedLoop);

};

class DiskMesherRef : public Mesher
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
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(FEAwriter* writer, int nNodesY, int nRefinements, bool closedLoop);
};