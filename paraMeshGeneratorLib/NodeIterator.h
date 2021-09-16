#pragma once
#include <vector>
#include "glm/glm.hpp"

/*
	Base class for node iterators
	in any shape, direction or dimensions
	
*/
class NodeIterator {
public:
	int first();	
	virtual int last() {return -1;};
	virtual int next() = 0;
	virtual void reset() = 0;

	int firstNodeID = 0;
	int nodeIDoffset = 0;

	virtual int numberOfNodes() { return -1; }
	
};



/*

	 5    8   11   14   17   20
	 x    x    x    x    x    x
	 |--->|--->|--->|--->|--->|
	  nodeIncr  = 3
	  firstNode = 5
	  preNode   = 0
	  nNodes    = 6

	 55    1   10   19   28   37   46
	  x    x    x    x    x    x    x
	  |--->|--->|--->|--->|--->|--->|
	  nodeIncr  = 9
	  firstNode = 1
	  preNode   = 55
	  nNodes    = 7  (6 passed to constructor, incremented by constructor because preNode > 0)
*/
class NodeIterator1D : public NodeIterator {
public:

	NodeIterator1D() {}
	NodeIterator1D(int _firstNode, int _nNodes, int _nodeIncr = 1, int _preNode = 0);
	int next();
	int last();
	void reset();
	int numberOfNodes() { return nNodes; }

protected:
	int nNodes;
	int nodeIncr;
	int preNode = 0;

	int currentIterIndex;
};

/*
	Chain of multiple NodeIterators1D
*/
class NodeIterator1Dm : public NodeIterator {
public:
	NodeIterator1Dm() {}
	NodeIterator1Dm(const std::vector<NodeIterator1D>& _iterators, bool overlappingNodes);
	int next();
	void reset();

	int numberOfNodes();


private:
	std::vector<NodeIterator1D> iterators;
	int currentNodeIterIndex;
	bool hasOverlappingNodes = false;
};


/*
	   ----> dirX

  |	   1x    2x    3x    4x    5x
  |
dirY   6x    7x    8x    9x   10x
  |
  V   11x   12x   13x   14x   15x


	Iterates in dirX first:

	x---iteration 1  --->x
	x---iteration 2  --->x
	x---iteration ...--->x
	x---iteration ...--->x


		   ----> dirX incrX = 25

  |	   1x   26x   51x   76x  101x
  |
dirY   6x   31x   56x   81x  106x
  |
  V   11x   36x   61x   86x  111x
incrY = 5

*/
/*
	Not implemented
*/
class NodeIterator2D : public NodeIterator {
public:
	NodeIterator2D() {}
	NodeIterator2D(int _firstNode, int _nNodesX, int _nNodesY, int _nodeIncrX, int _nodeIncrY);
	int next();
	int last();
	void reset();
	int numberOfNodes() { return nodeIncr[0] * nodeIncr[1]; }

private:	
	glm::ivec2 currentIterIndices;
	glm::ivec2 nNodes;
	glm::ivec2 nodeIncr;
};

/*
	Not implemented
*/
class NodeIterator3D : public NodeIterator {
public:
	NodeIterator3D();
};