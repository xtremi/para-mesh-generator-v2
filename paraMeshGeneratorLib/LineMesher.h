#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class LineMesher : public Mesher {
public:
	static void writeNodesLineQ(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		double				dx,
		int					nrows,
		direction			dir,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLineQ(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec3&	ds,
		int					nnodes,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLineQ_nth(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		double				dx,
		int					nnodes,
		int					skipNth,
		direction			dir,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLineQ_nth(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::dvec3&	ds,
		int					nnodes,
		int					skipNth,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodesLine(FEAwriter* writer, const glm::dvec3& spos, double length, int nrows, direction dir, glm::dmat3x3* csys = nullptr);
	static void writeNodesLine(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec3& sposEnd, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineXq(FEAwriter* writer, const glm::dvec3& spos, double dx, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineYq(FEAwriter* writer, const glm::dvec3& spos, double dx, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineZq(FEAwriter* writer, const glm::dvec3& spos, double dx, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineX(FEAwriter* writer, const glm::dvec3& spos, double length, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineY(FEAwriter* writer, const glm::dvec3& spos, double length, int nnodes, glm::dmat3x3* csys = nullptr);
	static void writeNodesLineZ(FEAwriter* writer, const glm::dvec3& spos, double length, int nnodes, glm::dmat3x3* csys = nullptr);

	static void writeElementsLine(
		FEAwriter*	writer,
		int			nnodes,
		bool		closedLoop = false
	);
};
