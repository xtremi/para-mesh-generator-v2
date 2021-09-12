#pragma once
#include <vector>

class NodeIterator {
public:
	int first();	
	virtual int last() {return -1;};
	virtual int next() = 0;
	virtual void reset() = 0;

	int firstNodeID;
	int nodeIDoffset = 0;

	virtual int numberOfNodes() { return -1; }
	
};



/*

	 5    8   11   14   17   20
	 x    x    x    x    x    x
	 --> dir
*/
class NodeIterator1Dm;
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

class NodeIterator1Dm : public NodeIterator {
public:
	NodeIterator1Dm() {}
	NodeIterator1Dm(const std::vector<NodeIterator1D>& _iterators);
	int next();
	void reset();

	int numberOfNodes();


private:
	std::vector<NodeIterator1D> iterators;

	int currentNodeIterIndex;
	
};


/*
	   ----> dirX

  |	   x    x    x    x    x
  |
dirY   x    x    x    x    x
  |
  V	   x    x    x    x    x



*/
/*
	Not implemented
*/
class NodeIterator2D : public NodeIterator {
public:
	NodeIterator2D() {}
	NodeIterator2D(int _firstNode, int _nNodesX, int _nNodesY, int _nodeIncrX, int _nodeIncrY);
	int next();
private:
	void reset();
	int currentIterIndexX, currentIterIndexY;
	int nNodesX, nNodesY;
	int nodeIncrX, nodeIncrY;
};

/*
	Not implemented
*/
class NodeIterator3D : public NodeIterator {
public:
	NodeIterator3D();
};