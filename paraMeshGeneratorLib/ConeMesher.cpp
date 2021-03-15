#include "ConeMesher.h"
#include "PlaneMesher.h"
#include "ArcMesher.h"
#include "glm/gtc/constants.hpp"

/*
	Cone around X,Y or Z-axis
	nnodes.x - along circumferance
	nndoes.y - along height
*/
void ConeMesher::writeNodes(
	FEAwriter*			writer,
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
		ArcMesher::writeNodesCircularQ(writer, coords, radius, startAng, dang, nnodes.x, rotaxis, csys);
		coords[(size_t)rotaxis] += dH;
		radius += dR;
	}

	nodeID1 = firstNodeID;
}

void ConeMesher::writeNodesX(FEAwriter*	writer, const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	writeNodes(writer, centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, direction::x, csys);
}
void ConeMesher::writeNodesY(FEAwriter*	writer, const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	writeNodes(writer, centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, direction::y, csys);
}
void ConeMesher::writeNodesZ(FEAwriter*	writer, const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, glm::dmat3x3* csys)
{
	writeNodes(writer, centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, direction::z, csys);
}

/*

	In the code X-dir refers to refinement direction (along height)
	Y-dir refers to around the perimeter
*/
void ConeMesherRef::writeNodes(
	FEAwriter*			writer,
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
		ArcMesher::writeNodesCircular(writer, coords, currentRadius, startAng, endAng, currentNodesPerArc, rotaxis, csys);
		currentConeLength += curElSize.x;
		currentRadius = radiusStart + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;

		//row m:  |  x--x--x  |  x--x--x  |
		ArcMesher::writeNodesCircular_nth(writer, coords, currentRadius, startAng, endAng, currentNodesPerArc, 4, rotaxis, csys);
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
		ArcMesher::writeNodesCircular(writer, coords, currentRadius, startAng, endAng, currentNodesPerArc, rotaxis, csys);
		curElSize.x *= 2.0;
		currentConeLength += curElSize.x;
		currentRadius = radiusStart + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;
	}

	Mesher::nodeID1 = firstNodeID;
}

void ConeMesher::writeElements(FEAwriter* writer, glm::ivec2 nnodes, bool closedLoop)
{
	PlaneMesher::writeElementsPlane(writer, nnodes, closedLoop);
}

void ConeMesherRef::writeElements(FEAwriter* writer, int nNodesY, int nRefinements, bool closedLoop)
{
	PlaneMesherRef::writeElementsPlane_ref(writer, nNodesY, nRefinements, closedLoop);
}