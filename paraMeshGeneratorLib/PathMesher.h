#pragma once
#include "Mesher.h"
#include "FeaWrite.h"
#include "Path.h"

class PathMesher : private Mesher {
public:
	static void writeNodes(
		const glm::dvec3&	 pos,
		MeshCsys&			 csys,
		const MeshDensity1D& meshDens,
		const Path&		     path,
		const VecD&		     requiredNodeLocation);

	static void writeNodes(
		const glm::dvec3&    pos,
		MeshCsys&		     csys,
		const MeshDensity1D& meshDens,
		const Path&		     path,
		bool			     setRequiredNodeLocationAtPathCorners);

	static void writeNodes__(
		const glm::dvec3&    pos,
		MeshCsys&		     csys,
		const MeshDensity1D& meshDens,
		const Path&		     path);

	static void writeElements(const MeshDensity1D& meshDens);
};
