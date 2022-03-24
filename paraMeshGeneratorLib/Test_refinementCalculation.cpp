#include "Test_refinementCalculation.h"
#include "RefinementCalculations.h"
#include "CuboidMesher.h"
#include "Test_convinient_functions.h"

/*
b0 = 8 el / 9 nodes
m0 = 6 nodes
to = 4 el / 5 nodes
b1 = 4 el / 5 nodes
m1 = 3 nodes
t1 = 2 el / 3 nodes
b2 = 2 el / 3 nodes
*/
int refinementCalc2D(const std::string& fileName) {

	if (refinement::nElementsLayerB_2d(0, 8) != 8) return 1;
	if (refinement::nNodesLayerB_2d(0, 8) != 9) return 1;
	if (refinement::nNodesLayerM_2d(0, 8) != 6) return 1;
	if (refinement::nElementsLayerT_2d(0, 8) != 4) return 1;
	if (refinement::nNodesLayerT_2d(0, 8) != 5) return 1;

	if (refinement::nElementsLayerB_2d(1, 8) != 4) return 1;
	if (refinement::nNodesLayerB_2d(1, 8) != 5) return 1;
	if (refinement::nNodesLayerM_2d(1, 8) != 3) return 1;
	if (refinement::nElementsLayerT_2d(1, 8) != 2) return 1;
	if (refinement::nNodesLayerT_2d(1, 8) != 3) return 1;

	if (refinement::nElementsLayerB_2d(2, 8) != 2) return 1;
	if (refinement::nNodesLayerB_2d(2, 8) != 3) return 1;

	if (refinement::nNodesTot_2d(3, 16) != 69) return 1;



	return 0;
}

int refinementCalc3D(const std::string& fileName) {

	TEST_START2
	MeshDensity3Dref meshDens(2, 9, 17);
	CuboidMesherRef::writeNodes(pos, glCsys, meshDens, glm::dvec3(20., 20., 40.), false, plane::yz);
	CuboidMesherRef::writeElements(meshDens);
	writer.close();


	if (refinement::nNodesLayerB_3d(0, 8, 8) != 81) return 1;
	if (refinement::nNodesLayerM1_3d(0, 8, 8) != 54) return 1;
	if (refinement::nNodesLayerM2_3d(0, 8, 8) != 45) return 1;
	if (refinement::nNodesLayerM3_3d(0, 8, 8) != 30) return 1;
	if (refinement::nNodesLayerT_3d(0, 8, 8) != 25) return 1;

	if (refinement::nNodesLayerB_3d(1, 8, 8) != 25) return 1;
	if (refinement::nNodesLayerM1_3d(1, 8, 8) != 15) return 1;
	if (refinement::nNodesLayerM2_3d(1, 8, 8) != 15) return 1;
	if (refinement::nNodesLayerM3_3d(1, 8, 8) != 9) return 1;
	if (refinement::nNodesLayerT_3d(1, 8, 8) != 9) return 1;


	if (refinement::nNodesLayerB_3d(0, 8, 16) != 153) return 1;
	if (refinement::nNodesLayerM1_3d(0, 8, 16) != 102) return 1;
	if (refinement::nNodesLayerM2_3d(0, 8, 16) != 85) return 1;
	if (refinement::nNodesLayerM3_3d(0, 8, 16) != 60) return 1;
	if (refinement::nNodesLayerT_3d(0, 8, 16) != 45) return 1;

	if (refinement::nNodesLayerB_3d(1, 8, 16) != 45) return 1;
	if (refinement::nNodesLayerM1_3d(1, 8, 16) != 27) return 1;
	if (refinement::nNodesLayerM2_3d(1, 8, 16) != 27) return 1;
	if (refinement::nNodesLayerM3_3d(1, 8, 16) != 18) return 1;
	if (refinement::nNodesLayerT_3d(1, 8, 16) != 15) return 1;

	return 0;
}