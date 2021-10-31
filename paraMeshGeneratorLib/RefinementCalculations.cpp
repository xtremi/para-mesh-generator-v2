#include "RefinementCalculations.h"
#include "math_utilities.h"

/*
b2	 x_______________x_______________x             
	 |               |               |			
	 |				 |				 |			
	 |				 |				 |			
	 |				 |				 |			
	 |				 |				 |             
t1	 x_______________x_______________x			     2 el / 3 nodes
	 |\              |              /|			
	 |  \ 			 |		      /	 |			
m1	 |	   \ x_______x_______x  / 	 |			            3 nodes
	 |       |       |       |		 |			
	 |	     |       |       |       |             
b1	 x_______x_______x_______x_______x               4 el / 5 nodes
	 |       |       |       |       |			
	 |       |       |       |       |			
t0   x_______x_______x_______x_______x  row t (top)  4 el / 5 nodes
	 | \     |     / |  \    |     / |  elRow t	
m0	 |   x___x___x/	 |   x___x___x	 |  row m (mid)         6 nodes
	 |   |   |   |   |   |   |   |   |	elRow m	          
b0   x___x___x___x___x___x___x___x___x  row b (bot)  8 el / 9 nodes



b0 = 8 el / 9 nodes
m0 = 6 nodes
to = 4 el / nodes
b1 = 4 el / nodes
m1 = 3 nodes
t1 = 2 el / 3 nodes
b2 = 2 el / 3 nodes

*/
int refinement::nNodesLayerB_2d(int refLayer, int nElementsEdge0) {
	return nElementsLayerB_2d(refLayer, nElementsEdge0) + 1;
}
int refinement::nNodesLayerM_2d(int refLayer, int nElementsEdge0) {
	return 3 * nElementsLayerB_2d(refLayer, nElementsEdge0) / 4;
}
int refinement::nNodesLayerT_2d(int refLayer, int nElementsEdge0) {
	return nElementsLayerT_2d(refLayer, nElementsEdge0) + 1;
}
int refinement::nNodesLayerMT_2d(int refLayer, int nElementsEdge0) {
	return nNodesLayerM_2d(refLayer, nElementsEdge0) + nNodesLayerT_2d(refLayer, nElementsEdge0);
}
int refinement::nNodesLayerBM_2d(int refLayer, int nElementsEdge0) {
	return nNodesLayerB_2d(refLayer, nElementsEdge0) + nNodesLayerM_2d(refLayer, nElementsEdge0);
}


int refinement::nElementsLayerB_2d(int refLayer, int nElementsEdge0) {
	return nElementsEdge0 / twoPow(refLayer);
}
int refinement::nElementsLayerT_2d(int refLayer, int nElementsEdge0) {
	return nElementsLayerB_2d(refLayer + 1, nElementsEdge0);
}

int refinement::minElementsEdge0_2d(int nRef) {
	return twoPow(nRef + 1);
}
int refinement::minNodesEdge0_2d(int nRef) {
	return minElementsEdge0_2d(nRef) + 1;
}

/*!

	Total number of nodes in a mesh created by PlaneMesherRef

	total = (9/2)N x sum(1/2^i, i=1, i=nRef) + 2 x nRef
	total = (9/2)N x (1 - 1/2^nRef) + 2 x nRef 
*/
int refinement::nNodesTot_2d(int nRef, int nElementsEdge0) {	
	return (int)(4.5*(float)nElementsEdge0 * (1 - 1. / (float)twoPow(nRef))) + 2 * nRef;
}


int refinement::nNodesLayerB_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerB_2d(refLayer, nElementsFace0x) * nNodesLayerB_2d(refLayer, nElementsFace0y);
}
int refinement::nNodesLayerM1_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerM_2d(refLayer, nElementsFace0x) * nNodesLayerB_2d(refLayer, nElementsFace0y);
}
int refinement::nNodesLayerM2_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerB_2d(refLayer + 1, nElementsFace0x) * nNodesLayerB_2d(refLayer, nElementsFace0y);
}
int refinement::nNodesLayerM3_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerM_2d(refLayer, nElementsFace0y) * nNodesLayerT_2d(refLayer, nElementsFace0x);
}
int refinement::nNodesLayerT_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerB_3d(refLayer + 1, nElementsFace0x, nElementsFace0y);
}

int refinement::nNodesLayerBM1_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerB_3d(refLayer, nElementsFace0x, nElementsFace0y) +
		nNodesLayerM1_3d(refLayer, nElementsFace0x, nElementsFace0y);
}
int refinement::nNodesLayerM1M2_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerM1_3d(refLayer, nElementsFace0x, nElementsFace0y) +
		nNodesLayerM2_3d(refLayer, nElementsFace0x, nElementsFace0y);
}
int refinement::nNodesLayerM2M3_3d(int refLayer, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerM2_3d(refLayer, nElementsFace0x, nElementsFace0y) +
		nNodesLayerM3_3d(refLayer, nElementsFace0x, nElementsFace0y);
}
int refinement::nNodesTotSection_3d(int nRef, int nElementsFace0x, int nElementsFace0y) {
	return nNodesLayerBM1_3d(nRef, nElementsFace0x, nElementsFace0y)
		+ nNodesLayerM2M3_3d(nRef, nElementsFace0x, nElementsFace0y)
		+ nNodesLayerT_3d(nRef, nElementsFace0x, nElementsFace0y);
}
int refinement::nNodesTot_3d(int nRef, int nElementsFace0x, int nElementsFace0y) {
	int nTot = 0;
	for (int i = 0; i < nRef; i++) {
		nTot += refinement::nNodesTotSection_3d(i, nElementsFace0x, nElementsFace0y);
	}
	return nTot;
}

