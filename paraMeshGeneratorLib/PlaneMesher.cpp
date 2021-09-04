#include "PlaneMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"

void PlaneMesher::writeNodesQ(
	MeshCsys&				spos,
	const MeshDensity2D&	meshDens,
	const glm::dvec2&		dsize,
	plane					pln,
	const glm::dvec3&		pos)
{
	glm::dvec3 curPos(pos);

	int firstNode = writer->getNextNodeID();

	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	for (int i2 = 0; i2 < meshDens.dir2(); i2++){
		LineMesher::writeNodesLineQ(spos, meshDens.dir1(), dsize.x, dir1, curPos);
		curPos[(size_t)dir2] += dsize.y;
	}
	Mesher::nodeID1 = firstNode;
}

/*

	- plane pln defines dir1-dir2
	- skipNth defines the nth rows to skip in direction dir1 or dir2
	- if skipAlongDir1 == true,  the dir2 rows are skipped along dir1
	- if skipAlongDir1 == false, the dir1 rows are skipped along dir2
	- NOTE: first row is always skipped!
	- Nodes numbers are incremented along the direction that does not skip (see examples)

	- Example skipAlongDir1 == true and skipNth == 4:
    - MeshDensity2D with dir1 nodes == 10, dir2 nodes == 5

	   row0 row1 r2 r3  r4  r5  r6  r7  r8 row9
		 ....x5__x10_x___.___x___x___x30_.___x35
         :   |   |   |   :   |   |   |   :   |
         :...x4__x___x___:___x___x___x___:___x
         :   |   |   |   :   |   |   |   :   |
         :...x3__x___x___:___x___x___x___:___x
         :   |   |   |   :   |   |   |   :   |   
  ^      :...x2__x7__x___:___x___x___x___:___x32 
  |      :   |   |   |   :   |   |   |   :   |   
  |      :...x1__x6__x11_:___x16_x___x___:___x31 
 dir2    
 dir1 ---->

	- Example skipAlongDir1 == false and skipNth == 4:
	- MeshDensity2D with dir1 nodes == 10, dir2 nodes == 5

		 .....................................    row 4
		 :   :   :   :   :   :   :   :   :   :    
		 x21_x22_x___x___x___x___x___x___x___x30  row 3
		 |   |   |   |   |   |   |   |   |   |    
		 x11_x12_x___x___x___x___x___x___x___x    row 2
		 |   |   |   |   |   |   |   |   |   |    
  ^      x1__x2__x3__x4__x___x___x___x___x___x10  row 1
  |      :   :   :   :   :   :   :   :   :   :    
  |      :...:...:...:...:...:...:...:...:...:    row 0
 dir2
 dir1 ---->

*/
void PlaneMesher::writeNodesQ_nth(
	MeshCsys&		 spos,
	const MeshDensity2D& meshDens,
	const glm::dvec2&	 dsize,
	plane				 pln,
	int					 skipNth,
	bool				 skipAlongDir1)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(&spos);
	
	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	int nRowSkip = skipAlongDir1 ? meshDens.dir1() : meshDens.dir2();
	int nRowNotSkip = skipAlongDir1 ? meshDens.dir2() : meshDens.dir1();
	direction skipDir    = skipAlongDir1 ? dir1 : dir2;
	direction nonSkipDir = skipAlongDir1 ? dir2 : dir1;
	

	for (int i = 0; i < nRowSkip; i++) {
		curPos.pos[(size_t)skipDir] = spos.pos[(size_t)skipDir] + (double)i*dsize[(size_t)skipDir];
		curPos.update();
		if (i % 4) {
			LineMesher::writeNodesLineQ(curPos, nRowNotSkip, dsize[(size_t)nonSkipDir], nonSkipDir);
		}
	}
	Mesher::nodeID1 = firstNode;
}

void PlaneMesher::writeNodes(
	MeshCsys&		 spos,
	const MeshDensity2D& meshDens,
	const glm::dvec2&	 size,
	plane				 pln)
{
	glm::dvec2 dsize = glm::dvec2(size.x / (double)meshDens.nElDir1(), size.y / (double)meshDens.nElDir2());
	PlaneMesher::writeNodesQ(spos, meshDens, dsize,  pln);
}

void PlaneMesher::writeNodesXZq(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxz) {
	writeNodesQ(spos, meshDens, dxz, plane::xz);
}
void PlaneMesher::writeNodesXYq(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxy) {
	writeNodesQ(spos, meshDens, dxy, plane::xy);
}
void PlaneMesher::writeNodesYZq(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dyz) {
	writeNodesQ(spos, meshDens, dyz, plane::yz);
}
void PlaneMesher::writeNodesXZ(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	writeNodes(spos, meshDens, size, plane::xz);
}
void PlaneMesher::writeNodesXY(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	writeNodes(spos, meshDens, size, plane::xy);
}
void PlaneMesher::writeNodesYZ(MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	writeNodes(spos, meshDens, size, plane::yz);
}



/*

  5x_10x_15x___x  nnodes.x = 5 (nElx = 4)
   |   |   |   |  nnodes.y = 4 (nEly = 3)
  4x__9x_14x___x
   |   |   |   |
  3x__8x_13x___x
   |   |   |   |    ^
  2x__7x_12x___x    |
   |   |   |   |    |
  1x__6x_11x___x    | Xdir


  IF closed loop

  5x_10x_15x___x    nnodes.x = 5 (nElx = 5)
   |   |   |   |    nnodes.y = 4 (nEly = 3)
  4x__9x_14x___x
   |   |   |   |
  3x__8x_13x___x
   |   |   |   |
  2x__7x_12x___x
   |   |   |   |
  1x__6x_11x___x
   |   |   |   |
  5x_10x_15x___x
*/
void PlaneMesher::writeElements(const MeshDensity2D& meshDens)
{
	int n[4];
	int c = nodeID1;

	int elementProp = 1;

	for (int iy = 0; iy < meshDens.nElDir2(); iy++) {
		writer->setCurrentElementPropertyID(elementProp++);
		for (int ix = 0; ix < meshDens.nElDir1(); ix++) {
			n[0] = c++;
			n[1] = n[0] + 1;
			n[2] = n[1] + meshDens.dir1();
			n[3] = n[2] - 1;

			if (meshDens.closedLoop && ix == (meshDens.nElDir1() - 1)) {
				n[1] -= meshDens.dir1();
				n[2] -= meshDens.dir1();
			}

			writer->write4nodedShell(n);
		}
		if (!meshDens.closedLoop) c++;
	}
}



/*
	 x_______________x_______________x                 -
	 |               |               |				   |
	 |				 |				 |				   |
	 |				 |				 |				   |  4el
	 |				 |				 |				   |
	 |				 |				 |                 |
	 x_______________x_______________x				   -
	 |\              |              /|				   |  2elL
	 |  \ 			 |		      /	 |				   |
	 |	   \ x_______x_______x  / 	 |				   -
	 |       |       |       |		 |				   |  2elL
	 |	     |       |       |       |                 |
	 x_______x_______x_______x_______x                 -        ^
	 |       |       |       |       |				   |  2elL  |
	 |       |       |       |       |				   |		|
t0   x_______x_______x_______x_______x  row t (top)	   -		|
	 | \     |     / |  \    |     / |  elRow t		   |  1elL  |
	 |m0 x___x___x/	 |   x___x___x	 |  row m (mid)	   -     	|  L
	 |   |   |   |   |   |   |   |   |	elRow m		   |  1elL	|		  ^
b0   x___x___x___x___x___x___x___x___x  row b (bot)	   -		|		  |
	 |   |   |   |   |   |   |   |   |	elRow b		   |  1elL  |         | X
	 x___x___x___x___x___x___x___x___x ref2			   -	    -         -
	 |   |   |   |   |   |   |   |   |
	 x___x___x___x___x___x___x___x___x ref1


	------------------------------------------------------
   | First row starts at x = elSize
   |------------------------------------------------------
   | 			nRefinements --->
   |  L =  elL * (3 + 6 + 12 + 24 + ...)
   |  L = 3elL * (1 + 2 +  4 +  8 + ...)
   |  L = 3elL * sum(2^k) (k = 0 -> nRefinements - 1)
   |
   |  L =  3elL * (1 - 2^nRef) / (1 - 2)
   |  L = -3elL * (1 - 2^nRef)
   |
   |   L = 3elL * (2^nRef - 1)
   | elL = L / (4 * (2^nRef - 1))
   |
   |------------------------------------------------------
   | First row starts at distance = 0.0
   |------------------------------------------------------
   |
   | 			nRefinements --->
   |  L =   elL * ((3-1) + 6 + 12 + 24 + ...)
   |  L =   elL * (6 + 12 + 24 + ...) + 2elL
   |  L =  6elL * sum(2^k) (k = 0 -> nRefinements - 2) + 2elL
   |  L =  6elL * (2^(nRef-1) - 1) + 2elL
   |  L =  6elL * 2^(nRef-1) - 6elL + 2elL
   |  L =  6elL * 2^(nRef-1) - 4elL
   |  L =   elL * (6*2^(nRef-1) - 4)
   |
   |
   |   L = elL * (6 * 2^(nRef-1) - 4)   vs   L =  3elL * (2^nRef - 1)
   | elL =   L / (6 * 2^(nRef-1) - 4)   vs elL = L / (4 * (2^nRef - 1))
   |
   |------------------------------------------------------

*/
void PlaneMesherRef::writeNodes(
	MeshCsys&			spos,
	const MeshDensity2Dref& meshDens,
	const glm::dvec2&		size,
	bool					startWithOffset,
	plane					pln)
{
	int firstNode = writer->getNextNodeID();

	RefShapeData rsData;
	rsData.meshDens = &meshDens;
	getPlaneDirections(pln, rsData.refDir, rsData.edgeDir);
	
	RefLayerData rlData;
	rlData.curPos = spos;	
	double elSizeRefDir = initialRefElSize2D(size.x, meshDens.nRefs(), startWithOffset);
	rlData.curElSize = glm::dvec2(elSizeRefDir, size.y / (double)meshDens.nElCirc());

	if (startWithOffset) {
		rlData.curPos.pos[(size_t)rsData.refDir] += rlData.curElSize.x;
	}

	for(int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++){
		writeNodes_layerB(rsData, rlData, refLayer);
		writeNodes_layerM(rsData, rlData, refLayer);
		writeNodes_layerT(rsData, rlData, refLayer);
	}

	Mesher::nodeID1 = firstNode;
}

//row b: x--x--x--x--x--x--x--x--x
void PlaneMesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	LineMesher::writeNodesLineQ(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer), rlData.curElSize.y, rsData.edgeDir);
	rlData.curPos.pos[(size_t)rsData.refDir] += rlData.curElSize.x;
}
//row m:  |  x--x--x  |  x--x--x  |
void PlaneMesherRef::writeNodes_layerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	LineMesher::writeNodesLineQ_nth(rlData.curPos, rsData.meshDens->nNodesRowB(refLayer), rlData.curElSize.y, 4, rsData.edgeDir);
	rlData.curPos.pos[(size_t)rsData.refDir] += rlData.curElSize.x;
}
//row t: x----x----x----x----x
void PlaneMesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer) {
	rlData.curElSize.y *= 2.0;
	LineMesher::writeNodesLineQ(rlData.curPos, rsData.meshDens->nNodesRowT(refLayer), rlData.curElSize.y, rsData.edgeDir);
	rlData.curElSize.x *= 2.0;
	rlData.curPos.pos[(size_t)rsData.refDir] += rlData.curElSize.x;
}


void PlaneMesherRef::writeNodesXY(MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodes(spos, meshDens, size, startWithOffset, plane::xy);
}
void PlaneMesherRef::writeNodesXZ(MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodes(spos, meshDens, size, startWithOffset, plane::xz);
}
void PlaneMesherRef::writeNodesYZ(MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodes(spos, meshDens, size, startWithOffset, plane::yz);
}


/*
	 x_______________x_______________x                 -
	 |               |               |				   |
	 |				 |				 |				   |
	 |				 |				 |				   |  4el
	 |				 |				 |				   |
	 |				 |				 |                 |
	 x_______________x_______________x				   -
	 |\              |              /|				   |  2elL
	 |  \ 			 |		      /	 |  elRow t        |
	 |	   \ x_______x_______x  / 	 |				   -
	 |       |       |       |		 |				   |  2elL
	 |	     |       |       |       |  elRow m        |
	 x_______x_______x_______x_______x                 -
	 |       |       |       |       |				   |  2elL
	 |       |       |       |       |	elRow b		   |
t0   x_______x_______x_______x_______x             	   -
	 | \     |     / |  \    |     / |  elRow t		   |  1elL
	 |m0 x___x___x/	 |   x___x___x	 |                 -
	 |   |   |   |   |   |   |   |   |	elRow m		   |  1elL
b0   x___x___x___x___x___x___x___x___x  	           -
										elRow b		   |  1elL
*/
void PlaneMesherRef::writeElements(const MeshDensity2Dref& meshDens)
{
	int c = nodeID1;
	
	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {

		if (refLayer != 0) {
			writer->setCurrentElementPropertyID(1);
			writeElements_layerB(meshDens, c, refLayer);
		}
		writer->setCurrentElementPropertyID(2);
		writeElements_layersMT(meshDens, c, refLayer);

	}
}

void PlaneMesherRef::writeElements_layerB(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer){
	int n[4];
	int nElrowB    = meshDens.nElRowB(curRefLayer);
	int nNodesRowB = meshDens.nNodesRowB(curRefLayer);

	for (int i = 0; i < nElrowB; i++) {
		n[0] = nid1++;								//  3x------x2
		n[1] = n[0] + 1;							//   |      |
		n[2] = n[1] + nNodesRowB;			//   |      |
		n[3] = n[2] - 1;							//  0x------x1

		if (meshDens.closedLoop && i == (nElrowB - 1)) {
			n[1] -= nNodesRowB;
			n[2] -= nNodesRowB;
		}
		writer->write4nodedShell(n);
	}
	if (!meshDens.closedLoop) nid1++;

}
void PlaneMesherRef::writeElements_layersMT(const MeshDensity2Dref& meshDens, int& nid1, int curRefLayer) {

	int nElRowB		= meshDens.nElRowB(curRefLayer);
	int nNodesRowB	= meshDens.nNodesRowB(curRefLayer);
	int nnodesRowM  = meshDens.nNodesRowM(curRefLayer);
	int nnodesRowT  = meshDens.nNodesRowT(curRefLayer);

	int b0, m0, t0;
	int b[5], m[3], t[3];
	int* elNodes[6];

	for (int i = 0; i < nElRowB; i += 4) {

		b0 = nid1;
		m0 = b0 + nNodesRowB - i / 4;
		t0 = m0 + nnodesRowM - i / 4;
		for (int j = 0; j < 5; j++) b[j] = b0 + j;
		for (int j = 0; j < 3; j++) m[j] = m0 + j;
		for (int j = 0; j < 3; j++) t[j] = t0 + j;

		if (meshDens.closedLoop && i == (nElRowB - 4)) {
			b[4] -= nNodesRowB;
			t[2] -= nnodesRowT;
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

	nid1 += (nnodesRowM + 1);
	if (meshDens.closedLoop) nid1--;

}