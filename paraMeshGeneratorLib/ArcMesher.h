#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


class ArcMesher : private Mesher {
public:
	static void writeNodesCircular(
		FEAwriter*			feaWriter,
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				endAng,
		int					nnodes,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesCircularQ(
		FEAwriter*			feaWriter,
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				dAng,
		int					nnodes,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesCircularX(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double endAng, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularY(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double endAng, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularZ(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double endAng, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularXq(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double dAng,  int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularYq(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double dAng,  int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularZq(FEAwriter* feaWriter, const glm::dvec3& centerPos, double radius, double startAng, double dAng,  int nnodes, glm::dmat3x3* csys = nullptr);

	static void writeNodesCircular_nth(
		FEAwriter*			feaWriter,
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				endAng,
		int					nnodes,
		int					skipNth,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesCircularQ_nth(
		FEAwriter*			feaWriter,
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				dAng,
		int					nnodes,
		int					skipNth,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElementsLine(FEAwriter* writer, int nnodes, bool closedLoop = false);

};

