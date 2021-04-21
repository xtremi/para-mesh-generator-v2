#include "RecTubeMesher.h"
#include "RecEdgeMesher.h"
#include "PlaneMesher.h"

/*
Creates nodes in a rectangular shape.
Size is set by RecTubeSize (inner and outer rectangle sizes)
A layer of nodes is created for each dir2/norm nodes in MeshDensity2D meshDens.
The inner layer of nodes has dir1/circ number of nodes.
For each layer of nodes, the number of in the layer is incremented by 8.
To have equaly spaced elements in dir1 and dir2, the outer rectangle size
can be set by RecTubeSize::setOuterSize().

Example with nNodesInner = 32, nLayers = 8 (MeshDens = MeshDensity2D(32, 8))

  73x---x---x---x---x---x---x---x---x---x---x---x---x
    |   |   |   |   |   |   |   |   |   |   |   |   |
 120x---x33-x---x---x---x---x---x---x---x---x---x---x
    |   |   |   |   |   |   |   |   |   |   |   |   |
    x---x72-x1--x2--x3--x4--x5--x6--x7--x8--x9--x---x
    |   |   |                               |   |   |
    x---x---x32                           10x---x---x     
    |   |   |                               |   |   |
    x---x---x					          11x---x---x
    |   |   |                               |   |   |
    x---x---x					            x---x---x
    |   |   |                               |   |   |
    x---x---x					            x---x---x
    |   |   |                               |   |   |
    x---x---x					            x---x---x
    |   |   |                               |   |   |
    x---x---x					            x---x---x
    |   |   |                               |   |   |
    x---x---x26					            x---x---x
    |   |   |                               |   |   |
    x---x---x25-x---x---x---x---x---x---x18-x17-x---x
    |   |   |   |   |   |   |   |   |   |   |   |   |
    x---x---x---x---x---x---x---x---x---x---x---x---x
    |   |   |   |   |   |   |   |   |   |   |   |   |
    x---x---x---x---x---x---x---x---x---x---x---x---x

*/
void RecTubeMesher::writeNodes(
	const MeshCsys&				spos,
	const MeshDensity2DrecTube& meshDens,
	const RecTubeSize&			size,
	plane						pln)
{
	int firstNode = writer->getNextNodeID();

	glm::dvec2 curSize = size.inner;
	glm::dvec2 dSize = (size.outer - size.inner) / (double)(meshDens.nElNorm());
	//int nnodes = meshDens.circ();
	
	for (int i2 = 0; i2 < meshDens.norm(); i2++) {
		int nNodesW = meshDens.nNodesWidth(i2);
		int nNodesH = meshDens.nNodesHeight(i2);

		RecEdgeMesher::writeNodes(spos, nNodesW, nNodesH, curSize, pln);
		//nnodes += 8;
		curSize += dSize;
	}

	Mesher::nodeID1 = firstNode;
}

void RecTubeMesher::writeNodes2(
	const MeshCsys&				spos,
	const MeshDensity2DrecTube& meshDens,
	const RecTubeSize&			size,
	plane						pln) 
{
	/*Rectangle rec(size.inner);
	int nElW, nElH;
	rec.elementsPerSides(meshDens.circ(), nElW, nElH);
	writeNodes2(spos, nElW, nElH, meshDens.norm(), size, pln);*/

	int firstNode = writer->getNextNodeID();

	glm::dvec2 curSize = size.inner;
	glm::dvec2 dSize = (size.outer - size.inner) / (double)meshDens.nElNorm();

	for (int i2 = 0; i2 < meshDens.norm(); i2++) {
		RecEdgeMesher::writeNodes(spos, meshDens.nNodesWidth(0), meshDens.nNodesHeight(0), curSize, pln);
		curSize += dSize;
	}

	Mesher::nodeID1 = firstNode;

}
/*
void RecTubeMesher::writeNodes2(
	const MeshCsys&		spos,
	int					nNodesWidth,
	int					nNodesHeight,
	int					nLayers,
	const RecTubeSize&	size,
	plane				pln) 
{
	int firstNode = writer->getNextNodeID();

	glm::dvec2 curSize = size.inner;
	glm::dvec2 dSize   = (size.outer - size.inner) / (double)(nLayers - 1);
	
	for (int i2 = 0; i2 < nLayers; i2++) {
		RecEdgeMesher::writeNodes(spos, nNodesWidth, nNodesHeight, curSize, pln);
		curSize += dSize;
	}

	Mesher::nodeID1 = firstNode;
}*/

void RecTubeMesher::writeElements2(const MeshDensity2DrecTube& meshDens) {
	PlaneMesher::writeElements(meshDens);
}

/*
	Write elements from nodes created with RecTubeMesher::writeNodes()
*/
void RecTubeMesher::writeElements(const MeshDensity2DrecTube& meshDens){
	int n[4];
	int c = nodeID1;

	int elementProp = 1;
	int nNodesPerimeter; 
	int nNodesPerimeterNext; 
	int nCornerOffset = -1;					//For every corner we need an additional offset

	//Corner nodes (only correct if equal node density on edges)
	int cornerNodes[4];
	int currentCorner = 0; // 0,1,2,3

	for (int iy = 0; iy < meshDens.nElNorm(); iy++) {
		writer->setCurrentElementPropertyID(elementProp++);

		nNodesPerimeter = meshDens.nNodePerimeter(iy);
		nNodesPerimeterNext = meshDens.nNodePerimeter(iy + 1);
		meshDens.cornerNodes(cornerNodes, iy);

		for (int ix = 0; ix < nNodesPerimeter; ix++) {
			
			if (ix == cornerNodes[currentCorner]) {
				nCornerOffset += 2;				
				n[0] = c; n[1] = n[0] + nNodesPerimeter + nCornerOffset;
				n[2] = n[1] - 1; n[3] = n[2] - 1;
				if (currentCorner == 0) {
					n[3] += nNodesPerimeterNext;
				}
				writer->write4nodedShell(n);
				currentCorner++;
			}
			
			n[0] = c++;
			n[1] = n[0] + 1;
			n[2] = n[1] + nNodesPerimeter + nCornerOffset;
			n[3] = n[2] - 1;

			if (meshDens.closedLoop && ix == (nNodesPerimeter - 1)) {
				n[1] -= nNodesPerimeter;
			}

			writer->write4nodedShell(n);
		}

		nCornerOffset = -1;
		currentCorner = 0;
		if (!meshDens.closedLoop) c++;
	}
}





void RecTubeMesherRef::writeNodes(
	const MeshCsys&		spos,
	const MeshDensity2Dref&	meshDens,
	int					nRefs,
	const RecTubeSize&	size,
	plane				pln)
{}

void RecTubeMesherRef::writeNodes2(
	const MeshCsys&		spos,
	const MeshDensity2Dref&	meshDens,
	const RecTubeSize&	size,
	plane				pln)
{
	Rectangle rec(size.inner);
	int nElW, nElH;
	rec.elementsPerSides(meshDens.circ(), nElW, nElH);
	if ((2 * nElW + 2 * nElH) != meshDens.circ()) {
		int notMatching = 1;
	}

	writeNodes2(spos, meshDens, nElW, nElH, size, pln);
}

void RecTubeMesherRef::writeNodes2(
	const MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	int						nNodesWidth,
	int						nNodesHeight,
	const RecTubeSize&		size,
	plane					pln)
{
	int firstNode = writer->getNextNodeID();

	RefShapeData rsData;
	rsData.meshDens = &meshDens;
	rsData.size = &size;
	//rsData.height = height;
	rsData.pln = pln;
	rsData.length = 0.5*(size.outer.x - size.inner.x); //std::sqrt(pow2(radius.dR()) + pow2(height));

	RefLayerData rlData;
	rlData.curPos = spos;
	rlData.curElSize.x = initialRefElSize2D(rsData.length, meshDens.nRefs(), false);
	rlData.curSize = size.inner;
	rlData.curLength = 0.0;

	//double	arcLengthStart = angle.angleSize() * radius.start();
	//rlData.curElSize.y = arcLengthStart / (double)meshDens.nElCirc();

	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		writeNodes_layerB(rsData, rlData, refLayer);
		writeNodes_layerM(rsData, rlData, refLayer);
		writeNodes_layerT(rsData, rlData, refLayer);
	}


	Mesher::nodeID1 = firstNode;

}

void RecTubeMesherRef::incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData){
	rlData.curLength += rlData.curElSize.x;
	rlData.curSize += 2.*glm::dvec2(rlData.curElSize.x);
}
void RecTubeMesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	RecEdgeMesher::writeNodes(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer), rlData.curSize, rsData.pln);
	incrementConeStep(rsData, rlData);
}
void RecTubeMesherRef::writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	RecEdgeMesher::getOrWriteCoords_nth(MesherMode::write, rlData.curPos, RecEdgeMesher::default_empty_coord_vec, 
		rsData.meshDens->nNodesRowB(refLayer), rlData.curSize, 4, false, rsData.pln);
	incrementConeStep(rsData, rlData);
}
void RecTubeMesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	//rlData.curElSize.y *= 2.0;
	RecEdgeMesher::writeNodes(rlData.curPos, rsData.meshDens->nNodesRowT(refLayer), rlData.curSize, rsData.pln);
	rlData.curElSize.x *= 2.0;
	incrementConeStep(rsData, rlData);
}