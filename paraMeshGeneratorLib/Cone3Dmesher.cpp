#include "Cone3Dmesher.h"
#include "ConeMesher.h"
#include "CuboidMesher.h"
#include "DiskMesher.h"
#include "LineMesher.h"
#include "ArcMesher.h"
#include "glm/gtc/constants.hpp"
#include "math_utilities.h"

void Cone3Dmesher::writeNodes(
	const glm::dvec3	spos,
	MeshDensity3D&		meshDens,
	const Pipe3Dradius&	radius,
	const ArcAngles&	angle,
	double				height,
	direction			axis,
	glm::dmat3x3*		csys)
{
	int firstNodeID = writer->getNextNodeID();
	glm::dvec3  coords(spos);

	double dH  = height       / (double)meshDens.nElAxis();
	double dRi = radius.dRi() / (double)meshDens.nElAxis();
	double dRo = radius.dRo() / (double)meshDens.nElAxis();

	Pipe2Dradius currentRadius(radius.start.inner, radius.start.outer);

	for (int i = 0; i < meshDens.nodes.axis(); i++) {
	  DiskMesher::writeNodes(coords, currentRadius.inner, currentRadius.outer, angle.start, angle.end,
		  glm::ivec2(meshDens.nodes.dir1, meshDens.nodes.dir2), axis, csys);
		
		coords[(size_t)axis] += dH;
		currentRadius.inner += dRi;
		currentRadius.outer += dRo;
	}

	nodeID1 = firstNodeID;


}

void Cone3Dmesher::writeElements(
	MeshDensity3D&  meshDens,
	bool			closedLoop)
{
	CuboidMesher::writeElements(glm::ivec3(meshDens.nodes.dir1, meshDens.nodes.dir2, meshDens.nodes.dir3), closedLoop);
}

/*
	Draws the nodes for a cylinders structure of dimensions <size>.
	The nodes are structured in such a way that ConeMesherRef::writeElements
	can create the Hexagonal elements making up the cone.

	The cube is refined along its local Z-axis (same as its rotation axis)
	The number of nodes along the circumeference is set by nNodes12.x
	The number of nodes along the radii is set by nNodes12.y
	The number of nodes along the local Z-axis is dependent in the number of refinements.

	[in] writer			  : 	
	[in] spos			  : 
	[in] nNodes12		  : 
	[in] nRefinements	  : 
	[in] radiusStartOuter : 
	[in] radiusEndOuter	  : 
	[in] radiusStartInner : 
	[in] radiusEndInner	  : 
	[in] startAng		  : 
	[in] endAng			  : 
	[in] height			  : 
	[in] rotaxis		  : 
	[in] csys			  : 

*/
void Cone3DmesherRef::writeNodes(
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
	glm::dvec3 coords(spos);

	glm::ivec2	currentNodes12(nNodes12);	//.x around - .y outwards

	bool	fullCircle = (startAng < 0.0) && (endAng < 0.0);
	
	double  dRi = (radiusEndInner - radiusStartInner);
	double  dRo = (radiusEndOuter - radiusStartOuter);
	//int	  nElAroundStart = fullCircle ? nNodes12.x : nNodes12.x - 1;
	//double  arcLengthInnerStart = 2.0 * glm::pi<double>() * radiusStartInner;
	//double  arcLengthOuterStart = 2.0 * glm::pi<double>() * radiusStartOuter;
	double	coneLengthInner = std::sqrt(std::pow(dRi, 2.0) + std::pow(height, 2.0));
	double	coneLengthOuter = std::sqrt(std::pow(dRo, 2.0) + std::pow(height, 2.0));

	double	currentElSize3 = initialRefinementElementSize(coneLengthOuter, nRefinements, false) / 2.0; 

	int		currentRefFactor1 = 1;
	int		currentRefFactor2 = 1;
	int		currentRefinement = 0;
	double	currentRadiusInner = radiusStartInner;
	double	currentRadiusOuter = radiusStartOuter;
	double  currentConeLength  = 0.0;
	double  currentDh		   = 0.0;
	

	while (currentRefinement < nRefinements) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x		
		DiskMesher::writeNodes(coords, currentRadiusInner, currentRadiusOuter, startAng, endAng, currentNodes12, rotaxis, csys);
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;

		//row m1:  |  x--x--x  |  x--x--x  |
		double dang = calcArcIncrement(startAng, endAng, currentNodes12.x);
		writeNodes_refLayerM1(coords, currentNodes12, currentRadiusInner, currentRadiusOuter, startAng < 0.0 ? 0.0 : startAng, dang, 4, rotaxis, csys);
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
		DiskMesher::writeNodes(coords, currentRadiusInner, currentRadiusOuter, startAng, endAng, currentNodes12, rotaxis, csys);
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;

		//row m3:  |  x--x--x  |  x--x--x  | (dir1)		
		dang = calcArcIncrement(startAng, endAng, currentNodes12.x);
		writeNodes_refLayerM2(coords, currentNodes12, currentRadiusInner, currentRadiusOuter, startAng < 0.0 ? 0.0 : startAng, dang, 4, rotaxis, csys);
		currentConeLength += currentElSize3;
		currentRadiusInner = radiusStartInner + dRi * (currentConeLength / coneLengthOuter);
		currentRadiusOuter = radiusStartOuter + dRo * (currentConeLength / coneLengthInner);
		currentDh = (height / coneLengthOuter)*currentElSize3;
		coords[(size_t)rotaxis] += currentDh;


		//Refine dir2
		currentRefFactor2 *= 2;
		currentNodes12.y = (nNodes12.y - 1) / currentRefFactor2 + 1;


		//row t:  x-----x-----x-----x-----x (dir2)
		DiskMesher::writeNodes(coords, currentRadiusInner, currentRadiusOuter, 
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


void Cone3DmesherRef::writeNodes_refLayerM1(
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
	ConeMesher::writeNodes_nthLine(spos, nnodes12, radiusInner, radiusOuter, startAng, dang, 0.0, skipNth, rotaxis, csys);
}

void Cone3DmesherRef::writeNodes_refLayerM2(
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
	ConeMesher::writeNodes_nthArc(spos, nnodes12, radiusInner, radiusOuter, startAng, dang, 0.0, skipNth, rotaxis, csys);
}




void Cone3DmesherRef::writeElements(
	glm::ivec2	nnodes12,
	int			nRefinements,
	bool		closedLoop) 
{
	CuboidMesherRef::writeElements(nnodes12, nRefinements, closedLoop);
}

