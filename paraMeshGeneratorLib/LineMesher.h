#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class LineMesher : private Mesher {
public:
	static void writeNodesLineQ(
		const MeshCsys&			spos,
		int						nnodes,
		double					ds,		
		direction				dir);

	static void writeNodesLineQ(
		const MeshCsys&			spos,
		int						nnodes,
		const glm::dvec3&		ds);

	static void writeNodesLineQ_nth(
		const MeshCsys&			spos,
		int						nnodes,
		double					ds,		
		int						skipNth,
		direction				dir);

	static void writeNodesLineQ_nth(
		const MeshCsys&			spos,
		int						nnodes,
		const glm::dvec3&		ds,
		int						skipNth);

	static void writeNodesLine(const MeshCsys& spos, int nnodes, double length,  direction dir);
	static void writeNodesLine(const MeshCsys& spos, int nnodes, const glm::dvec3& sposEnd);
	static void writeNodesLineXq(const MeshCsys& spos, int nnodes, double dx);
	static void writeNodesLineYq(const MeshCsys& spos, int nnodes, double dy);
	static void writeNodesLineZq(const MeshCsys& spos, int nnodes, double dz);
	static void writeNodesLineX(const MeshCsys& spos,  int nnodes, double length);
	static void writeNodesLineY(const MeshCsys& spos, int nnodes, double length);
	static void writeNodesLineZ(const MeshCsys& spos, int nnodes, double length);

	static void writeElementsLine(
		int			nnodes,
		bool		closedLoop = false
	);
};
