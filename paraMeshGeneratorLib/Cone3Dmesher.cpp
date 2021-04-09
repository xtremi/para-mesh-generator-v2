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
	bool					startWithOffset,
	direction				rotaxis)
{
	MESHER_NODE_WRITE_START

	MeshDensity2D curMeshDensD12 = meshDens.meshDensD12B(0);
	
	double  dRi = radius.dRi();
	double  dRo = radius.dRo();
	double	coneLengthInner = std::sqrt(pow2(dRi) + pow2(height));
	double	coneLengthOuter = std::sqrt(pow2(dRo) + pow2(height));

	double	curElSize3 = initialRefElSize3D(coneLengthOuter, meshDens.nRefs(), startWithOffset); 

	int		currentRefFactor1 = 1;
	int		currentRefFactor2 = 1;
	int		currentRefinement = 0;
	Disk2Dradius curRadius = radius.start;
	double  curConeLength = 0.0;
	
	if (startWithOffset) {
		//fix radius?
		curPos.pos[(size_t)rotaxis] += curElSize3;
	}

	while (currentRefinement < meshDens.nRefs()) {
		currentRefinement++;
			
		//Layer B + M1
		writeNodes_layerB(curPos, curMeshDensD12, radius, angle, curElSize3, 
			coneLengthOuter, coneLengthInner, height, curRadius, curConeLength, rotaxis);		
		writeNodes_layerM1(curPos, curMeshDensD12, radius, angle, curElSize3,
			coneLengthOuter, coneLengthInner, height, curRadius, curConeLength, rotaxis);

		//Refine dir1
		currentRefFactor1 *= 2;
		curMeshDensD12.nodes().x = meshDens.nElDir1() / currentRefFactor1;
		if (!meshDens.closedLoop)
			curMeshDensD12.nodes().x++;

		//Layer M2 + M3
		writeNodes_layerM2(curPos, curMeshDensD12, radius, angle, curElSize3,
			coneLengthOuter, coneLengthInner, height, curRadius, curConeLength, rotaxis);
		writeNodes_layerM3(curPos, curMeshDensD12, radius, angle, curElSize3,
			coneLengthOuter, coneLengthInner, height, curRadius, curConeLength, rotaxis);

		//Refine dir2
		currentRefFactor2 *= 2;
		curMeshDensD12.nodes().y = meshDens.nElDir2() / currentRefFactor2 + 1;

		writeNodes_layerT(curPos, curMeshDensD12, radius, angle, curElSize3,
			coneLengthOuter, coneLengthInner, height, curRadius, curConeLength, rotaxis);
	}

	MESHER_NODE_WRITE_END
}

void Cone3DmesherRef::incrementConeStep(
	MeshCsys&			curPos,
	double&				curConeLength,
	Disk2Dradius&		curRadius,
	double				coneLengthOuter,
	double				coneLengthInner,
	double				elSizeRefDir,
	double				height,
	const Pipe3Dradius&	radius,
	direction			rotaxis)
{
	curConeLength += elSizeRefDir;
	curRadius.setInner(radius.start.inner() + radius.dRi() * (curConeLength / coneLengthOuter));
	curRadius.setOuter(radius.start.outer() + radius.dRo() * (curConeLength / coneLengthInner));
	double currentDh = (height / coneLengthOuter)*elSizeRefDir;
	curPos.pos[(size_t)rotaxis] += currentDh;
}

//row b: x--x--x--x--x--x--x--x--x	
void Cone3DmesherRef::writeNodes_layerB(
	MeshCsys&				curPos,
	const MeshDensity2D&	meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					elSizeRefDir,
	double					coneLengthOuter,
	double					coneLengthInner,
	double					height,
	Disk2Dradius&			curRadius,
	double&					curConeLength,
	direction				rotaxis)
{
	DiskMesher::writeNodes(curPos, meshDens, curRadius, angle, rotaxis);
	incrementConeStep(curPos, curConeLength, curRadius, coneLengthOuter, coneLengthInner, elSizeRefDir, height, radius, rotaxis);
}

//row m1:  |  x--x--x  |  x--x--x  |
void Cone3DmesherRef::writeNodes_layerM1(
	MeshCsys&				curPos,
	const MeshDensity2D&	meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					elSizeRefDir,
	double					coneLengthOuter,
	double					coneLengthInner,
	double					height,
	Disk2Dradius&			curRadius,
	double&					curConeLength,
	direction				rotaxis)
{
	double dang = angle.angStep(meshDens.dir1());
	ConeMesher::writeNodes_nthLine(curPos, meshDens, curRadius, angle.start, dang, 0.0, 4, rotaxis);
	incrementConeStep(curPos, curConeLength, curRadius, coneLengthOuter, coneLengthInner, elSizeRefDir, height, radius, rotaxis);
}

//row m2:  x-----x-----x-----x-----x
void Cone3DmesherRef::writeNodes_layerM2(
	MeshCsys&				curPos,
	const MeshDensity2D&	meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					elSizeRefDir,
	double					coneLengthOuter,
	double					coneLengthInner,
	double					height,
	Disk2Dradius&			curRadius,
	double&					curConeLength,
	direction				rotaxis)
{
	DiskMesher::writeNodes(curPos, meshDens, curRadius, angle, rotaxis);
	incrementConeStep(curPos, curConeLength, curRadius, coneLengthOuter, coneLengthInner, elSizeRefDir, height, radius, rotaxis);
	
}
//row m3:  |  x--x--x  |  x--x--x  | (dir1)		
void Cone3DmesherRef::writeNodes_layerM3(
	MeshCsys&				curPos,
	const MeshDensity2D&	meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					elSizeRefDir,
	double					coneLengthOuter,
	double					coneLengthInner,
	double					height,
	Disk2Dradius&			curRadius,
	double&					curConeLength,
	direction				rotaxis)
{
	double dang = angle.angStep(meshDens.dir1());
	ConeMesher::writeNodes_nthArc(curPos, meshDens, curRadius, angle.start, dang, 0.0, 4, rotaxis);
	incrementConeStep(curPos, curConeLength, curRadius, coneLengthOuter, coneLengthInner, elSizeRefDir, height, radius, rotaxis);
}

//row t:  x-----x-----x-----x-----x (dir2)
void Cone3DmesherRef::writeNodes_layerT(
	MeshCsys&				curPos,
	const MeshDensity2D&	meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double&					elSizeRefDir,
	double					coneLengthOuter,
	double					coneLengthInner,
	double					height,
	Disk2Dradius&			curRadius,
	double&					curConeLength,
	direction				rotaxis)
{	
	DiskMesher::writeNodes(curPos, meshDens, curRadius, angle, rotaxis);
	elSizeRefDir *= 2.0;
	incrementConeStep(curPos, curConeLength, curRadius, coneLengthOuter, coneLengthInner, elSizeRefDir, height, radius, rotaxis);
}



void Cone3DmesherRef::writeElements(const MeshDensity3Dref& meshDens)
{
	CuboidMesherRef::writeElements(meshDens);
}

