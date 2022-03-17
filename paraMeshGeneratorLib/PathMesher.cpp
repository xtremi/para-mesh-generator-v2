#include "PathMesher.h"
#include "LineMesher.h"



void PathMesher::writeNodes__(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	int				  nnodes,
	const Path&		  path,
	bool			  closedLoop,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START
	for (int i = 0; i < nnodes; i++) {
		if (!skip(i, nnodes, nskip)) {
			writer->writeNode(pos + path.position(i, closedLoop ? nnodes + 1 : nnodes), NULL_POS, nullptr, &csys);
		}
	}
	MESHER_NODE_WRITE_END
}

void PathMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	int				  nnodes,
	const Path&		  path,
	const VecD&		  requiredNodeLocations,
	bool			  closedLoop,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START

	VecI nNodesPerSegment;
	VecD nodeSpacingPerSegment;
	if (!calculateNodeSpacing(nnodes, requiredNodeLocations, nNodesPerSegment, nodeSpacingPerSegment, closedLoop)) {
		throw("calculateNodeSpacing - failed");
	}

	double curPathLoc = 0.0;
	int node_i = 0;
	for (int j = 0; j < nNodesPerSegment.size(); j++) {

		for(int i = 0; i < nNodesPerSegment[j]; i++){
			if (!skip(node_i, nnodes, nskip)) {
				writer->writeNode(pos + path.position(curPathLoc), glm::dvec3(0.), nullptr, &csys);
			}
			curPathLoc += nodeSpacingPerSegment[j];
			node_i++;
		}
	}
	MESHER_NODE_WRITE_END
}

void PathMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	int				  nnodes,
	const Path&		  path,
	bool			  setRequiredNodeLocationAtPathCorners,
	bool			  closedLoop,
	node_skip		  nskip)
{
	if (setRequiredNodeLocationAtPathCorners) {
		VecD requiredNodeLocatiosn = path.getCornerPathFactors();
		writeNodes(pos, csys, nnodes, path, requiredNodeLocatiosn, closedLoop, nskip);
	}
	else{
		writeNodes__(pos, csys, nnodes, path, closedLoop, nskip);
	}
}

void PathMesher::writeElements(int nnodes, bool closedLoop) {
	LineMesher::writeElementsLine(nnodes, closedLoop);
}