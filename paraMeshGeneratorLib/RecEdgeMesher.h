#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecEdgeMesher : public Mesher
{

public:
	static void writeNodes(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nNodesWidth,
		int					nNodesHeight,
		const glm::dvec2&	size,
		plane				pln);

	static void writeNodes(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nnodes,
		const glm::dvec2&	size,
		plane				pln);

	static void writeNodes_nth(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nnodes,
		const glm::dvec2&	size,
		int					skipNth,
		plane				pln);

	static void writeNodes_nth(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nNodesWidth,
		int					nNodesHeight,
		const glm::dvec2&	size,
		int					skipNth,
		plane				pln);

	static void getLocalCoords(
		std::vector<glm::dvec2>& coords, 
		int						 nnodes,
		const glm::dvec2&		 size);

	static void writeElements(int nnodes, bool closedLoop = true);

public:
	static void getOrWriteCoords_nth(
		MesherMode					mode,
		const glm::dvec3&			pos,
		MeshCsys&					csys,
		std::vector<glm::dvec2>&	coords,
		int							nnodes,
		const glm::dvec2&			size,
		int							skipNth,
		bool						skipCorners,
		plane						pln);

	static void getOrWriteCoords_nth(
		MesherMode					mode,
		const glm::dvec3&			pos,
		MeshCsys&					csys,
		std::vector<glm::dvec2>&	coords,
		int							nNodesWidth,
		int							nNodesHeight,
		const glm::dvec2&			size,
		int							skipNth,
		bool						skipCorners,
		plane						pln);

	static std::vector<glm::dvec2> default_empty_coord_vec;
};

