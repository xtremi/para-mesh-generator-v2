#include "RecToEllipseMesher.h"
#include "RecEdgeMesher.h"
#include "EllipseMesher.h"
#include "LineMesher.h"
#include "ConeMesher.h"
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
void RecToEllipseMesher::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity2D&	meshDens,
	const EllipseRadius&	radius,
	const glm::dvec2&		recSize,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
	std::vector<glm::dvec2> coordsSquare(meshDens.dir1());
	std::vector<glm::dvec2> coordsElipse(meshDens.dir1());
	std::vector<glm::dvec3> recToEllipseDirs(meshDens.dir1());
	std::vector<double> recToEllipseDist(meshDens.dir1());
	RecEdgeMesher::getLocalCoords(coordsSquare, meshDens.dir1(), recSize);
	EllipseMesher::getLocalCoords(coordsElipse, meshDens.dir1(), radius, angle);
	
	calculateRecToEllipseDirections(meshDens, recSize, height, coordsSquare, coordsElipse, recToEllipseDirs, recToEllipseDist);
	writeNodes(spos, meshDens, coordsSquare, recToEllipseDirs, recToEllipseDist, height, rotaxis);
}

void RecToEllipseMesher::calculateRecToEllipseDirections(
	const MeshDensity2D&			meshDens,
	const glm::dvec2&				recSize,
	double							height,
	const std::vector<glm::dvec2>&	recCoords,
	const std::vector<glm::dvec2>&	ellipseCoords,
	std::vector<glm::dvec3>&		recToEllipseDirs,
	std::vector<double>&			recToEllipseDist)
{
	glm::dvec3 dirRtoL;

	Rectangle rec(recSize);
	int nElRecW, nElRecH;
	rec.elementsPerSides(meshDens.circ(), nElRecW, nElRecH);
	int iEllipse_start = meshDens.circ() - nElRecW / 2;	//ellipse first node start at top center of rec
	int iEllipse = iEllipse_start;

	for (int i = 0; i < meshDens.dir1(); i++) {
		dirRtoL = glm::dvec3(ellipseCoords[iEllipse++], height) - glm::dvec3(recCoords[i], 0.0);
		recToEllipseDist[i] = glm::length(dirRtoL);
		recToEllipseDirs[i] = glm::normalize(dirRtoL);

		if (iEllipse == meshDens.circ())
			iEllipse = 0;
	}
}


void RecToEllipseMesher::writeNodes(
	const MeshCsys&			       spos,
	const MeshDensity2D&		   meshDens,
	const std::vector<glm::dvec2>& startCoords,
	const std::vector<glm::dvec3>& directions,
	const std::vector<double>&	   distances,
	double						   height,
	direction					   rotaxis)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);

	double currentHeight = 0.0;
	double dH = height / (double)meshDens.nElNorm();
	double curFactor = 0.0;
	glm::dvec3 curCoord;
	glm::dvec3 curStep;

	plane pln = getPlaneFromNormal(rotaxis);
	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	for (int i2 = 0; i2 < meshDens.norm(); i2++) {
		curFactor = (double)i2 / (double)meshDens.nElNorm();

		for (int i1 = 0; i1 < meshDens.circ(); i1++) {
			curStep = curFactor * distances[i1] * directions[i1];
			curCoord = glm::dvec3(startCoords[i1], 0.0) + curStep;
			
			glm::dvec3 mCurCoord(0.0);
			mCurCoord[(size_t)dir1] = curCoord.x;
			mCurCoord[(size_t)dir2] = curCoord.y;
			mCurCoord[(size_t)rotaxis] = curCoord.z;
			Mesher::writer->writeNode(mCurCoord, curPos.pos, curPos.csys);
		}
		//curPos.pos[(size_t)rotaxis] += curStep.z;
	}
	Mesher::nodeID1 = firstNode;

}

void RecToEllipseMesher::writeElements(const MeshDensity2D& meshDens) {
	ConeMesher::writeElements(meshDens);
}