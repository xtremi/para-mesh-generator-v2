#pragma once
#include "FeaWrite.h"
#include "Mesher.h"

class DiskMesher :	private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&	centerPos,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				endAng,		
		const glm::ivec2&	nnodes,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesX(const glm::dvec3& centerPos,
		double radiusStart, double radiusEnd, double startAng, double endAng,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesY(const glm::dvec3& centerPos,
		double radiusStart, double radiusEnd, double startAng, double endAng, 
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesZ(const glm::dvec3& centerPos,
		double radiusStart, double radiusEnd, double startAng, double endAng,
		const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElements(glm::ivec2 nnodes, bool closedLoop);

};

class DiskMesherRef : public Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&	centerPos,
		int					nNodesEdge,
		int					nRefinements,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				endAng,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(int nNodesY, int nRefinements, bool closedLoop);
};