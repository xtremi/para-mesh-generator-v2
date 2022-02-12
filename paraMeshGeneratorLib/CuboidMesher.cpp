#include "CuboidMesher.h"
#include "PlaneMesher.h"
#include "LineMesher.h"
#include "math_utilities.h"
#include "RefinementCalculations.h"


void CuboidMesher::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity3D&	meshDens,
	const glm::dvec3&		size,
	plane					pln)
{
	glm::dvec3 dpos = size / glm::dvec3((double)meshDens.dir1(), (double)meshDens.dir2(), (double)meshDens.dir3());
	writeNodesQ(pos, csys, meshDens, dpos, pln);
}

void CuboidMesher::writeNodesQ(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity3D&	meshDens,
	const glm::dvec3&		dp,
	plane					pln)
{
MESHER_NODE_WRITE_START
	
	direction normal = getPlaneNormal(pln);

	glm::dvec2 dp12(dp.x, dp.y);
	MeshDensity2D planeMeshDens = meshDens.meshDensD12();

	for (int i3 = 0; i3 < meshDens.dir3(); i3++){
		PlaneMesher::writeNodesQ(curPos, csys, planeMeshDens, dp12,  pln);
		curPos[(size_t)normal] += dp.z;
	}
MESHER_NODE_WRITE_END
}

void CuboidMesher::writeNodesXYZq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity3D& meshDens, const glm::dvec3& dxyz){
	writeNodesQ(pos, csys, meshDens, dxyz, plane::xy);
}
void CuboidMesher::writeNodesXYZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity3D& meshDens, const glm::dvec3& size){
	writeNodes(pos, csys, meshDens, size, plane::xy);
}


/*

	 6x-----x5
	 /     /
   7x-----x4

     2x-----x1  
	 /     /
   3x-----x0

*/

void CuboidMesher::writeElements(const MeshDensity3D& meshDens)
{
	int n[8];
	int c = nodeID1;

	int nNodesFace12 = meshDens.nnodesPlaneD12();
	int nEld1 = meshDens.nElDir1();
	int nEld2 = meshDens.nElDir2();
	int nEld3 = meshDens.nElDir3();

	for (int i3 = 0; i3 < nEld3; i3++) {
		for (int i2 = 0; i2 < nEld2; i2++) {
			for (int i1 = 0; i1 < nEld1; i1++) {

				n[0] = c + 1;
				n[1] = n[0] + meshDens.dir1();
				n[2] = n[1] - 1;
				n[3] = c;

				n[4] = n[0] + nNodesFace12;
				n[5] = n[1] + nNodesFace12;
				n[6] = n[2] + nNodesFace12;
				n[7] = n[3] + nNodesFace12;

				if (meshDens.closedLoop && i1 == (nEld1 - 1)) {
					n[0] -= meshDens.dir1();
					n[1] -= meshDens.dir1();
					n[4] -= meshDens.dir1();
					n[5] -= meshDens.dir1();
				}

				c++;
				writer->write8nodedHexa(n);
			}
			if (!meshDens.closedLoop) c++;
		}
		c += meshDens.dir1();
	}
}


/*
	Draws the nodes for a cube of dimensions <size>.
	The nodes are structured in such a way that CuboidMesherRef::writeElements
	can create the Hexagonal elements making up the cube.

	The cube is refined along its local Z-axis. 
	The number of nodes along the local X and Y axes (at local Z = 0.0 / bottom) are defined by <nNodesFace>
	The number of nodes along the local Z-axis is dependent in the number of refinements.

	With pln == plane::xy: [local X] = X, [local Y] = Y, [local Z] = Z, 
	With pln == plane::xz: [local X] = X, [local Y] = Z, [local Z] = Y, 
	With pln == plane::yz: [local X] = Y, [local Y] = Z, [local Z] = X, 

	[in] writer			 :
	[in] spos			 :
	[in] size			 :
	[in] nNodesFace		 :
	[in] nRefinements	 :
	[in] startWithOffset :
	[in] pln			 :
	[in] csys			 :

*/
void CuboidMesherRef::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity3Dref&	meshDens,
	const glm::dvec3&		size,
	bool					startWithOffset,
	plane					pln)
{
	int firstNode = writer->getNextNodeID();

	RefShapeData rsData;
	rsData.csys		= &csys;
	rsData.meshDens = &meshDens;
	rsData.pln	    = pln;
	rsData.refDir   = getPlaneNormal(pln);
	
	RefLayerData rlData;
	rlData.curPos			= pos;
	rlData.curElSize		= glm::dvec2(size.x / (double)meshDens.nElDir1(), size.y / (double)meshDens.nElDir3());
	rlData.curElSizeRefDir  = refinement::initialRefElSize3D(size.z, meshDens.nRefs(), startWithOffset);
	
	if (startWithOffset) {
		rlData.curPos[(size_t)rsData.refDir] += rlData.curElSizeRefDir;	
	}

	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		writeNodes_layerB(rsData, rlData, refLayer);
		writeNodes_layerM1(rsData, rlData, refLayer);
		writeNodes_layerM2(rsData, rlData, refLayer);
		writeNodes_layerM3(rsData, rlData, refLayer);
		writeNodes_layerT(rsData, rlData, refLayer);
	}

	Mesher::nodeID1 = firstNode;
}

//row b:  x-----x-----x-----x-----x
void CuboidMesherRef::writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	PlaneMesher::writeNodesQ(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12B(refLayer), rlData.curElSize, rsData.pln);
	rlData.curPos[(size_t)rsData.refDir] += rlData.curElSizeRefDir;
}

//row m1:  |  x--x--x  |  x--x--x  | 
void CuboidMesherRef::writeNodes_layerM1(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	PlaneMesher::writeNodesQ_nth(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12B(refLayer), rlData.curElSize, rsData.pln, 4, true);
	rlData.curPos[(size_t)rsData.refDir] += rlData.curElSizeRefDir;
}
//row m2:  x-----x-----x-----x-----x
void CuboidMesherRef::writeNodes_layerM2(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	rlData.curElSize.x *= 2.0;
	PlaneMesher::writeNodesQ(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12M2(refLayer), rlData.curElSize, rsData.pln);
	rlData.curPos[(size_t)rsData.refDir] += rlData.curElSizeRefDir;
}
//row m3:  |  x--x--x  |  x--x--x  |
void CuboidMesherRef::writeNodes_layerM3(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	PlaneMesher::writeNodesQ_nth(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12M2(refLayer), rlData.curElSize, rsData.pln, 4, false);
	rlData.curPos[(size_t)rsData.refDir] += rlData.curElSizeRefDir;
}
//row t:  x-----x-----x-----x-----x 
void CuboidMesherRef::writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer){
	rlData.curElSize.y *= 2.0;
	PlaneMesher::writeNodesQ(rlData.curPos, *rsData.csys, rsData.meshDens->meshDensD12T(refLayer), rlData.curElSize, rsData.pln);
	rlData.curElSizeRefDir *= 2.0;
	rlData.curPos[(size_t)rsData.refDir] += rlData.curElSizeRefDir;
}

void CuboidMesherRef::writeElements(const MeshDensity3Dref& meshDens)
{
	int n[8];	
	int currentRefFactor = 1;
	MeshDensity2D curMeshDens12 = meshDens.meshDensD12B(0);
	MeshDensity2D nextMeshDens12(curMeshDens12);

	int c = nodeID1;

	for (int refLayer = 0; refLayer < meshDens.nRefs(); refLayer++) {
		if(refLayer != 0){
			writeElements_layerB(meshDens, c, refLayer); writer->writeComment("layer B finish");
		}
		writeElements_layerBM1M2(meshDens, c, refLayer); writer->writeComment("layer BM1M2 finish");
		writeElements_layerM2M3T(meshDens, c, refLayer); writer->writeComment("layer M2M3T finish");
	}
}

void CuboidMesherRef::writeElements_layerB(const MeshDensity3Dref& meshDens, int& nid1, int curRefLayer) {
	MeshDensity2D meshDensB = meshDens.meshDensD12B(curRefLayer);
	Mesher::nodeID1 = nid1 - meshDensB.nNodes();
	CuboidMesher::writeElements(MeshDensity3D(meshDensB.dir1(), meshDensB.dir2(), 2, meshDens.closedLoop));
}

	void CuboidMesherRef::writeElements_layerBM1M2(const MeshDensity3Dref& meshDens, int& nid1, int curRefLayer)
	{
		MeshDensity2D meshDensB  = meshDens.meshDensD12B(curRefLayer);
		MeshDensity2D meshDensM1 = meshDens.meshDensD12M1(curRefLayer);
		MeshDensity2D meshDensM2 = meshDens.meshDensD12M2(curRefLayer);

		int b0  = nid1;
		int m10 = b0  + meshDensB.nNodes();
		int m20 = m10 + meshDensM1.nNodes();
		nid1 = m20;

		int bf_0, m1f_0, m2f_0;
		int bb_0, m1b_0, m2b_0;
		int bf[5], m1f[3], m2f[3];
		int bb[5], m1b[3], m2b[3];
		int* elNodes[6];

		for (int i2 = 0; i2 < meshDensB.nElDir2(); i2++) {
			for (int i1 = 0; i1 < meshDensB.nElDir1(); i1 += 4) {

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
				bf_0  =  b0 + 4 * (i1 / 4);
				m1f_0 = m10 + 3 * (i1 / 4) * meshDensB.dir2();
				m2f_0 = m20 + 2 * (i1 / 4);		

				bb_0 = bf_0 + meshDensB.dir1();
				m1b_0 = m1f_0 + 1;
				m2b_0 = m2f_0 + meshDensM2.dir1();

				for (int k = 0; k < 5; k++) bf[k]  = bf_0 + k;
				for (int k = 0; k < 3; k++) m1f[k] = m1f_0 + meshDensB.dir2() * k;
				for (int k = 0; k < 3; k++) m2f[k] = m2f_0 + k;
				
				for (int k = 0; k < 5; k++) bb[k]  = bb_0 + k;
				for (int k = 0; k < 3; k++) m1b[k] = m1b_0 + meshDensB.dir2() * k;
				for (int k = 0; k < 3; k++) m2b[k] = m2b_0 + k;

				if (meshDens.closedLoop && i1 == (meshDensB.nElDir1() - 4)) {
					bf[4]  -= meshDensB.dir1();
					bb[4]  -= meshDensB.dir1();
					m2f[2] -= meshDensM2.dir1();
					m2b[2] -= meshDensM2.dir1();
				}


				
				int n_el1[8] = { bf[0], bf[1], bb[1], bb[0],  m2f[0], m1f[0], m1b[0], m2b[0] }; elNodes[0] = n_el1;
				int n_el2[8] = { bf[1], bf[2], bb[2], bb[1],  m1f[0], m1f[1], m1b[1], m1b[0] }; elNodes[1] = n_el2;
				int n_el3[8] = { bf[2], bf[3], bb[3], bb[2],  m1f[1], m1f[2], m1b[2], m1b[1] }; elNodes[2] = n_el3;
				int n_el4[8] = { bf[3], bf[4], bb[4], bb[3],  m1f[2], m2f[2], m2b[2], m1b[2] }; elNodes[3] = n_el4;
				int n_el5[8] = { m1f[0], m1f[1], m1b[1], m1b[0],  m2f[0], m2f[1], m2b[1], m2b[0] }; elNodes[4] = n_el5;
				int n_el6[8] = { m1f[1], m1f[2], m1b[2], m1b[1],  m2f[1], m2f[2], m2b[2], m2b[1] }; elNodes[5] = n_el6;

				for (int i = 0; i < 6; i++) {
					writer->write8nodedHexa(elNodes[i]);
				}
			}
			b0 += meshDensB.dir1();
			m10 += 1;
			m20 += meshDensM2.dir1();
		}

		
	}

	void CuboidMesherRef::writeElements_layerM2M3T(const MeshDensity3Dref& meshDens, int& nid1, int curRefLayer)
	{

		MeshDensity2D meshDensM2 = meshDens.meshDensD12M2(curRefLayer);
		MeshDensity2D meshDensM3 = meshDens.meshDensD12M3(curRefLayer);
		MeshDensity2D meshDensT = meshDens.meshDensD12T(curRefLayer);

		int m20 = nid1;
		int m30 = m20 + meshDensM2.nNodes();
		int t0  = m30 + meshDensM3.nNodes();
		nid1 = t0 + meshDensT.nNodes();


		int m2f_0, m3f_0, tf_0;

		int m2f[5], m3f[3], tf[3];
		int m2b[5], m3b[3], tb[3];
		int* elNodes[6];

		for (int i1 = 0; i1 < meshDensM2.nElDir1(); i1++) {
			for (int i2 = 0; i2 < meshDensM2.nElDir2(); i2 += 4) {

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

				m2f_0 = m20 + 4 * (i2 / 4) * meshDensT.dir1();
				m3f_0 = m30 + 3 * (i2 / 4) * meshDensT.dir1();
				tf_0  = t0  + 2 * (i2 / 4) * meshDensT.dir1();
		
				for (int i = 0; i < 5; i++) {
					m2f[i] = m2f_0 + i * meshDensT.dir1();
					m2b[i] = m2f[i] + 1;
					if(meshDens.closedLoop && i1 == (meshDensT.nElDir1() - 1)) m2b[i] -= meshDensT.dir1();
				}
				for (int i = 0; i < 3; i++) {
					m3f[i] = m3f_0 + i * meshDensT.dir1();
					m3b[i] = m3f[i] + 1;
					if (meshDens.closedLoop && i1 == (meshDensT.nElDir1() - 1)) m3b[i] -= meshDensT.dir1();

					tf[i] = tf_0 + i * meshDensT.dir1();
					tb[i] = tf[i] + 1;
					if (meshDens.closedLoop && i1 == (meshDensT.nElDir1() - 1)) tb[i] -= meshDensT.dir1();
				}

				if (meshDens.closedLoop && i1 == (meshDensT.nElDir1() - 1)) {
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
					writer->write8nodedHexa(elNodes[i]);
				}

			}
			m20 += 1;
			m30 += 1;
			t0  += 1;
		}

	}


