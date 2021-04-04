#include "Cone3Dmesher.h"
#include "ConeMesher.h"
#include "CuboidMesher.h"
#include "DiskMesher.h"
#include "LineMesher.h"
#include "ArcMesher.h"
#include "glm/gtc/constants.hpp"
#include "math_utilities.h"

void Cone3Dmesher::writeNodes(
	const MeshCsys&			spos,
	const MeshDensity3D&	meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	direction				axis)
{
MESHER_NODE_WRITE_START

	double dH  = height       / (double)meshDens.nElAxis();
	double dRi = radius.dRi() / (double)meshDens.nElAxis();
	double dRo = radius.dRo() / (double)meshDens.nElAxis();

	Cone2Dradius currentRadius(radius.start);
	MeshDensity2D meshDensDisk = meshDens.meshDensD12();

	for (int i = 0; i < meshDens.axis(); i++) {
		DiskMesher::writeNodes(curPos, meshDensDisk, currentRadius, angle, axis);
		curPos.pos[(size_t)axis] += dH;
		currentRadius.radi.x += dRi;
		currentRadius.radi.y += dRo;
	}

MESHER_NODE_WRITE_END
}

void Cone3Dmesher::writeElements(const MeshDensity3D& meshDens){
	CuboidMesher::writeElements(meshDens);
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
	const MeshCsys&			spos,
	const MeshDensity3Dref& meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	direction				rotaxis)
{
	MESHER_NODE_WRITE_START

	MeshDensity2D curMeshDensD12 = meshDens.meshDensD12();
	
	double  dRi = radius.dRi();
	double  dRo = radius.dRo();
	double	coneLengthInner = std::sqrt(std::pow(dRi, 2.0) + std::pow(height, 2.0));
	double	coneLengthOuter = std::sqrt(std::pow(dRo, 2.0) + std::pow(height, 2.0));

	double	currentElSize3 = initialRefinementElementSize(coneLengthOuter, meshDens.nRefs(), false) / 2.0; 

	int		currentRefFactor1 = 1;
	int		currentRefFactor2 = 1;
	int		currentRefinement = 0;
	Disk2Dradius currentRadius = radius.start;
	double  currentConeLength  = 0.0;
	double  currentDh		   = 0.0;
	

	while (currentRefinement < meshDens.nRefs()) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x		
		DiskMesher::writeNodes(curPos, curMeshDensD12, currentRadius, angle, rotaxis);
		currentConeLength += currentElSize3;
		currentRadius.setInner(radius.start.inner() + dRi * (currentConeLength / coneLengthOuter));
		currentRadius.setOuter(radius.start.outer() + dRo * (currentConeLength / coneLengthInner));
		currentDh = (height / coneLengthOuter)*currentElSize3;
		curPos.pos[(size_t)rotaxis] += currentDh;

		//row m1:  |  x--x--x  |  x--x--x  |
		double dang = angle.angStep(curMeshDensD12.dir1());
		writeNodes_refLayerM1(curPos, curMeshDensD12, currentRadius, angle.start, dang, 4, rotaxis);
		currentConeLength += currentElSize3;
		currentRadius.setInner(radius.start.inner() + dRi * (currentConeLength / coneLengthOuter));
		currentRadius.setOuter(radius.start.outer() + dRo * (currentConeLength / coneLengthInner));
		currentDh = (height / coneLengthOuter)*currentElSize3;
		curPos.pos[(size_t)rotaxis] += currentDh;


		//Refine dir1
		currentRefFactor1 *= 2;
		curMeshDensD12.nodes().x = meshDens.nElDir1() / currentRefFactor1;
		if (!meshDens.closedLoop)
			curMeshDensD12.nodes().x++;

		//row m2:  x-----x-----x-----x-----x
		DiskMesher::writeNodes(curPos, curMeshDensD12, currentRadius, angle, rotaxis);
		currentConeLength += currentElSize3;
		currentRadius.setInner(radius.start.inner() + dRi * (currentConeLength / coneLengthOuter));
		currentRadius.setOuter(radius.start.outer() + dRo * (currentConeLength / coneLengthInner));
		currentDh = (height / coneLengthOuter)*currentElSize3;
		curPos.pos[(size_t)rotaxis] += currentDh;

		//row m3:  |  x--x--x  |  x--x--x  | (dir1)		
		dang = angle.angStep(curMeshDensD12.dir1());
		writeNodes_refLayerM2(curPos, curMeshDensD12,currentRadius, angle.start, dang, 4, rotaxis);
		currentConeLength += currentElSize3;
		currentRadius.setInner(radius.start.inner() + dRi * (currentConeLength / coneLengthOuter));
		currentRadius.setOuter(radius.start.outer() + dRo * (currentConeLength / coneLengthInner));
		currentDh = (height / coneLengthOuter)*currentElSize3;
		curPos.pos[(size_t)rotaxis] += currentDh;


		//Refine dir2
		currentRefFactor2 *= 2;
		curMeshDensD12.nodes().y = meshDens.nElDir2() / currentRefFactor2 + 1;


		//row t:  x-----x-----x-----x-----x (dir2)
		DiskMesher::writeNodes(curPos, curMeshDensD12, currentRadius, angle, rotaxis);
		currentElSize3 *= 2.0;
		currentConeLength += currentElSize3;
		currentRadius.setInner(radius.start.inner() + dRi * (currentConeLength / coneLengthOuter));
		currentRadius.setOuter(radius.start.outer() + dRo * (currentConeLength / coneLengthInner));
		currentDh = (height / coneLengthOuter)*currentElSize3;
		curPos.pos[(size_t)rotaxis] += currentDh;
	}

	MESHER_NODE_WRITE_END
}


void Cone3DmesherRef::writeNodes_refLayerM1(
	const MeshCsys&		spos,
	const MeshDensity2D& meshDens,
	Disk2Dradius&		radius,
	double				startAng,
	double				dang,
	int					skipNth,
	direction			rotaxis)
{
	ConeMesher::writeNodes_nthLine(spos, meshDens, radius, startAng, dang, 0.0, skipNth, rotaxis);
}

void Cone3DmesherRef::writeNodes_refLayerM2(
	const MeshCsys&		spos,
	const MeshDensity2D& meshDens,
	Disk2Dradius&		radius,
	double				startAng,
	double				dang,
	int					skipNth,
	direction			rotaxis)
{
	ConeMesher::writeNodes_nthArc(spos, meshDens, radius, startAng, dang, 0.0, skipNth, rotaxis);
}




void Cone3DmesherRef::writeElements(const MeshDensity3Dref& meshDens)
{
	CuboidMesherRef::writeElements(meshDens);
}

