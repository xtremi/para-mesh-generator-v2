#include "pmgMesher.h"
using namespace pmg;



void Mesher::write(Mesh1D& mesh) {
	mesh.firstNodeID = writer->nextNodeID();
	mesh.firstElementID = writer->nextElementID();

	for (int i = 0; i < mesh.meshDensity.nNodes(); i++) {
		if(!pmg::skip(i, mesh.meshDensity.nNodes(), mesh.meshDensity.nodeSkip)){
			glm::dvec3 pos = mesh.path->positionI(i, mesh.meshDensity.nNodes(), mesh.meshDensity.closedLoop);
			writer->writeNode(pos, *mesh.csys, *mesh.transformer);
		}
	}

	int nodeIDs[2] = { mesh.firstNodeID, mesh.firstNodeID + 1 };
	for (int i = 0; i < mesh.meshDensity.nElements(); i++) {
		writer->write2nodedBeam(nodeIDs);
		nodeIDs[0]++; nodeIDs[1]++;

		if (mesh.meshDensity.closedLoop && i == mesh.meshDensity.nElements() - 2) {
			nodeIDs[1] = mesh.firstNodeID;
		}
	}

}
