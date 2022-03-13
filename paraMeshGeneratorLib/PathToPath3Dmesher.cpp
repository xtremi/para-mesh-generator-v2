#include "PathToPath3Dmesher.h"
#include "PathToPathMesher.h"
#include "CuboidMesher.h"

/*!
	This mesher creates a 3D mesh where the sections is defined by
	the area between \p pathInner and \p pathOuter (see PathToPathMesher).
	This section is extruded along the path \p extrudePath.

*/
void PathToPath3Dmesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys&		  csys,
	MeshDensity3D&	  meshDens,
	const Path&		  pathInner,
	const Path&		  pathOuter,
	const Path&		  extrudePath,
	const glm::dvec3& outerPathTranslation,
	node_skip		  nskip)
{
	MESHER_NODE_WRITE_START
	
	int nNodesExtrude = meshDens.dir3();
	VecGLM3d extrudeCoords = extrudePath.getPathCoordinates(nNodesExtrude, false);
	
	MeshDensity2D meshDens2D = meshDens.meshDensD12();
	VecGLM3d innerCoords, outDirs;
	VecD distances;
	getPathToPathData(pathInner, pathOuter, meshDens2D, innerCoords, outDirs, distances, outerPathTranslation);

	for (int i = 0; i < nNodesExtrude; i++) {
		PathToPathMesher::writeNodes(pos + extrudeCoords[i], csys, meshDens.dir2(), innerCoords, outDirs, distances, nskip);
	}

	MESHER_NODE_WRITE_END
}

void PathToPath3Dmesher::writeElements(const MeshDensity3D& meshDens)
{
	CuboidMesher::writeElements(meshDens);
}