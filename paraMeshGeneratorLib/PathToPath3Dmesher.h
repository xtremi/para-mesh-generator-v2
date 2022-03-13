#pragma once
#include "Mesher.h"
#include "FeaWrite.h"
#include "Path.h"


class PathToPath3Dmesher : private Mesher {
public:
	static void writeNodes(
		const glm::dvec3& pos,
		MeshCsys&		  csys,
		MeshDensity3D&	  meshDens,
		const Path&		  pathInner,
		const Path&		  pathOuter,
		const Path&		  extrudePath,
		const glm::dvec3& outerPathTranslation = NULL_POS,
		node_skip		  nskip = node_skip::none);

	static void writeElements(const MeshDensity3D& meshDens);
};
