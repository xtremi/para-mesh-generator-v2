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

NodeIterator1Dref::NodeIterator1Dref(int firstNodeID, int _nRef, int _nElementEdge0, Type _type, int _preNode)
	: NodeIterator1D(firstNodeID, 0, 0, _preNode)
{
	type = _type;
	nElementEdge0 = _nElementEdge0;
	nRef = _nRef;
}
int NodeIterator1Dref::next() {
		
	if (currentIterIndex == 0){

		if (preNode > 0 && !preNodeReturned) {
			preNodeReturned = true;
			return preNode;
		}

		currentIterIndex++;
		currentNodeID = 0;
		return currentNodeID + firstNodeID + nodeIDoffset;
	}
	else {
		if (currentRef == nRef) {
			return 0;
		}

		if (currentIterIndex % 2) {
			if(type == Type::edge3){
				currentNodeID += refinement::nNodesLayerMT_2d(currentRef, nElementEdge0);
				currentRef++;
			}
			else if(type == Type::edge1) {
				currentNodeID += refinement::nNodesLayerBM_2d(currentRef, nElementEdge0);
				currentRef++;
			}
			
		}
		else {
			if (type == Type::edge3 || type == Type::edge1) {
				currentNodeID += refinement::nNodesLayerB_2d(currentRef, nElementEdge0);
			}
		}
		currentIterIndex++;
		return currentNodeID + firstNodeID + nodeIDoffset;
	}

	
	return 0;
}

/*
	Returns the i-th nodeID of the iterator.

	Obs!: Iterates from start to i-th value to get i-th value
*/
int NodeIterator1Dref::get(int i) {

	int nId = 0;
	reset();
	for (int j = 0; j < (i + 1); j++) {
		nId = next();
	}

	return nId;
}

/*
	Returns the last nodeID of the iterator.

	Obs!: Iterates from start to end to find last ID
*/
int NodeIterator1Dref::last() {
	int nid = 0;
	for (int nthId = first(); nthId; nthId = next()) {
		nid = nthId;
	}
	return nid;
}
void NodeIterator1Dref::reset() {
	NodeIterator1D::reset();
	currentRef = 0;
}


/*

	1D multi

*/
NodeIterator1Dm::NodeIterator1Dm(const std::vector<NodeIterator1D>& _iterators, bool overlappingNodes) 
	: NodeIterator1D(0, 0, 0){
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
	int _nNodesX, 
	int _nNodesY,
	int _nRef,
	Type _type,
	const NodeIterator1D& _preNodes) :
	NodeIterator2D(_firstNode, _nNodesX, _nNodesY, 0, 0, preNodes)
{
	type = _type;
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
	curRefLayer.setData(0, nNodes);
	switch (type)
	{
	case NodeIterator2Dref::Type::face1: 
		currentNodeID1 = 0;
		currentNodeID2 = curRefLayer.nBM1;
		curRowType = RowType::b0m2;	break;
	case NodeIterator2Dref::Type::face3: 
		currentNodeID1 = nNodes.x - 1;
		currentNodeID2 = curRefLayer.nBM1 + curRefLayer.nx1 - 1;
		curRowType = RowType::b0m2;	break;
	case NodeIterator2Dref::Type::face4:
		break;
	case NodeIterator2Dref::Type::face5:
		currentNodeID1 = curRefLayer.nB - curRefLayer.nx0;
		currentNodeID2 = curRefLayer.nBM1M2 - curRefLayer.nx1;
		curRowType = RowType::b0m1m2; break;
		break;
	default:
		break;
	}
	localCounter1 = 0;
	localCounter2 = 0;
	currentFirstNodeInRefLayer = 0;
	return next4(n1,n2,n3,n4);
}
/*

   x---------x
   |    4    | 
   x---------x
   |    3    |
   x---------x
   |    2    |
   x---------x
   |    1    |
   x---------x
   b   m1    m2  
*/
bool NodeIterator2Dref::next4_b0m2(int& n1, int& n2, int& n3, int& n4){

	n1 = currentNodeID1;
	n2 = currentNodeID2;
	n3 = n2 + curRefLayer.nx1;
	n4 = n1 + curRefLayer.nx0;
	localCounter1++;

	currentNodeID1 = n4;
	currentNodeID2 = n3;
	incrementFirstNodeID(n1, n2, n3, n4);

	//Prepare for next RowType:
	if (localCounter1 == (curRefLayer.ny0 - 1)) {
		curRowType = RowType::m2m3t0;
		localCounter1 = 0;
		if(type == Type::face1){
			currentNodeID1 = currentFirstNodeInRefLayer + curRefLayer.nBM1; 
			currentNodeID2 = currentNodeID1 + curRefLayer.nM2M3;
		}
		else if (type == Type::face3) {
			currentNodeID1 = currentFirstNodeInRefLayer + curRefLayer.nBM1 + curRefLayer.nx1 - 1;
			currentNodeID2 = currentNodeID1 + curRefLayer.nM2M3;
		}

	}
	return true;
}

bool NodeIterator2Dref::next4_m2t0(int& n1, int& n2, int& n3, int& n4) {

	n1 = currentNodeID1;
	n2 = currentNodeID2;
	n3 = n2 + 1;
	n4 = n1 + 1;
	localCounter1++;

	currentNodeID1 = n4;
	currentNodeID2 = n3;
	incrementFirstNodeID(n1, n2, n3, n4);

	//Prepare for next RowType:
	if (localCounter1 == (curRefLayer.ny1 - 1)) {
		localCounter1 = 0;
		if (type == Type::face5) {
			curRowType = RowType::t0b0;
			currentNodeID1 = currentFirstNodeInRefLayer + curRefLayer.nTot - curRefLayer.nx1;
			currentNodeID2 = currentNodeID1 + curRefLayer.nT;
		}

	}
	return true;

}


/*

		x---------x   ---
		|   6   / |	   |
		x-----x   |	   |
		|   4 | 5 |	   |
		x-----x---x	   |  2
		|   3 | 2 |	   |
		x-----x   |	   |
		|    1  \ |	   |
	    x---------x   ---
		|   6   / |    |
	    x-----x/  |    |
		|   4 | 5 |    |  1
	    x-----x---x    |  
		|   3 | 2 |    |
	    x-----x   |    |
		|    1  \ |    |
	    x---------x   ---
       m2    m3   t     

	   - localCounter1 is used for iterating the elements 1-6 in one ref section
	   - localCounter2 is used for counting elements along row b
*/
bool NodeIterator2Dref::next4_m2m3t0(int& n1, int& n2, int& n3, int& n4) {
	if (localCounter1 == 0) {
		int nM2edges = curRefLayer.ny0 - localCounter2;
		int nM3edges = 3 * localCounter2 / 4;

		refSectionNodeBuffer[0] = currentNodeID1;
		for (int i = 1; i < 5; i++) refSectionNodeBuffer[i] = refSectionNodeBuffer[i - 1] + curRefLayer.nx1;

		refSectionNodeBuffer[8] = currentNodeID2;
		for (int i = 9; i < 11; i++) refSectionNodeBuffer[i] = refSectionNodeBuffer[i - 1] + curRefLayer.nx1;

		refSectionNodeBuffer[5] = currentNodeID1 + curRefLayer.nx1 * (nM2edges + nM3edges);
		for (int i = 6; i < 8; i++) refSectionNodeBuffer[i] = refSectionNodeBuffer[i - 1] + curRefLayer.nx1;

		currentNodeID1 = refSectionNodeBuffer[4];
		currentNodeID2 = refSectionNodeBuffer[10];
		localCounter2 += 4;
		int tmp = 1;
	}

	setRefSectionNodeNumbers(localCounter1++, n1, n2, n3, n4);
	incrementFirstNodeID(n1, n2, n3, n4);

	//If last iterated element in ref section:
	static const int N_ELEMENTS_PER_REF_SECTION = 6;
	if (localCounter1 == N_ELEMENTS_PER_REF_SECTION) {
		localCounter1 = 0;
	
		//Prepare for next RowType:
		if ((localCounter2) == (curRefLayer.ny0 - 1)) {
			curRowType = RowType::t0b0;
			localCounter2 = 0;
			if(type == Type::face1){
				currentNodeID2 = currentFirstNodeInRefLayer + curRefLayer.nTot;
				currentNodeID1 = currentNodeID2 - curRefLayer.nT;
			}
			else if (type == Type::face3) {
				currentNodeID2 = currentFirstNodeInRefLayer + curRefLayer.nTot + curRefLayer.nx1 - 1;
				currentNodeID1 = currentNodeID2 - curRefLayer.nT;
			}
		}
	}

	return true;
}

bool NodeIterator2Dref::next4_b0m1m2(int& n1, int& n2, int& n3, int& n4) {
	if (localCounter1 == 0) {
		int nBnodes  = curRefLayer.nx0 - localCounter2;
		int nM1nodes = 3 * localCounter2 / 4 + 1;

		refSectionNodeBuffer[0] = currentNodeID1;
		for (int i = 1; i < 5; i++) refSectionNodeBuffer[i] = refSectionNodeBuffer[i - 1] + 1;

		refSectionNodeBuffer[8] = currentNodeID2;
		for (int i = 9; i < 11; i++) refSectionNodeBuffer[i] = refSectionNodeBuffer[i - 1] + 1;

		refSectionNodeBuffer[5] = currentNodeID1 + nBnodes + curRefLayer.ny0 * nM1nodes - 1;
		for (int i = 6; i < 8; i++) refSectionNodeBuffer[i] = refSectionNodeBuffer[i - 1] + curRefLayer.ny0;

		currentNodeID1 = refSectionNodeBuffer[4];
		currentNodeID2 = refSectionNodeBuffer[10];
		localCounter2 += 4;
		int tmp = 1;
	}

	setRefSectionNodeNumbers(localCounter1++, n1, n2, n3, n4);
	incrementFirstNodeID(n1, n2, n3, n4);

	//If last iterated element in ref section:
	static const int N_ELEMENTS_PER_REF_SECTION = 6;
	if (localCounter1 == N_ELEMENTS_PER_REF_SECTION) {
		localCounter1 = 0;

		//Prepare for next RowType:
		if ((localCounter2) == (curRefLayer.ny0 - 1)) {
			
			localCounter2 = 0;
			if (type == Type::face1) {
				curRowType = RowType::t0b0;
				currentNodeID2 = currentFirstNodeInRefLayer + curRefLayer.nTot;
				currentNodeID1 = currentNodeID2 - curRefLayer.nT;
			}
			else if (type == Type::face3) {
				curRowType = RowType::t0b0;
				currentNodeID2 = currentFirstNodeInRefLayer + curRefLayer.nTot + curRefLayer.nx1 - 1;
				currentNodeID1 = currentNodeID2 - curRefLayer.nT;
			}
			else if (type == Type::face5) {
				curRowType = RowType::m2t0;
				currentNodeID1 = currentFirstNodeInRefLayer + curRefLayer.nBM1M2 - curRefLayer.nx1;
				currentNodeID2 = currentFirstNodeInRefLayer + curRefLayer.nTot - curRefLayer.nx1;
			}
		}
	}

	return true;

}



/*

   x---------x
   |    4    |
   x---------x
   |    3    |
   x---------x
   |    2    |
   x---------x
   |    1    |
   x---------x
   t         b
*/
bool NodeIterator2Dref::next4_t0b0(int& n1, int& n2, int& n3, int& n4) {
	
	//If end of iteration:
	if (curRefLayer.ref == (nRef - 1)) {
		return false;
	}

	n1 = currentNodeID1;
	n2 = currentNodeID2;
	if(type == Type::face1 || type == Type::face3){
		n3 = n2 + curRefLayer.nx1;
		n4 = n1 + curRefLayer.nx1;
	}
	else if (type == Type::face4 || type == Type::face5) {
		n3 = n2 + 1;
		n4 = n1 + 1;
	}
	
	currentNodeID1 = n4;
	currentNodeID2 = n3;
	localCounter1++;
	incrementFirstNodeID(n1, n2, n3, n4);

	//Prepare for next rowType:
	if (localCounter1 == (curRefLayer.nx1 - 1)) {
		
		localCounter1 = 0;
		currentFirstNodeInRefLayer += curRefLayer.nTot;
		curRefLayer.setData(curRefLayer.ref + 1, nNodes);
		if (type == Type::face1) {
			curRowType = RowType::b0m2;
			currentNodeID1 = currentFirstNodeInRefLayer;
			currentNodeID2 = currentNodeID1 + curRefLayer.nBM1;
		}
		else if (type == Type::face3) {
			curRowType = RowType::b0m2;
			currentNodeID1 = currentFirstNodeInRefLayer + curRefLayer.nx0 - 1;
			currentNodeID2 = currentFirstNodeInRefLayer + curRefLayer.nBM1 + curRefLayer.nx1 - 1;
		}
		else if (type == Type::face5) {
			curRowType = RowType::b0m1m2;
			currentNodeID1 = currentFirstNodeInRefLayer + curRefLayer.nB - curRefLayer.nx0;
			currentNodeID2 = currentFirstNodeInRefLayer + curRefLayer.nBM1M2 - curRefLayer.nx1;;
		}
	}

	return true;
}

bool NodeIterator2Dref::next4(int& n1, int& n2, int& n3, int& n4) {
	
	if (curRowType == RowType::b0m2) {
		return next4_b0m2(n1, n2, n3, n4);
	}
	else if (curRowType == RowType::m2m3t0) {
		return next4_m2m3t0(n1, n2, n3, n4);
	}
	else if (curRowType == RowType::t0b0) {
		return next4_t0b0(n1, n2, n3, n4);
	}
	else if (curRowType == RowType::b0m1m2) {
		return next4_b0m1m2(n1, n2, n3, n4);
	}
	else if (curRowType == RowType::m2t0) {
		return next4_m2t0(n1, n2, n3, n4);
	}
	return false;
}


void NodeIterator2Dref::incrementFirstNodeID(int& n1, int& n2, int& n3, int& n4) {
	n1 += firstNodeID;
	n2 += firstNodeID;
	n3 += firstNodeID;
	n4 += firstNodeID;
}

void NodeIterator2Dref::setRefSectionNodeNumbers(int i1, int i2, int i3, int i4, int& n1, int& n2, int& n3, int& n4) {
	n1 = refSectionNodeBuffer[i1];
	n2 = refSectionNodeBuffer[i2];
	n3 = refSectionNodeBuffer[i3];
	n4 = refSectionNodeBuffer[i4];
}

void NodeIterator2Dref::setRefSectionNodeNumbers(int elementIndex, int& n1, int& n2, int& n3, int& n4) {
	switch (elementIndex)
	{
	case 0: setRefSectionNodeNumbers(0, 8, 5, 1, n1, n2, n3, n4); break;
	case 1: setRefSectionNodeNumbers(5, 8, 9, 6, n1, n2, n3, n4); break;
	case 2: setRefSectionNodeNumbers(1, 5, 6, 2, n1, n2, n3, n4); break;
	case 3: setRefSectionNodeNumbers(2, 6, 7, 3, n1, n2, n3, n4); break;
	case 4: setRefSectionNodeNumbers(6, 9, 10, 7, n1, n2, n3, n4); break;
	case 5: setRefSectionNodeNumbers(3, 7, 10, 4, n1, n2, n3, n4); break;
	default:
		throw("Invalid localCounter1 (current element in ref section) in NodeIterator2Dref::setRefSectionNodeNumbers");
	}
}


void NodeIterator2Dref::RefLayerData::setData(int currLayer, const glm::ivec2& nNodesXY) {
	ref = currLayer;
	nx0 = refinement::nNodesLayerB_2d(ref, nNodesXY.x - 1);
	nx1 = refinement::nNodesLayerT_2d(ref, nNodesXY.x - 1);
	ny0 = refinement::nNodesLayerB_2d(ref, nNodesXY.y - 1);
	ny1 = refinement::nNodesLayerT_2d(ref, nNodesXY.y - 1);
	nB  = refinement::nNodesLayerB_3d(ref, nNodesXY.x - 1, nNodesXY.y - 1);
	nBM1 = refinement::nNodesLayerBM1_3d(ref, nNodesXY.x - 1, nNodesXY.y - 1);
	nBM1M2 = nBM1 + refinement::nNodesLayerM2_3d(ref, nNodesXY.x - 1, nNodesXY.y - 1);
	nM2M3 = refinement::nNodesLayerM2M3_3d(ref, nNodesXY.x - 1, nNodesXY.y - 1);
	nTot = refinement::nNodesTotSection_3d(ref, nNodesXY.x - 1, nNodesXY.y - 1);
	nT = refinement::nNodesLayerT_3d(ref, nNodesXY.x - 1, nNodesXY.y - 1);
}

/*

	3D

*/
NodeIterator3D::NodeIterator3D() {

}