#pragma once
#include "pmgNodeIterator.h"
#include <memory>

namespace pmg{


enum class node_skip { 
	none = 0, first = 1, last = -1, 
	first_and_last = -2, 
	every_2 = 2, every_3 = 3, every_4 = 4, every_5 = 5, every_6 = 6 };

bool skip(int i, int last, node_skip nskip);

int nNonSkippedNodes(int nNodes, node_skip nskip);

/*
	Base MeshDensity
*/

class NodeIndexIterator;
class MeshDensity {
public:
	MeshDensity(bool isClosedLoop) : closedLoop{ isClosedLoop } {}
	bool closedLoop = false;
};

class NodeIndexIterator {
protected:
	MeshDensity* meshDensity = nullptr;
public:
	NodeIndexIterator(MeshDensity* md) : meshDensity{ md } {}
};



/*
	1D MeshDensity
*/
class NodeIndexIterator1D;
class MeshDensity1D : public MeshDensity {
public:
	MeshDensity1D() : MeshDensity(false) {}
	MeshDensity1D(int nx, node_skip _nodeSkip = node_skip::none, bool isClosedLoop = false)
		: MeshDensity(isClosedLoop), x{nx}, nodeSkip{_nodeSkip}
	{}

	NodeIterator1D getNodeIter() const;

	int nNodes() const { return x; }
	int nNodesNotSkipped() const { return nNonSkippedNodes(x, nodeSkip); }
	int nElements() const { return closedLoop ? nNodesNotSkipped() : nNodesNotSkipped() - 1; }

	int x = 0;
	node_skip nodeSkip = node_skip::none;

	virtual std::shared_ptr<NodeIndexIterator1D> nodeIndexIterator();
};

class NodeIndexIterator1D : public NodeIndexIterator {
	int currentIndex = -1;
public:
	NodeIndexIterator1D(MeshDensity1D* md) : NodeIndexIterator(md) {}
	virtual bool first(int& id);
	virtual bool next(int& id);
};

/*
	2D MeshDensity
*/
class NodeIndexIterator2D;
class MeshDensity2D : public MeshDensity {
public:
	MeshDensity2D() : MeshDensity(false) {}
	MeshDensity2D(
		int			nx, 
		int			ny, 
		node_skip   _nodeSkipX = node_skip::none, 
		node_skip   _nodeSkipY = node_skip::none, 
		bool		isClosedLoopX = false)
		: MeshDensity(isClosedLoopX), x{ nx }, y{ ny }, nodeSkipX{ _nodeSkipX }, nodeSkipY{ _nodeSkipY }
	{}

	NodeIterator1D getNodeIter(int edge) const;

	int nNodesX() const { return x; }
	int nNodesY() const { return y; }
	virtual int circ() const { return nNodesX(); }	//Circular direction, around cicumference
	virtual int norm() const { return nNodesY(); }  //Radial/normal direction, out of circle

	int nNodes() const { return nNodesX() * nNodesY(); }
	int nNodesNotSkipped() const { return nNodesNotSkippedX() * nNodesNotSkippedY(); }
	int nNodesNotSkippedX() const { return nNonSkippedNodes(x, nodeSkipX); }
	int nNodesNotSkippedY() const { return nNonSkippedNodes(y, nodeSkipY); }
	int nElementsX() const { return closedLoop ? nNodesNotSkippedX() : nNodesNotSkippedX() - 1; }
	int nElementsY() const { return nNodesNotSkippedY() - 1; }
	int nElements() const { return nElementsX() * nElementsY(); }

	int x, y;
	node_skip nodeSkipX = node_skip::none;
	node_skip nodeSkipY = node_skip::none;

	virtual std::shared_ptr<NodeIndexIterator2D> nodeIndexIterator();
};

class NodeIndexIterator2D : public NodeIndexIterator {
	int currentIndexX = -1;
	int currentIndexY = -1;
public:
	NodeIndexIterator2D(MeshDensity2D* md) : NodeIndexIterator(md) {}
	virtual bool first(int& idX, int& idY);
	virtual bool next(int& idX, int& idY);
};


/*
	2D MeshDensity
*/
class NodeIndexIterator2Dref;
class MeshDensity2Dref : public MeshDensity {
public:
	MeshDensity2Dref() : MeshDensity(false) {}
	MeshDensity2Dref(
		int			_nRefDirX,
		int			_nNodesDirY,
		bool		isClosedLoopX = false)
		: MeshDensity(isClosedLoopX), nRefX{ _nRefDirX }, y{ _nNodesDirY }
	{}

	NodeIterator1D getNodeIter(int edge) const;

	int nRefs() { return nRefX; }

	//Only for first row:
	int nNodesDirY() const { return y; }
	int nElDirY() const { return closedLoop ? nNodesDirY() : nNodesDirY() - 1; }

	//First layer is refLayer = 0
	int nElRowB(int refLayer) const;
	int nElRowT(int refLayer) const;
	int nNodesRowB(int refLayer) const;
	int nNodesRowM(int refLayer) const;
	int nNodesRowT(int refLayer) const;

	int nNodes() const;
	int nElements() const;

	int nRefX, y;

	virtual std::shared_ptr<NodeIndexIterator2Dref> nodeIndexIterator();
};

class NodeIndexIterator2Dref : public NodeIndexIterator {
	int currentIndexX = -1;
	int currentIndexY = -1;
	int currentRef = -1;
	MeshDensity1D currentRowMeshDens;
	std::shared_ptr<NodeIndexIterator1D> currentNodeIter1D;
	int rowType = 1; //1 = B, 2 = M, 3 = T
public:
	NodeIndexIterator2Dref(MeshDensity2Dref* md) : NodeIndexIterator(md) {}
	virtual bool first(int& idX, int& idY);
	virtual bool next(int& idX, int& idY);
};


class NodeIndexIterator2Dref_2 : public NodeIndexIterator {
	int currentIndexX = -1;
	int currentIndexY = -1;
	int currentRef = -1;
	int rowType = 1; //1 = B, 2 = M, 3 = T
public:
	NodeIndexIterator2Dref_2(MeshDensity2Dref* md) : NodeIndexIterator(md) {}
	virtual bool first(int& idX, int& idY);
	virtual bool next(int& idX, int& idY);
};


/*
	3D MeshDensity
*/
class NodeIndexIterator3D;
class MeshDensity3D : public MeshDensity {
public:
	NodeIterator2D getNodeIter(int face);

	int x, y, z;
	virtual std::shared_ptr<NodeIndexIterator3D> nodeIndexIterator();
};

class NodeIndexIterator3D : public NodeIndexIterator {
	int currentIndexX = -1;
	int currentIndexY = -1;
	int currentIndexZ = -1;
public:
	NodeIndexIterator3D(MeshDensity3D* md) : NodeIndexIterator(md) {}
	virtual bool first(int& idX, int& idY, int& idZ);
	virtual bool next(int& idX, int& idY, int& idZ);
};

}