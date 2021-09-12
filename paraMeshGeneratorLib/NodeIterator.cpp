#include "NodeIterator.h"

int NodeIterator::first() {
	reset();
	return next();
}

/*

	1D

*/
NodeIterator1D::NodeIterator1D(int _firstNode, int _nNodes, int _nodeIncr, int _preNode) {
	firstNodeID = _firstNode; nNodes = _nNodes; nodeIncr = _nodeIncr; preNode = _preNode;
	if (preNode > 0) nNodes++;
}

/*


Without "preNode":
	nNodes = 4

   n0      n1      n2      n3 
 	x - - - x - - - x - - - x

	

With "preNode":
	nNodes = 4

preNode    n0      n1      n2
	x - - - x - - - x - - - x

	- Returns 0 when reaching last node
	- if first iteration and preNode exist, returns preNode
	- if second iteration and preNode exist, (iterator - 1) is used.

*/
int NodeIterator1D::next() {
	//End of iteration:
	if (currentIterIndex == nNodes){
		return 0;
	}

	//First iteration with preNode:
	if ((currentIterIndex == 0) && (preNode > 0)) {
		currentIterIndex++;
		return preNode + nodeIDoffset;
	}
	else{
		if (preNode > 0) {
			return ((currentIterIndex++) - 1) * nodeIncr + firstNodeID + nodeIDoffset;
		}
		else {
			return (currentIterIndex++) * nodeIncr + firstNodeID + nodeIDoffset;
		}		
	}
}
int NodeIterator1D::last() {
	currentIterIndex = nNodes - 1;
	return next();
}
void NodeIterator1D::reset() {
	currentIterIndex = 0;
}

/*

	1D multi

*/
NodeIterator1Dm::NodeIterator1Dm(const std::vector<NodeIterator1D>& _iterators) {
	iterators = _iterators;
}
int NodeIterator1Dm::next() {
	if (int nextNode = iterators[currentNodeIterIndex].next()) {
		return nextNode;
	}
	else {
		currentNodeIterIndex++;
		if (currentNodeIterIndex == iterators.size())
			return 0;
		else
			return next();
	}
}

void NodeIterator1Dm::reset() {
	currentNodeIterIndex = 0;
	for (NodeIterator& ni : iterators) {
		ni.reset();
	}
}
int NodeIterator1Dm::numberOfNodes() {
	int nNodes = 0;
	for (NodeIterator& ni : iterators) {
		nNodes+= ni.numberOfNodes();
	}
	return nNodes;
}


/*

	2D

*/
NodeIterator2D::NodeIterator2D(int _firstNode, int _nNodesX, int _nNodesY, int _nodeIncrX, int _nodeIncrY) {
	firstNodeID = _firstNode;
	nNodesX = _nNodesX;
	nNodesY = _nNodesY;
	nodeIncrX = _nodeIncrX;
	nodeIncrY = _nodeIncrY;
}

int NodeIterator2D::next() {

	if (currentIterIndexY == nNodesY) {
		if (currentIterIndexX == nNodesX) {

		}
	}

	if (currentIterIndexX == nNodesX && currentIterIndexY == nNodesY)
		return 0;
	return 0;
}

void NodeIterator2D::reset() {
	currentIterIndexX = 0;
	currentIterIndexY = 0;
}

/*

	3D

*/
NodeIterator3D::NodeIterator3D() {

}