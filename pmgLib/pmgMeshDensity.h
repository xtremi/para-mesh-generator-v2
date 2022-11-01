#pragma once

namespace pmg{

class NodeIterator1D {};
class NodeIterator2D {};


class MeshDensity {
public:
	MeshDensity(bool isClosedLoop) : closedLoop{ isClosedLoop } {}
	bool closedLoop = false;
};

class MeshDensity1D : public MeshDensity {
public:
	MeshDensity1D() : MeshDensity(false) {}
	MeshDensity1D(int nx, bool isClosedLoop = false) : MeshDensity(isClosedLoop) {
		x = nx;
	}

	NodeIterator1D getNodeIter();

	int nNodes() { return x; }
	int nElements() { return closedLoop ? x : x - 1; }

protected:
	int x;
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