#include "LineMesher.h"


void LineMesher::writeNodesXq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double dx) {
	writeNodesQ(pos, csys, meshDens, dx, direction::x);
}
void LineMesher::writeNodesYq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double dy) {
	writeNodesQ(pos, csys, meshDens, dy, direction::y);
}
void LineMesher::writeNodesZq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double dz) {
	writeNodesQ(pos, csys, meshDens, dz, direction::z);
}
void LineMesher::writeNodesX(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length) {
	writeNodes(pos, csys, meshDens, length, direction::x);
}
void LineMesher::writeNodesY(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length) {
	writeNodes(pos, csys, meshDens, length, direction::y);
}
void LineMesher::writeNodesZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length) {
	writeNodes(pos, csys, meshDens, length, direction::z);
}

void LineMesher::writeNodes(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length, direction dir)
{
	double ds = length / (double)(meshDens.nEl());
	writeNodesQ(pos, csys, meshDens, ds, dir);
}

void LineMesher::writeNodes(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, const glm::dvec3& sposEnd)
{
	glm::dvec3 ds = (sposEnd - pos) / (double)(meshDens.nEl());
	writeNodesQ(pos, csys, meshDens, ds);
}

void LineMesher::writeNodesQ(
	const glm::dvec3&	 pos,
	MeshCsys&			 spos,
	const MeshDensity1D& meshDens,
	double			     ds,	
	direction		     dir)
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < meshDens.nnodes; i++) {
		if(!meshDens.skip(i)){
			writer->writeNode(curPos, glm::dvec3(0.), nullptr, &spos);
		}
		curPos[(size_t)dir] += ds;
	}
MESHER_NODE_WRITE_END
}

void LineMesher::writeNodesQ(
	const glm::dvec3&	 pos,
	MeshCsys&			 csys,
	const MeshDensity1D& meshDens,
	const glm::dvec3&	 ds)			
{
MESHER_NODE_WRITE_START
	for (int i = 0; i < meshDens.nnodes; i++) {
		if (!meshDens.skip(i)) {
			writer->writeNode(curPos, glm::dvec3(0.), nullptr, &csys);
		}
		curPos += ds;
	}
MESHER_NODE_WRITE_END
}
/*
void LineMesher::writeNodesQ_nth(
	const glm::dvec3&	pos,
	MeshCsys&			spos,
	const MeshDensity1D& meshDens,
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

void LineMesher::writeNodesQ_nth(
	const glm::dvec3&	pos,
	MeshCsys&			spos,
	const MeshDensity1D& meshDens,
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
*/



void LineMesher::writeElements(const MeshDensity1D& meshDens)
{
	int n[2];
	int c = nodeID1;

	int nEl = meshDens.nEl();

	for (int i = 0; i < nEl; i++) {
		n[0] = c++;
		n[1] = n[0] + 1;

		if (meshDens.closedLoop && i == (nEl - 1)) {
			n[1] -= meshDens.nnodes;
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


