#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

/*#include "MeshPart.h"
class Line : public MeshPart1D
{
public:
	Line(double length, glm::dvec3& pos, MeshCsys& csys) : MeshPart1D(){}

private:
	direction dir;
};*/

enum class node_skip{none = 0, first = 1, last = -1, first_and_last = -2, every_2 = 2, every_3 = 3, every_4 = 4, every_5 = 5, every_6 = 6};

class LineMesher : private Mesher {

public:

	static void writeNodesLineQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		double					ds,		
		direction				dir,
		node_skip				nskip = node_skip::none);

	static void writeNodesLineQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		const glm::dvec3&		ds,
		node_skip				nskip = node_skip::none);

	static void writeNodesLineQ_nth(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		double					ds,		
		//int					skipNth,
		direction				dir,
		node_skip				nskip = node_skip::none);

	static void writeNodesLineQ_nth(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		const glm::dvec3&		ds,
		//int					skipNth,
		node_skip				nskip = node_skip::none);



	static void writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length,  direction dir, node_skip nskip = node_skip::none);
	static void writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, const glm::dvec3& sposEnd, node_skip nskip = node_skip::none);
	static void writeNodesLineXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dx, node_skip nskip = node_skip::none);
	static void writeNodesLineYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dy, node_skip nskip = node_skip::none);
	static void writeNodesLineZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dz, node_skip nskip = node_skip::none);
	static void writeNodesLineX(const glm::dvec3& pos, MeshCsys& csys,  int nnodes, double length, node_skip nskip = node_skip::none);
	static void writeNodesLineY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, node_skip nskip = node_skip::none);
	static void writeNodesLineZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, node_skip nskip = node_skip::none);

	static void writeElementsLine(int nnodes, bool closedLoop = false);

	static void writeNodes(const std::vector<glm::dvec2>& xycoords, double zcoord);

	static NodeIterator1D nodeIterator(int nnodes);

};

class LineStripMesher : private Mesher {

public:

	static void writeNodesLine(
		const glm::dvec3&			  pos,
		MeshCsys&					  csys,
		const LineStrip&			  lineStrip,
		const MeshDensity1DlineStrip& meshDens,
		node_skip				      skipFirst = node_skip::none);

	static void writeElements(const MeshDensity1DlineStrip& meshDens);

};