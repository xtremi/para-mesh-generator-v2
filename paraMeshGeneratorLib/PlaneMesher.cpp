#include "PlaneMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"

void PlaneMesher::writeNodesPlaneQ(
	const MeshCsys&			spos,
	const MeshDensity2D&	meshDens,
	const glm::dvec2&		dsize,
	plane					pln)
{
	MeshCsys curPos(spos);	
	int firstNode = writer->getNextNodeID();

	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	for (int i2 = 0; i2 < meshDens.dir2(); i2++){
		LineMesher::writeNodesLineQ(curPos, meshDens.dir1(), dsize.x, dir1);
		curPos.pos[(size_t)dir2] += dsize.y;
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
void PlaneMesher::writeNodesPlaneQ_nth(
	const MeshCsys&		 spos,
	const MeshDensity2D& meshDens,
	const glm::dvec2&	 dsize,
	plane				 pln,
	int					 skipNth,
	bool				 skipAlongDir1)
{
	MeshCsys curPos(spos);
	int firstNode = writer->getNextNodeID();

	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	int nRowSkip = skipAlongDir1 ? meshDens.dir1() : meshDens.dir2();
	int nRowNotSkip = skipAlongDir1 ? meshDens.dir2() : meshDens.dir1();
	direction skipDir    = skipAlongDir1 ? dir1 : dir2;
	direction nonSkipDir = skipAlongDir1 ? dir2 : dir1;
	

	for (int i = 0; i < nRowSkip; i++) {
		curPos.pos[(size_t)skipDir] = spos.pos[(size_t)skipDir] + (double)i*dsize[(size_t)skipDir];
		if (i % 4) {
			LineMesher::writeNodesLineQ(curPos, nRowNotSkip, dsize[(size_t)nonSkipDir], nonSkipDir);
		}
	}
	Mesher::nodeID1 = firstNode;
}

void PlaneMesher::writeNodesPlane(
	const MeshCsys&		 spos,
	const MeshDensity2D& meshDens,
	const glm::dvec2&	 size,
	plane				 pln)
{
	glm::dvec2 dsize = glm::dvec2(size.x / (double)meshDens.nElDir1(), size.y / (double)meshDens.nElDir2());
	PlaneMesher::writeNodesPlaneQ(spos, meshDens, dsize,  pln);
}

void PlaneMesher::writeNodesPlaneXZq(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxz) {
	writeNodesPlaneQ(spos, meshDens, dxz, plane::xz);
}
void PlaneMesher::writeNodesPlaneXYq(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dxy) {
	writeNodesPlaneQ(spos, meshDens, dxy, plane::xy);
}
void PlaneMesher::writeNodesPlaneYZq(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& dyz) {
	writeNodesPlaneQ(spos, meshDens, dyz, plane::yz);
}
void PlaneMesher::writeNodesPlaneXZ(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	writeNodesPlane(spos, meshDens, size, plane::xz);
}
void PlaneMesher::writeNodesPlaneXY(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	writeNodesPlane(spos, meshDens, size, plane::xy);
}
void PlaneMesher::writeNodesPlaneYZ(const MeshCsys& spos, const MeshDensity2D& meshDens, const glm::dvec2& size) {
	writeNodesPlane(spos, meshDens, size, plane::yz);
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
void PlaneMesher::writeElementsPlane(const MeshDensity2D& meshDens)
{
	int n[4];
	int c = nodeID1;

	for (int iy = 0; iy < meshDens.nElDir2(); iy++) {
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
	const MeshCsys&			spos,
	const MeshDensity2Dref& meshDens,
	const glm::dvec2&		size,
	bool					startWithOffset,
	plane					pln)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);

	int		currentNodesPerRow = meshDens.circ();
	int		currentRefFactor = 1;
	int		currentRefinement = 0;

	double elSizeRefDir = initialRefElSize2D(size.x, meshDens.nRefs(), startWithOffset);
	glm::dvec2 curElSize(elSizeRefDir, size.y / (double)meshDens.nElCirc());

	direction edgeDir, refDir;
	getPlaneDirections(pln, refDir, edgeDir);

	if (startWithOffset) {
		curPos.pos[(size_t)refDir] += curElSize.x;
	}

	while (currentRefinement < meshDens.nRefs()) {
		currentRefinement++;
		
		writeNodes_layerB(curPos, currentNodesPerRow, curElSize, refDir, edgeDir);
		writeNodes_layerM(curPos, currentNodesPerRow, curElSize, refDir, edgeDir);
		
		//Refine: [9 nodes / 8 elements] -> [5 nodes / 4 elements] -> [3 nodes / 2 elements]
		currentRefFactor *= 2;
		currentNodesPerRow = meshDens.nElDir2() / currentRefFactor + 1;
		
		writeNodes_layerT(curPos, currentNodesPerRow, curElSize, refDir, edgeDir);
	}

	Mesher::nodeID1 = firstNode;
}

//row b: x--x--x--x--x--x--x--x--x
void PlaneMesherRef::writeNodes_layerB(MeshCsys& curPos, int nodesPerRow, const glm::dvec2& elSize, direction refDir, direction edgeDir) {
	LineMesher::writeNodesLineQ(curPos, nodesPerRow, elSize.y, edgeDir);
	curPos.pos[(size_t)refDir] += elSize.x;
}
//row m:  |  x--x--x  |  x--x--x  |
void PlaneMesherRef::writeNodes_layerM(MeshCsys& curPos, int nodesPerRow, const glm::dvec2& elSize, direction refDir, direction edgeDir) {
	LineMesher::writeNodesLineQ_nth(curPos, nodesPerRow, elSize.y, 4, edgeDir);
	curPos.pos[(size_t)refDir] += elSize.x;
}
//row t: x----x----x----x----x
void PlaneMesherRef::writeNodes_layerT(MeshCsys& curPos, int nodesPerRow, glm::dvec2& elSize, direction refDir, direction edgeDir) {
	elSize.y *= 2.0;
	LineMesher::writeNodesLineQ(curPos, nodesPerRow, elSize.y, edgeDir);
	elSize.x *= 2.0;
	curPos.pos[(size_t)refDir] += elSize.x;
}


void PlaneMesherRef::writeNodesXY(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodes(spos, meshDens, size, startWithOffset, plane::xy);
}
void PlaneMesherRef::writeNodesXZ(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodes(spos, meshDens, size, startWithOffset, plane::xz);
}
void PlaneMesherRef::writeNodesYZ(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
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
	int currentRefFactor		= 1;
	int	currentNodesPerRow		= meshDens.dir2();
	int currentElementPerRow	= meshDens.nElDir2();

	int c = nodeID1;
	int n[4];

	for (int r = 0; r < meshDens.nRefs(); r++) {


		if (r != 0) {
			//elRow b
			for (int i = 0; i < currentElementPerRow; i++) {
				n[0] = c++;									//  3x------x2
				n[1] = n[0] + 1;							//   |      |
				n[2] = n[1] + currentNodesPerRow;			//   |      |
				n[3] = n[2] - 1;							//  0x------x1

				if (meshDens.closedLoop && i == (currentElementPerRow - 1)) {
					n[1] -= currentNodesPerRow;
					n[2] -= currentNodesPerRow;
				}
				writer->write4nodedShell(n);
			}
			if (!meshDens.closedLoop) c++;
		}


		//elRow m + t 
		int nnodesRowM = 3 * currentElementPerRow / 4;
		currentRefFactor *= 2;
		int nnodesRowT = meshDens.nElDir2() / currentRefFactor;
		if (!meshDens.closedLoop) nnodesRowT++;

		for (int i = 0; i < currentElementPerRow; i += 4) {

			int b0 = c;
			int m0 = b0 + currentNodesPerRow - i / 4;
			int t0 = m0 + nnodesRowM - i / 4;

			int b[5] = { b0, b0 + 1, b0 + 2, b0 + 3 , b0 + 4 };
			int m[3] = { m0, m0 + 1, m0 + 2 };
			int t[3] = { t0, t0 + 1, t0 + 2 };


			if (meshDens.closedLoop && i == (currentElementPerRow - 4)) {
				b[4] -= (currentNodesPerRow);
				t[2] -= (nnodesRowT);
			}

			std::vector<int*> elNodes;                                      //  t0       t1     t2
			int n1[4] = { b[0], b[1], m[0], t[0] }; elNodes.push_back(n1);  //   x_______x_______x
			int n2[4] = { b[1], b[2], m[1], m[0] }; elNodes.push_back(n2);  //   | \     |     / |
			int n3[4] = { b[2], b[3], m[2], m[1] }; elNodes.push_back(n3);  //   | m0x_m1x___x/m2|
			int n4[4] = { b[3], b[4], t[2], m[2] }; elNodes.push_back(n4);  //   |   |   |   |   |
			int n5[4] = { m[0], m[1], t[1], t[0] }; elNodes.push_back(n5);  //   x___x___x___x___x
			int n6[4] = { m[1], m[2], t[2], t[1] }; elNodes.push_back(n6);  //  b0   b1  b2  b3  b4


			for (int i = 0; i < 6; i++) {
				writer->write4nodedShell(elNodes[i]);
			}

			c += 4;
		}

		currentNodesPerRow = nnodesRowT;
		currentElementPerRow = meshDens.closedLoop ? currentNodesPerRow : currentNodesPerRow - 1;

		c += (nnodesRowM + 1);
		if (meshDens.closedLoop) c--;
	}
}