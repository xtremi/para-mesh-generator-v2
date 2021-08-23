#include "RecToEllipse3Dmesher.h"
#include "RecToEllipseMesher.h"
#include "RecEdgeMesher.h"
#include "EllipseMesher.h"
#include "LineMesher.h"
#include "Cone3Dmesher.h"
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
void RecToEllipse3Dmesher::writeNodes(
	MeshCsys&			spos,
	const MeshDensity3D&	meshDens,
	const EllipseRadius&	radiusStart,
	const EllipseRadius&	radiusEnd,
	const glm::dvec2&		recSizeStart,
	const glm::dvec2&		recSizeEnd,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);

	std::vector<glm::dvec2> coordsSquare(meshDens.circ());
	std::vector<glm::dvec2> coordsElipse(meshDens.circ());
	std::vector<glm::dvec3> recToEllipseDirs(meshDens.circ());
	std::vector<double> recToEllipseDist(meshDens.circ());

	EllipseRadius curEllipseRad(radiusStart);
	glm::dvec2    curRecSize(recSizeStart);
	double		  curHeight = 0.0;
	double dr1 = (radiusEnd.rad1 - radiusStart.rad1) / (double)meshDens.nElAxis();
	double dr2 = (radiusEnd.rad2 - radiusStart.rad2) / (double)meshDens.nElAxis();
	double dwidth = (recSizeEnd.x - recSizeStart.x) / (double)meshDens.nElAxis();
	double dheight = (recSizeEnd.y - recSizeStart.y) / (double)meshDens.nElAxis();

	double elSizeD3 = height / (double)meshDens.nElAxis();

	MeshDensity2D meshDensD12 = meshDens.meshDensD12();

	for (int i = 0; i < meshDens.axis(); i++) {
		RecEdgeMesher::getLocalCoords(coordsSquare, meshDens.circ(), curRecSize);
		EllipseMesher::getLocalCoords(coordsElipse, meshDens.circ(), curEllipseRad, angle);

		RecToEllipseMesher::calculateRecToEllipseDirections(meshDensD12, curRecSize, 0.0 /*flat*/, coordsSquare, coordsElipse, recToEllipseDirs, recToEllipseDist);
		RecToEllipseMesher::writeNodes(curPos, meshDensD12, coordsSquare, recToEllipseDirs, recToEllipseDist, 0.0 /*flat*/, rotaxis);

		curEllipseRad.rad1 += dr1;
		curEllipseRad.rad2 += dr2;
		curRecSize += glm::dvec2(dwidth, dheight);
		curPos.pos[(size_t)rotaxis] += elSizeD3;
	}

	Mesher::nodeID1 = firstNode;
}




void RecToEllipse3Dmesher::writeElements(const MeshDensity3D& meshDens) {
	Cone3Dmesher::writeElements(meshDens);
}


void RecToEllipse3DmesherRef::writeNodes(
	MeshCsys&			spos,
	const MeshDensity3Dref&	meshDens,
	const EllipseRadius&	radiusStart,
	const EllipseRadius&	radiusEnd,
	const glm::dvec2&		recSizeStart,
	const glm::dvec2&		recSizeEnd,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{

}

void RecToEllipse3DmesherRef::writeElements(const MeshDensity3Dref& meshDens)
{

}

void RecToEllipse3DmesherRef::incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData){

}
void RecToEllipse3DmesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {

}
void RecToEllipse3DmesherRef::writeNodes_layerM1(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {

}
void RecToEllipse3DmesherRef::writeNodes_layerM2(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {

}
void RecToEllipse3DmesherRef::writeNodes_layerM3(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {

}
void RecToEllipse3DmesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {}