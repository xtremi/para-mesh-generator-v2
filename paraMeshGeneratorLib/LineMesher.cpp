#include "LineMesher.h"

void LineMesher::writeNodesLineXq(FEAwriter* writer, const glm::dvec3& spos, double dx, int nnodes, glm::dmat3x3* csys) {
	writeNodesLineQ(writer, spos, dx, nnodes, direction::x, csys);
}
void LineMesher::writeNodesLineYq(FEAwriter* writer, const glm::dvec3& spos, double dy, int nnodes, glm::dmat3x3* csys) {
	writeNodesLineQ(writer, spos, dy, nnodes, direction::y, csys);
}
void LineMesher::writeNodesLineZq(FEAwriter* writer, const glm::dvec3& spos, double dz, int nnodes, glm::dmat3x3* csys) {
	writeNodesLineQ(writer, spos, dz, nnodes, direction::z, csys);
}
void LineMesher::writeNodesLineX(FEAwriter* writer, const glm::dvec3&	spos, double length, int nnodes, glm::dmat3x3* csys) {
	writeNodesLine(writer, spos, length, nnodes, direction::x, csys);
}
void LineMesher::writeNodesLineY(FEAwriter* writer, const glm::dvec3&	spos, double length, int nnodes, glm::dmat3x3* csys) {
	writeNodesLine(writer, spos, length, nnodes, direction::y, csys);
}
void LineMesher::writeNodesLineZ(FEAwriter* writer, const glm::dvec3&	spos, double length, int nnodes, glm::dmat3x3* csys) {
	writeNodesLine(writer, spos, length, nnodes, direction::z, csys);
}

void LineMesher::writeNodesLine(FEAwriter* writer, const glm::dvec3& spos, double length, int nnodes, direction dir, glm::dmat3x3* csys)
{
	double ds = length / (double)(nnodes - 1);
	writeNodesLineQ(writer, spos, ds, nnodes, dir, csys);
}

void LineMesher::writeNodesLine(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec3& sposEnd, int nnodes,	glm::dmat3x3* csys)
{
	glm::dvec3 ds = (sposEnd - spos) / (double)(nnodes - 1);
	writeNodesLineQ(writer, spos, ds, nnodes, csys);
}

void LineMesher::writeNodesLineQ(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	double				ds,
	int					nnodes,
	direction			dir,
	glm::dmat3x3*		csys)
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coords, spos, csys);
		coords[(size_t)dir] += ds;
	}
}

void LineMesher::writeNodesLineQ(
	FEAwriter* writer,
	const glm::dvec3&	spos,
	const glm::dvec3&	ds,			/*any direction*/
	int					nnodes,
	glm::dmat3x3*		csys)
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coords, spos, csys);
		coords += ds;
	}
}

void LineMesher::writeNodesLineQ_nth(
	FEAwriter* writer,
	const glm::dvec3&	spos,
	double				ds,
	int					nnodes,
	int					skipNth,
	direction			dir,
	glm::dmat3x3*		csys)
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {		
		if (i%skipNth) {
			writer->writeNode(coords, spos, csys);
		}
		coords[(size_t)dir] += ds;
	}
}

void LineMesher::writeNodesLineQ_nth(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::dvec3&	ds,
	int					nnodes,
	int					skipNth,
	glm::dmat3x3*		csys)
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth) {
			writer->writeNode(coords, spos, csys);
		}
		coords += ds;
	}
}


void LineMesher::writeElementsLine(
	FEAwriter*	writer,
	int			nnodes,
	bool		closedLoop
)
{
	int n[2];
	int c = nodeID1;

	int nEl = closedLoop ? nnodes : nnodes - 1;

	for (int i = 0; i < nEl; i++) {
		n[0] = c++;
		n[1] = n[0] + 1;

		if (closedLoop && i == (nEl - 1)) {
			n[1] -= nnodes;
		}
		writer->write2nodedBeam(n);
	}
}