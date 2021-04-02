#include "PlaneMesher.h"
#include "LineMesher.h"

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
	nodeID1 = firstNode;
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
void PlaneMesherRef::writeNodesPlane_ref(
	const MeshCsys&			spos,
	const MeshDensity2Dref& meshDens,
	const glm::dvec2&		size,
	bool					startWithOffset,
	plane					pln)
{
	int firstNode = writer->getNextNodeID();
	MeshCsys curPos(spos);

	int		currentNodesPerRow = meshDens.dir2();
	int		currentRefFactor = 1;
	int		currentRefinement = 0;

	double elSizeX = initialRefinementElementSize(size.x, meshDens.nRefs(), startWithOffset);

	glm::dvec2 curElSize(elSizeX, size.y / (double)(currentNodesPerRow - 1)); //start with square elements (??)(??)

	direction edgeDirection, refinementDirection;
	getPlaneDirections(pln, refinementDirection, edgeDirection);

	if (startWithOffset) {
		curPos.pos[(size_t)refinementDirection] += curElSize.x;
	}

	while (currentRefinement < meshDens.nRefs()) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x
		LineMesher::writeNodesLineQ(curPos, currentNodesPerRow, curElSize.y, edgeDirection);
		curPos.pos[(size_t)refinementDirection] += curElSize.x;

		//row m:  |  x--x--x  |  x--x--x  |
		LineMesher::writeNodesLineQ_nth(curPos, currentNodesPerRow, curElSize.y,  4, edgeDirection);
		curPos.pos[(size_t)refinementDirection] += curElSize.x;
		
		//Refine
		// [9 nodes / 8 elements] -> [5 nodes / 4 elements] -> [3 nodes / 2 elements]
		currentRefFactor *= 2;
		currentNodesPerRow = meshDens.nElDir2() / currentRefFactor + 1;
		curElSize.y *= 2.0;

		//row t: x----x----x----x----x
		LineMesher::writeNodesLineQ(curPos, currentNodesPerRow, curElSize.y, edgeDirection);
		curElSize.x *= 2.0;
		curPos.pos[(size_t)refinementDirection] += curElSize.x;
	}

	nodeID1 = firstNode;
}


void PlaneMesherRef::writeNodesPlaneXY_ref(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodesPlane_ref(spos, meshDens, size, startWithOffset, plane::xy);
}
void PlaneMesherRef::writeNodesPlaneXZ_ref(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodesPlane_ref(spos, meshDens, size, startWithOffset, plane::xz);
}
void PlaneMesherRef::writeNodesPlaneYZ_ref(const MeshCsys& spos, const MeshDensity2Dref& meshDens, const glm::dvec2& size, bool startWithOffset){
	return writeNodesPlane_ref(spos, meshDens, size, startWithOffset, plane::yz);
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
void PlaneMesherRef::writeElementsPlane_ref(const MeshDensity2Dref& meshDens)
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