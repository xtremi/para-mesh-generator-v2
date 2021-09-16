#pragma once
#include <glm/glm.hpp>
#include "math_utilities.h"
#include <vector>
#include "NodeIterator.h"

enum class Dim { DIM1D, DIM2D, DIM3D };

enum class direction{x,y,z, custom};
enum class plane {xy, xz, yz, custom};

void getPlaneDirections(plane pln, direction& d1, direction& d2);
direction getPlaneNormal(plane pln);
plane getPlaneFromNormal(direction normal);
glm::dvec3 coordsOnCircle(double angle, double radius, direction rotAxis);
glm::dvec3 coordsOnEllipse(double angle, double rad1, double rad2, direction rotAxis);





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


struct Rectangle {
	Rectangle(const glm::dvec2& _size) : size{ _size } {}
	Rectangle() {}

	double perimeter() {
		return 2.0 * size.x + 2.0 * size.y;
	}
	void elementsPerSides(int nNodesPerimeter, int& nElWidthX, int& nElHeightY) {
		double recWfactor = size.x / perimeter();
		nElWidthX = (int)(recWfactor * (double)nNodesPerimeter);
		nElHeightY = (nNodesPerimeter - 2 * nElWidthX) / 2;
	}

	glm::dvec2 getCornerCoord(int corner) {
		switch (corner)
		{
		case 1: return  0.5*size * glm::dvec2(-1.0, 1.0); break;
		case 2: return  0.5*size * glm::dvec2(1.0, 1.0); break;
		case 3: return  0.5*size * glm::dvec2(1.0, -1.0); break;
		default: return 0.5*size * glm::dvec2(-1.0, -1.0); break;
		}
	}

	glm::dvec2 size;

	static const glm::dvec2 recDirs[4];
};


struct MeshDensity2DrecTube : public MeshDensity2D {
	MeshDensity2DrecTube(){}
	MeshDensity2DrecTube(int nNodePerimeterInner, int nNodeLayers, const glm::dvec2& sizeInner) 
		: MeshDensity2D(nNodePerimeterInner, nNodeLayers, true)
	{
		setNodesInner(nNodePerimeterInner, sizeInner);
	}
	MeshDensity2DrecTube(int nNodesWidth, int nNodesHeight, int nNodeLayers) 
		: MeshDensity2D(0, nNodeLayers, true)
	{
		setNodesInner(nNodesWidth, nNodesHeight);
	}

	void setNodesInner(int nNodePerimeterInner, const glm::dvec2& sizeInner) {
		setCirc(nNodePerimeterInner);
		Rectangle rec(sizeInner);
		rec.elementsPerSides(nNodePerimeterInner, nNodesW, nNodesH);		
	}
	void setNodesInner(int nNodesWidth, int nNodesHeight) {
		nNodesW = nNodesWidth;
		nNodesH = nNodesHeight;
		setCirc(2 * (nNodesWidth + nNodesHeight));
	}
	void setNodesLayer(int nLayers) {
		setNorm(nLayers);
	}

	int nNodePerimeter(int layer) const { return circ() + 8 * layer; }

	int nNodesWidth(int layer) const {
		return nNodesW + 2 * layer;
	}
	int nNodesHeight(int layer) const {
		return nNodesH + 2 * layer;
	}
	void cornerNodes(int n[4], int layer) const {
		n[0] = 0;
		n[1] = nNodesWidth(layer);
		n[2] = n[1] + nNodesHeight(layer);
		n[3] = n[2] + nNodesWidth(layer);
	}


private:
	int nNodesW, nNodesH;
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
	MeshCsys() : pos{ glm::dvec3(0.0) } {
		reset();
	}
	MeshCsys(const glm::dvec3& _pos, glm::dmat3x3* _csys = nullptr) : pos{ _pos }, csys{ _csys } {
		reset();
	}
	MeshCsys(MeshCsys* parent, const glm::dvec3& _pos = glm::dvec3(0.0), glm::dmat3x3* _csys = nullptr) 
		: MeshCsys(_pos, _csys) {
		setParentCsys(parent);
	}

	void reset() {
		hasRotToGlobal = csys ? true : false;
		local2globalT = pos; //csys ? (*csys) * pos : pos;
		local2globalR = csys ? (*csys) : glm::dmat3x3(1.0);		
	}	
	/*
		Moves along its own axes
		(Changing pos directly moves the CSYS along the axes of parent CSYS)
	*/
	void moveInLocalCsys(const glm::dvec3& transl) {
		pos += (*csys) * transl;
	}

	glm::dvec3		pos;
	glm::dmat3x3*	csys = nullptr;

	glm::dvec3		local2globalT;
	glm::dmat3x3	local2globalR;
	bool			hasRotToGlobal = false;

	glm::dvec3		dirX() const { return csys ? (*csys)[0] : X_DIR; }
	glm::dvec3		dirY() const { return csys ? (*csys)[1] : Y_DIR; }
	glm::dvec3		dirZ() const { return csys ? (*csys)[2] : Z_DIR; }

	glm::dvec3		dirXg() const { return local2globalR[0]; }
	glm::dvec3		dirYg() const { return local2globalR[1]; }
	glm::dvec3		dirZg() const { return local2globalR[2]; }

	glm::dvec3 getGlobalCoords(const glm::dvec3& localCoords) const {
		glm::dvec3 glcoords(localCoords);
		glcoords = local2globalR * glcoords;
		glcoords += local2globalT;
		return glcoords;
	}

	/*
		Go through parentCsys up to the last and inserts them in a std::vector
		last parent (global csys) is the first item in the vector
		(includes it self!)
	*/
	std::vector<MeshCsys*> getAllParentCsys() {
		std::vector<MeshCsys*> parents;
		MeshCsys* curCsys = this;
		while (curCsys) {
			parents.insert(parents.begin(), curCsys);
			curCsys = curCsys->parentCsys;
		}
		return parents;
	}

	void updateParents() {
		std::vector<MeshCsys*> parents = getAllParentCsys();
		parents[0]->reset();
		for (size_t i = 0; i < parents.size() - 1; i++) {
			parents[i + 1]->setParentCsys(parents[i]);
		}
	}

	void update() {
		if(parentCsys) setParentCsys(parentCsys);
		else {
			if (csys) local2globalR = *csys;
		}
	}

	void setParentCsys(MeshCsys* _parentCsys) {
		parentCsys = _parentCsys;
		if (parentCsys->hasRotToGlobal) {
			hasRotToGlobal = true;
		}

		if (csys && parentCsys->hasRotToGlobal) {
			local2globalR = _parentCsys->local2globalR * (*csys);
		}
		else if (!csys && parentCsys->hasRotToGlobal) {
			local2globalR = _parentCsys->local2globalR;
		}
		else if (csys) {
			local2globalR = *csys;
		}
		
		if (parentCsys->hasRotToGlobal) {
			local2globalT = _parentCsys->local2globalT + _parentCsys->local2globalR*pos;
		}
		else{
			local2globalT = _parentCsys->local2globalT + pos;
		}
	}

private:
	MeshCsys* parentCsys = nullptr;
};



struct EllipseRadius {
	EllipseRadius(){}
	EllipseRadius(double _rad1, double _rad2) : rad1{ _rad1 }, rad2{ _rad2 } {}
	double rad1, rad2;

	double perimeter() const {
		double h = pow2(rad1 - rad2) / pow2(rad1 + rad2);
		
		double d = 10 + std::sqrt(4.0 - 3 * h);
		double A = 1 + 3 * h / d;
		
		double per = GLMPI * (rad1 + rad2)*A;
		return per;
	}
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

struct RecTubeSize {
	RecTubeSize(){}
	RecTubeSize(const glm::dvec2& _inner, const glm::dvec2& _outer = glm::dvec2()) : inner{ _inner }, outer{ _outer } {}

	glm::dvec2 inner;
	glm::dvec2 outer;

	glm::dvec2 dsize() const { return outer - inner; }

	glm::dvec2 calculateOuterSize(int nNodesEdge, int nPerimeterNodeLayers) {		
		return inner + 2.* (double)(nPerimeterNodeLayers - 1)*inner / (double)nNodesEdge;		
	}
	glm::dvec2 calculateOuterSize(int nNodesWidth, int nNodesHeight, int nPerimeterNodeLayers) {
		glm::dvec2 outerSize;
		outerSize.x = inner.x + 2.* (double)(nPerimeterNodeLayers - 1)*inner.x / (double)nNodesWidth;
		outerSize.y = inner.y + 2.* (double)(nPerimeterNodeLayers - 1)*inner.y / (double)nNodesHeight;
		return outerSize;
	}
	void setOuterSize(int nNodesEdge, int nPerimeterNodeLayers) {
		outer = calculateOuterSize(nNodesEdge, nPerimeterNodeLayers);
	}
	void setOuterSize(int nNodesWidth, int nNodesHeight, int nPerimeterNodeLayers) {
		outer = calculateOuterSize(nNodesWidth, nNodesHeight, nPerimeterNodeLayers);
	}
};

bool limitArcAngles(ArcAngles& arcAngles, double& dang, int nnodes);