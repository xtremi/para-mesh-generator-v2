#include "SquareToCircleMesher.h"
#include "RecEdgeMesher.h"
#include "EllipseMesher.h"
#include "LineMesher.h"

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
	const EllipseRadius&	radius,
	const glm::dvec2&		recSize,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);

	std::vector<glm::dvec2> coordsSquare(meshDens.dir1());
	std::vector<glm::dvec2> coordsElipse(meshDens.dir1());
	std::vector<glm::dvec3> dirSqrToElps(meshDens.dir1());
	std::vector<double> distSqrToElps(meshDens.dir1());
	RecEdgeMesher::getLocalCoords(coordsSquare, meshDens.dir1(), recSize);
	EllipseMesher::getLocalCoords(coordsElipse, meshDens.dir1(), radius, angle);
	glm::dvec3 dirRtoL;
	
	Rectangle rec(recSize);
	int nElRecW, nElRecH;
	rec.elementsPerSides(meshDens.circ(), nElRecW, nElRecH);
	int iEllipse_start = meshDens.circ() - nElRecW / 2;	//ellipse first node start at top center of rec
	int iEllipse = iEllipse_start;
	
	for (int i = 0; i < meshDens.dir1(); i++) {
		dirRtoL = glm::dvec3(coordsElipse[iEllipse++], height) - glm::dvec3(coordsSquare[i], 0.0);
		distSqrToElps[i] = glm::length(dirRtoL);
 		dirSqrToElps[i]  = glm::normalize(dirRtoL);

		if (iEllipse == meshDens.circ())
			iEllipse = 0;
	}

	double currentHeight = 0.0;
	double dH = height / (double)meshDens.nElNorm();
	double curFactor = 0.0;
	glm::dvec3 curCoord;
	glm::dvec3 curStep;

	for (int i2 = 0; i2 < meshDens.norm(); i2++) {
		
		curFactor = (double)i2 / (double)meshDens.nElNorm();

		for (int i1 = 0; i1 < meshDens.circ(); i1++) {
			
			curStep   = curFactor * distSqrToElps[i1] * dirSqrToElps[i1];
			curCoord  = glm::dvec3(coordsSquare[i1], 0.0)  + curStep;
			Mesher::writer->writeNode(curCoord, curPos.pos, curPos.csys);
		}
		curPos.pos[(size_t)rotaxis] += curStep.z;
	}

	Mesher::nodeID1 = firstNode;
}

void SquareToCircleMesher::writeElements(const MeshDensity2D& meshDens) {
	//LineMesher::writeElementsLine(meshDens.nElDir1(), true);
	//Mesher::nodeID1 += meshDens.dir1();
	//LineMesher::writeElementsLine(meshDens.nElDir1(), true);
}