#include "Extrusion.h"
#include "PlaneMesher.h"
#include "CuboidMesher.h"
#include "Cone3Dmesher.h"
#include "ConeMesher.h"

MeshExtrusion::MeshExtrusion(int _nElements, MeshExtrusion* previousExtrusion) {
	nElements = _nElements;
	_isStart = (bool)!previousExtrusion;
}

/*!
	Number of nodes along extrusion
*/
int MeshExtrusion::nNodes() { 
	return isStart() ? (nElements + 1) : nElements; 
}


MeshEdgeExtrusion::MeshEdgeExtrusion(
	int	   _nElements,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshEdgeExtrusion* previousExtrusion) : MeshExtrusion( _nElements, previousExtrusion)
{}

MeshEdgeExtrusion_noRef::MeshEdgeExtrusion_noRef(
	int	   _nElements,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshEdgeExtrusion* previousExtrusion) : MeshEdgeExtrusion(_nElements, nnodeEdge1, firstNodeID, previousExtrusion)
{
	initEdges(nnodeEdge1, firstNodeID, previousExtrusion);
}
void MeshEdgeExtrusion_noRef::writeElements() {
	PlaneMesher::writeElements(meshDens);
}

/*!

isStart extrusion:

nElements = 3
nNodes    = 4
x5  x6  x7  x8
x1  x2  x3  x4


 ------------------

normal extrusion:

nElements = 3
nNodes    = 3
-   x5  x6  x7
-   x1  x2  x3



  initial  second         third
  extr.    extrusion      extrusion
|--------|<------------|<-----------------|
|        |             |                  |

x--x--x--o2-----x------o2-----x---x---x---o2
|  |  |  |      |      |      |   |   |   |
x--x--x--x------x------x------x---x---x---x
|  |  |  |      |      |      |   |   |   |
x--x--x--o1-----x------o1-----x---x---x---o1

o1/o2 - preNode1 / preNode2


*/
void MeshEdgeExtrusion_noRef::initEdges(int nnodeEdge1, int firstNodeID, MeshEdgeExtrusion* previousExtrusion) {
	int nnodesExtr = nNodes();
	meshDens = MeshDensity2D(nnodesExtr, nnodeEdge1);

	//the end nodes of the previous extrusion
	int preNodeEdge1 = 0, preNodeEdge2 = 0;
	if (!isStart()) {
		preNodeEdge1 = previousExtrusion->endCornerNode1();
		preNodeEdge2 = previousExtrusion->endCornerNode2();
	}

	edges[1] = MeshEdge(std::make_shared<NodeIterator1D>(meshDens.edgeNodeIterator(1, firstNodeID, preNodeEdge1)));
	edges[2] = MeshEdge(std::make_shared<NodeIterator1D>(meshDens.edgeNodeIterator(2, firstNodeID)));
	edges[3] = MeshEdge(std::make_shared<NodeIterator1D>(meshDens.edgeNodeIterator(3, firstNodeID, preNodeEdge2)));
	edges[4] = MeshEdge(std::make_shared<NodeIterator1D>(meshDens.edgeNodeIterator(0, firstNodeID)));
	if (isStart()) {
		edges[0] = edges[4];
	}
	else {
		edges[0] = ((MeshEdgeExtrusion*)previousExtrusion)->edges[2];
	}
}

MeshEdgeExtrusion_ref::MeshEdgeExtrusion_ref(
	int	   _nRef,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshEdgeExtrusion* previousExtrusion) 
	: MeshEdgeExtrusion(0, nnodeEdge1, firstNodeID, previousExtrusion), MeshExtrusion_refProp(_nRef)
{
	initEdges(nnodeEdge1, firstNodeID, previousExtrusion);
}
void MeshEdgeExtrusion_ref::writeElements() {
	PlaneMesherRef::writeElements(meshDens);
}

void MeshEdgeExtrusion_ref::initEdges(int nnodeEdge1, int firstNodeID, MeshEdgeExtrusion* previousExtrusion)
{
	int nnodesExtr = nNodes();
	meshDens = MeshDensity2Dref(nRef, nnodeEdge1);

	//the end nodes of the previous extrusion
	int preNodeEdge1 = 0, preNodeEdge2 = 0;
	if (!isStart()) {
		preNodeEdge1 = previousExtrusion->endCornerNode1();
		preNodeEdge2 = previousExtrusion->endCornerNode2();
	}

	edges[1] = MeshEdge(std::make_shared<NodeIterator1Dref>(meshDens.edgeNodeIteratorRefDir(1, firstNodeID, preNodeEdge1)));
	edges[2] = MeshEdge(std::make_shared<NodeIterator1D>(meshDens.edgeNodeIterator(2, firstNodeID)));
	edges[3] = MeshEdge(std::make_shared<NodeIterator1Dref>(meshDens.edgeNodeIteratorRefDir(3, firstNodeID, preNodeEdge2)));
	edges[4] = MeshEdge(std::make_shared<NodeIterator1D>(meshDens.edgeNodeIterator(0, firstNodeID)));
	if (isStart()) {
		edges[0] = edges[4];
	}
	else {
		edges[0] = ((MeshEdgeExtrusion*)previousExtrusion)->edges[2];
	}
}


/************************************************
	MeshEdgeExtrusionLinear
************************************************/
MeshEdgeExtrusionLinear::MeshEdgeExtrusionLinear(
	double _length,
	int	   _nElements,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshEdgeExtrusion* previousExtrusion) 
	: MeshEdgeExtrusion_noRef(_nElements, nnodeEdge1, firstNodeID, previousExtrusion),
	MeshExtrusion_linearProp(_length)
{}

double MeshEdgeExtrusionLinear::spacing() {
	return length / (double)(nElements);
}
void MeshEdgeExtrusionLinear::writeNodes(ExtrudeStepData* curExtrData) {
	curExtrData->pos = glm::dvec3(curExtrData->startSpace, 0., 0.);
	PlaneMesher::writeNodesXYq(
		curExtrData->pos,
		curExtrData->csys,
		meshDens,
		((ExtrudeEdgeStepData*)curExtrData)->dxy);

	curExtrData->csys.moveInLocalCsys(glm::dvec3(length, 0., 0.));
}

/************************************************
	MeshEdgeExtrusionArc
************************************************/
MeshEdgeExtrusionArc::MeshEdgeExtrusionArc(
	double _radius,
	double _endAngle,
	int	   _nElements,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshEdgeExtrusion* previousExtrusion)
	: MeshEdgeExtrusion_noRef(_nElements, nnodeEdge1, firstNodeID, previousExtrusion),
	MeshExtrusion_arcProp(_radius, _endAngle)
{}
double MeshEdgeExtrusionArc::spacing() {
	return endAngle / (double)(nElements);
}
void MeshEdgeExtrusionArc::writeNodes(ExtrudeStepData* _curExtrData) {
	ExtrudeEdgeStepData* curExtrData = (ExtrudeEdgeStepData*)_curExtrData;
	
	curExtrData->pos = glm::dvec3(0., 0., radius);

	ArcAngles ang;
	ang.setStart(-curExtrData->startSpace - GLMPI);
	ang.setEnd(-(GLMPI + endAngle));

	ConeMesher::writeNodesY(
		curExtrData->pos,
		curExtrData->csys,
		meshDens,
		Cone2Dradius(radius, radius),
		ang, curExtrData->lengthY);

	curExtrData->arcAngle += endAngle;
	curExtrData->csys.moveInLocalCsys(coordsOnCircle(ang.end, radius, direction::y) + glm::dvec3(0, 0, radius));
	(*curExtrData->csys.csys) = makeCsysMatrix(Y_DIR, curExtrData->arcAngle);
}

/************************************************
	MeshEdgeExtrusionLinearRef
************************************************/
MeshEdgeExtrusionLinearRef::MeshEdgeExtrusionLinearRef(
	double _length,
	int	   _nRef,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshEdgeExtrusion* previousExtrusion)
	: MeshEdgeExtrusion_ref(_nRef, nnodeEdge1, firstNodeID, previousExtrusion),
	MeshExtrusion_linearProp(_length)
{}
double MeshEdgeExtrusionLinearRef::spacing() {
	return 0.0;
}
void MeshEdgeExtrusionLinearRef::writeNodes(ExtrudeStepData* _curExtrData) {
	ExtrudeEdgeStepData* curExtrData = (ExtrudeEdgeStepData*)_curExtrData;

	curExtrData->pos = glm::dvec3(curExtrData->startSpace, 0., 0.);
	PlaneMesherRef::writeNodesXY(
		curExtrData->pos,
		curExtrData->csys,
		meshDens,
		glm::dvec2(length, curExtrData->lengthY), true);

	curExtrData->csys.moveInLocalCsys(glm::dvec3(length, 0., 0.));
}


/************************************************
	MeshEdgeExtrusionArcRef
************************************************/
MeshEdgeExtrusionArcRef::MeshEdgeExtrusionArcRef(
	double _radius,
	double _endAngle,
	int	   _nRef,
	int	   nnodeEdge1,
	int	   firstNodeID,
	MeshEdgeExtrusion* previousExtrusion)
	: MeshEdgeExtrusion_ref(_nRef, nnodeEdge1, firstNodeID, previousExtrusion),
	MeshExtrusion_arcProp(_radius, _endAngle)
{}
double MeshEdgeExtrusionArcRef::spacing() {
	return 0.0;
}
void MeshEdgeExtrusionArcRef::writeNodes(ExtrudeStepData* curStepData) {

}

/************************************************
	MeshFaceExtrusion
************************************************/
MeshFaceExtrusion::MeshFaceExtrusion(
	int					 _nElements,
	const MeshDensity2D& face0nodes,
	int					 firstNodeID,
	MeshFaceExtrusion*	 previousExtrusion)
	: MeshExtrusion(_nElements, previousExtrusion)
{}


/************************************************
	MeshFaceExtrusion_noRef
************************************************/
MeshFaceExtrusion_noRef::MeshFaceExtrusion_noRef(
	int	   _nElements,
	const  MeshDensity2D& face0nodes,
	int	   firstNodeID,
	MeshFaceExtrusion* previousExtrusion)
	: MeshFaceExtrusion(_nElements, face0nodes, firstNodeID, previousExtrusion)
{
	initFaces(face0nodes, firstNodeID, previousExtrusion);
}

void MeshFaceExtrusion_noRef::writeElements(){
	CuboidMesher::writeElements(meshDens);
}

/*!
	Returns the number of nodes in the two directions
	of the end face (normal to extrusion direction)

	For a noref extrusion this is the same as a the start face.
*/
MeshDensity2D MeshFaceExtrusion_noRef::meshDensFaceEnd() {
	return meshDens.meshDensD23();
}



/************************************************
	MeshFaceExtrusion_ref
************************************************/
MeshFaceExtrusion_ref::MeshFaceExtrusion_ref(
	int					  _nRef,
	const  MeshDensity2D& face0nodes,
	int					  firstNodeID,
	MeshFaceExtrusion*	  previousExtrusion)
	: MeshFaceExtrusion(0, face0nodes, firstNodeID, previousExtrusion), MeshExtrusion_refProp(_nRef)
{
	initFaces(face0nodes, firstNodeID, previousExtrusion);
}

void MeshFaceExtrusion_ref::writeElements() {
	CuboidMesherRef::writeElements(meshDens);
}

/*
	Returns the number of nodes in the two directions
	of the end face (normal to extrusion direction)

	For a ref extrusion this is not the same as a the start face.
*/
MeshDensity2D MeshFaceExtrusion_ref::meshDensFaceEnd() {
	return meshDens.meshDensD12end();
}



/************************************************
	MeshFaceExtrusionLinear
************************************************/
MeshFaceExtrusionLinear::MeshFaceExtrusionLinear(
	double _length,
	int	   _nElements,
	const MeshDensity2D& face0nodes,
	int	   firstNodeID,
	MeshFaceExtrusion* previousExtrusion)
	: MeshFaceExtrusion_noRef(_nElements, face0nodes, firstNodeID, previousExtrusion),
	MeshExtrusion_linearProp(_length)
{}
double MeshFaceExtrusionLinear::spacing() {
	return length / (double)(nElements);
}
void MeshFaceExtrusionLinear::writeNodes(ExtrudeStepData* curExtrData) {
	curExtrData->pos = glm::dvec3(curExtrData->startSpace, 0., 0.);
	CuboidMesher::writeNodesQ(
		curExtrData->pos,
		curExtrData->csys,
		meshDens,
		((ExtrudeFaceStepData*)curExtrData)->dxyz,
		plane::xy); //needs to be yz for extruding after ref?

	curExtrData->csys.moveInLocalCsys(glm::dvec3(length, 0., 0.));
}


/************************************************
	MeshFaceExtrusionArc
************************************************/
MeshFaceExtrusionArc::MeshFaceExtrusionArc(
	double				 _radiusInner,
	double				 _endAngle,
	int					 _nElements,
	const MeshDensity2D& face0nodes,
	int					 firstNodeID,
	MeshFaceExtrusion*		 previousExtrusion)
	: MeshFaceExtrusion_noRef(_nElements, face0nodes, firstNodeID, previousExtrusion),
	MeshExtrusion_arcProp(_radiusInner, _endAngle)
{}
double MeshFaceExtrusionArc::spacing() {
	return endAngle / (double)(nElements);
}
void MeshFaceExtrusionArc::writeNodes(ExtrudeStepData* _curExtrData) {

	ExtrudeFaceStepData* curExtrData = (ExtrudeFaceStepData*)_curExtrData;

	curExtrData->pos = glm::dvec3(0., 0., radius);

	ArcAngles ang;
	ang.setStart(-curExtrData->startSpace - GLMPI);
	ang.setEnd(-(GLMPI + endAngle));

	Cone3Dmesher::writeNodes(
		curExtrData->pos,
		curExtrData->csys,
		meshDens,
		Pipe3Dradius(
			radius, radius - curExtrData->sizeYZ[1],//curFextr->radiusOuter, 
			radius, radius - curExtrData->sizeYZ[1]),//curFextr->radiusOuter),
		ang, curExtrData->sizeYZ[0], direction::y);

	curExtrData->arcAngle += endAngle;
	curExtrData->csys.moveInLocalCsys(coordsOnCircle(ang.end, radius, direction::y) + glm::dvec3(0, 0, radius));
	(*curExtrData->csys.csys) = makeCsysMatrix(Y_DIR, curExtrData->arcAngle);
}

/*!
	This offset allows to reuse the extrusion with different nodeIDs
	The offset should normally be equal to the firstNodeID of its Mesh
*/
void MeshEdgeExtrusion::setNodeOffset(int nOffs) {
	for (MeshEdge& edg : edges)
		edg.nodeIter->nodeIDoffset = nOffs;
}
void MeshFaceExtrusion::setNodeOffset(int nOffs) {
	for (MeshFace& face : faces)
		face.nodeIter.nodeIDoffset = nOffs;
}

void MeshEdgeExtrusion::addToFirstNodeID(int n) {
	for (MeshEdge& edg : edges)
		edg.nodeIter->firstNodeID += n;
}
void MeshFaceExtrusion::addToFirstNodeID(int n) {
	for (MeshFace& face : faces)
		face.nodeIter.firstNodeID += n;
	for (NodeIterator1D& nIt : endEdgeIterators)
		nIt.firstNodeID += n;
}

int MeshEdgeExtrusion::endCornerNode1() {
	return edges[2].startNode();
}
int MeshEdgeExtrusion::endCornerNode2() {
	return edges[2].endNode();
}



/*
   ^ Z-dir
   |
   x-----2----x
   |		  |
   3		  1
   |          |
   x-----0----x ----> Y-dir
        
		x X-dir

*/
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator0(){
	return endEdgeIterators[0];
}
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator1() {
	return endEdgeIterators[1];
}
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator2(){
	return endEdgeIterators[2];
}
NodeIterator1D MeshFaceExtrusion::getEndEdgeIterator3(){
	return endEdgeIterators[3];
}


void MeshFaceExtrusion_noRef::initFaces(
	const MeshDensity2D&	face0nodes, 
	int						firstNodeID, 
	MeshFaceExtrusion*		prevExtr) 
{
	int nExtr = nNodes();

	meshDens = MeshDensity3D(nExtr, face0nodes.dir1(), face0nodes.dir2());

	NodeIterator1D prevIterEdg0, prevIterEdg1, prevIterEdg2, prevIterEdg3;
	if (prevExtr) {
		prevIterEdg0 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator0();
		prevIterEdg1 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator1();
		prevIterEdg2 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator2();
		prevIterEdg3 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator3();
	}


	faces[6] = MeshFace(meshDens.faceNodeIterator(0, firstNodeID));
	faces[1] = MeshFace(meshDens.faceNodeIterator(1, firstNodeID, prevIterEdg1));
	faces[2] = MeshFace(meshDens.faceNodeIterator(2, firstNodeID));
	faces[3] = MeshFace(meshDens.faceNodeIterator(3, firstNodeID, prevIterEdg3));
	faces[4] = MeshFace(meshDens.faceNodeIterator(4, firstNodeID, prevIterEdg0));
	faces[5] = MeshFace(meshDens.faceNodeIterator(5, firstNodeID, prevIterEdg2));
	
	if (isStart()) {
		faces[0] = faces[6];
	}
	else {
		faces[0] = ((MeshFaceExtrusion*)prevExtr)->faces[2];
	}

	int nD1 = face0nodes.dir1();
	int nD2 = face0nodes.dir2();
	int nFace4 = nExtr * nD1;
	endEdgeIterators[0] = NodeIterator1D(firstNodeID + nExtr - 1, nD1, nExtr);
	endEdgeIterators[1] = NodeIterator1D(firstNodeID + nExtr - 1, nD2, nFace4);
	endEdgeIterators[2] = NodeIterator1D(endEdgeIterators[1].last(), nD1, nExtr);
	endEdgeIterators[3] = NodeIterator1D(endEdgeIterators[0].last(), nD2, nFace4);

}


void MeshFaceExtrusion_ref::initFaces(const MeshDensity2D& face0nodes, int firstNodeID, MeshFaceExtrusion* prevExtr) {

	int nExtr = nNodes();

	meshDens = MeshDensity3Dref(nRef, face0nodes.dir1(), face0nodes.dir2());

	NodeIterator1D prevIterEdg0, prevIterEdg1, prevIterEdg2, prevIterEdg3;
	if (prevExtr) {
		prevIterEdg0 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator0();
		prevIterEdg1 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator1();
		prevIterEdg2 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator2();
		prevIterEdg3 = ((MeshFaceExtrusion*)prevExtr)->getEndEdgeIterator3();
	}


	faces[6] = MeshFace(meshDens.faceNodeIterator(0, firstNodeID));
	faces[1] = MeshFace(meshDens.faceNodeIteratorRefDir(1, firstNodeID, prevIterEdg1));
	faces[2] = MeshFace(meshDens.faceNodeIterator(2, firstNodeID));
	faces[3] = MeshFace(meshDens.faceNodeIteratorRefDir(3, firstNodeID, prevIterEdg3));
	faces[4] = MeshFace(meshDens.faceNodeIteratorRefDir(4, firstNodeID, prevIterEdg0));
	faces[5] = MeshFace(meshDens.faceNodeIteratorRefDir(5, firstNodeID, prevIterEdg2));

	if (isStart()) {
		faces[0] = faces[6];
	}
	else {
		faces[0] = ((MeshFaceExtrusion*)prevExtr)->faces[2];
	}

	int nD1 = face0nodes.dir1();
	int nD2 = face0nodes.dir2();
	int nFace4 = nExtr * nD1;
	endEdgeIterators[0] = NodeIterator1D(firstNodeID + nExtr - 1, nD1, nExtr);
	endEdgeIterators[1] = NodeIterator1D(firstNodeID + nExtr - 1, nD2, nFace4);
	endEdgeIterators[2] = NodeIterator1D(endEdgeIterators[1].last(), nD1, nExtr);
	endEdgeIterators[3] = NodeIterator1D(endEdgeIterators[0].last(), nD2, nFace4);
}


/************************************************
	MeshFaceExtrusionLinearRef
************************************************/
MeshFaceExtrusionLinearRef::MeshFaceExtrusionLinearRef(
	double				 _length,
	int					 _nRef,
	const MeshDensity2D& face0nodes,
	int					 firstNodeID,
	MeshFaceExtrusion*	 previousExtrusion) :
	MeshFaceExtrusion_ref(_nRef, face0nodes, firstNodeID, previousExtrusion), MeshExtrusion_linearProp(_length){}

double MeshFaceExtrusionLinearRef::spacing() {
	return -1.;
}
void MeshFaceExtrusionLinearRef::writeNodes(ExtrudeStepData* curExtrData) {
	curExtrData->pos = glm::dvec3(curExtrData->startSpace, 0., 0.);
	CuboidMesherRef::writeNodes(
		curExtrData->pos,
		curExtrData->csys,
		meshDens,
		//((ExtrudeFaceStepData*)curExtrData)->dxyz, 
		glm::dvec3(((ExtrudeFaceStepData*)curExtrData)->sizeYZ, length),
		false, 
		plane::yz);

	curExtrData->csys.moveInLocalCsys(glm::dvec3(length, 0., 0.));

}

/************************************************
	MeshFaceExtrusionArcRef
************************************************/
MeshFaceExtrusionArcRef::MeshFaceExtrusionArcRef(
	double				 _radiusInner,
	double				 _endAngle,
	int					 _nRef,
	const MeshDensity2D& face0nodes,
	int					 firstNodeID,
	MeshFaceExtrusion*	 previousExtrusion) :
	MeshFaceExtrusion_ref(_nRef, face0nodes, firstNodeID, previousExtrusion), MeshExtrusion_arcProp(_radiusInner, _endAngle)
{}

double MeshFaceExtrusionArcRef::spacing() {
	return -1.0;
}
void MeshFaceExtrusionArcRef::writeNodes(ExtrudeStepData* curStepData) {}