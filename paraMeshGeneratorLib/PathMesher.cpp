#include "PathMesher.h"
#include "LineMesher.h"

void PathMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	int				  nnodes,
	const Path&		  path,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if (!skip(i, nnodes, nskip)) {
			writer->writeNode(pos + path.position(i, nnodes), glm::dvec3(0.), nullptr, &csys);
		}
	}
	MESHER_NODE_WRITE_END
}

void PathMesher::writeElements(int nnodes, bool closedLoop) {
	LineMesher::writeElementsLine(nnodes, closedLoop);
}