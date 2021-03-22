#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class LineMesher : private Mesher {
public:
	static void writeNodesLineQ(
		const glm::dvec3&	spos,
		double				dx,
		int					nnodes,
		direction			dir,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLineQ(
		const glm::dvec3&	spos,
		const glm::dvec3&	ds,
		int					nnodes,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLineQ_nth(
		const glm::dvec3&	spos,
		double				dx,
		int					nnodes,
		int					skipNth,
		direction			dir,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLineQ_nth(
		const glm::dvec3&	spos,
		const glm::dvec3&	ds,
		int					nnodes,
		int					skipNth,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLine(const glm::dvec3& spos, double length, int nnodes, direction dir, glm::dmat3x3* csys = nullptr);
	static void writeNodesLine(const glm::dvec3& spos, const glm::dvec3& sposEnd, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineXq(const glm::dvec3& spos, double dx, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineYq(const glm::dvec3& spos, double dx, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineZq(const glm::dvec3& spos, double dx, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineX(const glm::dvec3& spos, double length, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineY(const glm::dvec3& spos, double length, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineZ(const glm::dvec3& spos, double length, int nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElementsLine(
		int			nnodes,
		bool		closedLoop = false
	);
};
