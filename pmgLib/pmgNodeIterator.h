#pragma once
#include <vector>
#include "glm/glm.hpp"


namespace pmg{

/*
	Base class for node iterators
	in any shape, direction or dimensions

*/
class NodeIterator {
public:
	int first();
	virtual int last() { return -1; };
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

	NodeIterator1D() = default;
	NodeIterator1D(int _firstNode, int _nNodes, int _nodeIncr = 1, int _preNode = 0);
	virtual int next();
	virtual int last();
	virtual int get(int i);
	virtual void reset();
	virtual int numberOfNodes() { return nNodes; }

protected:
	int nNodes;
	int nodeIncr;
	int preNode = 0;

	int currentIterIndex = 0;
};

/*
	Chain of multiple NodeIterators1D
*/
class NodeIterator1Dm : public NodeIterator1D {
public:
	NodeIterator1Dm() {}
	NodeIterator1Dm(const std::vector<NodeIterator1D>& _iterators, bool overlappingNodes);
	int next();
	void reset();
	int get(int i) {
		throw("NodeIterator1Dm::get() not implemented");
		return 0;
	}

	int numberOfNodes();


private:
	std::vector<NodeIterator1D> iterators;
	int currentNodeIterIndex;
	bool hasOverlappingNodes = false;
};


/*
	x_______________x_______________x
	|               |               |
	|			    |			    |
	|			    |			    |
	|			    |			    |
	|			    |			    |
	x_______________x_______________x
	|\              |              /|
	|  \        	|		    /   |
	|	 \  x_______x_______x /     |
	|       |       |       |	    |
	|	    |       |       |       |
	x_______x_______x_______x_______x
	|       |       |       |       |
  ^ |       |       |       |       |   ^
  | x_______x_______x_______x_______x   |
  | | \     |     / |  \    |     / |   |
  | |   x___x___x/  |   x___x___x   |   |
  | |   |   |   |   |   |   |   |   |	|
  | x___x___x___x___x___x___x___x___x	|
  |										|
edge3    	  --edge0-->			  edge 1

*/
class NodeIterator1Dref : public NodeIterator1D {
public:
	enum class Type { edge1, edge3 };
	NodeIterator1Dref() {}
	NodeIterator1Dref(int firstNodeID, int _nRef, int _nElementEdge0, Type _type, int _preNode = 0);
	virtual int next();
	virtual int last();
	virtual int get(int i);
	void reset();

protected:
	Type type;
	int nRef = 0;
	int currentRef = 0;
	int currentNodeID = 0;
	int nElementEdge0;
	bool preNodeReturned = false;
};









/*!
	NodeIterator for nodes in 2D grid.

	Iterates in dirX first, then dirY

		x---iteration 1  --->x
		x---iteration 2  --->x
		x---iteration ...--->x
		x---iteration N..--->x

	With a NodeIterator1D preNodes (Will be used if nNodes() of preNodes is > 0)
		The first node in each x-direction iteration will
		be the next node of preNodes (See Example 2)

::::::::::::::::::::::::::::::::::::::::::::
	Example 1:
::::::::::::::::::::::::::::::::::::::::::::
	nNodesX = 5, nNodesY = 3
		   ----> dirX incrX = 25

  |	   1x   26x   51x   76x  101x
  |
dirY   6x   31x   56x   81x  106x
  |
  V   11x   36x   61x   86x  111x
incrY = 5

Iteration result = 1,26,51,76,101, 6,31,...,106, 11,...,111


::::::::::::::::::::::::::::::::::::::::::::
	Example 2:
::::::::::::::::::::::::::::::::::::::::::::

	nNodesX = 3, nNodesY = 3

incrY = 12
  ^	   (103)x---25x---26x---27x
  |         |     |	   |     |
dirY   (102)x---13x---14x---15x
  |	        |     |     |     |
  |    (101)x--- 1x----2x----3x
	preNodes
	----> dirX incrX = 1

Iteration result = 101,1,2,3, 102,13,14,15, 103,25,26,27


*/
class NodeIterator2D : public NodeIterator {
public:
	NodeIterator2D() {}
	NodeIterator2D(
		int _firstNode,
		int _nNodesX, int _nNodesY,
		int _nodeIncrX, int _nodeIncrY,
		const NodeIterator1D& _preNodes = NodeIterator1D());
	virtual int next();
	virtual int last();
	virtual int get(int ix, int iy);
	virtual void reset();
	virtual int numberOfNodes() { return nodeIncr[0] * nodeIncr[1]; }

	virtual bool first4(int& n1, int& n2, int& n3, int& n4);
	virtual bool next4(int& n1, int& n2, int& n3, int& n4);

protected:
	glm::ivec2 currentIterIndices;
	//std::vector<glm::ivec2> currentIterIndices4;
	glm::ivec2 nNodes;
	glm::ivec2 nodeIncr;
	NodeIterator1D preNodes;
	bool hasPreNodes = false;
};

class NodeIterator2Dref : public NodeIterator2D {
public:
	enum class Type { face1, face3, face4, face5 };
	NodeIterator2Dref() {}
	NodeIterator2Dref(
		int _firstNode,
		int _nNodesX,
		int _nNodesY,
		int _nRef,
		Type _type,
		const NodeIterator1D& _preNodes = NodeIterator1D());
	virtual int next();
	virtual int last();
	virtual int get(int ix, int iy);
	virtual void reset();
	virtual int numberOfNodes();

	virtual bool first4(int& n1, int& n2, int& n3, int& n4);

	/*
		Face 1 and 3: b0m2   -> m2m3t0 -> t0b0 -> repeat
		Face 4 and 5: b0m1m2 -> m2t0   -> t0b0 -> repeat
	*/
	virtual bool next4(int& n1, int& n2, int& n3, int& n4);

protected:
	enum class RowType {
		b0m2, m2m3t0, t0b0,
		b0m1m2, m2t0
	};

	Type	type;
	RowType curRowType;
	int nRef = 0;
	int	currentNodeID1 = 0;
	int currentNodeID2 = 0;
	int currentFirstNodeInRefLayer = 0;

	struct RefLayerData {
		/*Sets the values for the layer currLayer*/
		void setData(int currLayer, const glm::ivec2& nNodesXY);
		int ref;	/*The current ref layer (first is 0)*/
		int nx0;	/*number of nodes in x-direction at start of refinement*/
		int nx1;	/*number of nodes in x-direction at end of refinement*/
		int ny0;	/*number of nodes in y-direction at start of refinement*/
		int ny1;	/*number of nodes in y-direction at end of refinement*/
		int nB;     /*total nodes in sub-layers B*/
		int nBM1;   /*total nodes in sub-layers B+M1*/
		int nBM1M2; /*total nodes in sub-layers B+M1+M2*/
		int nM2M3;  /*total nodes in sub-layers M2+M3*/
		int nT;		/*total nodes in sub-layer T*/
		int nTot;	/*Total nodes in layer (B+M1+M2+M3+T)*/
	};
	RefLayerData curRefLayer;

	int localCounter1 = 0; //counter used for counting f.ex. number of M1 layers that have been iterated during node iteration
	int localCounter2 = 0;

	/*
		Face 1 and 3: b0m2   -> m2m3t0 -> t0b0 -> repeat
	*/
	bool next4_b0m2(int& n1, int& n2, int& n3, int& n4);
	bool next4_m2m3t0(int& n1, int& n2, int& n3, int& n4);
	/*
		Face 4 and 5: b0m1m2 -> m2t0   -> t0b0 -> repeat
	*/
	bool next4_b0m1m2(int& n1, int& n2, int& n3, int& n4);
	bool next4_m2t0(int& n1, int& n2, int& n3, int& n4);

	bool next4_t0b0(int& n1, int& n2, int& n3, int& n4);

	void incrementFirstNodeID(int& n1, int& n2, int& n3, int& n4);
	void setRefSectionNodeNumbers(int i1, int i2, int i3, int i4, int& n1, int& n2, int& n3, int& n4);
	void setRefSectionNodeNumbers(int elementIndex, int& n1, int& n2, int& n3, int& n4);




	/*

	   4x---------x10
		|  e6   / |
	   3x-----x7  |
		|  e4 | e5|
	   2x-----x6--x8
		|  e3 | e2|
	   1x-----x5  |
		|   e1  \ |
	   0x---------x8

	   When iterating with first4/next4 and reaching a section as above,
	   the nodes of the 6 elements are calculated together, but
	   retreived one by one from the buffer.
	   - Index by each "x" indicates the position in the buffer
	   - eN indicates the order of element iteration
	*/
	int refSectionNodeBuffer[11];
};

/*
	Not implemented
*/
class NodeIterator3D : public NodeIterator {
public:
	NodeIterator3D();
};

}