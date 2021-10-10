#pragma once

namespace refinement {

	int nNodesLayerB_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerM_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerT_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerMT_2d(int refLayer, int nElementsEdge0);
	int nNodesLayerBM_2d(int refLayer, int nElementsEdge0);

	int nElementsLayerB_2d(int refLayer, int nElementsEdge0);
	int nElementsLayerT_2d(int refLayer, int nElementsEdge0);

	int minElementsEdge0_2d(int nRef);
	int minNodesEdge0_2d(int nRef);

}

