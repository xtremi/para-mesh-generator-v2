#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecEdgeMesher : public Mesher
{

public:
	static void writeNodes(
		MeshCsys&		spos,
		int					nNodesWidth,
		int					nNodesHeight,
		const glm::dvec2&	size,
		plane				pln);

	static void writeNodes(
		MeshCsys&		spos,
		int					nnodes,
		const glm::dvec2&	size,
		plane				pln);

	static void writeNodes_nth(
		MeshCsys&		spos,
		int					nnodes,
		const glm::dvec2&	size,
		int					skipNth,
		plane				pln);

	static void writeNodes_nth(
		MeshCsys&		spos,
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
		MeshCsys&				spos,
		std::vector<glm::dvec2>&	coords,
		int							nnodes,
		const glm::dvec2&			size,
		int							skipNth,
		bool						skipCorners,
		plane						pln);

	static void getOrWriteCoords_nth(
		MesherMode					mode,
		MeshCsys&				spos,
		std::vector<glm::dvec2>&	coords,
		int							nNodesWidth,
		int							nNodesHeight,
		const glm::dvec2&			size,
		int							skipNth,
		bool						skipCorners,
		plane						pln);

	static std::vector<glm::dvec2> default_empty_coord_vec;
};

