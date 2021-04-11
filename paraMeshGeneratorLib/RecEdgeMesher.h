#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecEdgeMesher : public Mesher
{

public:
	static void writeNodes(
		const MeshCsys&		spos,
		int					nnodes,
		const glm::dvec2&	size,
		plane				pln);

	static void getLocalCoords(
		std::vector<glm::dvec2>& coords, 
		int						 nnodes,
		const glm::dvec2&		 size);

	static void writeElements(int nnodes, bool closedLoop = true);

private:
	static void getOrWriteCoords(
		MesherMode				 mode,
		const MeshCsys&			 spos,
		std::vector<glm::dvec2>& coords,
		int						 nnodes,
		const glm::dvec2&		 size,
		plane					 pln);

	static std::vector<glm::dvec2> default_empty_coord_vec;
};

