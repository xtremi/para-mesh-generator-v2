#pragma once
#include "glm/vec3.hpp"
enum class Dim { DIM1D, DIM2D, DIM3D };

enum class direction{x,y,z};
enum class plane {xy, xz, yz};

void getPlaneDirections(plane pln, direction& d1, direction& d2);
direction getPlaneNormal(plane pln);

class NodeIterator {
public:
	int first() {
		reset();
		return next();
	}
	virtual int next() = 0;

	int firstNodeID;
	int nodeIDoffset = 0;

protected:
	virtual void reset() = 0;	
};

/*

	 5    8   11   14   17   20
	 x    x    x    x    x    x
	 --> dir
*/
class NodeIterator1D : public NodeIterator {
public:
	NodeIterator1D(){}
	NodeIterator1D(int _firstNode, int _nNodes, int _nodeIncr) {
		firstNodeID = _firstNode; nNodes = _nNodes; nodeIncr = _nodeIncr;
	}
	int next() {
		if (currentIterIndex == nNodes)
			return 0;
		return firstNodeID + (currentIterIndex++) * nodeIncr + nodeIDoffset;
	}

	int nNodes;
	int nodeIncr;
private:
	void reset() {
		currentIterIndex = 0;
	}
	int currentIterIndex;

};

class MesherInput {
public:
	MesherInput() {}
	MesherInput(const glm::dvec3& _pos) : pos{ _pos } {}
	glm::dvec3 pos;
};

struct NodeVec2D {
	NodeVec2D() {}
	NodeVec2D(int _dir1, int _dir2) : dir1{ _dir1 }, dir2{ _dir2 } {}
	int dir1, dir2;
	int circ() { return dir1; }
	int norm() { return dir2; }
};
struct NodeVec3D : public NodeVec2D {
	NodeVec3D() {}
	NodeVec3D(int _dir1, int _dir2, int _dir3) : NodeVec2D(_dir1, _dir2), dir3{ _dir3 } {}

	int dir3;
	int axis() { return dir3; }
	int refDir() { return dir3; }
};

struct MeshDensity2D {
	MeshDensity2D() {}
	MeshDensity2D(int _dir1, int _dir2) : nodes{ NodeVec2D(_dir1, _dir2) } {}
	MeshDensity2D(const NodeVec2D& _nodes) : nodes{ _nodes } {}

	NodeVec2D nodes;
	int nElDir1(bool closedLoop = false) { return closedLoop ? nodes.dir1 - 1 : nodes.dir1; }
	int nElDir2(bool closedLoop = false) { return closedLoop ? nodes.dir2 - 1 : nodes.dir2; }
	int nElCirc(bool closedLoop = false) { return nElDir1(closedLoop); }
	int nElNorm(bool closedLoop = false) { return nElDir2(closedLoop); }
};

struct MeshDensity3D {
	MeshDensity3D() {}
	MeshDensity3D(int _dir1, int _dir2, int _dir3) : nodes{ NodeVec3D(_dir1, _dir2, _dir3) } {}
	MeshDensity3D(const NodeVec3D& _nodes) : nodes{ _nodes } {}

	NodeVec3D nodes;
	int nElDir1(bool closedLoop = false) { return closedLoop ? nodes.dir1 - 1 : nodes.dir1; }
	int nElDir2(bool closedLoop = false) { return closedLoop ? nodes.dir2 - 1 : nodes.dir2; }
	int nElDir3(bool closedLoop = false) { return closedLoop ? nodes.dir3 - 1 : nodes.dir3; }
	int nElCirc(bool closedLoop = false) { return nElDir1(closedLoop); }
	int nElNorm(bool closedLoop = false) { return nElDir2(closedLoop); }
	int nElAxis(bool closedLoop = false) { return nElDir3(closedLoop); }
	int nElRefDir(bool closedLoop = false) { return nElDir3(closedLoop); }
};

struct Pipe2Dradius {
	Pipe2Dradius() {}
	Pipe2Dradius(double _inner, double _outer) : inner{ _inner }, outer{ _outer }{}

	double inner, outer;
	double dR() const { return outer - inner; }
};
struct Pipe3Dradius {
	Pipe3Dradius() {}
	Pipe3Dradius(double startInner, double startOuter, double endInner, double endOuter)
		: start{ Pipe2Dradius(startInner, startOuter) }, end{ Pipe2Dradius(endInner, endOuter) } {}

	Pipe2Dradius start, end;
	double dRi() const { return end.inner - start.inner; }
	double dRo() const { return end.outer - start.outer; }
};
struct ArcAngles {
	ArcAngles() {}
	ArcAngles(double _start, double _end) : start{ _start }, end{ _end }{}
	double start, end;
	void setFullCircle() {
		start = -1.0; end = -1.0;
	}
};