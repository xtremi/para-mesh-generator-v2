#pragma once


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
	NodeIterator1D() {}
	NodeIterator1D(int _firstNode, int _nNodes, int _nodeIncr = 1) {
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