#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


class ArcMesher : private Mesher {
public:
	static void writeNodesCircular(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				endAng,
		int					nnodes,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesCircularQ(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				dAng,
		int					nnodes,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesCircularX(const glm::dvec3& centerPos, double radius, double startAng, double endAng, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularY(const glm::dvec3& centerPos, double radius, double startAng, double endAng, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularZ(const glm::dvec3& centerPos, double radius, double startAng, double endAng, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularXq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,  int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularYq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,  int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesCircularZq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,  int nnodes, glm::dmat3x3* csys = nullptr);

	static void writeNodesCircular_nth(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				endAng,
		int					nnodes,
		int					skipNth,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesCircularQ_nth(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				dAng,
		int					nnodes,
		int					skipNth,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElementsLine(int nnodes, bool closedLoop = false);

};

