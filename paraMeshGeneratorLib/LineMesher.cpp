#include "LineMesher.h"

void LineMesher::writeNodesLineXq(MeshCsys& spos, int nnodes, double dx) {
	writeNodesLineQ(spos, nnodes, dx, direction::x);
}
void LineMesher::writeNodesLineYq(MeshCsys& spos, int nnodes, double dy) {
	writeNodesLineQ(spos, nnodes, dy, direction::y);
}
void LineMesher::writeNodesLineZq(MeshCsys& spos, int nnodes, double dz) {
	writeNodesLineQ(spos, nnodes, dz, direction::z);
}
void LineMesher::writeNodesLineX(MeshCsys& spos, int nnodes, double length) {
	writeNodesLine(spos, nnodes, length, direction::x);
}
void LineMesher::writeNodesLineY(MeshCsys& spos, int nnodes, double length) {
	writeNodesLine(spos, nnodes, length, direction::y);
}
void LineMesher::writeNodesLineZ(MeshCsys& spos, int nnodes, double length) {
	writeNodesLine(spos, nnodes, length, direction::z);
}

void LineMesher::writeNodesLine(MeshCsys& spos, int nnodes, double length, direction dir)
{
	double ds = length / (double)(nnodes - 1);
	writeNodesLineQ(spos, nnodes, ds, dir);
}

void LineMesher::writeNodesLine(MeshCsys& spos, int nnodes, const glm::dvec3& sposEnd)
{
	glm::dvec3 ds = (sposEnd - spos.pos) / (double)(nnodes - 1);
	writeNodesLineQ(spos, nnodes, ds);
}

void LineMesher::writeNodesLineQ(
	MeshCsys&		spos,
	int					nnodes,
	double				ds,	
	direction			dir)
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coords, glm::dvec3(0.), nullptr, &spos);
		coords[(size_t)dir] += ds;
	}
}

void LineMesher::writeNodesLineQ(
	MeshCsys&		spos,
	int					nnodes,
	const glm::dvec3&	ds)			/*any direction*/
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(coords, glm::dvec3(0.), nullptr, &spos);
		coords += ds;
	}
}

void LineMesher::writeNodesLineQ_nth(
	MeshCsys&	spos,
	int				nnodes,
	double			ds,	
	int				skipNth,
	direction		dir)
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {		
		if (i%skipNth) {
			writer->writeNode(coords, glm::dvec3(0.), nullptr, &spos);
		}
		coords[(size_t)dir] += ds;
	}
}

void LineMesher::writeNodesLineQ_nth(
	MeshCsys&		spos,
	int					nnodes,
	const glm::dvec3&	ds,	
	int					skipNth)
{
	nodeID1 = writer->getNextNodeID();

	glm::dvec3 coords(0.0);
	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth) {
			writer->writeNode(coords, glm::dvec3(0.), nullptr, &spos);
		}
		coords += ds;
	}
}


void LineMesher::writeElementsLine(int nnodes, bool closedLoop)
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


void LineMesher::writeNodes(const std::vector<glm::dvec2>& xycoords, double zcoord) {
	int firstNode = writer->getNextNodeID();

	for (const glm::dvec2& coords : xycoords) {
		writer->writeNode(glm::dvec3(coords, zcoord), glm::dvec3(0.0, 0.0, zcoord), nullptr);
	}
	Mesher::nodeID1 = firstNode;

}