#pragma once
#include "math_utilities.h"
#include "NodeIterator.h"


/*
	Wrapper for glm::ivec2 representing number of nodes for a "2D structured mesh"
*/
struct NodeVec2D {
	NodeVec2D() {}
	NodeVec2D(int _dir1, int _dir2) : nnodes{ glm::ivec2(_dir1, _dir2) } {}

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
	virtual int dir1() const { return nnodes.x; }
	virtual int dir2() const { return nnodes.y; }
	virtual int dir3() const { return nnodes.z; }
	/*
		Circular direction, around cicumference (for cone/cylinder)
	*/
	int circ() const { return dir1(); }
	/*
		Radial/normal direction, out of circle (for cone/cylinder)
	*/
	int norm() const { return dir3(); }
	/*
		Along axis/height of a cone/cylinder
	*/
	int axis()   const { return dir2(); }

	void setDir1(int n) { nnodes.x = n; }
	void setDir2(int n) { nnodes.y = n; }
	void setDir3(int n) { nnodes.z = n; }
	void setCirc(int n) { setDir1(n); }
	void setNorm(int n) { setDir3(n); }
	void setAxis(int n) { setDir2(n); }

protected:
	glm::ivec3 nnodes;
};

/*
NOT USED
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

	dir2 (default = y)

	 ^
 [c3]|       edg3       [c2]
	 x13-x---x---x---x---x18
	 |   |   |   |   |   |
edg0 x7--x---x---x---x---x12  edg2
	 |   |   |   |   |   |
	 x1--x2--x---x---x---x6  --->dir1 (default = x)
 [c0] 		edg1	     [c1]

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
	int nElements() const { return nElDir1() * nElDir2(); }

	int cornerNode(int cornerID);
	NodeIterator1D edgeNodeIterator(int edgeID, int firstNodeID, int preNode = 0);
};


struct MeshDensity2DrecTube : public MeshDensity2D {
	MeshDensity2DrecTube() {}
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

	void setNodesInner(int nNodePerimeterInner, const glm::dvec2& sizeInner);
	void setNodesInner(int nNodesWidth, int nNodesHeight);
	void setNodesLayer(int nLayers);

	int nNodePerimeter(int layer) const;

	int nNodesWidth(int layer) const;
	int nNodesHeight(int layer) const;
	void cornerNodes(int n[4], int layer) const;

private:
	int nNodesW, nNodesH;
};

/*
	Extends NodeVec3D with element number functions and closed loop definition
  ^
  |	         F3 (back)
  |dir3 (z-dir)

	 [c2]_________e10_______[c6]
		|\                  |\
		| \                 | \
		|  \e2   F5(top)    |  \e6
		|   \             e7|   \
	  e3|    \              |    \
		| [c1]\_______e9__________\[c5]
F0(F6)->|     |             |     |  <-F2
    [c3]| _ __| _ _ e11 _ _ |[c7] |	    
		 \    |              \    |e5
		  \   |e1  F1(front)  \e4 |
  ^ 	 e0\  |                \  |
   \		\ |                 \ |
dir2\		 \|__________________\|   -----> dir1 (default = x-dir)
(y-dir)		  [c0]	  ^   e8          [c4]
				      |
				 F4 (bottom)

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
	int nElNorm() const { return nElDir3(); }
	int nElAxis() const { return nElDir2(); }


	MeshDensity2D meshDensD12() const { return MeshDensity2D(dir1(), dir2(), closedLoop); }
	MeshDensity2D meshDensD13() const { return MeshDensity2D(dir1(), dir3(), closedLoop); }
	MeshDensity2D meshDensD23() const { return MeshDensity2D(dir2(), dir3(), closedLoop); }
	MeshDensity2D meshDensDisk() const { return meshDensD13(); }
	int nnodesPlaneD12() const { return dir1() * dir2(); }

	int nNodes() const { return dir1() * dir2() * dir3(); }
	int nElements() const { return nElDir1() * nElDir2() * nElDir3(); }

	int cornerNode(int cornerID);
	NodeIterator2D faceNodeIterator(int faceID, int firstNodeID, const NodeIterator1D& preNodes = NodeIterator1D());
};

/*
	Extends NodeVec2D with element number functions and closed loop definition,
	for a 2D structured mesh with refinement in direction 1.

*/
struct MeshDensity2Dref : public NodeVec2D {
	MeshDensity2Dref() {}
	MeshDensity2Dref(int _nRefDir1, int _nNodesDir2, bool closedLoopDir2 = false) 
		: NodeVec2D(_nRefDir1, _nNodesDir2), closedLoop{ closedLoopDir2 } {}

	void setClosedLoop(bool _closedLoop = true) { closedLoop = _closedLoop; }
	bool closedLoop;

	int nRefs() const { return dir1(); }
	int nElDir2() const { return closedLoop ? dir2() : dir2() - 1; }
	int nElCirc() const { return nElDir2(); }

	/*!number of nodes in dir2 after refinement (nodes on edge 2)*/
	int dir2ref();
	/*!number of elements in dir2 after refinement (elements on edge 2)*/
	int nElDir2ref();

	void setNrefs(int n) { setDir1(n); }

	//virtual from NodeVec2D
	void setCirc(int n) { setDir2(n); }
	void setNorm(int n) { setDir1(n); }
	int circ() const { return dir2(); }
	int norm() const { return dir1(); }

	//First layer is refLayer = 0
	int nElRowB(int refLayer) const;
	int nElRowT(int refLayer) const;
	int nNodesRowB(int refLayer) const;
	int nNodesRowM(int refLayer) const;
	int nNodesRowT(int refLayer) const;

	int nNodes() const;
	int nElements() const;

	int cornerNode(int cornerID);
	NodeIterator1Dref edgeNodeIteratorRefDir(int edgeID, int firstNodeID, int preNode = 0);
	NodeIterator1D    edgeNodeIterator(int edgeID, int firstNodeID, int preNode = 0);
};

/*
	Extends NodeVec3D with element number functions and closed loop definition,
	for a 3D structured mesh with refinement in direction 2
*/
struct MeshDensity3Dref : public NodeVec3D {
	MeshDensity3Dref() {}
	MeshDensity3Dref(int _nRefDir2, int _nNodesDir1, int _nNodesDir3, bool closedLoopDir1 = false)
		: NodeVec3D(_nNodesDir1, _nRefDir2, _nNodesDir3), closedLoop{ closedLoopDir1 } {}

	bool closedLoop;
	void setClosedLoop(bool _closedLoop = true) { closedLoop = _closedLoop; }

	int nRefs() const { return dir2(); }
	int nElDir1() const { return closedLoop ? dir1() : dir1() - 1; }
	int nElDir3() const { return dir3() - 1; }
	int nElCirc() const { return nElDir1(); }

	/*!number of nodes in dir1 after refinement*/
	int dir1ref();
	/*!number of nodes in dir3 after refinement*/
	int dir3ref();
	/*!number of elements in dir1/3 after refinement*/
	int nElDir2ref();
	/*!number of elements in dir1/3 after refinement*/
	int nElDir3ref();

	MeshDensity2D meshDensD12B(int refLayer) const {
		int n1 = nElDir1() / std::pow(2, refLayer);
		if (!closedLoop) n1++;
		int n2 = nElDir3() / std::pow(2, refLayer) + 1;
		return MeshDensity2D(n1, n2, closedLoop);
	}
	MeshDensity2D meshDensD12M1(int refLayer) const {
		int n1 = 3 * (nElDir1() / std::pow(2, refLayer)) / 4;
		int n2 = nElDir3() / std::pow(2, refLayer) + 1;
		return MeshDensity2D(n1, n2, closedLoop);
	}
	MeshDensity2D meshDensD12M2(int refLayer) const {
		int n1 = nElDir1() / std::pow(2, refLayer + 1);
		if (!closedLoop) n1++;
		int n2 = nElDir3() / std::pow(2, refLayer) + 1;
		return MeshDensity2D(n1, n2, closedLoop);
	}
	MeshDensity2D meshDensD12M3(int refLayer) const {
		int n1 = nElDir1() / std::pow(2, refLayer + 1);
		if (!closedLoop) n1++;
		int n2 = 3 * (nElDir3() / std::pow(2, refLayer)) / 4;// +1;
		return MeshDensity2D(n1, n2, closedLoop);
	}
	MeshDensity2D meshDensD12T(int refLayer) const {
		return meshDensD12B(refLayer + 1);
	}
	MeshDensity2D meshDensD12end() {
		return meshDensD12B(nRefs());
	}
	int nnodesPlaneD13() const { return dir1() * dir3(); }

	int setNrefs(int n) { setDir2(n); }

	int cornerNode(int cornerID);
	NodeIterator2Dref faceNodeIteratorRefDir(int faceID, int firstNodeID, const NodeIterator1D& preNodes = NodeIterator1D());
	NodeIterator2D    faceNodeIterator(int faceID, int firstNodeID, const NodeIterator1D& preNodes = NodeIterator1D());


	int nNodes() const;
	int nElements() const;
};