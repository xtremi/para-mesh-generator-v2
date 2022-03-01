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

class LineStrip {
public:
	LineStrip(){}
	LineStrip(const std::vector<glm::dvec3>& points);
	
	int nLines() const;
	bool getLineEndPoints(int line_i, glm::dvec3& p1, glm::dvec3& p2) const;

	std::vector<glm::dvec3> points;
};

class LineMesher : private Mesher {

public:

	static void writeNodesLineQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		double					ds,		
		direction				dir,
		bool					skipFirst = false);

	static void writeNodesLineQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		const glm::dvec3&		ds,
		bool					skipFirst = false);

	static void writeNodesLineQ_nth(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		double					ds,		
		int						skipNth,
		direction				dir,
		bool					skipFirst = false);

	static void writeNodesLineQ_nth(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		int						nnodes,
		const glm::dvec3&		ds,
		int						skipNth,
		bool					skipFirst = false);



	static void writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length,  direction dir, bool skipFirst = false);
	static void writeNodesLine(const glm::dvec3& pos, MeshCsys& csys, int nnodes, const glm::dvec3& sposEnd, bool skipFirst = false);
	static void writeNodesLineXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dx, bool skipFirst = false);
	static void writeNodesLineYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dy, bool skipFirst = false);
	static void writeNodesLineZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double dz, bool skipFirst = false);
	static void writeNodesLineX(const glm::dvec3& pos, MeshCsys& csys,  int nnodes, double length, bool skipFirst = false);
	static void writeNodesLineY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, bool skipFirst = false);
	static void writeNodesLineZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double length, bool skipFirst = false);

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
		bool				          skipFirst = false);

	static void writeElements(const MeshDensity1DlineStrip& meshDens);

};