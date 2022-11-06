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

	virtual std::shared_ptr<NodeIndexIterator> nodeIndexIterator() = 0;
};

class NodeIndexIterator {
protected:
	MeshDensity* meshDensity = nullptr;
	int currentIndex = -1;
public:
	NodeIndexIterator(MeshDensity* md) : meshDensity{ md } {}
	virtual bool first(int& id) = 0;
	virtual bool next(int& id) = 0;
};



/*
	1D MeshDensity
*/

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

	std::shared_ptr<NodeIndexIterator> nodeIndexIterator();
};

class NodeIndexIterator1D : public NodeIndexIterator {
public:
	NodeIndexIterator1D(MeshDensity1D* md) : NodeIndexIterator(md) {}
	bool first(int& id);
	bool next(int& id);
};

/*
	2D MeshDensity
*/
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

	std::shared_ptr<NodeIndexIterator> nodeIndexIterator();
};

class NodeIndexIterator2D : public NodeIndexIterator {
public:
	NodeIndexIterator2D(MeshDensity2D* md) : NodeIndexIterator(md) {}
	virtual bool first(int& id);
	virtual bool next(int& id);
};



/*
	3D MeshDensity
*/
class MeshDensity3D : public MeshDensity {
public:
	NodeIterator2D getNodeIter(int face);

	int x, y, z;
	std::shared_ptr<NodeIndexIterator> nodeIndexIterator();
};

}