#pragma once
#include "Mesher.h"
#include "FeaWrite.h"



class CylinderMesher :	private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		double				radius,
		double				height,
		double				startAng,
		double				endAng,		
		const glm::ivec2&	nnodes,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesX(FEAwriter* writer, const glm::dvec3& spos, double radius, double height,
		double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesY(FEAwriter* writer, const glm::dvec3& spos, double radius, double height,
		double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesZ(FEAwriter* writer, const glm::dvec3& spos, double radius, double height,
		double startAng, double endAng, const glm::ivec2& nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElements(FEAwriter* writer, glm::ivec2 nnodes, bool closedLoop);
};

class CylinderMesherRef : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		int					nNodesEdge,
		int					nRefinements,
		double				radius,
		double				height,
		double				startAng,
		double				endAng,		
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(FEAwriter* writer, int nNodesY, int nRefinements, bool closedLoop);
};