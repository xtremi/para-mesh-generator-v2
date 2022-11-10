#include "pmgMeshDensity.h"
#include "pmgRefinementCalculations.h"
#include "pmgMath.h"
using namespace pmg;

bool pmg::skip(int i, int last, node_skip nskip) {
	if (nskip == node_skip::none) {
		return false;
	}
	if (i == 0) {
		return nskip == node_skip::first || nskip == node_skip::first_and_last || ((int)nskip >= 2);
	}
	else if (i == (last - 1) && ((int)nskip < 2)) {
		return nskip == node_skip::last || nskip == node_skip::first_and_last;
	}
	else if ((int)nskip >= 2) {
		return !(bool)(i % (int)nskip);
	}
	else {
		return false;
	}
}

/*
(
N - N/m - max(1,N%m)


1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20
x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x
----x-------x-------x-------x-------x-------x-------x-------x-------x-------x (10)
N - N/m = 20 - 10 = 10

1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 
x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x
----x---x-------x---x-------x---x-------x---x-------x---x-------x---x-------x (13)
N - N/m = 20 - 20/3 = 20 - 6 = 14

1   2   3   4   5   6   7   8   9  10  
x---x---x---x---x---x---x---x---x---x
----x-------x-------x-------x-------x (5)
N - N/m = 10 - 10/2 = 10 - 5 = 5

1   2   3   4   5   6   7   8   9  10  11  12  13  14
x---x---x---x---x---x---x---x---x---x---x---x---x---x
----x---x-------x---x-------x---x-------x---x-------x (9)
N - N/m = 14 - 14/3 = 14 - 4 = 10

1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
x---x---x---x---x---x---x---x---x---x---x---x---x---x---x
----x---x-------x---x-------x---x-------x---x-------x---x (10)
N - N/m = 15 - 15/3 = 15 - 5 = 10

1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
x---x---x---x---x---x---x---x---x---x---x---x---x---x---x---x
----x---x-------x---x-------x---x-------x---x-------x---x---- (10)
N - N/m = 16 - 16/3 = 16 - 5 = 11


*/

int pmg::nNonSkippedNodes(int nNodes, node_skip nskip) {
	switch (nskip)
	{
	case pmg::node_skip::none:
		return nNodes; break;
	case pmg::node_skip::first:
	case pmg::node_skip::last:
		return nNodes - 1; break;
	case pmg::node_skip::first_and_last:
		return nNodes - 2; break;
	case pmg::node_skip::every_2:
	case pmg::node_skip::every_3:
	case pmg::node_skip::every_4:
	case pmg::node_skip::every_5:
	case pmg::node_skip::every_6:
	default:
		//N - N / m - max(1, N % m)
		return nNodes - (int)(nNodes / (int)nskip) - (int)(nNodes%(int)nskip > 0);
		break;
	}
}

NodeIterator1D MeshDensity1D::getNodeIter() const {
	return NodeIterator1D();
}


bool NodeIndexIterator1D::first(int& id)
{
	currentIndex = 0;
	return next(id);
}

bool NodeIndexIterator1D::next(int& id) {
	if (currentIndex == ((MeshDensity1D*)meshDensity)->nNodes()) {
		return false;
	}

	while (pmg::skip(currentIndex, ((MeshDensity1D*)meshDensity)->nNodes(), ((MeshDensity1D*)meshDensity)->nodeSkip)) {
		currentIndex++;
		return next(id);
	}
	id = currentIndex++;
	return true;
}

bool NodeIndexIterator2D::first(int& idX, int& idY) {
	currentIndexX = 0;
	currentIndexY = 0;
	idY = 0;
	return next(idX, idY);
}

bool NodeIndexIterator2D::next(int& idX, int& idY) {

	//Reached end:
	if (currentIndexY == ((MeshDensity2D*)meshDensity)->nNodesY()){
		return false;
	}

	while (pmg::skip(currentIndexY, ((MeshDensity2D*)meshDensity)->nNodesY(), ((MeshDensity2D*)meshDensity)->nodeSkipY)) {
		currentIndexY++;
	}

	while (pmg::skip(currentIndexX, ((MeshDensity2D*)meshDensity)->nNodesX(), ((MeshDensity2D*)meshDensity)->nodeSkipX)) {
		currentIndexX++;
	}

	if (currentIndexX >= ((MeshDensity2D*)meshDensity)->nNodesX()) {
		currentIndexX = 0;
		currentIndexY++;
		idY = currentIndexY;
		return next(idX, idY);
	}
	idX = currentIndexX++;
	return true;
}

bool NodeIndexIterator2Dref::first(int& idX, int& idY) {
	
	currentRowMeshDens = MeshDensity1D(
		((MeshDensity2Dref*)meshDensity)->nNodesRowB(0),
		pmg::node_skip::none,
		meshDensity->closedLoop);
	currentNodeIter1D = currentRowMeshDens.nodeIndexIterator();	
	
	currentRef = 0;
	currentIndexY = idY = 0;
	return currentNodeIter1D->first(idX);
}
bool NodeIndexIterator2Dref::next(int& idX, int& idY){

	if (!currentNodeIter1D->next(currentIndexX)) {
		
		currentIndexY = currentIndexY + pmg::twoPow(currentRef);
		idY = currentIndexY;
		
		rowType++;
		int nNodesRow = ((MeshDensity2Dref*)meshDensity)->nNodesRowB(currentRef);
		pmg::node_skip nodeSkipRow = pmg::node_skip::none;

		if(rowType == 1){
			if (currentRef == ((MeshDensity2Dref*)meshDensity)->nRefs()) {
				return false;
			}
		}
		else if (rowType == 2) {
			nodeSkipRow = pmg::node_skip::every_4;
		}
		else /*(rowType == 3)*/ {
			nNodesRow = ((MeshDensity2Dref*)meshDensity)->nNodesRowT(currentRef);
			currentRef++;
			rowType = 0;
		}

		currentRowMeshDens = MeshDensity1D(nNodesRow, nodeSkipRow, meshDensity->closedLoop);
		currentNodeIter1D = currentRowMeshDens.nodeIndexIterator();

		if (!currentNodeIter1D->first(currentIndexX)) {
			return false;
		}
	}

	idX = currentIndexX * pmg::twoPow(currentRef);
	return true;
}

bool NodeIndexIterator2Dref_2::first(int& idX, int& idY) {
	currentRef = 0;
	currentIndexX = 0;
	currentIndexY = 0;
	return next(idX, idY);
}

bool NodeIndexIterator2Dref_2::next(int& idX, int& idY) {

	idX = currentIndexX;
	idY = currentIndexY;

	if (idX == ((MeshDensity2Dref*)meshDensity)->nNodesRowB(currentRef)){
		currentIndexY += pmg::twoPow(currentRef);
		currentIndexX = 0;
	}
	else {

		if (rowType == 1) {
			idX = currentIndexX * pmg::twoPow(currentRef);

		}
		else if (rowType == 2) {

		}
		else {

		}


	}



	return true;
}


std::shared_ptr<NodeIndexIterator1D> MeshDensity1D::nodeIndexIterator() {
	return std::make_shared<NodeIndexIterator1D>(this);
}
std::shared_ptr<NodeIndexIterator2D> MeshDensity2D::nodeIndexIterator() {
	return std::make_shared<NodeIndexIterator2D>(this);
}
std::shared_ptr<NodeIndexIterator2Dref> MeshDensity2Dref::nodeIndexIterator() {
	return std::make_shared<NodeIndexIterator2Dref>(this);
}

int MeshDensity2Dref::nElRowB(int refLayer) const {
	return refinement::nElementsLayerB_2d(refLayer, nElDirY());
}
int MeshDensity2Dref::nElRowT(int refLayer) const {
	return refinement::nElementsLayerT_2d(refLayer, nElDirY());
}
int MeshDensity2Dref::nNodesRowB(int refLayer) const {
	return closedLoop ? nElRowB(refLayer) : nElRowB(refLayer) + 1;
}
int MeshDensity2Dref::nNodesRowM(int refLayer) const {
	return 3 * nElRowB(refLayer) / 4;
}
int MeshDensity2Dref::nNodesRowT(int refLayer) const {
	return nNodesRowB(refLayer + 1);
}
int MeshDensity2Dref::nNodes() const {
	return refinement::nNodesTot_2d(nRefX, nElDirY()); //is this correct? depends on closed loop?
}
int MeshDensity2Dref::nElements() const {
	throw("MeshDensity2Dref::nElements() - is not implemented");
}


