#include "ConeMesher.h"
#include "PlaneMesher.h"
#include "ArcMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"
#include "glm/gtc/constants.hpp"
#include "RefinementCalculations.h"

/*
	Cone around X,Y or Z-axis
	nnodes.x - along circumferance
	nndoes.y - along height

		     _.-""""-._
           .'          `.
		  /      ___     \
		 |      /   \     |
		 |     |     |    |
		 |      \___/     |
		  \              /
		   `._        _.'
			  `-....-'

*/
void ConeMesher::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity2D&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
MESHER_NODE_WRITE_START

	double dang = angle.angStep(meshDens.circ());
	double dH = height / (double)meshDens.nElNorm();
	double dR = radius.dR() / (double)meshDens.nElNorm();

	double	curRadius = radius.start();

	for (int i = 0; i < meshDens.norm(); i++) {
		ArcMesher::writeNodesCircularQ(curPos, csys, meshDens.dir1(), curRadius, angle.start, dang, rotaxis);
		curPos[(size_t)rotaxis] += dH;
		curRadius += dR;
	}

MESHER_NODE_WRITE_END
}

void ConeMesher::writeNodesX(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height){
	writeNodes(pos, csys, meshDens, radius, angle, height, direction::x);
}
void ConeMesher::writeNodesY(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height){
	writeNodes(pos, csys, meshDens, radius, angle, height, direction::y);
}
void ConeMesher::writeNodesZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height){
	writeNodes(pos, csys, meshDens, radius, angle, height, direction::z);
}

/*
	Writes the nodes of a cone shape, and skips certain nodes.	
	Used for refinements node structures.

	The node numbering is starting from inner radius, out to outer radius, then same again
	for incremented angle.
	Every <skipNth> angle increment are skipped / jumped over.

	              2   4 
			 _.-""x"-.x
           .'     |  / `.
		  /     1_x_x 3  \
		 |      /   \     |
		 |     |   5 x----x 6
		 |      \___/     |
		  \              /
		   `._        _.'
			  `-....-'

*/
void ConeMesher::writeNodes_nthLine(
	const glm::dvec3&	 pos,
	MeshCsys&			 csys,
	const MeshDensity2D& meshDens,
	const Cone2Dradius&	 radius,
	double				 startAng,
	double				 dang,
	double				 height,
	int					 skipNth,
	direction			 rotaxis)
{
	MeshCsys curPos(pos);
	glm::dvec3 endPos;
	double currentAng = startAng;

	for (int i = 0; i < meshDens.dir1(); i++) {

		curPos.pos = coordsOnCircle(currentAng, radius.start(), rotaxis) + pos;
		endPos = coordsOnCircle(currentAng, radius.end(), rotaxis) + pos;

		if (i % skipNth) {
			LineMesher::writeNodes(curPos.pos, csys, meshDens.dir2(), endPos);
		}
		currentAng += dang;
	}
}


/*
	Writes the nodes of a cone shape, and skips certain nodes.
	Used for refinements node structures.

	The node numbering is going along the arcs of the shape.
	Every <skipNth> arc are skipped / jumped over.

				  4   5
			 _.-""x"-.x
		   .'     |  / `.
		  /     1_x_x 2  \
		 |      /   \     |
		 |     |   3 x----x 6
		 |      \___/     |
		  \              /
		   `._        _.'
			  `-....-'

*/
void ConeMesher::writeNodes_nthArc(
	const glm::dvec3&	 pos,
	MeshCsys&			 csys,
	const MeshDensity2D& meshDens,
	const Cone2Dradius&	 radius,
	double				 startAng,
	double				 dang,
	double				 height,
	int					 skipNth,
	direction			 rotaxis)
{
	double currentRadius = radius.start();
	double dr = radius.dR() / (double)meshDens.nElNorm();

	for (int i = 0; i < meshDens.dir2(); i++) {
		if (i % skipNth) {
			ArcMesher::writeNodesCircularQ(pos, csys, meshDens.dir1(), currentRadius, startAng, dang, rotaxis);
		}
		currentRadius += dr;
	}
}

/*

	In the code X-dir refers to refinement direction (along height)
	Y-dir refers to around the perimeter
*/
void ConeMesherRef::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity2Dref&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	bool					startWithOffset,
	direction				rotaxis)
{
	int firstNodeID = writer->getNextNodeID();

	RefShapeData rsData;	
	rsData.csys		= &csys;
	rsData.meshDens = &meshDens;
	rsData.radius	= &radius;
	rsData.angle	= &angle;
	rsData.height	= height;
	rsData.rotAxis	= rotaxis;
	rsData.coneLength = std::sqrt(pow2(radius.dR()) + pow2(height));

	RefLayerData rlData;
	rlData.curPos = pos;	
	rlData.curElSize.x = refinement::initialRefElSize2D(rsData.coneLength, meshDens.nRefs(), startWithOffset);
	rlData.curRadius = radius.start();
	rlData.curConeLength = 0.0;

	double	arcLengthStart = angle.angleSize() * radius.start();
	rlData.curElSize.y = arcLengthStart / (double)meshDens.nElCirc();
	
	if (startWithOffset) {
		//fix radius?
		rlData.curPos[(size_t)rotaxis] += rlData.curElSize.x;
	}

	rlData.meshDensM.skipNth = 4;
	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		rlData.meshDensB.nnodes = rsData.meshDens->nNodesRowB(refLayer);
		rlData.meshDensM.nnodes = rsData.meshDens->nNodesRowB(refLayer);
		rlData.meshDensT.nnodes = rsData.meshDens->nNodesRowT(refLayer);
		writeNodes_layerB(rsData, rlData, refLayer);
		writeNodes_layerM(rsData, rlData, refLayer);
		writeNodes_layerT(rsData, rlData, refLayer);
	}

	Mesher::nodeID1 = firstNodeID;
}

void ConeMesherRef::incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData)
{
	rlData.curConeLength += rlData.curElSize.x;
	rlData.curRadius = 
		rsData.radius->start() + rsData.radius->dR() * (rlData.curConeLength / rsData.coneLength);
	double curDh = (rsData.height / rsData.coneLength)*rlData.curElSize.x;
	rlData.curPos[(size_t)rsData.rotAxis] += curDh;
}

//row b: x--x--x--x--x--x--x--x--x
void ConeMesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	ArcMesher::writeNodesCircular(rlData.curPos, *rsData.csys, rlData.meshDensB, rlData.curRadius, *rsData.angle, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}
//row m:  |  x--x--x  |  x--x--x  |
void ConeMesherRef::writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	ArcMesher::writeNodesCircular_nth(rlData.curPos, *rsData.csys, rlData.meshDensM, rlData.curRadius, *rsData.angle, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}
//row t: x----x----x----x----x
void ConeMesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	rlData.curElSize.y *= 2.0;
	ArcMesher::writeNodesCircular(rlData.curPos, *rsData.csys, rlData.meshDensT, rlData.curRadius, *rsData.angle, rsData.rotAxis);
	rlData.curElSize.x *= 2.0;
	incrementConeStep(rsData, rlData);
}

void ConeMesher::writeElements(const MeshDensity2D& meshDens){
	PlaneMesher::writeElements(meshDens);
}

void ConeMesherRef::writeElements(const MeshDensity2Dref& meshDens){
	PlaneMesherRef::writeElements(meshDens);
}


/*

	In the code X-dir refers to refinement direction (along height)
	Y-dir refers to around the perimeter
*/
void ConeMesherRef2::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity2Dref& meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	bool					startWithOffset,
	direction				rotaxis)
{
	int firstNodeID = writer->getNextNodeID();

	RefShapeData rsData;
	rsData.csys = &csys;
	rsData.meshDens = &meshDens;
	rsData.radius = &radius;
	rsData.angle = &angle;
	rsData.height = height;
	rsData.rotAxis = rotaxis;
	rsData.coneLength = std::sqrt(pow2(radius.dR()) + pow2(height));

	RefLayerData rlData;
	rlData.curPos = pos;
	rlData.curAngle = angle.start;
	rlData.curAngleStep = refinement::initialRefElSize2D(rsData.angle->angleSize(false), 
		meshDens.nRefs(), startWithOffset);

	//TODO: not sure if this is correct
	if (startWithOffset) {
		rlData.curAngle += rlData.curAngleStep;
	}

	rlData.meshDensM.skipNth = 4;
	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		rlData.meshDensB.nnodes = rsData.meshDens->nNodesRowB(refLayer);
		rlData.meshDensM.nnodes = rsData.meshDens->nNodesRowB(refLayer);
		rlData.meshDensT.nnodes = rsData.meshDens->nNodesRowT(refLayer);
		writeNodes_layerB(rsData, rlData, refLayer);
		writeNodes_layerM(rsData, rlData, refLayer);
		writeNodes_layerT(rsData, rlData, refLayer);
	}

	Mesher::nodeID1 = firstNodeID;
}

void ConeMesherRef2::updateLayerData(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	glm::dvec3 lineStart, lineEnd, line;
	lineStart = coordsOnCircle(rlData.curAngle, rsData.radius->rad1(), rsData.rotAxis);
	lineEnd = coordsOnCircle(rlData.curAngle, rsData.radius->rad2(), rsData.rotAxis);
	lineEnd[(size_t)rsData.rotAxis] += rsData.height;
	line = lineEnd - lineStart;

	int nElsOnLine = rsData.meshDens->nElRowB(refLayer);
	rlData.curConeLineStep = line / (double)nElsOnLine;
	rlData.curAngle += rlData.curAngleStep;
	rlData.curLineStartPos = rlData.curPos + lineStart;
}
void ConeMesherRef2::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	updateLayerData(rsData, rlData, refLayer);
	LineMesher::writeNodesQ(rlData.curLineStartPos, *rsData.csys, rlData.meshDensB, rlData.curConeLineStep);
}
void ConeMesherRef2::writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	updateLayerData(rsData, rlData, refLayer);
	LineMesher::writeNodesQ(rlData.curLineStartPos, *rsData.csys, rlData.meshDensM, rlData.curConeLineStep);
	rlData.curAngleStep *= 2.0;
}
void ConeMesherRef2::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	updateLayerData(rsData, rlData, refLayer + 1);
	LineMesher::writeNodesQ(rlData.curLineStartPos, *rsData.csys, rlData.meshDensT, rlData.curConeLineStep);
}

void ConeMesherRef2::writeElements(const MeshDensity2Dref& meshDens) {
	PlaneMesherRef::writeElements(meshDens);
}
