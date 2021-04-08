#include "ConeMesher.h"
#include "PlaneMesher.h"
#include "ArcMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"
#include "glm/gtc/constants.hpp"

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
	const MeshCsys&			spos,
	const MeshDensity2D&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);
	
	double dang = angle.angStep(meshDens.circ());
	double dH = height / (double)meshDens.nElNorm();
	double dR = radius.dR() / (double)meshDens.nElNorm();

	double	curRadius = radius.start();

	for (int i = 0; i < meshDens.norm(); i++) {
		ArcMesher::writeNodesCircularQ(curPos, meshDens.dir1(), curRadius, angle.start, dang, rotaxis);
		curPos.pos[(size_t)rotaxis] += dH;
		curRadius += dR;
	}

	Mesher::nodeID1 = firstNode;
}

void ConeMesher::writeNodesX(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height){
	writeNodes(spos, meshDens, radius, angle, height, direction::x);
}
void ConeMesher::writeNodesY(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height){
	writeNodes(spos, meshDens, radius, angle, height, direction::y);
}
void ConeMesher::writeNodesZ(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height){
	writeNodes(spos, meshDens, radius, angle, height, direction::z);
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
	const MeshCsys&		 spos,
	const MeshDensity2D& meshDens,
	const Cone2Dradius&	 radius,
	double				 startAng,
	double				 dang,
	double				 height,
	int					 skipNth,
	direction			 rotaxis)
{
	MeshCsys curPos(spos);
	glm::dvec3 endPos;
	double currentAng = startAng;

	for (int i = 0; i < meshDens.dir1(); i++) {

		curPos.pos = coordsOnCircle(currentAng, radius.start(), rotaxis) + spos.pos;
		endPos = coordsOnCircle(currentAng, radius.end(), rotaxis) + spos.pos;

		if (i % skipNth) {
			LineMesher::writeNodesLine(curPos, meshDens.dir2(), endPos);
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
	const MeshCsys&		 spos,
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
			ArcMesher::writeNodesCircularQ(spos, meshDens.dir1(), currentRadius, startAng, dang, rotaxis);
		}
		currentRadius += dr;
	}
}

/*

	In the code X-dir refers to refinement direction (along height)
	Y-dir refers to around the perimeter
*/
void ConeMesherRef::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	const Cone2Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	bool					startWithOffset,
	direction				rotaxis)
{
	int firstNodeID = writer->getNextNodeID();

	RefShapeData rsData;	
	rsData.meshDens = &meshDens;
	rsData.radius	= &radius;
	rsData.angle	= &angle;
	rsData.height	= height;
	rsData.rotAxis	= rotaxis;
	rsData.coneLength = std::sqrt(pow2(radius.dR()) + pow2(height));

	RefLayerData rlData;
	rlData.curPos = spos;	
	rlData.curElSize.x = initialRefElSize2D(rsData.coneLength, meshDens.nRefs(), startWithOffset);
	rlData.curRadius = radius.start();
	rlData.curConeLength = 0.0;

	double	arcLengthStart = angle.angleSize() * radius.start();
	rlData.curElSize.y = arcLengthStart / (double)meshDens.nElCirc();
	
	if (startWithOffset) {
		//fix radius?
		rlData.curPos.pos[(size_t)rotaxis] += rlData.curElSize.x;
	}

	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		writeNodes_refLayerB(rsData, rlData, refLayer);
		writeNodes_refLayerM(rsData, rlData, refLayer);
		writeNodes_refLayerT(rsData, rlData, refLayer);
	}

	Mesher::nodeID1 = firstNodeID;
}

void ConeMesherRef::incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData)
{
	rlData.curConeLength += rlData.curElSize.x;
	rlData.curRadius = rsData.radius->start() + rsData.radius->dR() * (rlData.curConeLength / rsData.coneLength);
	double curDh = (rsData.height / rsData.coneLength)*rlData.curElSize.x;
	rlData.curPos.pos[(size_t)rsData.rotAxis] += curDh;
}

//row b: x--x--x--x--x--x--x--x--x
void ConeMesherRef::writeNodes_refLayerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer)
{
	ArcMesher::writeNodesCircular(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer), rlData.curRadius, *rsData.angle, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}
//row m:  |  x--x--x  |  x--x--x  |
void ConeMesherRef::writeNodes_refLayerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	ArcMesher::writeNodesCircular_nth(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer), rlData.curRadius, *rsData.angle, 4, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}
//row t: x----x----x----x----x
void ConeMesherRef::writeNodes_refLayerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer)
{
	rlData.curElSize.y *= 2.0;
	ArcMesher::writeNodesCircular(rlData.curPos, rsData.meshDens->nNodesRowT(refLayer), rlData.curRadius, *rsData.angle, rsData.rotAxis);
	rlData.curElSize.x *= 2.0;
	incrementConeStep(rsData, rlData);
}

void ConeMesher::writeElements(const MeshDensity2D& meshDens){
	PlaneMesher::writeElementsPlane(meshDens);
}

void ConeMesherRef::writeElements(const MeshDensity2Dref& meshDens){
	PlaneMesherRef::writeElements(meshDens);
}