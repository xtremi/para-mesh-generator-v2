#pragma once
#include <glm/glm.hpp>
#include "math_utilities.h"

enum class Dim { DIM1D, DIM2D, DIM3D };

enum class direction{x,y,z};
enum class plane {xy, xz, yz};

void getPlaneDirections(plane pln, direction& d1, direction& d2);
direction getPlaneNormal(plane pln);
glm::dvec3 coordsOnCircle(double angle, double radius, direction rotAxis);


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
#ifdef TO_BE_REMOVED_MAYBE
class MesherInput {
public:
	MesherInput() {}
	MesherInput(const glm::dvec3& _pos) : pos{ _pos } {}
	glm::dvec3 pos;
};
#endif

/*
	Wrapper for glm::ivec2 representing number of nodes for a "2D structured mesh"	
*/
struct NodeVec2D {
	NodeVec2D() {}
	NodeVec2D(int _dir1, int _dir2) : nnodes{ glm::ivec2(_dir1, _dir2) }{}
	
	glm::ivec2& nodes() { return nnodes; }
	int dir1() const { return nnodes.x; }
	int dir2() const { return nnodes.y; }
	/*
		Circular direction, around cicumference
	*/
	virtual int circ() const { return dir1(); }
	/*
		Radial/normal direction, out of circle
	*/
	virtual int norm() const { return dir2(); }


	void setDir1(int n) { nnodes.x = n; }
	void setDir2(int n) { nnodes.y = n; }
	virtual void setCirc(int n) { setDir1(n); }
	virtual void setNorm(int n) { setDir2(n); }

protected:
	glm::ivec2 nnodes;
};

/*
	Wrapper for glm::ivec3 representing number of nodes for a "3D structured mesh"
*/
struct NodeVec3D {
	NodeVec3D() {}
	NodeVec3D(int _dir1, int _dir2, int _dir3) : nnodes{ glm::ivec3(_dir1, _dir2, _dir3) } {}

	glm::ivec3& nodes() { return nnodes; }
	int dir1() const { return nnodes.x; }
	int dir2() const { return nnodes.y; }
	int dir3() const { return nnodes.z; }
	/*
		Circular direction, around cicumference (for cone/cylinder)
	*/
	int circ() const { return dir1(); }
	/*
		Radial/normal direction, out of circle (for cone/cylinder)
	*/
	int norm() const { return dir2(); }
	/*
		Along axis/height of a cone/cylinder
	*/
	int axis()   const { return dir3(); }

	void setDir1(int n) { nnodes.x = n; }
	void setDir2(int n) { nnodes.y = n; }
	void setDir3(int n) { nnodes.z = n; }
	void setCirc(int n) { setDir1(n); }
	void setNorm(int n) { setDir2(n); }
	void setAxis(int n) { setDir3(n); }

protected:
	glm::ivec3 nnodes;
};

/*
	Mesh density for 1D structured mesh with element number function and closed loop definition
*/
struct MeshDensity1D {
	MeshDensity1D(int _nnodes, bool _closedLoop = false) : nnodes{ _nnodes }, closedLoop{ _closedLoop }{}
	int nEl() const { return closedLoop ? nnodes : nnodes - 1; }	
	bool closedLoop;
	int nnodes;
};

/*
	Extends NodeVec2D with element number functions and closed loop definition
*/
struct MeshDensity2D : public NodeVec2D {
	MeshDensity2D() {}
	MeshDensity2D(int _dir1, int _dir2, bool closedLoopDir1 = false) : NodeVec2D(_dir1, _dir2), closedLoop{ closedLoopDir1 } {}
	
	bool closedLoop;
	void setClosedLoop(bool _closedLoop = true) { closedLoop = _closedLoop; }

	int nElDir1() const { return closedLoop ? dir1() : dir1() - 1; }
	int nElDir2() const { return dir2() - 1; }
	int nElCirc() const { return nElDir1(); }
	int nElNorm() const { return nElDir2(); }

	int nNodes() const { return dir1() * dir2(); }
};

/*
	Extends NodeVec3D with element number functions and closed loop definition
*/
struct MeshDensity3D : public NodeVec3D {
	MeshDensity3D() {}
	MeshDensity3D(int _dir1, int _dir2, int _dir3, bool closedLoopDir1 = false) : NodeVec3D(_dir1, _dir2, _dir3), closedLoop{ closedLoopDir1 } {}

	bool closedLoop;
	void setClosedLoop(bool _closedLoop = true) { closedLoop = _closedLoop; }

	int nElDir1() const { return closedLoop ? dir1() : dir1() - 1; }
	int nElDir2() const { return dir2() - 1; }
	int nElDir3() const { return dir3() - 1; }
	int nElCirc() const { return nElDir1(); }
	int nElNorm() const { return nElDir2(); }
	int nElAxis() const { return nElDir3(); }

	MeshDensity2D meshDensD12() const { return MeshDensity2D(dir1(), dir2(), closedLoop); }
	int nnodesPlaneD12() const { return dir1() * dir2(); }
};

/*
	Extends NodeVec2D with element number functions and closed loop definition,
	for a 2D structured mesh with refinement in direction 1.
*/
struct MeshDensity2Dref : public NodeVec2D {
	MeshDensity2Dref() {}
	MeshDensity2Dref(int _nRefDir1, int _nNodesDir2, bool closedLoopDir2 = false) : NodeVec2D(_nRefDir1, _nNodesDir2), closedLoop{ closedLoopDir2 } {}

	void setClosedLoop(bool _closedLoop = true) { closedLoop = _closedLoop; }
	bool closedLoop;
	
	int nRefs() const { return dir1(); }
	int nElDir2() const { return closedLoop ? dir2() : dir2() - 1; }
	int nElCirc() const { return nElDir2(); }

	void setNrefs(int n) { setDir1(n); }

	//virtual from NodeVec2D
	void setCirc(int n) { setDir2(n); }
	void setNorm(int n) { setDir1(n); }
	int circ() const { return dir2(); }
	int norm() const { return dir1(); }

	//First layer is refLayer = 0
	int nElRowB(int refLayer) const {
		return nElDir2() / std::pow(2, refLayer);
	}
	int nElRowT(int refLayer) const {
		return nElRowB(refLayer + 1);
	}

	int nNodesRowB(int refLayer) const {
		return closedLoop ? nElRowB(refLayer) : nElRowB(refLayer) + 1;
	}
	int nNodesRowM(int refLayer) const {
		return 3 * nElRowB(refLayer) / 4;
	}
	int nNodesRowT(int refLayer) const {
		return nNodesRowB(refLayer + 1);
	}

};

/*
	Extends NodeVec3D with element number functions and closed loop definition,
	for a 3D structured mesh with refinement in direction 3
*/
struct MeshDensity3Dref : public NodeVec3D {
	MeshDensity3Dref() {}
	MeshDensity3Dref(int _nRefDir3, int _nNodesDir1, int _nNodesDir2, bool closedLoopDir1 = false) 
		: NodeVec3D(_nNodesDir1, _nNodesDir2, _nRefDir3), closedLoop{ closedLoopDir1 } {}

	bool closedLoop;
	void setClosedLoop(bool _closedLoop = true) { closedLoop = _closedLoop; }

	int nRefs() const { return dir3(); }
	int nElDir1() const { return closedLoop ? dir1() : dir1() - 1; }
	int nElDir2() const { return dir2() - 1; }
	int nElCirc() const { return nElDir1(); }

	MeshDensity2D meshDensD12B(int refLayer) const { 
		int n1 = nElDir1() / std::pow(2, refLayer);
		if (!closedLoop) n1++;
		int n2 = nElDir2() / std::pow(2, refLayer) + 1;
		return MeshDensity2D(n1, n2, closedLoop); 
	}
	MeshDensity2D meshDensD12M1(int refLayer) const {
		int n1 = 3 * (nElDir1() / std::pow(2, refLayer)) / 4;		
		int n2 = nElDir2() / std::pow(2, refLayer) + 1;
		return MeshDensity2D(n1, n2, closedLoop);
	}
	MeshDensity2D meshDensD12M2(int refLayer) const {
		int n1 = nElDir1() / std::pow(2, refLayer + 1);
		if (!closedLoop) n1++;
		int n2 = nElDir2() / std::pow(2, refLayer) + 1;
		return MeshDensity2D(n1, n2, closedLoop);
	}
	MeshDensity2D meshDensD12M3(int refLayer) const {
		int n1 = nElDir1() / std::pow(2, refLayer + 1);
		if (!closedLoop) n1++;
		int n2 = 3 * (nElDir2() / std::pow(2, refLayer)) / 4;// +1;
		return MeshDensity2D(n1, n2, closedLoop);
	}
	MeshDensity2D meshDensD12T(int refLayer) const {
		return meshDensD12B(refLayer + 1);
	}
	int nnodesPlaneD12() const { return dir1() * dir2(); }

	int setNrefs(int n) { setDir1(n); }

};

/*
	Defines a mesh position and orientation:
	- pos : position of mesh
	- csys: 3x3 rotation matrix (if null, not used)
*/
struct MeshCsys {
	MeshCsys(){}
	MeshCsys(const glm::dvec3& _pos, glm::dmat3x3* _csys = nullptr) : pos{ _pos }, csys{ _csys } {}

	glm::dvec3		pos;
	glm::dmat3x3*	csys = nullptr;
};

struct Pipe2Dradius {
	Pipe2Dradius() {}
	Pipe2Dradius(double _rad1, double _rad2) : radi{ glm::dvec2(_rad1, _rad2) }{}

	double rad1() const { return radi.x; };
	double rad2() const { return radi.y; };
	double dR() const { return radi.y - radi.x; }
	void setRad1(double r) { radi.x = r; }
	void setRad2(double r) { radi.y = r; }
	void incrRad1(double dr) { radi.x += dr; }
	void incrRad2(double dr) { radi.y += dr; }
	glm::dvec2 radi;
};

struct Disk2Dradius : public Pipe2Dradius{
	Disk2Dradius() {}
	Disk2Dradius(const Pipe2Dradius& pipe2drad) { radi = pipe2drad.radi; }
	Disk2Dradius(double _inner, double _outer) : Pipe2Dradius(_inner, _outer) {}
	double inner() const { return radi.x; }
	double outer() const { return radi.y; }
	void setInner(double r) { setRad1(r); }
	void setOuter(double r) { setRad2(r); }
	void incrInner(double dr) { incrRad1(dr); }
	void incrOuter(double dr) { incrRad2(dr); }
};

struct Cone2Dradius : public Pipe2Dradius {
	Cone2Dradius() {}
	Cone2Dradius(const Pipe2Dradius& pipe2drad) { radi = pipe2drad.radi; }
	Cone2Dradius(double _start, double _end) : Pipe2Dradius(_start, _end) {}
	double start() const { return radi.x; }
	double end() const { return radi.y; }
	void setStart(double r) { setRad1(r); }
	void setEnd(double r) { setRad2(r); }
	void incrStart(double dr) { incrRad1(dr); }
	void incrEnd(double dr) { incrRad2(dr); }
};

struct Pipe3Dradius {
	Pipe3Dradius() {}
	Pipe3Dradius(double startInner, double startOuter, double endInner, double endOuter)
		: start{ Disk2Dradius(startInner, startOuter) }, end{ Disk2Dradius(endInner, endOuter) } {}

	Disk2Dradius start, end;
	double dRi() const { return end.inner() - start.inner(); }
	double dRo() const { return end.outer() - start.outer(); }
};


struct ArcAngles {	
	ArcAngles(double _start, double _end) : start{ _start }, end{ _end }{
		m_fullCircle = false;
	}
	ArcAngles() {
		setFullCircle();
	}
	
	double start, end;	
	double angleSize() const { return std::abs(end - start); }
	bool fullCircle() const { return m_fullCircle; }
	
	void setFullCircle();
	double angStep(int nnodes) const;

	void setStart(double rad) { start = rad; m_fullCircle = false; }
	void setEnd(double rad) { end = rad; m_fullCircle = false; }
private:
	bool m_fullCircle;
};


bool limitArcAngles(ArcAngles& arcAngles, double& dang, int nnodes);