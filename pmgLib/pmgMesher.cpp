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

}


void pmg::getPathToPathData(
	const Path& innerPath				/*!The inner path*/,
	const Path& outerPath				/*!The outer path*/,
	const MeshDensity2D& meshDens				/*!dir1 is along the paths, dir2 between the paths*/,
	VecGLM3d& innerCoords			/*![out] The coordinates along innerPath*/,
	VecGLM3d& outDirections			/*![out] The directions from innerPath to outerPath*/,
	VecD& distances				/*![out] The distances between the coords on innerPath and outerPath*/,
	const glm::dvec3& outerPathTranslation	/*![in][optional] Is added to the coordinates of outerPath*/)
{
	VecI nodesPerSegmentInner;
	VecGLM3d outerCoords;
	innerCoords = innerPath.getPathCoordinates(meshDens.circ(), meshDens.closedLoop, &nodesPerSegmentInner);
	outerCoords = outerPath.getPathCoordinates(nodesPerSegmentInner, meshDens.circ(), meshDens.closedLoop);

	for (int i = 0; i < meshDens.circ(); i++) {
		outerCoords[i] += outerPathTranslation;
		outDirections.push_back(glm::normalize(outerCoords[i] - innerCoords[i]));
		distances.push_back(glm::distance(outerCoords[i], innerCoords[i]));
	}
}