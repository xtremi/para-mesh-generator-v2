#include "LineMesher.h"


void LineMesher::writeNodesLineXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dx, node_skip nskip) {
	writeNodesLineQ(pos, csys, nnodes, dx, direction::x, nskip);
}
void LineMesher::writeNodesLineYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dy, node_skip nskip) {
	writeNodesLineQ(pos, csys, nnodes, dy, direction::y, nskip);
}
void LineMesher::writeNodesLineZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dz, node_skip nskip) {
	writeNodesLineQ(pos, csys, nnodes, dz, direction::z, nskip);
}
void LineMesher::writeNodesLineX(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, node_skip nskip) {
	writeNodesLine(pos, csys, nnodes, length, direction::x, nskip);
}
void LineMesher::writeNodesLineY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, node_skip nskip) {
	writeNodesLine(pos, csys, nnodes, length, direction::y, nskip);
}
void LineMesher::writeNodesLineZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, node_skip nskip) {
	writeNodesLine(pos, csys, nnodes, length, direction::z, nskip);
}

void LineMesher::writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, direction dir, node_skip nskip)
{
	double ds = length / (double)(nnodes - 1);
	writeNodesLineQ(pos, csys, nnodes, ds, dir, nskip);
}

void LineMesher::writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, const glm::dvec3& sposEnd, node_skip nskip)
{
	glm::dvec3 ds = (sposEnd - pos) / (double)(nnodes - 1);
	writeNodesLineQ(pos, csys, nnodes, ds, nskip);
}

void LineMesher::writeNodesLineQ(
	const glm::dvec3& pos,
	MeshCsys&	      spos,
	int				  nnodes,
	double			  ds,	
	direction		  dir,
	node_skip		  nskip)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if(!skip(i, nnodes, nskip)){
			writer->writeNode(curPos, glm::dvec3(0.), nullptr, &spos);
		}
		curPos[(size_t)dir] += ds;
	}
MESHER_NODE_WRITE_END
}

void LineMesher::writeNodesLineQ(
	const glm::dvec3&	pos,
	MeshCsys&			csys,
	int					nnodes,
	const glm::dvec3&	ds,
	node_skip			nskip)			/*any direction*/
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if (!skip(i, nnodes, nskip)) {
			writer->writeNode(curPos, glm::dvec3(0.), nullptr, &csys);
		}
		curPos += ds;
	}
MESHER_NODE_WRITE_END
}

void LineMesher::writeNodesLineQ_nth(
	const glm::dvec3&	pos,
	MeshCsys&			spos,
	int					nnodes,
	double				ds,	
	//int				skipNth,
	direction			dir,
	node_skip			nskip)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {		
		if (!skip(i, nnodes, nskip)) {
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
	//int					skipNth,
	node_skip			nskip)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if (!skip(i, nnodes, nskip)) {
			writer->writeNode(curPos, glm::dvec3(0.), nullptr, &spos);
		}
		curPos += ds;
	}
MESHER_NODE_WRITE_END
}

void LineStripMesher::writeNodesLine(
	const glm::dvec3&			  pos,
	MeshCsys&					  csys,
	const LineStrip&			  lineStrip,
	const MeshDensity1DlineStrip& meshDens,
	node_skip					  nskip)
{
	MESHER_NODE_WRITE_START
	int nLineStrips = lineStrip.nLines();	
	node_skip nskipNode = nskip;
	glm::dvec3 p1, p2;

	for (int i = 0; i < nLineStrips; i++) {
		lineStrip.getLineEndPoints(i, p1, p2);				
		if (meshDens.closedLoop) {
			if (i == (nLineStrips - 1)) {
				nskipNode = node_skip::first_and_last;
			}
		}
		LineMesher::writeNodesLine(pos + p1, csys, meshDens.nodesDistribution[i], pos + p2, nskipNode);
		nskipNode = node_skip::first;
	}
	MESHER_NODE_WRITE_END
}

void LineStripMesher::writeElements(const MeshDensity1DlineStrip& meshDens) {
	LineMesher::writeElementsLine(meshDens.nnodes, meshDens.closedLoop);
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