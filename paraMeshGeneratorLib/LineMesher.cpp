#include "LineMesher.h"

LineStrip::LineStrip(const std::vector<glm::dvec3>& _points) : points{ _points } {

}

int LineStrip::nLines() const { return points.size() - 1; }
bool LineStrip::getLineEndPoints(int line_i, glm::dvec3& p1, glm::dvec3& p2) const {
	if (line_i < nLines()) {
		p1 = points[line_i]; p2 = points[line_i + 1];
		return true;
	}
	return false;
}


void LineMesher::writeNodesLineXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dx, bool skipFirst) {
	writeNodesLineQ(pos, csys, nnodes, dx, direction::x, skipFirst);
}
void LineMesher::writeNodesLineYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dy, bool skipFirst) {
	writeNodesLineQ(pos, csys, nnodes, dy, direction::y, skipFirst);
}
void LineMesher::writeNodesLineZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dz, bool skipFirst) {
	writeNodesLineQ(pos, csys, nnodes, dz, direction::z, skipFirst);
}
void LineMesher::writeNodesLineX(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, bool skipFirst) {
	writeNodesLine(pos, csys, nnodes, length, direction::x, skipFirst);
}
void LineMesher::writeNodesLineY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, bool skipFirst) {
	writeNodesLine(pos, csys, nnodes, length, direction::y, skipFirst);
}
void LineMesher::writeNodesLineZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, bool skipFirst) {
	writeNodesLine(pos, csys, nnodes, length, direction::z, skipFirst);
}

void LineMesher::writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, direction dir, bool skipFirst)
{
	double ds = length / (double)(nnodes - 1);
	writeNodesLineQ(pos, csys, nnodes, ds, dir, skipFirst);
}

void LineMesher::writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, const glm::dvec3& sposEnd, bool skipFirst)
{
	glm::dvec3 ds = (sposEnd - pos) / (double)(nnodes - 1);
	writeNodesLineQ(pos, csys, nnodes, ds, skipFirst);
}

void LineMesher::writeNodesLineQ(
	const glm::dvec3& pos,
	MeshCsys&	      spos,
	int				  nnodes,
	double			  ds,	
	direction		  dir,
	bool			  skipFirst)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if(!(skipFirst && i == 0)){
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
	bool			    skipFirst)			/*any direction*/
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if (!(skipFirst && i == 0)) {
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
	int					skipNth,
	direction			dir,
	bool			    skipFirst)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {		
		if (i%skipNth && !(skipFirst && i == 0)) {
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
	int					skipNth,
	bool			    skipFirst)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if (i%skipNth && !(skipFirst && i == 0)) {
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
	bool					      skipFirst)
{
	MESHER_NODE_WRITE_START
	int nLineStrips = lineStrip.nLines();	
	bool skipFirstNode = skipFirst;
	glm::dvec3 p1, p2;

	for (int i = 0; i < nLineStrips; i++) {
		lineStrip.getLineEndPoints(i, p1, p2);				
		LineMesher::writeNodesLine(pos + p1, csys, meshDens.nodesDistribution[i], pos + p2, skipFirstNode);
		skipFirstNode = true;
	}
	MESHER_NODE_WRITE_END
}

void LineStripMesher::writeElements(const MeshDensity1DlineStrip& meshDens) {
	LineMesher::writeElementsLine(meshDens.nEl(), meshDens.closedLoop);
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