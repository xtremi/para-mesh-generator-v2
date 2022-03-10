#include "PathMesher.h"
#include "LineMesher.h"

/*!
	This is currently used for PathLineStrip (but could have more use later. TBD.)

	Given a set of normalized locations (\p requiredNodeLocations) (along a path for example), between 0.0 and 1.0:	

  [0.0]  0.15       0.4   0.5                   1.0
	x-----x----------x-----x---------------------x
		NOTE: The location 0.0 should not be included in \p requiredNodeLocations


	and a number of nodes \totalNodes, this function calculates how many nodes
	should be between each locations (segments), and what the spacing between each
	nodes within each segment should be, for correctly distributing the nodes
	as evenly as possibly along the path (path of segment).

	The returned size of \p nodesPerSegment and \p nodeSpacingPerSegment vectors 
	are the same size as \p requiredLocations
*/
bool calculateNodeSpacing(
	int			totalNodes				/*!Total number of nodes*/,
	const VecD& requiredLocations		/*!Required locations [0.0 -> 1.0] where a nodes is required*/,
	VecI&		nodesPerSegment			/*!Calculated number of nodes between each req. locations (segment)*/,
	VecD&		nodeSpacingPerSegemnt	/*!Spacing between nodes for segment*/,
	bool		closedLoop				/*!Will account for a closed loop*/)
{
	VecI nnodesVec;
	double dpath	  = 0.;			//estimated spacing between nodes based on rest of path length
	int	   node_i	  = 0;			//the number of nodes accouted for at the iteration
	double curPathLoc = 0.0;		//the current location (0.0 -> 1.0) at the iteration
	double remainer  = 0.0;			//the remainer after flooring/rounding the node spacing
	int totalElements = totalNodes - 1;
	double endLoc = 1.0;
	if (closedLoop) {
		totalElements++;
		endLoc -= (1.0 / totalNodes);
	}

	for (int j = 0; j < requiredLocations.size(); j++) {

		double pathRemaining     = (endLoc - curPathLoc);
		int    elementsRemaining = totalElements - node_i;

		dpath = pathRemaining / (double)elementsRemaining;	//normalized approx. path step

		double segmentLength = requiredLocations[j] - curPathLoc;

		int nElseg = (int)(segmentLength / dpath);						//number of element that fit on segment with dpath size
		if (nElseg == 0) nElseg = 1;									//cannot have 0 elements

		if (j == (requiredLocations.size() - 1)) {
			if ((node_i + nElseg + 1) < totalNodes) {
				nElseg++;
			}
		}
		double actualDpath = segmentLength / (double)nElseg;			//actual normalized dpath with nElSeg on segment

		curPathLoc += (double)nElseg * actualDpath;
		node_i += nElseg;
		nodesPerSegment.push_back(nElseg);
		nodeSpacingPerSegemnt.push_back(actualDpath);
	}
	nodesPerSegment[nodesPerSegment.size() - 1] += 1;
	return true;
}

void PathMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys& csys,
	int				  nnodes,
	const Path& path,
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

void PathMesher::writeElements(int nnodes, bool closedLoop) {
	LineMesher::writeElementsLine(nnodes, closedLoop);
}