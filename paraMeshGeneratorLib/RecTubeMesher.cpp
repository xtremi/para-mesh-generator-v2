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
	double						height,
	plane						pln) 
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);

	direction heightAxis = getPlaneNormal(pln);

	glm::dvec2 curSize = size.inner;
	glm::dvec2 dSize = (size.outer - size.inner) / (double)meshDens.nElNorm();
	double dH = height / (double)meshDens.nElNorm();

	for (int i2 = 0; i2 < meshDens.norm(); i2++) {
		RecEdgeMesher::writeNodes(curPos, meshDens.nNodesWidth(0), meshDens.nNodesHeight(0), curSize, pln);
		curPos.pos[(size_t)heightAxis] += dH;
		curSize += dSize;
	}

	Mesher::nodeID1 = firstNode;

}


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
	const MeshCsys&			spos,
	const glm::ivec2&		nNodesWH,
	int						nRefs,
	const glm::dvec2&		recSizeInner,
	plane					pln)
{
	int firstNode = writer->getNextNodeID();

	RefShapeData rsData;
	rsData.pln = pln;

	RefLayerData rlData;
	rlData.curRecSize = recSizeInner;
	rlData.curElSizeWH = rlData.curRecSize / glm::dvec2((double)nNodesWH.x, (double)nNodesWH.y);
	rlData.curPos = MeshCsys(spos);
	rlData.curNodesWH = nNodesWH;


	for (int curRef = 0; curRef < nRefs; curRef++) {
		writeNodes_layerB(rsData, rlData, curRef);
		writeNodes_layerM(rsData, rlData, curRef);
		writeNodes_layerT(rsData, rlData, curRef);
	}

	Mesher::nodeID1 = firstNode;
}

void RecTubeMesherRef::incrementStep(const RefShapeData& rsData, RefLayerData& rlData){
	rlData.curRecSize += 2.*rlData.curElSizeWH;
	rlData.curNodesWH += 2;
}
void RecTubeMesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	RecEdgeMesher::writeNodes(rlData.curPos, rlData.curNodesWH.x, rlData.curNodesWH.y, rlData.curRecSize, rsData.pln);
	incrementStep(rsData, rlData);
}
void RecTubeMesherRef::writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	RecEdgeMesher::writeNodes_nth(rlData.curPos, rlData.curNodesWH.x, rlData.curNodesWH.y, rlData.curRecSize, 4, rsData.pln);
	incrementStep(rsData, rlData);

	rlData.curNodesWH /= 2;
	rlData.curElSizeWH = rlData.curRecSize / glm::dvec2((double)rlData.curNodesWH.x, (double)rlData.curNodesWH.y);
}
void RecTubeMesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	RecEdgeMesher::writeNodes(rlData.curPos, rlData.curNodesWH.x, rlData.curNodesWH.y, rlData.curRecSize, rsData.pln);
	incrementStep(rsData, rlData);
}

void RecTubeMesherRef::writeNodes2(
	const MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	const RecTubeSize&		size,
	double					height,
	plane					pln)
{
	Rectangle rec(size.inner);
	int nElW, nElH;
	rec.elementsPerSides(meshDens.circ(), nElW, nElH);
	writeNodes2(spos, meshDens, glm::ivec2(nElW, nElH), size, height, pln);
}

void RecTubeMesherRef::writeNodes2(
	const MeshCsys&			spos,
	const MeshDensity2Dref&	meshDens,
	const glm::ivec2&		nNodesWidthHeight,
	const RecTubeSize&		size,
	double					height,
	plane					pln)
{
	int firstNode = writer->getNextNodeID();

	RefShapeData2 rsData;
	rsData.meshDens = &meshDens;
	rsData.size		= &size;
	rsData.height	= height;
	rsData.pln		= pln;
	rsData.upAxis	= getPlaneNormal(pln);
	rsData.length	= std::sqrt(pow2(0.5*(size.outer.x - size.inner.x)) + pow2(height)); //std::sqrt(pow2(radius.dR()) + pow2(height));

	RefLayerData2 rlData;
	rlData.curPos = spos;
	rlData.curElSize.x = initialRefElSize2D(rsData.length, meshDens.nRefs(), false);
	rlData.curSize = size.inner;
	rlData.curLength = 0.0;

	//double	arcLengthStart = angle.angleSize() * radius.start();
	//rlData.curElSize.y = arcLengthStart / (double)meshDens.nElCirc();

	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		writeNodes2_layerB(rsData, rlData, refLayer);
		writeNodes2_layerM(rsData, rlData, refLayer);
		writeNodes2_layerT(rsData, rlData, refLayer);
	}


	Mesher::nodeID1 = firstNode;
}

void RecTubeMesherRef::incrementStep2(const RefShapeData2& rsData, RefLayerData2& rlData){
	rlData.curLength += rlData.curElSize.x;
	rlData.curSize += 2.*glm::dvec2(rlData.curElSize.x);

	rlData.curSize = rsData.size->inner + rsData.size->dsize() * (rlData.curLength / rsData.length);

	double curDh = (rsData.height / rsData.length) * rlData.curElSize.x;
	rlData.curPos.pos[(size_t)rsData.upAxis] += curDh;
}
void RecTubeMesherRef::writeNodes2_layerB(const RefShapeData2& rsData, RefLayerData2& rlData, int refLayer){
	RecEdgeMesher::writeNodes(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer), rlData.curSize, rsData.pln);
	incrementStep2(rsData, rlData);
}
void RecTubeMesherRef::writeNodes2_layerM(const RefShapeData2& rsData, RefLayerData2& rlData, int refLayer){
	RecEdgeMesher::getOrWriteCoords_nth(MesherMode::write, rlData.curPos, RecEdgeMesher::default_empty_coord_vec, 
		rsData.meshDens->nNodesRowB(refLayer), rlData.curSize, 4, false, rsData.pln);
	incrementStep2(rsData, rlData);
}
void RecTubeMesherRef::writeNodes2_layerT(const RefShapeData2& rsData, RefLayerData2& rlData, int refLayer){
	//rlData.curElSize.y *= 2.0;
	RecEdgeMesher::writeNodes(rlData.curPos, rsData.meshDens->nNodesRowT(refLayer), rlData.curSize, rsData.pln);
	rlData.curElSize.x *= 2.0;
	incrementStep2(rsData, rlData);
}

void RecTubeMesherRef::writeElements1(const glm::ivec2& nNodesWidthHeight, int	nRefs) {
	int c = nodeID1;

	glm::ivec2 curNodesWidthHeight(nNodesWidthHeight);

	for (int refLayer = 0; refLayer < nRefs; refLayer++) {

		if (refLayer != 0) {
			writer->setCurrentElementPropertyID(1);
			writeElements_layerB(curNodesWidthHeight, c, refLayer);
			curNodesWidthHeight += 2;
		}
		writer->setCurrentElementPropertyID(2);
		writeElements_layersMT(curNodesWidthHeight, c, refLayer);
		curNodesWidthHeight += 2;
		curNodesWidthHeight += 2;
		curNodesWidthHeight /= 2;
	}
}


void RecTubeMesherRef::writeElements_layerB(const glm::ivec2& nNodesWidthHeight, int& nid1, int curRefLayer) {
	MeshDensity2DrecTube meshDens(nNodesWidthHeight.x, nNodesWidthHeight.y, 2);
	Mesher::nodeID1 = nid1;
	RecTubeMesher::writeElements(meshDens);
	nid1 += meshDens.nNodePerimeter(0);
}
void RecTubeMesherRef::writeElements_layersMT(const glm::ivec2& nNodesWidthHeight, int& nid1, int curRefLayer) {

	MeshDensity2DrecTube meshDens(nNodesWidthHeight.x, nNodesWidthHeight.y, 0);
	int nNodesRowB = meshDens.nNodePerimeter(0);
	int nNodesRowM = 3 * nNodesRowB / 4;
	int nNodesRowT = meshDens.nNodePerimeter(2) / 2;

	int b0, m0, t0;
	int b[5], m[3], t[3];
	int* elNodes[6];

	int cornerNodes[4];
	meshDens.cornerNodes(cornerNodes, 0);
	int currentCorner = 0; // 0,1,2,3
	int nCornerOffsets[4];
	nCornerOffsets[0] = 1 + nNodesRowB + nNodesRowM;
	nCornerOffsets[1] = 1 + nCornerOffsets[0] - meshDens.nNodesWidth(0)  + meshDens.nNodesWidth(1) / 2;
	nCornerOffsets[2] = 1 + nCornerOffsets[1] - meshDens.nNodesHeight(0) + meshDens.nNodesHeight(1) / 2;
	nCornerOffsets[3] = 1 + nCornerOffsets[2] - meshDens.nNodesWidth(0)  + meshDens.nNodesWidth(1) / 2;

	int n[4];
	for (int i = 0; i < nNodesRowB; i += 4) {

		if (i == cornerNodes[currentCorner]) {
			//nCornerOffset += 2;
			n[0] = nid1;		n[1] = n[0] + nCornerOffsets[currentCorner];
			n[2] = n[1] - 1;	n[3] = n[2] - 1;
			if (currentCorner == 0) {
				n[3] += (nNodesRowT);
			}
			writer->write4nodedShell(n);
			currentCorner++;
		}

		b0 = nid1;
		m0 = b0 + nNodesRowB - i / 4;
		t0 = m0 + nNodesRowM - i / 4 + 1 + (currentCorner - 1)*2;
		for (int j = 0; j < 5; j++) b[j] = b0 + j;
		for (int j = 0; j < 3; j++) m[j] = m0 + j;
		for (int j = 0; j < 3; j++) t[j] = t0 + j;

		if (meshDens.closedLoop && i == (nNodesRowB - 4)) {
			b[4] -= nNodesRowB;
		}
		//  t0       t1     t2
		int n1[4] = { b[0], b[1], m[0], t[0] }; elNodes[0] = n1;    //   x_______x_______x  x_______x_______x t
		int n2[4] = { b[1], b[2], m[1], m[0] }; elNodes[1] = n2;    //   | \    5|    6/ |  |\      |      /|
		int n3[4] = { b[2], b[3], m[2], m[1] }; elNodes[2] = n3;    //   | m0x_m1x___x/m2|  |  \ ___|____ / | m
		int n4[4] = { b[3], b[4], t[2], m[2] }; elNodes[3] = n4;    //   |  1|  2|  3|  4|  |   |   |   |   |
		int n5[4] = { m[0], m[1], t[1], t[0] }; elNodes[4] = n5;    //   x___x___x___x___x  x___x___x___x___x b
		int n6[4] = { m[1], m[2], t[2], t[1] }; elNodes[5] = n6;    //  b0   b1  b2  b3  b4


		for (int i = 0; i < 6; i++) {
			writer->write4nodedShell(elNodes[i]);
		}

		nid1 += 4;
	}

	nid1 += (nNodesRowM + 1);
	if (meshDens.closedLoop) nid1--;

}

void RecTubeMesherRef::writeElements2(const MeshDensity2Dref& meshDens){
	PlaneMesherRef::writeElements(meshDens);
}