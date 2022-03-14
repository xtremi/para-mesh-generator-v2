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
	VecGLM3d extrudeDirs = extrudePath.getPathTangents(nNodesExtrude, false);
	
	MeshDensity2D meshDens2D = meshDens.meshDensD12();
	VecGLM3d innerCoords, outDirs;
	VecD distances;
	getPathToPathData(pathInner, pathOuter, meshDens2D, innerCoords, outDirs, distances, outerPathTranslation);

	glm::dmat3x3 rotM = UNIT_MAT_3x3;

	MeshCsys csysPos(&csys, NULL_POS);
	MeshCsys csysExtrude(&csysPos, NULL_POS, &rotM);
	//MeshCsys csysTemp(NULL_POS);

	for (int i = 0; i < nNodesExtrude; i++) {
		glm::dvec3 xd(X_DIR);
		glm::dvec3 zd(extrudeDirs[i]);
		glm::dvec3 yd = glm::normalize(glm::cross(zd, xd));
		rotM  = makeCsysMatrix(xd, yd);
		csysPos.pos = pos + extrudeCoords[i];
		csysExtrude.updateParents();
		PathToPathMesher::writeNodes(NULL_POS, csysExtrude, meshDens.dir2(), innerCoords, outDirs, distances, nskip);
	}

	MESHER_NODE_WRITE_END
}

void PathToPath3Dmesher::writeElements(const MeshDensity3D& meshDens)
{
	CuboidMesher::writeElements(meshDens);
}