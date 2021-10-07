#include "Cone3Dmesher.h"
#include "ConeMesher.h"
#include "CuboidMesher.h"
#include "DiskMesher.h"
#include "LineMesher.h"
#include "ArcMesher.h"
#include "glm/gtc/constants.hpp"
#include "math_utilities.h"


void Cone3Dmesher::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity3D&	meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	direction				axis)
{
MESHER_NODE_WRITE_START

#define ordering1
#ifdef ordering1
	double dH  = height       / (double)meshDens.nElAxis();
	double dRi = radius.dRi() / (double)meshDens.nElAxis();
	double dRo = radius.dRo() / (double)meshDens.nElAxis();

	Cone2Dradius currentRadius(radius.start);
	MeshDensity2D meshDensDisk = meshDens.meshDensD12();

	for (int i = 0; i < meshDens.axis(); i++) {
		DiskMesher::writeNodes(curPos, csys, meshDensDisk, currentRadius, angle, axis);
		curPos[(size_t)axis] += dH;
		currentRadius.radi.x += dRi;
		currentRadius.radi.y += dRo;
	}
#else
	double dRi = radius.dRi() / (double)meshDens.nElAxis();
	double dRo = radius.dRo() / (double)meshDens.nElAxis();

	Cone2Dradius currentRadius(radius.start.inner(), radius.end.inner());
	double dRstart = (radius.start.outer() - radius.start.inner()) / (double)meshDens.nElAxis();
	double dRend   = (radius.end.outer() - radius.end.inner()) / (double)meshDens.nElAxis();

	MeshDensity2D meshDensCone(meshDens.circ(), meshDens.axis());

	for (int i = 0; i < meshDens.norm(); i++) {
		ConeMesher::writeNodes(curPos, csys, meshDensCone, currentRadius, angle, height, axis);
		currentRadius.incrStart(dRstart);
		currentRadius.incrEnd(dRend);
	}
#endif

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
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity3Dref& meshDens,
	const Pipe3Dradius&		radius,
	const ArcAngles&		angle,
	double					height,
	bool					startWithOffset,
	direction				rotaxis)
{
	int firstNode = writer->getNextNodeID();

	MeshDensity2D curMeshDensD12 = meshDens.meshDensD12B(0);
	
	RefShapeData rsData;
	rsData.csys = &csys;
	rsData.meshDens = &meshDens;
	rsData.radius = &radius;
	rsData.angle = &angle;
	rsData.height = height;
	rsData.rotAxis = rotaxis;
	rsData.coneLengthInner = std::sqrt(pow2(radius.dRi()) + pow2(height));
	rsData.coneLengthOuter = std::sqrt(pow2(radius.dRo()) + pow2(height));

	RefLayerData rlData;
	rlData.curPos = pos;
	rlData.curElSizeRefDir = initialRefElSize3D(rsData.coneLengthOuter, meshDens.nRefs(), startWithOffset);
	rlData.curRadius = radius.start;
	rlData.curConeLength = 0.0;

	
	if (startWithOffset) {
		//fix radius?
		rlData.curPos[(size_t)rotaxis] += rlData.curElSizeRefDir;
	}

	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		writeNodes_layerB(rsData, rlData, refLayer);		
		writeNodes_layerM1(rsData, rlData, refLayer);
		writeNodes_layerM2(rsData, rlData, refLayer);
		writeNodes_layerM3(rsData, rlData, refLayer);
		writeNodes_layerT(rsData, rlData, refLayer);
	}

	MESHER_NODE_WRITE_END
}

void Cone3DmesherRef::incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData)
{
	rlData.curConeLength += rlData.curElSizeRefDir;
	rlData.curRadius.setInner(rsData.radius->start.inner() + rsData.radius->dRi() * (rlData.curConeLength / rsData.coneLengthOuter));
	rlData.curRadius.setOuter(rsData.radius->start.outer() + rsData.radius->dRo() * (rlData.curConeLength / rsData.coneLengthInner));
	double currentDh = (rsData.height / rsData.coneLengthOuter)*rlData.curElSizeRefDir;
	rlData.curPos[(size_t)rsData.rotAxis] += currentDh;
}

//row b: x--x--x--x--x--x--x--x--x	
void Cone3DmesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer)
{
	DiskMesher::writeNodes(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12B(refLayer), 
		rlData.curRadius, *rsData.angle, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}

//row m1:  |  x--x--x  |  x--x--x  |
void Cone3DmesherRef::writeNodes_layerM1(const RefShapeData& rsData, RefLayerData& rlData, int refLayer)
{
	MeshDensity2D meshDensB = rsData.meshDens->meshDensD12B(refLayer);
	double dang = rsData.angle->angStep(meshDensB.dir1());
	ConeMesher::writeNodes_nthLine(rlData.curPos, *rsData.csys, meshDensB, rlData.curRadius, 
		rsData.angle->start, dang, 0.0, 4, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}

//row m2:  x-----x-----x-----x-----x
void Cone3DmesherRef::writeNodes_layerM2(const RefShapeData& rsData, RefLayerData& rlData, int refLayer)
{
	DiskMesher::writeNodes(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12M2(refLayer), 
		rlData.curRadius, *rsData.angle, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
	
}
//row m3:  |  x--x--x  |  x--x--x  | (dir1)		
void Cone3DmesherRef::writeNodes_layerM3(const RefShapeData& rsData, RefLayerData& rlData, int refLayer)
{
	MeshDensity2D meshDensM2 = rsData.meshDens->meshDensD12M2(refLayer);
	double dang = rsData.angle->angStep(meshDensM2.circ());
	ConeMesher::writeNodes_nthArc(rlData.curPos, *rsData.csys, meshDensM2, rlData.curRadius, 
		rsData.angle->start, dang, 0.0, 4, rsData.rotAxis);
	incrementConeStep(rsData, rlData);
}

//row t:  x-----x-----x-----x-----x (dir2)
void Cone3DmesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer)
{	
	DiskMesher::writeNodes(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12T(refLayer),
		rlData.curRadius, *rsData.angle, rsData.rotAxis);
	rlData.curElSizeRefDir *= 2.0;
	incrementConeStep(rsData, rlData);
}



void Cone3DmesherRef::writeElements(const MeshDensity3Dref& meshDens)
{
	CuboidMesherRef::writeElements(meshDens);
}

