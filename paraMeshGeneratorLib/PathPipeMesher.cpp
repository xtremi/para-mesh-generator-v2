#include "PathPipeMesher.h"
#include "PathMesher.h"
#include "PlaneMesher.h"

void PathPipeMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	MeshDensity2D&	  meshDens,
	const Path&		  pathInner,
	const Path&		  pathOuter,
	const glm::dvec3& outerPathTranslation,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START

	VecGLM3d innerCoords, outerCoords;
	VecGLM3d outDirs;
	VecD distances;

	VecI nodesPerSegmentInner;
	innerCoords = getPathCoordinates(pathInner, meshDens.circ(), meshDens.closedLoop, &nodesPerSegmentInner);
	outerCoords = getPathCoordinates(pathOuter, nodesPerSegmentInner, meshDens.circ(), meshDens.closedLoop);

	for (int i = 0; i < meshDens.circ(); i++) {
		//innerCoords.push_back(pathInner.position(i, meshDens.circ()));
		//outerCoords.push_back(pathOuter.position(i, meshDens.circ()));
		outerCoords[i] += outerPathTranslation;
		outDirs.push_back(glm::normalize(outerCoords[i] - innerCoords[i]));
		distances.push_back(glm::distance(outerCoords[i],innerCoords[i]));
	}

	PathPipeMesher::writeNodes(pos, csys, meshDens.norm(), innerCoords, outDirs, distances, nskip);
	MESHER_NODE_WRITE_END
}

/*!
	Writes a \p nElout + 1 layers of nodes between (and including) \p innerCoords coordinates
	and the outer coords defined by the \p outwardsDirections and \p distances.
*/
void PathPipeMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	int				  nNodesOut				/*!number of elements from innerCoords and outwards*/,
	const VecGLM3d&	  innerCoords			/*!node coordinates on "the inside"*/,
	const VecGLM3d&	  outwardsDirections	/*!direction from innerCoords and out*/,
	const VecD&		  distances				/*!distances from innerCoords to end*/,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START
	for (int i = 0; i < nNodesOut; i++) {
		double distanceFactor = (double)i / (double)(nNodesOut - 1);
		writeNodesPerimeter(pos, csys, innerCoords, outwardsDirections, distances, distanceFactor, nskip);
	}
	MESHER_NODE_WRITE_END
}

/*!
	For each node in \p in startCoords, writes a node at the distance = \p distanceFactor x \p distance[i]
	away from the node in the direction \p directions[i]
*/
void PathPipeMesher::writeNodesPerimeter(
	const glm::dvec3&			   pos,
	MeshCsys&					   csys,
	const VecGLM3d&				   startCoords 		/*!original coordinates*/,
	const VecGLM3d&				   directions 		/*!direction from orginal coordinates and out*/,
	const VecD&					   distances 		/*!max distances from original coordinates and out*/,
	double						   distanceFactor	/*!the factor to multiply the distances with*/,
	node_skip					   nskip)
{
	int nNodes = startCoords.size();
	glm::dvec3 curCoord, curStep;
	for (int i = 0; i < nNodes; i++) {
		if (!skip(i, nNodes, nskip)) {
			curCoord = startCoords[i] + distanceFactor * distances[i] * directions[i];
			Mesher::writer->writeNode(pos + curCoord, NULL_POS, nullptr, &csys);
		}
	}

}


void PathPipeMesher::writeElements(const MeshDensity2D& meshDens) {
	PlaneMesher::writeElements(meshDens);
}