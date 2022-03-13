#pragma once
#include "Mesher.h"
#include "FeaWrite.h"
#include "Path.h"

void getPathToPathData(
	const Path&			 innerPath,
	const Path&			 outerPath,
	const MeshDensity2D& meshDens,
	VecGLM3d&			 innerCoords,
	VecGLM3d&			 outDirections,
	VecD&				 distances,
	const glm::dvec3&	 outerPathTranslation = NULL_POS);

class PathToPathMesher : private Mesher {
public:
	static void writeNodes(
		const glm::dvec3& pos,
		MeshCsys&		  csys,
		MeshDensity2D&	  meshDens,
		const Path&		  pathInner,
		const Path&		  pathOuter,
		const glm::dvec3& outerPathTranslation = NULL_POS,
		node_skip		  nskip = node_skip::none);


	static void writeNodes(
		const glm::dvec3& pos,
		MeshCsys&		  csys,
		int				  nNodesOut,
		const VecGLM3d&	  innerCoords,
		const VecGLM3d&	  outwardsDirections,
		const VecD&		  distances,
		node_skip		  nskip = node_skip::none);

	static void writeNodesPerimeter(
		const glm::dvec3& pos,
		MeshCsys&		  csys,
		const VecGLM3d&	  startCoords,
		const VecGLM3d&	  directions,
		const VecD&		  distances,
		double			  distanceFactor,
		node_skip		  nskip = node_skip::none);

	static void writeElements(const MeshDensity2D& meshDens);
};
