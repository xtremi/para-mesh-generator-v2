#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class LineMesher : private Mesher {
public:

	static void writeNodesLineQ(
		MeshCsys&			spos,
		int						nnodes,
		double					ds,		
		direction				dir);

	static void writeNodesLineQ(
		MeshCsys&			spos,
		int						nnodes,
		const glm::dvec3&		ds);

	static void writeNodesLineQ_nth(
		MeshCsys&			spos,
		int						nnodes,
		double					ds,		
		int						skipNth,
		direction				dir);

	static void writeNodesLineQ_nth(
		MeshCsys&			spos,
		int						nnodes,
		const glm::dvec3&		ds,
		int						skipNth);

	static void writeNodesLine(MeshCsys& spos, int nnodes, double length,  direction dir);
	static void writeNodesLine(MeshCsys& spos, int nnodes, const glm::dvec3& sposEnd);
	static void writeNodesLineXq(MeshCsys& spos, int nnodes, double dx);
	static void writeNodesLineYq(MeshCsys& spos, int nnodes, double dy);
	static void writeNodesLineZq(MeshCsys& spos, int nnodes, double dz);
	static void writeNodesLineX(MeshCsys& spos,  int nnodes, double length);
	static void writeNodesLineY(MeshCsys& spos, int nnodes, double length);
	static void writeNodesLineZ(MeshCsys& spos, int nnodes, double length);

	static void writeElementsLine(int nnodes, bool closedLoop = false);

	static void writeNodes(const std::vector<glm::dvec2>& xycoords, double zcoord);



};
