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

	nodeID1 = firstNode;
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
	direction				rotaxis)
{
	int firstNodeID = writer->getNextNodeID();
	MeshCsys curPos(spos);

	int			currentNodesPerArc = meshDens.circ();
	int			currentRefFactor = 1;
	int			currentRefinement = 0;

	double	dR				= radius.dR();
	int		nElAroundStart	= meshDens.nElCirc();
	double	arcLengthStart	= 2.0 * glm::pi<double>() * radius.start(); //TODO: Is this right? this should depend on angle...
	double	coneLength		= std::sqrt(std::pow(dR, 2.0) + std::pow(height, 2.0));
	double	elSizeX			= initialRefElSize2D(coneLength, meshDens.nRefs(), false);

	glm::dvec2	curElSize(elSizeX, arcLengthStart / (double)(nElAroundStart));
	
	double		currentRadius		= radius.start();
	double		currentConeLength	= 0.0;
	double		currentDh			= 0.0;
	

	while (currentRefinement < meshDens.nRefs()) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x
		ArcMesher::writeNodesCircular(curPos, currentNodesPerArc, currentRadius, angle, rotaxis);
		currentConeLength += curElSize.x;
		currentRadius = radius.start() + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		curPos.pos[(size_t)rotaxis] += currentDh;

		//row m:  |  x--x--x  |  x--x--x  |
		ArcMesher::writeNodesCircular_nth(curPos, currentNodesPerArc, currentRadius, angle, 4, rotaxis);
		currentConeLength += curElSize.x;
		currentRadius = radius.start() + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		curPos.pos[(size_t)rotaxis] += currentDh;

		//Refine
		currentRefFactor *= 2;
		// [9 nodes / 8 elements] -> [5 nodes / 4 elements] -> [3 nodes / 2 elements]
		currentNodesPerArc = meshDens.nElCirc() / currentRefFactor;
		if (!meshDens.closedLoop)
			currentNodesPerArc++;
		curElSize.y *= 2.0;

		//row t:
		ArcMesher::writeNodesCircular(curPos, currentNodesPerArc, currentRadius, angle, rotaxis);
		curElSize.x *= 2.0;
		currentConeLength += curElSize.x;
		currentRadius = radius.start() + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		curPos.pos[(size_t)rotaxis] += currentDh;
	}

	Mesher::nodeID1 = firstNodeID;
}

void ConeMesher::writeElements(const MeshDensity2D& meshDens){
	PlaneMesher::writeElementsPlane(meshDens);
}

void ConeMesherRef::writeElements(const MeshDensity2Dref& meshDens){
	PlaneMesherRef::writeElementsPlane_ref(meshDens);
}