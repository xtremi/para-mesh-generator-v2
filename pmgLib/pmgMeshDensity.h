#pragma once
#include "pmgNodeIterator.h"

namespace pmg{


enum class node_skip { 
	none = 0, first = 1, last = -1, 
	first_and_last = -2, 
	every_2 = 2, every_3 = 3, every_4 = 4, every_5 = 5, every_6 = 6 };

bool skip(int i, int last, node_skip nskip);

int nNonSkippedNodes(int nNodes, node_skip nskip);


class MeshDensity {
public:
	MeshDensity(bool isClosedLoop) : closedLoop{ isClosedLoop } {}
	bool closedLoop = false;
};

class MeshDensity1D : public MeshDensity {
public:
	MeshDensity1D() : MeshDensity(false) {}
	MeshDensity1D(int nx, node_skip _nodeSkip = node_skip::none, bool isClosedLoop = false)
		: MeshDensity(isClosedLoop), x{nx}, nodeSkip{_nodeSkip}
	{}

	NodeIterator1D getNodeIter();

	int nNodes() { return x; }
	int nNodesNotSkipped() { return nNonSkippedNodes(x, nodeSkip); }
	int nElements() { return closedLoop ? nNodesNotSkipped() : nNodesNotSkipped() - 1; }

	int x;
	node_skip nodeSkip = node_skip::none;
};



class MeshDensity2D : public MeshDensity {
public:
	NodeIterator1D getNodeIter(int edge);

protected:
	int x, y;
};
class MeshDensity3D : public MeshDensity {
public:
	NodeIterator2D getNodeIter(int face);

protected:
	int x, y, z;
};

}