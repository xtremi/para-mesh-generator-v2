#include "Cone3Dmesher.h"
#include "ConeMesher.h"
#include "CuboidMesher.h"
#include "DiskMesher.h"
#include "LineMesher.h"
#include "ArcMesher.h"
#include "glm/gtc/constants.hpp"
#include "math_utilities.h"

void Cone3Dmesher::writeNodes(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	double				radiusStartOuter,
	double				radiusEndOuter,
	double				radiusStartInner,
	double				radiusEndInner,
	double				startAng,
	double				endAng,
	double				height,
	const glm::ivec3&	nnodes, //.x - around, .y - along rad, .z - along rotaxis
	direction			rotaxis,
	glm::dmat3x3*		csys)
{

	int firstNodeID = writer->getNextNodeID();

	double dang = 0.0;
	//bool fullCircle = limitArcAngles(startAng, endAng, dang, nnodes.x);

	double dH = height / (double)(nnodes.z - 1);
	double dRi = (radiusEndInner - radiusStartInner) / (double)(nnodes.z - 1);
	double dRo = (radiusEndOuter - radiusStartOuter) / (double)(nnodes.z - 1);

	glm::dvec3  coords = spos;
	double		radiusInner = radiusStartInner;
	double		radiusOuter = radiusStartOuter;

	for (int i = 0; i < nnodes.z; i++) {
	  //DiskMesher::
		ConeMesher::writeNodes(writer, coords, radiusInner, radiusOuter, startAng, endAng, 0.0,
			glm::ivec2(nnodes.x, nnodes.y), rotaxis, csys);
		
		coords[(size_t)rotaxis] += dH;
		radiusInner += dRi;
		radiusOuter += dRo;
	}

	nodeID1 = firstNodeID;


}

void Cone3Dmesher::writeElements(
	FEAwriter*	writer,
	glm::ivec3	nnodes,
	bool		closedLoop)
{
	CuboidMesher::writeElements(writer, nnodes, closedLoop);
}


void Cone3DmesherRef::writeNodes(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::ivec2&	nNodes12,
	int					nRefinements,
	double				radiusStartOuter,
	double				radiusEndOuter,
	double				radiusStartInner,
	double				radiusEndInner,
	double				startAng,
	double				endAng,
	double				height,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	int firstNodeID = writer->getNextNodeID();

	bool	fullCircle = (startAng < 0.0) && (endAng < 0.0);
	
	double  dRi = (radiusEndInner - radiusStartInner);
	double  dRo = (radiusEndOuter - radiusStartOuter);
	int		nElAroundStart = fullCircle ? nNodes12.x : nNodes12.x - 1;
	double	arcLengthInnerStart = 2.0 * glm::pi<double>() * radiusStartInner;
	double  arcLengthOuterStart = 2.0 * glm::pi<double>() * radiusStartOuter;
	double	coneLengthInner = std::sqrt(std::pow(dRi, 2.0) + std::pow(height, 2.0));
	double	coneLengthOuter = std::sqrt(std::pow(dRo, 2.0) + std::pow(height, 2.0));
	double	elSize3 = initialRefinementElementSize(coneLengthOuter, nRefinements, false); //elsize along rotdir

	glm::ivec2	currentNodes12(nNodes12);	//.x around - .y outwards
	//glm::dvec2	currentElSize12(size.x / (double)(nNodesFace.x - 1), size.y / (double)(nNodesFace.y - 1));
	double		currentElSize3 = initialRefinementElementSize(coneLengthOuter, nRefinements, false) / 2.0; 

	int		currentRefFactor1 = 1;
	int		currentRefFactor2 = 1;
	int		currentRefinement = 0;
	double	currentRadiusInner = radiusStartInner;
	double	currentRadiusOuter = radiusStartOuter;
	double  currentConeLength  = 0.0;
	double  currentDh = 0.0;
	

	glm::dvec3 coords(spos);

	while (currentRefinement < nRefinements) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x		
		DiskMesher::writeNodes(writer, coords, currentRadiusInner, currentRadiusOuter, startAng, endAng, currentNodes12, rotaxis, csys);
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;

		//row m1:  |  x--x--x  |  x--x--x  |
		double dang = calcArcIncrement(startAng, endAng, currentNodes12.x);
		writeNodes_refLayer_2(writer, coords, currentNodes12, currentRadiusInner, currentRadiusOuter, startAng < 0.0 ? 0.0 : startAng,
			dang, 4, fullCircle, rotaxis, csys);
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;


		//Refine dir1
		currentRefFactor1 *= 2;
		if (!fullCircle)
			currentNodes12.x = (nNodes12.x - 1) / currentRefFactor1 + 1;
		else
			currentNodes12.x = nNodes12.x / currentRefFactor1;


		//row m2:  x-----x-----x-----x-----x
		DiskMesher::writeNodes(writer, coords, currentRadiusInner, currentRadiusOuter, startAng, endAng, currentNodes12, rotaxis, csys);
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;

		//row m3:  |  x--x--x  |  x--x--x  | (dir1)		
		dang = calcArcIncrement(startAng, endAng, currentNodes12.x);
		writeNodes_refLayer_1(writer, coords, currentNodes12, currentRadiusInner, currentRadiusOuter, 
			startAng < 0.0 ? 0.0 : startAng, dang, 4, rotaxis, csys);
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;


		//Refine dir2
		currentRefFactor2 *= 2;
		//if (!fullCircle)
			currentNodes12.y = (nNodes12.y - 1) / currentRefFactor2 + 1;
		//else
		//	currentNodes12.y = nNodes12.y / currentRefFactor2;


		//row t:  x-----x-----x-----x-----x (dir2)
		DiskMesher::writeNodes(writer, coords, currentRadiusInner, currentRadiusOuter, 
			startAng, endAng, currentNodes12, rotaxis, csys);
		currentElSize3 *= 2.0;
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;
	}

	nodeID1 = firstNodeID;
}

//possibly move to ConeMesher/DiskMesher/CylinderMesher::writeNodes_nth
void Cone3DmesherRef::writeNodes_refLayer_1(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::ivec2&	nnodes12,
	double				radiusInner,
	double				radiusOuter,
	double				startAng,
	double				dang,
	int					skipNth,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	double currentRadius = radiusInner;
	double dr = (radiusOuter - radiusInner) / (double)(nnodes12.y - 1);

	for (int i = 0; i < nnodes12.y; i++) {
		if (i % skipNth) {
			ArcMesher::writeNodesCircularQ(writer, spos, currentRadius, startAng, dang, nnodes12.x, rotaxis, csys);
		}
		currentRadius += dr;
	}
}

//possibly move to ConeMesher/DiskMesher/CylinderMesher::writeNodes_nth
void Cone3DmesherRef::writeNodes_refLayer_2(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::ivec2&	nnodes12,
	double				radiusInner,
	double				radiusOuter,
	double				startAng,
	double				dang,
	int					skipNth,
	bool				closedLoop,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	double currentRadius = radiusInner;
	double dR = (radiusOuter - radiusInner);
	glm::dvec3 coordsStart, coordsEnd;
	double currentAng = startAng;

	int nElementsAround = closedLoop ? nnodes12.x : nnodes12.x - 1;

	for (int i = 0; i < nnodes12.x; i++) {		
		
		coordsStart = coordsOnCircle(currentAng, radiusInner, rotaxis) + spos;
		coordsEnd   = coordsOnCircle(currentAng, radiusOuter, rotaxis) + spos;

		if (i % skipNth) {
			LineMesher::writeNodesLine(writer, coordsStart, coordsEnd, nnodes12.y, csys);
		}
		currentAng += dang;
	}
}


void Cone3DmesherRef::writeElements(
	FEAwriter*	writer,
	glm::ivec2	nnodes12,
	int			nRefinements,
	bool		closedLoop) 
{
	CuboidMesherRef::writeElements(writer, nnodes12, nRefinements, closedLoop);
}

