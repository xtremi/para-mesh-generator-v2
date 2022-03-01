#include "QuadStripMesher.h"
#include "PlaneMesher.h"



void QuadStripMesher::writeNodes(
	const glm::dvec3& pos,
	MeshCsys& csys,
	const MeshDensity2DquadStrip& meshDens,
	const QuadStrip& quadStrip)
{
	if (quadStrip.nQuads() != meshDens.nQuads()) {
		throw("QuadStripMesher::writeNodes unequal number of quads");
	}
	
	MESHER_NODE_WRITE_START
	int nQuads = quadStrip.nQuads();
	MeshDensity2D quadMeshDens;
	for (int i = 0; i < nQuads; i++) {
		Quad q = quadStrip.quad(i);
		quadMeshDens = meshDens.meshDensQ(i);
		PlaneMesher::writeNodesQ(pos, csys, quadMeshDens, q);
	}
	MESHER_NODE_WRITE_END
}

void QuadStripMesher::writeElements(const MeshDensity2DquadStrip& meshDens)
{
	int nQuads = meshDens.nQuads();
	MeshDensity2D quadMeshDens;
	for (int i = 0; i < nQuads; i++) {
		quadMeshDens = meshDens.meshDensQ(i);
		PlaneMesher::writeElements(quadMeshDens);
		Mesher::nodeID1 += quadMeshDens.nNodes();
	}

}