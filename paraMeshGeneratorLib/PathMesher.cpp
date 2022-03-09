#include "PathMesher.h"
#include "LineMesher.h"

void PathMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	int				  nnodes,
	const Path&		  path,
	const VecD&		  requiredNodeLocations,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START
	
	std::vector<int> nnodesVec;
	double dpath = 0.;
	if (requiredNodeLocations.size() > 0) {
		

		int node_i = 0;
		double curPathLoc = 0.0;
		double remainder = 0.0;
		for (double nodeLoc : requiredNodeLocations) {

			dpath = (1. - curPathLoc) / (double)(nnodes - node_i - 1);	//normalized approx. path step

			double pathLength = nodeLoc - curPathLoc;
			int nElseg = (int)(pathLength / dpath);						//number of element that fit on segment with dpath size
			if (nElseg == 0) nElseg = 1;
			double actualDpath = pathLength / (double)nElseg;			//actual normalized dpath for nElSeg on segment

			for (int i = 0; i < nElseg; i++) {
				if (!skip(node_i, nnodes, nskip)) {
					writer->writeNode(pos + path.position(curPathLoc), glm::dvec3(0.), nullptr, &csys);
				}
				curPathLoc += actualDpath;
				node_i++;
			}
		}
		writer->writeNode(pos + path.position(1.0), glm::dvec3(0.), nullptr, &csys);
		MESHER_NODE_WRITE_END
		return;
	}

	
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