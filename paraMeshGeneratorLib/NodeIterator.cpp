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
NodeIterator1Dm::NodeIterator1Dm(const std::vector<NodeIterator1D>& _iterators, bool overlappingNodes) {
	iterators = _iterators;
	hasOverlappingNodes = overlappingNodes;
}
int NodeIterator1Dm::next() {
	if (int nextNode = iterators[currentNodeIterIndex].next()) {
		return nextNode;
	}
	else {
		currentNodeIterIndex++;
		if (currentNodeIterIndex == iterators.size()){
			return 0;
		}
		else{
			//If has overlapping nodes, skip the first of each iterators
			//Not for the first iterator
			if(hasOverlappingNodes){
				iterators[currentNodeIterIndex].next();
			}
			return next();
		}
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
	nNodes = glm::ivec2(_nNodesX, _nNodesY);
	nodeIncr = glm::ivec2(_nodeIncrX, _nodeIncrY);
	currentIterIndices = glm::ivec2(0, 0);
}

/*

		   ----> dirX incrX = 25

  |	   1x   26x   51x   76x  101x
  |
dirY   6x   31x   56x   81x  106x
  |
  V   11x   36x   61x   86x  111x
incrY = 5
*/
int NodeIterator2D::next() {

	if (currentIterIndices[0] == nNodes[0] && currentIterIndices[1] == (nNodes[1] - 1))
		return 0;

	if (currentIterIndices[0] == nNodes[0]) {
		currentIterIndices[0] = 0;
		currentIterIndices[1]++;
		return next();
	}
	else {
		return (currentIterIndices[0]++)*nodeIncr[0] + currentIterIndices[1]*nodeIncr[1] + firstNodeID + nodeIDoffset;
	}

	return 0;
}

int NodeIterator2D::last() {
	currentIterIndices = nNodes - 1;
	return next();
}

void NodeIterator2D::reset() {
	currentIterIndices = glm::ivec2(0, 0);
}

/*

	3D

*/
NodeIterator3D::NodeIterator3D() {

}