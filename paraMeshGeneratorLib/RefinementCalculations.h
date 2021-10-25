#pragma once

namespace refinement {

	int nNodesTot_2d(int nRef, int nElementsEdge0);

	int nNodesLayerB_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerM_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerT_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerMT_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerBM_2d(int refLayer, int nElementsEdge0);

	int nElementsLayerB_2d(int refLayer, int nElementsEdge0);
	int nElementsLayerT_2d(int refLayer, int nElementsEdge0);

	int minElementsEdge0_2d(int nRef);
	int minNodesEdge0_2d(int nRef);

	int nNodesLayerB_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);
	int nNodesLayerM1_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);
	int nNodesLayerM2_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);
	int nNodesLayerM3_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);
	int nNodesLayerT_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);

	int nNodesLayerBM1_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);
	int nNodesLayerM1M2_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);
	int nNodesLayerM2M3_3d(int refLayer, int nElementsFace0x, int nElementsFace0y);



}

