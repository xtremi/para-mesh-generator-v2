#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class EllipseMesher :	public Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		int						nnodes,
		const EllipseRadius&	radius,
		const ArcAngles& 		arcAngles,
		direction				rotAxis);
	
	static void writeNodesQ(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		int						nnodes,
		const EllipseRadius&	radius,
		double					startAng,
		double					dAng,
		direction				rotAxis);

	static void getLocalCoords(
		std::vector<glm::dvec2>& coords,
		int						 nnodes,
		const EllipseRadius&	 radius,
		const ArcAngles& 		 arcAngles);

	static void getLocalCoordsQ(
		std::vector<glm::dvec2>& coords,
		int						 nnodes,
		const EllipseRadius&	 radius,
		double					 startAng,
		double					 dAng);

	static void writeElements(int nnodes, bool closedLoop = true);

private:
	static void getOrWriteCoords(
		MesherMode				 mode,
		const glm::dvec3&		 pos,
		MeshCsys&				 csys,
		std::vector<glm::dvec2>& coords,
		int						 nnodes,
		const EllipseRadius&	 radius,
		const ArcAngles& 		 arcAngles,
		direction				 rotAxis);

	static void getOrWriteCoordsQ(
		MesherMode				 mode,
		const glm::dvec3&		 pos,
		MeshCsys&				 csys,
		std::vector<glm::dvec2>& coords,
		int						 nnodes,
		const EllipseRadius&	 radius,
		double					 startAng,
		double					 dAng,
		direction				 rotAxis);

	static std::vector<glm::dvec2> default_empty_coord_vec;
};

