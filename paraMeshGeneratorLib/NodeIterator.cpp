#include "NodeIterator.h"
#include "RefinementCalculations.h"

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
	if (currentIterIndex == nNodes) {
		return 0;
	}	
	return get(currentIterIndex++);
}
/*
	Does not check index limits
*/
int NodeIterator1D::get(int i) {
	if ((i == 0) && (preNode > 0)) {
		return preNode + nodeIDoffset;//TODO: why is nodeIDoffset used and not firstNodeID not?
	}
	else if (preNode > 0) {
		return (i - 1) * nodeIncr + firstNodeID + nodeIDoffset;
	}
	else {
		return i * nodeIncr + firstNodeID + nodeIDoffset;
	}
}
int NodeIterator1D::last() {
	currentIterIndex = nNodes - 1;
	return next();
}
void NodeIterator1D::reset() {
	currentIterIndex = 0;
}

NodeIterator1Dref::NodeIterator1Dref(int _nRef, int _nElementEdge0, Type _type, int _preNode)
	: NodeIterator1D(_type == Type::edge1 ? 1 : (_nElementEdge0 + 1), 0, 0, _preNode)
{
	type = _type;
	nElementEdge0 = _nElementEdge0;
	nRef = _nRef;
}
int NodeIterator1Dref::next() {
	
	if (currentIterIndex == 0){
		currentIterIndex++;
		previousNodeID = firstNodeID;
		return firstNodeID;
	}
	else {
		if (currentRef == nRef) {
			return 0;
		}

		if (currentIterIndex % 2) {
			if(type == Type::edge3){
				previousNodeID += refinement::nNodesLayerMT_2d(currentRef, nElementEdge0);
				currentRef++;
			}
			else if(type == Type::edge1) {
				previousNodeID += refinement::nNodesLayerBM_2d(currentRef, nElementEdge0);
				currentRef++;
			}
			
		}
		else {
			if (type == Type::edge3 || type == Type::edge1) {
				previousNodeID += refinement::nNodesLayerB_2d(currentRef, nElementEdge0);
			}
		}
		currentIterIndex++;
		return previousNodeID;
	}

	
	return 0;
}
int NodeIterator1Dref::last() {
	return 0;
}
int NodeIterator1Dref::get(int i) {
	return 0;
}
void NodeIterator1Dref::reset() {
	NodeIterator1D::reset();
	currentRef = 0;
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
NodeIterator2D::NodeIterator2D(
	int _firstNode, 
	int _nNodesX, 	int _nNodesY, 
	int _nodeIncrX, int _nodeIncrY, 
	const NodeIterator1D& _preNodes) 
{
	firstNodeID = _firstNode;
	nNodes		= glm::ivec2(_nNodesX, _nNodesY);
	nodeIncr	= glm::ivec2(_nodeIncrX, _nodeIncrY);
	preNodes	= _preNodes;
	currentIterIndices = glm::ivec2(0, 0);

	if (preNodes.numberOfNodes() > 0) {
		nNodes.x++;
		hasPreNodes = true;
		preNodes.reset();
	}
}

/*

		   ----> dirX incrX = 25

  |	   1x   26x   51x   76x  101x
  |
dirY   6x   31x   56x   81x  106x
  |
  V   11x   36x   61x   86x  111x
incrY = 5

incrY = 12
  ^	   (103)x---25x---26x---27x
  |         |     |	   |     |
dirY   (102)x---13x---14x---15x
  |	        |     |     |     |
  |    (101)x--- 1x----2x----3x
	preNodes
	----> dirX incrX = 1

*/
int NodeIterator2D::next() {
	//End of iteration:
	if (currentIterIndices[0] == nNodes[0] && currentIterIndices[1] == (nNodes[1] - 1)){
		return 0;
	}

	//if last in dirX - reset X iter - incr Y iter
	if (currentIterIndices[0] == nNodes[0]) {
		currentIterIndices[0] = 0;
		currentIterIndices[1]++;
		return next();
	}
	else {
		return get(currentIterIndices[0]++, currentIterIndices[1]);
	}

	return 0;
}

int NodeIterator2D::get(int ix, int iy) {
	
	if (ix == 0 && hasPreNodes) {
		return preNodes.get(iy) + nodeIDoffset;//TODO: why is nodeIDoffset used and not firstNodeID not?;
	}
	else if (hasPreNodes) {
		return (ix - 1)*nodeIncr[0] + iy * nodeIncr[1] + firstNodeID + nodeIDoffset;
	}
	else {
		return ix*nodeIncr[0] + iy * nodeIncr[1] + firstNodeID + nodeIDoffset;
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


bool NodeIterator2D::first4(int& n1, int& n2, int& n3, int& n4) {
	//currentIterIndices4 = { {0,0}, {1,0},{1,1}, {0,1} };
	currentIterIndices = glm::ivec2(0);
	return next4(n1,n2,n3,n4);
}
/*
	Returns the 4 next nodes in the following order:

	^
	|Y

	x---x---x---x---x---x---x
 	| 7 | 8 | 9 | 10| 11| 12|
	x---x---x---x---x---x---x        n4x----xn3 
	| 1 | 2 | 3 | 4 | 5 | 6 |          |    |
	x---x---x---x---x---x---x -->X   n1x----xn2

*/
bool NodeIterator2D::next4(int& n1, int& n2, int& n3, int& n4) {
	static const std::vector<glm::ivec2>iter4Offset = { {0,0}, {1,0},{1,1}, {0,1} };

	//End of iteration:
	if (currentIterIndices[1] == (nNodes[1] - 1)) {
		return false;
	}


	int i = 0;
	int* n[4] = { &n1,&n2,&n3,&n4 };
	for (int i = 0; i < 4; i++) {
		*n[i] = get(currentIterIndices[0] + iter4Offset[i][0], currentIterIndices[1] + iter4Offset[i][1]);
	}
	currentIterIndices[0]++;
	if (currentIterIndices[0] == (nNodes[0] - 1)) {
		currentIterIndices[0] = 0;
		currentIterIndices[1]++;
	}

	return true;
}


NodeIterator2Dref::NodeIterator2Dref(
	int _firstNode,
	int _nNodesX, int _nRef,
	const NodeIterator1D& _preNodes) :
	NodeIterator2D(_firstNode, _nNodesX, 0, 0, 0, preNodes)
{
	nRef = _nRef;
}


int NodeIterator2Dref::next() {
	return 0;
}
int NodeIterator2Dref::last() {
	return 0;
}
int NodeIterator2Dref::get(int ix, int iy) {
	return 0;
}
void NodeIterator2Dref::reset(){
	NodeIterator2D::reset();
}
int NodeIterator2Dref::numberOfNodes() {
	return 0;
}

bool NodeIterator2Dref::first4(int& n1, int& n2, int& n3, int& n4) {
	return false;
}
bool NodeIterator2Dref::next4(int& n1, int& n2, int& n3, int& n4) {
	return false;
}


/*

	3D

*/
NodeIterator3D::NodeIterator3D() {

}