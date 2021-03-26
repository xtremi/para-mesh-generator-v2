#pragma once
#include <glm/glm.hpp>

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
	int circ() const { return dir1(); }		
	/*
		Radial/normal direction, out of circle
	*/
	int norm() const { return dir2(); }

private:
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
	MeshDensity2D(int _dir1, int _dir2, bool closedLoopDir1) : NodeVec2D(_dir1, _dir2), closedLoop{ closedLoop } {}
	
	bool closedLoop;
	int nElDir1() const { return closedLoop ? dir1() : dir1() - 1; }
	int nElDir2() const { return dir2() - 1; }
	int nElCirc() const { return nElDir1(); }
	int nElNorm() const { return nElDir2(); }
};

/*
	Extends NodeVec3D with element number functions and closed loop definition
*/
struct MeshDensity3D : public NodeVec3D {
	MeshDensity3D() {}
	MeshDensity3D(int _dir1, int _dir2, int _dir3, bool closedLoopDir1) : NodeVec3D(_dir1, _dir2, _dir3), closedLoop{ closedLoop } {}

	bool closedLoop;
	int nElDir1() const { return closedLoop ? dir1() : dir1() - 1; }
	int nElDir2() const { return dir2() - 1; }
	int nElDir3() const { return dir3() - 1; }
	int nElCirc() const { return nElDir1(); }
	int nElNorm() const { return nElDir2(); }
	int nElAxis() const { return nElDir3(); }
};

/*
	Extends NodeVec2D with element number functions and closed loop definition,
	for a 2D structured mesh with refinement in direction 1.
*/
struct MeshDensity2Dref : public NodeVec2D {
	MeshDensity2Dref() {}
	MeshDensity2Dref(int _nRefDir1, int _nNodesDir2, bool closedLoopDir2) : NodeVec2D(_nRefDir1, _nNodesDir2), closedLoop{ closedLoop } {}

	bool closedLoop;
	//int nElDir1() const { return dir1(); }		//depends on refinement 
	int nRefs() const { return dir1(); }
	int nElDir2() const { return closedLoop ? dir2() : dir2() - 1; }
	int nElCirc() const { return nElDir2(); }

	int nNodesDir2(int curRef) const { return 0; }
	int nElDir2(int curRef) const { return 0; }

	//int nElNorm() const { return nElDir1(); }		//depends on refinement 
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
	ArcAngles(double _start, double _end) : start{ _start }, end{ _end }{
		m_fullCircle = false;
	}
	ArcAngles() {
		setFullCircle();
	}
	
	double start, end;	
	bool fullCircle() { return m_fullCircle; }
	
	void setFullCircle();
	double angStep(int nnodes);
private:
	bool m_fullCircle;
};