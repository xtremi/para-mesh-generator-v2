#pragma once
#include "LineMesher.h"
#include "PlaneMesher.h"

/*!
Writes lines in X, Y, Z
*/
void writeXYZlines(MeshCsys& csys, double length, int nNodes) {
	LineMesher::writeNodesX(NULL_POS, csys, nNodes, length);
	LineMesher::writeElements(nNodes);
	LineMesher::writeNodesY(NULL_POS, csys, nNodes, length);
	LineMesher::writeElements(nNodes);
	LineMesher::writeNodesZ(NULL_POS, csys, nNodes, length);
	LineMesher::writeElements(nNodes);
}

/*!
Writes 2d elements in XY, XZ, YZ planes
*/
void writeXYZplanes(MeshCsys& csys, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	PlaneMesher::writeNodesXY(NULL_POS, csys, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodesXZ(NULL_POS, csys, meshDens, size);
	PlaneMesher::writeElements(meshDens);
	PlaneMesher::writeNodes(NULL_POS, csys, meshDens, size, plane::yz);
	PlaneMesher::writeElements(meshDens);
}

void writeDebugBeamElements(FEAwriter* w, int firstNode, int lastNode) {
	int n[2] = { firstNode, 0 };
	for (int i = 2; i < lastNode; i++) {
		n[1] = i;
		w->write2nodedBeam(i, n);
	}
}


std::vector<int> linearlyOrderedVector(int iStart, int iEnd, int iStep) {
	std::vector<int> res;
	for (int i = iStart; i <= iEnd; i += iStep) {
		res.push_back(i);
	}
	return res;
}