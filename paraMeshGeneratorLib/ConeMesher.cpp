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
	const glm::dvec3&	centerPos,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				endAng,
	double				height,
	const glm::ivec2&	nnodes,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	int firstNodeID = writer->getNextNodeID();

	double dang = 0.0;
	bool fullCircle = limitArcAngles(startAng, endAng, dang, nnodes.x);

	double dH = height / (double)(nnodes.y - 1);
	double dR = (radiusEnd - radiusStart) / (double)(nnodes.y - 1);

	glm::dvec3  coords = centerPos;
	double		radius = radiusStart;

	for (int i = 0; i < nnodes.y; i++) {
		ArcMesher::writeNodesCircularQ(coords, radius, startAng, dang, nnodes.x, rotaxis, csys);
		coords[(size_t)rotaxis] += dH;
		radius += dR;
	}

	nodeID1 = firstNodeID;
}

void ConeMesher::writeNodesX(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	writeNodes(centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, direction::x, csys);
}
void ConeMesher::writeNodesY(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	writeNodes(centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, direction::y, csys);
}
void ConeMesher::writeNodesZ(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	writeNodes(centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, direction::z, csys);
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
	const glm::dvec3&	spos,
	const glm::ivec2&	nnodes,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				dang,
	double				height,
	int					skipNth,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coordsStart, coordsEnd;
	double currentAng = startAng;

	for (int i = 0; i < nnodes.x; i++) {

		coordsStart = coordsOnCircle(currentAng, radiusStart, rotaxis) + spos;
		coordsEnd = coordsOnCircle(currentAng, radiusEnd, rotaxis) + spos;

		if (i % skipNth) {
			LineMesher::writeNodesLine(coordsStart, coordsEnd, nnodes.y, csys);
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
	const glm::dvec3&	spos,
	const glm::ivec2&	nnodes,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				dang,
	double				height,
	int					skipNth,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	double currentRadius = radiusStart;
	double dr = (radiusEnd - radiusStart) / (double)(nnodes.y - 1);

	for (int i = 0; i < nnodes.y; i++) {
		if (i % skipNth) {
			ArcMesher::writeNodesCircularQ(spos, currentRadius, startAng, dang, nnodes.x, rotaxis, csys);
		}
		currentRadius += dr;
	}
}

/*

	In the code X-dir refers to refinement direction (along height)
	Y-dir refers to around the perimeter
*/
void ConeMesherRef::writeNodes(
	const glm::dvec3&	centerPos,
	int					nNodesEdge,
	int					nRefinements,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				endAng,
	double				height,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{

	int firstNodeID = writer->getNextNodeID();

	bool	fullCircle = (startAng < 0.0) && (endAng < 0.0);

	double	dR = radiusEnd - radiusStart;

	int		nElAroundStart = fullCircle ? nNodesEdge : nNodesEdge - 1;
	double	arcLengthStart = 2.0 * glm::pi<double>() * radiusStart;
	double	coneLength = std::sqrt(std::pow(dR, 2.0) + std::pow(height, 2.0));
	double	elSizeX = initialRefinementElementSize(coneLength, nRefinements, false);

	glm::dvec2	curElSize(elSizeX, arcLengthStart / (double)(nElAroundStart));
	int			currentNodesPerArc = nNodesEdge;
	int			currentRefFactor = 1;
	int			currentRefinement = 0;
	double		currentRadius = radiusStart;
	double		currentConeLength = 0.0;
	double		currentDh = 0.0;

	glm::dvec3 coords(centerPos);

	while (currentRefinement < nRefinements) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x
		ArcMesher::writeNodesCircular(coords, currentRadius, startAng, endAng, currentNodesPerArc, rotaxis, csys);
		currentConeLength += curElSize.x;
		currentRadius = radiusStart + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;

		//row m:  |  x--x--x  |  x--x--x  |
		ArcMesher::writeNodesCircular_nth(coords, currentRadius, startAng, endAng, currentNodesPerArc, 4, rotaxis, csys);
		currentConeLength += curElSize.x;
		currentRadius = radiusStart + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;

		//Refine
		currentRefFactor *= 2;
		// [9 nodes / 8 elements] -> [5 nodes / 4 elements] -> [3 nodes / 2 elements]
		if (!fullCircle)
			currentNodesPerArc = (nNodesEdge - 1) / currentRefFactor + 1;
		else
			currentNodesPerArc = nNodesEdge / currentRefFactor;

		curElSize.y *= 2.0;

		//row t:
		ArcMesher::writeNodesCircular(coords, currentRadius, startAng, endAng, currentNodesPerArc, rotaxis, csys);
		curElSize.x *= 2.0;
		currentConeLength += curElSize.x;
		currentRadius = radiusStart + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;
	}

	Mesher::nodeID1 = firstNodeID;
}

void ConeMesher::writeElements(glm::ivec2 nnodes, bool closedLoop)
{
	PlaneMesher::writeElementsPlane(nnodes, closedLoop);
}

void ConeMesherRef::writeElements(int nNodesY, int nRefinements, bool closedLoop)
{
	PlaneMesherRef::writeElementsPlane_ref(nNodesY, nRefinements, closedLoop);
}