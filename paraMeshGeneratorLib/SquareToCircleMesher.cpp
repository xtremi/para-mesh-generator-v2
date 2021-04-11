#include "SquareToCircleMesher.h"
#include "RecEdgeMesher.h"


/*
           \               /
            \             /   
	         x---x---x---x  /
	         |           | /
	H        x           x
	         |           |
	         x           x
	         |           | \
	         x---x---x---x  \
			/             \
		   /			   \
		          W
*/
void SquareToCircleMesher::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity2D&	meshDens,
	const EllipseRadius&		radius,
	const glm::dvec2&		recSize,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);

	std::vector<glm::dvec2> coordsSquare(meshDens.dir1());
	std::vector<glm::dvec2> coordsElipse(meshDens.dir1());
	std::vector<glm::dvec2> outDirVecs(meshDens.dir1());
	RecEdgeMesher::getLocalCoords(coordsSquare, meshDens.dir1(), recSize);
	for (int i = 0; i < meshDens.dir1(); i++) {
		writer->writeNode(glm::dvec3(coordsSquare[i], 0.0), spos.pos, spos.csys);
	}


	Mesher::nodeID1 = firstNode;
}

void SquareToCircleMesher::writeElements(const MeshDensity2D& meshDens) {
	int c = Mesher::nodeID1;
	int n[2] = { c, 0 };
	for (int i = 0; i < meshDens.nElDir1(); i++) {
		n[0] = c;
		n[1] = n[0] + 1;
		if (i == (meshDens.nElDir1() - 1)) {
			n[1] -= meshDens.dir1();
		}
		writer->write2nodedBeam(n);
		c++;
	}
}