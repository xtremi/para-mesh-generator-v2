#include "CuboidMesher.h"
#include "PlaneMesher.h"
#include "LineMesher.h"

void CuboidMesher::writeNodes(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::dvec3&	size,
	const glm::ivec3&	nnodes,
	plane				pln,
	glm::dmat3x3*		csys)
{
	glm::dvec3 dpos = size / glm::dvec3((double)nnodes.x, (double)nnodes.y, (double)nnodes.z);
	writeNodesQ(writer, spos, dpos, nnodes, pln, csys);
}

void CuboidMesher::writeNodesQ(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::dvec3&	dp,
	const glm::ivec3&	nnodes,
	plane				pln,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords = spos;
	int firstNode = writer->getNextNodeID();
	
	direction normal = getPlaneNormal(pln);

	glm::dvec2 dp12(dp.x, dp.y);
	glm::ivec2 nnodes12(nnodes.x, nnodes.y);

	for (int i3 = 0; i3 < nnodes.z; i3++)
	{
		PlaneMesher::writeNodesPlaneQ(writer, coords, dp12, nnodes12, pln, csys);
		coords[(size_t)normal] += dp.y;
	}
	Mesher::nodeID1 = firstNode;
}

void CuboidMesher::writeNodesXYZq(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec3& dxyz, const glm::ivec3& nnodes, glm::dmat3x3* csys){
	writeNodesQ(writer, spos, dxyz, nnodes, plane::xy, csys);
}
void CuboidMesher::writeNodesXYZ(FEAwriter* writer, const glm::dvec3& spos, const glm::dvec3& size, const glm::ivec3& nnodes, glm::dmat3x3* csys){
	writeNodes(writer, spos, size, nnodes, plane::xy, csys);
}


/*

	 6x-----x5
	 /     /
   7x-----x4

     2x-----x1  
	 /     /
   3x-----x0

*/

void CuboidMesher::writeElements(
	FEAwriter*	writer,
	glm::ivec3	nnodes,
	bool		closedLoop)
{
	int n[8];
	int c = nodeID1;

	int nNodesFace12 = nnodes.x * nnodes.y;

	int nEld1 = closedLoop ? nnodes.x : nnodes.x - 1;
	int nEld2 = nnodes.y - 1;
	int nEld3 = nnodes.z - 1;

	for (int i3 = 0; i3 < nEld3; i3++) {
		for (int i2 = 0; i2 < nEld2; i2++) {
			for (int i1 = 0; i1 < nEld1; i1++) {

				n[0] = c + 1;
				n[1] = n[0] + nnodes.x;
				n[2] = n[1] - 1;
				n[3] = c;

				n[4] = n[0] + nNodesFace12;
				n[5] = n[1] + nNodesFace12;
				n[6] = n[2] + nNodesFace12;
				n[7] = n[3] + nNodesFace12;

				if (closedLoop && i1 == (nEld1 - 1)) {
					n[0] -= nnodes.x;
					n[1] -= nnodes.x;
					n[4] -= nnodes.x;
					n[5] -= nnodes.x;
				}

				c++;
				writer->write8nodedHexa(n);
			}
			if (!closedLoop) c++;
		}
		c += nnodes.x;
	}
}



void CuboidMesherRef::writeNodes(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::dvec3&	size,
	const glm::ivec2&	nNodesFace,
	int					nRefinements,
	bool				startWithOffset,
	plane				pln,
	glm::dmat3x3*		csys)
{

	glm::dvec3 coords = spos;
	int firstNode = writer->getNextNodeID();

	direction refDirection = getPlaneNormal(pln);
	direction dir1, dir2;
	getPlaneDirections(pln, dir1, dir2);

	glm::ivec2	currentNodesPerFace(nNodesFace);
	glm::dvec2	currentElSize12(size.x / (double)(nNodesFace.x - 1), size.y / (double)(nNodesFace.y - 1)); 
	double		currentElSize3 = initialRefinementElementSize(size.z, nRefinements, startWithOffset) / 2.0; //this is not correct	

	int		currentRefFactor1 = 1;
	int		currentRefFactor2 = 1;
	int		currentRefinement = 0;

	while (currentRefinement < nRefinements) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x
		PlaneMesher::writeNodesPlaneQ(writer, coords, currentElSize12, currentNodesPerFace, pln, csys);
		coords[(size_t)refDirection] += currentElSize3;

		//row m1:  |  x--x--x  |  x--x--x  | //make this a function
		for (int i = 0; i < currentNodesPerFace.x; i++) {
			coords[(size_t)dir1] = spos[(size_t)dir1] + (double)i*currentElSize12.x;
			if (i % 4) {
				LineMesher::writeNodesLineQ(writer, coords, currentElSize12.y, currentNodesPerFace.y, dir2, csys);
			}
		}
		coords[(size_t)refDirection] += currentElSize3;
		coords[(size_t)dir1] = spos[(size_t)dir1];

		//Refine dir1
		currentRefFactor1 *= 2;
		currentNodesPerFace.x = (nNodesFace.x - 1) / currentRefFactor1 + 1;
		currentElSize12.x *= 2.0;

		//row m2:  x-----x-----x-----x-----x
		PlaneMesher::writeNodesPlaneQ(writer, coords, currentElSize12, currentNodesPerFace, pln, csys);
		coords[(size_t)refDirection] += currentElSize3;

		//row m3:  |  x--x--x  |  x--x--x  | //make this a function
		for (int i = 0; i < currentNodesPerFace.y; i++) {
			coords[(size_t)dir2] = (double)i*currentElSize12.y;
			if (i % 4){
				LineMesher::writeNodesLineQ(writer, coords, currentElSize12.x, currentNodesPerFace.x, dir1, csys);
			}
		}
		coords[(size_t)refDirection] += currentElSize3;
		coords[(size_t)dir2] = spos[(size_t)dir2];

		//Refine dir2
		currentRefFactor2 *= 2;
		currentNodesPerFace.y = (nNodesFace.y - 1) / currentRefFactor2 + 1;
		currentElSize12.y *= 2.0;

		//row t:  x-----x-----x-----x-----x 
		PlaneMesher::writeNodesPlaneQ(writer, coords, currentElSize12, currentNodesPerFace, pln, csys);
		currentElSize3 *= 2.0;
		coords[(size_t)refDirection] += currentElSize3;
	}

	Mesher::nodeID1 = firstNode;
}

void CuboidMesherRef::writeElements(
	FEAwriter*	writer,
	glm::ivec2	nNodesFace,
	int			nRefinements,
	bool		closedLoop)
{
	int n[8];
	int c = nodeID1;

	int currentRefFactor = 1;
	glm::ivec2 currentNodes12(nNodesFace);
	glm::ivec2 nextNodes12;


	for (int r = 0; r < nRefinements; r++) {

		//Nnodes for next refinement:
		if(!closedLoop){
			nextNodes12.x = (currentNodes12.x - 1) / 2 + 1;
		}
		else{
			nextNodes12.x = currentNodes12.x / 2;
		}
		nextNodes12.y = (currentNodes12.y - 1) / 2 + 1;

		//Number of nodes on each plane:
		int nnodesPlaneB	= currentNodes12.x * currentNodes12.y;
		int nnodesPlaneT    = nextNodes12.x * nextNodes12.y;

		int nElementsPerRowBx = closedLoop ? currentNodes12.x : currentNodes12.x - 1;
		int nElementsPerRowBy = currentNodes12.y - 1;
		int nElementsPerRowT  = closedLoop ? nextNodes12.x : nextNodes12.x - 1;

		int nnodesRowM1		= 3 * nElementsPerRowBx / 4;
		int nnodesPlaneM1	= nnodesRowM1 * currentNodes12.y;
		int nnodesPlaneM2	= nextNodes12.x * currentNodes12.y;
		int nnodesRowM3		= 3 * nElementsPerRowBy / 4;
		int nnodesPlaneM3	= nnodesRowM3 * nextNodes12.x;		
		int nnodesTotal		= nnodesPlaneB + nnodesPlaneM1 + nnodesPlaneM2 + nnodesPlaneM3 + nnodesPlaneT;

		

		//Initial nodes on planes:
		int firstNodeB		= c;
		int firstNodeM1		= firstNodeB + nnodesPlaneB;
		int firstNodeM2		= firstNodeM1 + nnodesPlaneM1;

		int firstNodeM2b	= firstNodeM2;
		int firstNodeM3		= firstNodeM2 + nnodesPlaneM2;
		int firstNodeT		= firstNodeM3 + nnodesPlaneM3;

		//elPlane B:
		if(r != 0){
			Mesher::nodeID1 = firstNodeB - nnodesPlaneB;
			CuboidMesher::writeElements(writer, glm::ivec3(currentNodes12.x, currentNodes12.y, 2), closedLoop);
		}

		writeElements_rows_bm1m2(writer, currentNodes12, nextNodes12, firstNodeB, firstNodeM1, firstNodeM2, closedLoop);
		writeElements_rows_m2m3t(writer, currentNodes12, nextNodes12, firstNodeM2b, firstNodeM3, firstNodeT, closedLoop);

		currentNodes12 = nextNodes12;
		c += nnodesTotal;
	}
}

	void CuboidMesherRef::writeElements_rows_bm1m2(
		FEAwriter*			writer,
		const glm::ivec2&	currentNodes12,
		glm::ivec2&			nextNodes12,
		int&				firstNodeBrow,
		int&				firstNodeM1row,
		int&				firstNodeM2row,
		bool				closedLoop)
	{

		glm::ivec2 currentElements12(closedLoop ? currentNodes12.x : currentNodes12.x - 1, currentNodes12.y - 1);

		for (int i2 = 0; i2 < currentElements12.y; i2++) {
			for (int i1 = 0; i1 < currentElements12.x; i1 += 4) {

				/*
					m2f0/m2b0   x_______x_______x m2
								| \     |     / |     ^ dir3
					m1f0/m1b0   |  x___x___x/	| m1  |
								|   |   |   |   |     |
					bf0/bb0     x___x___x___x___x b   |---->dir1

					b/m1/m2 -> node rows
					bf0/bb0   -> first nodes bottom front/back
					m1f0/m1b0 -> first nodes middle1 front/back
					m2f0/m2b0 -> first nodes middle2 front/back
				*/
				int bf_0 = firstNodeBrow   + 4 * (i1 / 4);
				int m1f_0 = firstNodeM1row + 3 * (i1 / 4) * currentNodes12.y;			
				int m2f_0 = firstNodeM2row + 2 * (i1 / 4);		

				int bb_0 = bf_0 + currentNodes12.x;
				int m1b_0 = m1f_0 + 1;
				int m2b_0 = m2f_0 + nextNodes12.x;


				int bf[5] = { bf_0, bf_0 + 1, bf_0 + 2, bf_0 + 3 , bf_0 + 4 };
				int m1f[3] = { m1f_0, m1f_0 + currentNodes12.y, m1f_0 + 2 * currentNodes12.y };
				int m2f[3] = { m2f_0, m2f_0 + 1, m2f_0 + 2 };

				int bb[5] = { bb_0, bb_0 + 1, bb_0 + 2, bb_0 + 3 , bb_0 + 4 };
				int m1b[3] = { m1b_0, m1b_0 + currentNodes12.y, m1b_0 + 2 * currentNodes12.y };
				int m2b[3] = { m2b_0, m2b_0 + 1, m2b_0 + 2 };


				if (closedLoop && i1 == (currentElements12.x - 4)) {
					bf[4] -= currentNodes12.x;
					bb[4] -= currentNodes12.x;
					m2f[2] -= nextNodes12.x;
					m2b[2] -= nextNodes12.x;
				}


				std::vector<int*> elNodes;
				int n_el1[8] = { bf[0], bf[1], bb[1], bb[0],  m2f[0], m1f[0], m1b[0], m2b[0] }; elNodes.push_back(n_el1);
				int n_el2[8] = { bf[1], bf[2], bb[2], bb[1],  m1f[0], m1f[1], m1b[1], m1b[0] }; elNodes.push_back(n_el2);
				int n_el3[8] = { bf[2], bf[3], bb[3], bb[2],  m1f[1], m1f[2], m1b[2], m1b[1] }; elNodes.push_back(n_el3);
				int n_el4[8] = { bf[3], bf[4], bb[4], bb[3],  m1f[2], m2f[2], m2b[2], m1b[2] }; elNodes.push_back(n_el4);
				int n_el5[8] = { m1f[0], m1f[1], m1b[1], m1b[0],  m2f[0], m2f[1], m2b[1], m2b[0] }; elNodes.push_back(n_el5);
				int n_el6[8] = { m1f[1], m1f[2], m1b[2], m1b[1],  m2f[1], m2f[2], m2b[2], m2b[1] }; elNodes.push_back(n_el6);


				for (int i = 0; i < elNodes.size(); i++) {
					writer->write8nodedHexa(elNodes[i]);
				}
			}
			firstNodeBrow += currentNodes12.x;
			firstNodeM1row += 1;
			firstNodeM2row += nextNodes12.x;
		}
	}

	void CuboidMesherRef::writeElements_rows_m2m3t(
		FEAwriter*			writer,
		const glm::ivec2&	currentNodes12,
		glm::ivec2&			nextNodes12,
		int&				firstNodeM2row,
		int&				firstNodeM3row,
		int&				firstNodeTrow,
		bool				closedLoop)
	{

		glm::ivec2 currentElements12(closedLoop ? currentNodes12.x : currentNodes12.x - 1, currentNodes12.y - 1);
		glm::ivec2 nextElements12(closedLoop ? nextNodes12.x : nextNodes12.x - 1, nextNodes12.y - 1);

		for (int i1 = 0; i1 < nextElements12.x; i1++) {
			for (int i2 = 0; i2 < (currentElements12.y); i2 += 4) {

				/*
					tf0/tb0     x_______x_______x t
								| \     |     / |     ^ dir3
					m3f0/m3b0   |  x___x___x/	| m3  |
								|   |   |   |   |     |
					m2f0/m2b0   x___x___x___x___x m2  |---->dir2

					m2/m3/t   -> node rows
					tf0/tb0   -> first nodes bottom front/back
					m3f0/m3b0 -> first nodes middle3 front/back
					m2f0/m2b0 -> first nodes middle2 front/back
				*/

				int m2f_0 = firstNodeM2row + 4 * (i2 / 4) * nextNodes12.x;
				int m3f_0 = firstNodeM3row + 3 * (i2 / 4) * nextNodes12.x;
				int tf_0 = firstNodeTrow +   2 * (i2 / 4) * nextNodes12.x;

				int m2f[5], m3f[3], tf[3];
				int m2b[5], m3b[3], tb[3];
				for (int i = 0; i < 5; i++) {
					m2f[i] = m2f_0 + i * nextNodes12.x;
					m2b[i] = m2f[i] + 1;
					if(closedLoop && i1 == (nextElements12.x - 1)) m2b[i] -= nextNodes12.x;
				}
				for (int i = 0; i < 3; i++) {
					m3f[i] = m3f_0 + i * nextNodes12.x;
					m3b[i] = m3f[i] + 1;
					if (closedLoop && i1 == (nextElements12.x - 1)) m3b[i] -= nextNodes12.x;

					tf[i] = tf_0 + i * nextNodes12.x;
					tb[i] = tf[i] + 1;
					if (closedLoop && i1 == (nextElements12.x - 1)) tb[i] -= nextNodes12.x;
				}

				if (closedLoop && i1 == (nextElements12.x - 1)) {
					int tmp = 1;
				}

				std::vector<int*> elNodes;
				int n_el1[8] = { m2f[0], m2f[1], m2b[1], m2b[0],  tf[0], m3f[0], m3b[0], tb[0] }; elNodes.push_back(n_el1);
				int n_el2[8] = { m2f[1], m2f[2], m2b[2], m2b[1],  m3f[0], m3f[1], m3b[1], m3b[0] }; elNodes.push_back(n_el2);
				int n_el3[8] = { m2f[2], m2f[3], m2b[3], m2b[2],  m3f[1], m3f[2], m3b[2], m3b[1] }; elNodes.push_back(n_el3);
				int n_el4[8] = { m2f[3], m2f[4], m2b[4], m2b[3],  m3f[2], tf[2], tb[2], m3b[2] }; elNodes.push_back(n_el4);
				int n_el5[8] = { m3f[0], m3f[1], m3b[1], m3b[0],  tf[0], tf[1], tb[1], tb[0] }; elNodes.push_back(n_el5);
				int n_el6[8] = { m3f[1], m3f[2], m3b[2], m3b[1],  tf[1], tf[2], tb[2], tb[1] }; elNodes.push_back(n_el6);

				for (int i = 0; i < elNodes.size(); i++) {
					//if( i != 0 && 
					//	//i != 1 &&
					//	i != 2 &&
					//	i != 3 &&
					//	i != 4 &&
					//	i != 5)
					//{
					writer->write8nodedHexa(elNodes[i]);
					//}
				}

			}
			firstNodeM2row += 1;
			firstNodeM3row += 1;
			firstNodeTrow += 1;
		}

	}


