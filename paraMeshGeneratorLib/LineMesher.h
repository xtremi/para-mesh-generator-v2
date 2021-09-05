#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class LineMesher : private Mesher {
public:

	static void writeNodesLineQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		double					ds,		
		direction				dir);

	static void writeNodesLineQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		const glm::dvec3&		ds);

	static void writeNodesLineQ_nth(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		double					ds,		
		int						skipNth,
		direction				dir);

	static void writeNodesLineQ_nth(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		const glm::dvec3&		ds,
		int						skipNth);

	static void writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length,  direction dir);
	static void writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, const glm::dvec3& sposEnd);
	static void writeNodesLineXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dx);
	static void writeNodesLineYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dy);
	static void writeNodesLineZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dz);
	static void writeNodesLineX(const glm::dvec3& pos, MeshCsys& csys,  int nnodes, double length);
	static void writeNodesLineY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length);
	static void writeNodesLineZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length);

	static void writeElementsLine(int nnodes, bool closedLoop = false);

	static void writeNodes(const std::vector<glm::dvec2>& xycoords, double zcoord);



};
