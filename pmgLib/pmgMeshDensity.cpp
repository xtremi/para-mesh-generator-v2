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
		return (nNodes - 1) - (int)(nNodes / (int)nskip);
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

bool NodeIndexIterator2D::first(int& id) {
	return false;
}

bool NodeIndexIterator2D::next(int& id) {
	return false;

}

std::shared_ptr<NodeIndexIterator> MeshDensity1D::nodeIndexIterator() {
	return std::make_shared<NodeIndexIterator1D>(this);
}
std::shared_ptr<NodeIndexIterator> MeshDensity2D::nodeIndexIterator() {
	return std::make_shared<NodeIndexIterator2D>(this);
}
