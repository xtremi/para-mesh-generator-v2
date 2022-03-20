#include "PathMesher.h"
#include "LineMesher.h"



void PathMesher::writeNodes__(
	const glm::dvec3&    pos,
	MeshCsys&		     csys,
	const MeshDensity1D& meshDens,
	const Path&		     path)
{
	MESHER_NODE_WRITE_START
	for (int i = 0; i < meshDens.nnodes; i++) {
		if (!meshDens.skip(i)) {
			writer->writeNode(
				pos + path.position(i, meshDens.closedLoop ? meshDens.nnodes + 1 : meshDens.nnodes), 
				NULL_POS, nullptr, &csys);
		}
	}
	MESHER_NODE_WRITE_END
}

void PathMesher::writeNodes(
	const glm::dvec3&    pos,
	MeshCsys&		     csys,
	const MeshDensity1D& meshDens,
	const Path&		     path,
	const VecD&		     requiredNodeLocations)
{
	MESHER_NODE_WRITE_START

	VecI nNodesPerSegment;
	VecD nodeSpacingPerSegment;
	if (!calculateNodeSpacing(meshDens.nnodes, requiredNodeLocations, nNodesPerSegment, nodeSpacingPerSegment, meshDens.closedLoop)) {
		throw("calculateNodeSpacing - failed");
	}

	double curPathLoc = 0.0;
	int node_i = 0;
	for (int j = 0; j < nNodesPerSegment.size(); j++) {

		for(int i = 0; i < nNodesPerSegment[j]; i++){
			if (!meshDens.skip(i)) {
				writer->writeNode(pos + path.position(curPathLoc), glm::dvec3(0.), nullptr, &csys);
			}
			curPathLoc += nodeSpacingPerSegment[j];
			node_i++;
		}
	}
	MESHER_NODE_WRITE_END
}

void PathMesher::writeNodes(
	const glm::dvec3&    pos,
	MeshCsys&		     csys,
	const MeshDensity1D& meshDens,
	const Path&			 path,
	bool				 setRequiredNodeLocationAtPathCorners)
{
	if (setRequiredNodeLocationAtPathCorners) {
		VecD requiredNodeLocatiosn = path.getCornerPathFactors();
		writeNodes(pos, csys, meshDens, path, requiredNodeLocatiosn);
	}
	else{
		writeNodes__(pos, csys, meshDens, path);
	}
}

void PathMesher::writeElements(const MeshDensity1D& meshDens) {
	LineMesher::writeElements(meshDens);
}