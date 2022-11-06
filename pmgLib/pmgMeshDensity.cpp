#include "pmgMeshDensity.h"
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


bool NodeIndexIterator1D::first(int& id) {
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
	return next(idX, idY);
}

bool NodeIndexIterator2D::next(int& idX, int& idY) {
	return false;

}

std::shared_ptr<NodeIndexIterator1D> MeshDensity1D::nodeIndexIterator() {
	return std::make_shared<NodeIndexIterator1D>(this);
}
std::shared_ptr<NodeIndexIterator2D> MeshDensity2D::nodeIndexIterator() {
	return std::make_shared<NodeIndexIterator2D>(this);
}
