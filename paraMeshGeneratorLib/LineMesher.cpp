#include "LineMesher.h"

void LineMesher::writeNodesLineXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dx) {
	writeNodesLineQ(pos, csys, nnodes, dx, direction::x);
}
void LineMesher::writeNodesLineYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dy) {
	writeNodesLineQ(pos, csys, nnodes, dy, direction::y);
}
void LineMesher::writeNodesLineZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dz) {
	writeNodesLineQ(pos, csys, nnodes, dz, direction::z);
}
void LineMesher::writeNodesLineX(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length) {
	writeNodesLine(pos, csys, nnodes, length, direction::x);
}
void LineMesher::writeNodesLineY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length) {
	writeNodesLine(pos, csys, nnodes, length, direction::y);
}
void LineMesher::writeNodesLineZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length) {
	writeNodesLine(pos, csys, nnodes, length, direction::z);
}

void LineMesher::writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, direction dir)
{
	double ds = length / (double)(nnodes - 1);
	writeNodesLineQ(pos, csys, nnodes, ds, dir);
}

void LineMesher::writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, const glm::dvec3& sposEnd)
{
	glm::dvec3 ds = (sposEnd - pos) / (double)(nnodes - 1);
	writeNodesLineQ(pos, csys, nnodes, ds);
}

void LineMesher::writeNodesLineQ(
	const glm::dvec3& pos,
	MeshCsys&	      spos,
	int				  nnodes,
	double			  ds,	
	direction		  dir)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(curPos, glm::dvec3(0.), nullptr, &spos);
		curPos[(size_t)dir] += ds;
	}
MESHER_NODE_WRITE_END
}

void LineMesher::writeNodesLineQ(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	const glm::dvec3&	ds)			/*any direction*/
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		writer->writeNode(curPos, glm::dvec3(0.), nullptr, &csys);
		curPos += ds;
	}
MESHER_NODE_WRITE_END
}

void LineMesher::writeNodesLineQ_nth(
	const glm::dvec3&	pos,
	MeshCsys&			spos,
	int					nnodes,
	double				ds,	
	int					skipNth,
	direction			dir)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {		
		if (i%skipNth) {
			writer->writeNode(curPos, glm::dvec3(0.), nullptr, &spos);
		}
		curPos[(size_t)dir] += ds;
	}
MESHER_NODE_WRITE_END
}

void LineMesher::writeNodesLineQ_nth(
	const glm::dvec3&	pos,
	MeshCsys&			spos,
	int					nnodes,
	const glm::dvec3&	ds,	
	int					skipNth)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth) {
			writer->writeNode(curPos, glm::dvec3(0.), nullptr, &spos);
		}
		curPos += ds;
	}
MESHER_NODE_WRITE_END
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

NodeIterator1D LineMesher::nodeIterator(int nnodes) {
	return NodeIterator1D(Mesher::nodeID1, nnodes);
}