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

/*!
	Writes an element between end of mesh1 to start of mesh2
*/
void Mesher::write(const Mesh1D& mesh1, const Mesh1D mesh2) {

	int nodeIDs[2] = {
		mesh1.firstNodeID + mesh1.meshDensity.nNodesNotSkipped() - 1,
		mesh2.firstNodeID };
	writer->write2nodedBeam(nodeIDs);
}


void Mesher::write(Mesh2D& mesh) {

	mesh.firstNodeID = writer->nextNodeID();
	mesh.firstElementID = writer->nextElementID();

	SurfaceData* surfaceData = mesh.surface->init(mesh.meshDensity.nNodesX(), mesh.meshDensity.nNodesY(), mesh.meshDensity.closedLoop);


	for (int iy = 0; iy < mesh.meshDensity.nNodesY(); iy++) {
		if (!pmg::skip(iy, mesh.meshDensity.nNodesY(), mesh.meshDensity.nodeSkipY)) {
			for (int ix= 0; ix< mesh.meshDensity.nNodesX(); ix++) {
				if (!pmg::skip(ix, mesh.meshDensity.nNodesX(), mesh.meshDensity.nodeSkipX)) {

					glm::dvec3 pos = mesh.surface->positionI(ix, iy, surfaceData);
					writer->writeNode(pos, *mesh.csys, *mesh.transformer);

				}
			}
		}
	}

	mesh.surface->cleanUp(surfaceData);

	int n[4];
	int c = mesh.firstNodeID;

	for (int iy = 0; iy < mesh.meshDensity.nElementsY(); iy++) {
		//writer->setCurrentElementPropertyID(elementProp++);
		for (int ix = 0; ix < mesh.meshDensity.nElementsX(); ix++) {
			n[0] = c++;
			n[1] = n[0] + 1;
			n[2] = n[1] + mesh.meshDensity.nNodesNotSkippedX();
			n[3] = n[2] - 1;

			if (mesh.meshDensity.closedLoop && ix == (mesh.meshDensity.nElementsX() - 1)) {
				n[1] -= mesh.meshDensity.nNodesNotSkippedX();
				n[2] -= mesh.meshDensity.nNodesNotSkippedX();
			}

			writer->write4nodedShell(n);
		}
		if (!mesh.meshDensity.closedLoop) c++;
	}

}


