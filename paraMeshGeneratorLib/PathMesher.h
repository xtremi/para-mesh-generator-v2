#pragma once
#include "Mesher.h"
#include "FeaWrite.h"
#include "Path.h"



class PathMesher : private Mesher {
public:
	static void writeNodes(
		const glm::dvec3& pos,
		MeshCsys&		  csys,
		int				  nnodes,
		const Path&		  path,
		const VecD&		  requiredNodeLocation,
		bool			  closedLoop = false,
		node_skip		  nskip = node_skip::none);

	static void writeNodes(
		const glm::dvec3& pos,
		MeshCsys&		  csys,
		int				  nnodes,
		const Path&		  path,
		bool			  setRequiredNodeLocationAtPathCorners,
		bool			  closedLoop = false,
		node_skip		  nskip = node_skip::none);

	static void writeNodes(
		const glm::dvec3& pos,
		MeshCsys&		  csys,
		int				  nnodes,
		const Path&		  path,
		node_skip		  nskip = node_skip::none);

	static void writeElements(int nnodes, bool closedLoop = false);
};
