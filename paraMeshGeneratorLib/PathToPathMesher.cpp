#include "PathToPathMesher.h"
#include "PathMesher.h"
#include "PlaneMesher.h"

void PathToPathMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	MeshDensity2D&	  meshDens,
	const Path&		  pathInner,
	const Path&		  pathOuter,
	const glm::dvec3& outerPathTranslation,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START
	VecGLM3d innerCoords, outDirs;
	VecD distances;

	getPathToPathData(pathInner, pathOuter, meshDens, innerCoords, outDirs, distances, outerPathTranslation);
	PathToPathMesher::writeNodes(pos, csys, meshDens.norm(), innerCoords, outDirs, distances, nskip);
	MESHER_NODE_WRITE_END
}

/*!
	Given the mesh density, retreives the node coordinates along \p innerCoords and
	\p outerCoords. 
	For each coordinate, this function calculates the directions from from inner and outer path, and 
	the distances.

	The coordinates for the paths are retreived using Path::getPathCoordinates.

*/
void getPathToPathData(
	const Path&			 innerPath				/*!The inner path*/,
	const Path&			 outerPath				/*!The outer path*/,
	const MeshDensity2D& meshDens				/*!dir1 is along the paths, dir2 between the paths*/,
	VecGLM3d&			 innerCoords			/*![out] The coordinates along innerPath*/,
	VecGLM3d&			 outDirections			/*![out] The directions from innerPath to outerPath*/,
	VecD&				 distances				/*![out] The distances between the coords on innerPath and outerPath*/,
	const glm::dvec3&	 outerPathTranslation	/*![in][optional] Is added to the coordinates of outerPath*/)
{
	VecI nodesPerSegmentInner;
	VecGLM3d outerCoords;
	innerCoords = innerPath.getPathCoordinates(meshDens.circ(), meshDens.closedLoop, &nodesPerSegmentInner);
	outerCoords = outerPath.getPathCoordinates(nodesPerSegmentInner, meshDens.circ(), meshDens.closedLoop);

	for (int i = 0; i < meshDens.circ(); i++) {
		outerCoords[i] += outerPathTranslation;
		outDirections.push_back(glm::normalize(outerCoords[i] - innerCoords[i]));
		distances.push_back(glm::distance(outerCoords[i], innerCoords[i]));
	}
}


/*!
	Writes a \p nElout + 1 layers of nodes between (and including) \p innerCoords coordinates
	and the outer coords defined by the \p outwardsDirections and \p distances.
*/
void PathToPathMesher::writeNodes(
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
void PathToPathMesher::writeNodesPerimeter(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	const VecGLM3d&	  startCoords 		/*!original coordinates*/,
	const VecGLM3d&	  directions 		/*!direction from orginal coordinates and out*/,
	const VecD&		  distances 		/*!max distances from original coordinates and out*/,
	double			  distanceFactor	/*!the factor to multiply the distances with*/,
	node_skip		  nskip)
{
	int nNodes = startCoords.size();
	glm::dvec3 curCoord, curStep;
	for (int i = 0; i < nNodes; i++) {
		if (!skipCheck(i, nNodes, node_skip::none, (int)nskip)) {
			curCoord = startCoords[i] + distanceFactor * distances[i] * directions[i];
			Mesher::writer->writeNode(pos + curCoord, NULL_POS, nullptr, &csys);
		}
	}

}


void PathToPathMesher::writeElements(const MeshDensity2D& meshDens) {
	PlaneMesher::writeElements(meshDens);
}