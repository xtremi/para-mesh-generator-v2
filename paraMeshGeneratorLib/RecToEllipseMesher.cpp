#include "RecToEllipseMesher.h"
#include "RecEdgeMesher.h"
#include "EllipseMesher.h"
#include "LineMesher.h"
#include "ConeMesher.h"
#include "PlaneMesher.h"

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

	for (int i2 = 0; i2 < meshDens.norm(); i2++) {
		curFactor = (double)i2 / (double)meshDens.nElNorm();
		writeNodesPerimeter_nth(curPos, meshDens.circ(), startCoords, directions, distances, curFactor, 0, rotaxis);
	}
	Mesher::nodeID1 = firstNode;
}

void RecToEllipseMesher::writeNodesPerimeter_nth(
	const MeshCsys&			       spos,
	int							   nnodes,
	const std::vector<glm::dvec2>& startCoords,
	const std::vector<glm::dvec3>& directions,
	const std::vector<double>&	   distances,
	double						   distanceFactor,
	int							   skipNth,
	direction					   rotaxis)
{
	glm::dvec3 curCoord;
	glm::dvec3 curStep;

	plane pln = getPlaneFromNormal(rotaxis);
	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	for (int i1 = 0; i1 < nnodes; i1++) {
		if (!skipNth  || (i1 % skipNth)) {//dont skip any if skipNth == 0
			curStep = distanceFactor * distances[i1] * directions[i1];
			curCoord = glm::dvec3(startCoords[i1], 0.0) + curStep;

			glm::dvec3 mCurCoord(0.0);
			mCurCoord[(size_t)dir1] = curCoord.x;
			mCurCoord[(size_t)dir2] = curCoord.y;
			mCurCoord[(size_t)rotaxis] = curCoord.z;
			Mesher::writer->writeNode(mCurCoord, spos.pos, spos.csys);
		}
	}
}

void RecToEllipseMesher::writeElements(const MeshDensity2D& meshDens) {
	ConeMesher::writeElements(meshDens);
}

void RecToEllipseMesherRef::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	const EllipseRadius&	radius,
	const glm::dvec2&		recSize,
	const ArcAngles&		angle,
	double					height,
	bool					startWithOffset,
	direction				rotaxis)
{
	int firstNodeID = writer->getNextNodeID();

	std::vector<glm::dvec2> coordsSquare(meshDens.circ());
	std::vector<glm::dvec2> coordsElipse(meshDens.circ());
	std::vector<glm::dvec3> recToEllipseDirs(meshDens.circ());
	std::vector<double> recToEllipseDist(meshDens.circ());
	RecEdgeMesher::getLocalCoords(coordsSquare, meshDens.circ(), recSize);
	EllipseMesher::getLocalCoords(coordsElipse, meshDens.circ(), radius, angle);

	RefShapeData rsData;
	rsData.meshDens = &meshDens;
	rsData.radius	= &radius;
	rsData.recSize	= recSize;
	rsData.angle	= &angle;
	rsData.height	= height;
	rsData.rotAxis	= rotaxis;
	rsData.coordsSquare		= &coordsSquare;
	rsData.recToEllipseDirs = &recToEllipseDirs;
	rsData.recToEllipseDist = &recToEllipseDist;

	double maxRecV = std::fmax(recSize.x, recSize.y);
	double maxElRad = std::fmax(radius.rad1, radius.rad2);
	rsData.maxLength = std::sqrt(pow2(maxElRad - maxRecV/2.0) + pow2(height));

	RefLayerData rlData;
	rlData.curPos = spos;
	rlData.curElSize.x = initialRefElSize2D(rsData.maxLength, meshDens.nRefs(), startWithOffset);
	rlData.curElSize.y = radius.perimeter() / (double)meshDens.nElCirc();
	rlData.curRadius = radius;
	rlData.curLength = 0.0;
	rlData.curFactor = 0.0;

	if (startWithOffset) {
		//fix radius?
		rlData.curPos.pos[(size_t)rotaxis] += rlData.curElSize.x;
	}

	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		writeNodes_layerB(rsData, rlData, refLayer);
		writeNodes_layerM(rsData, rlData, refLayer);
		writeNodes_layerT(rsData, rlData, refLayer);
	}

	Mesher::nodeID1 = firstNodeID;
}

void RecToEllipseMesherRef::incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData)
{
	rlData.curFactor += 0.1;
	//rlData.curLength += rlData.curElSize.x;
	//rlData.curRadius = rsData.radius->start() + rsData.radius->dR() * (rlData.curConeLength / rsData.coneLength);
	//double curDh = (rsData.height / rsData.coneLength)*rlData.curElSize.x;
	//rlData.curPos.pos[(size_t)rsData.rotAxis] += curDh;
}

//row b: x--x--x--x--x--x--x--x--x
void RecToEllipseMesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	RecToEllipseMesher::writeNodesPerimeter_nth(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer),
		*rsData.coordsSquare, *rsData.recToEllipseDirs, *rsData.recToEllipseDist, rlData.curFactor, 0, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}
//row m:  |  x--x--x  |  x--x--x  |
void RecToEllipseMesherRef::writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	RecToEllipseMesher::writeNodesPerimeter_nth(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer),
		*rsData.coordsSquare, *rsData.recToEllipseDirs, *rsData.recToEllipseDist, rlData.curFactor, 4, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}
//row t: x----x----x----x----x
void RecToEllipseMesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	rlData.curElSize.y *= 2.0;
	RecToEllipseMesher::writeNodesPerimeter_nth(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer),
		*rsData.coordsSquare, *rsData.recToEllipseDirs, *rsData.recToEllipseDist, rlData.curFactor, 0, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
	rlData.curElSize.x *= 2.0;
	incrementConeStep(rsData, rlData);
}

void RecToEllipseMesherRef::writeElements(const MeshDensity2Dref& meshDens) {
	PlaneMesherRef::writeElements(meshDens);
}