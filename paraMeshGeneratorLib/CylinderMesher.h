#pragma once
#include "Mesher.h"
#include "FeaWrite.h"



class CylinderMesher :	private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&	spos,
		double				radius,
		double				height,
		double				startAng,
		double				endAng,		
		const glm::ivec2&	nnodes,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesX(const glm::dvec3& spos, double radius, double height,
		double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesY(const glm::dvec3& spos, double radius, double height,
		double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesZ(const glm::dvec3& spos, double radius, double height,
		double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElements(const glm::ivec2& nnodes, bool closedLoop);
};

class CylinderMesherRef : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&	spos,
		int					nNodesEdge,
		int					nRefinements,
		double				radius,
		double				height,
		double				startAng,
		double				endAng,		
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(int nNodesY, int nRefinements, bool closedLoop);
};