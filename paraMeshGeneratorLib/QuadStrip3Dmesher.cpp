#include "QuadStrip3Dmesher.h"
#include "QuadStripMesher.h"
#include "CuboidMesher.h"


void QuadStrip3Dmesher::writeNodes(
	const glm::dvec3&			  pos,
	MeshCsys&					  csys,
	const MeshDensity3DquadStrip& meshDens,
	const QuadStrip3D&			  quadStrip)
{
	MESHER_NODE_WRITE_START
	MeshDensity2DquadStrip meshDens12 = meshDens.getMeshDensD12q();

	double elSizeD3 = glm::length(quadStrip.lengthVector) / (double)meshDens.nElDir3();
	glm::dvec3 direction = glm::normalize(quadStrip.lengthVector);

	for (int i3 = 0; i3 < meshDens.dir3(); i3++) {
		QuadStripMesher::writeNodes(curPos, csys, meshDens12, quadStrip);
		curPos += elSizeD3* direction;
	}
	MESHER_NODE_WRITE_END
}

void QuadStrip3Dmesher::writeElements(const MeshDensity3DquadStrip& meshDens) {
	CuboidMesher::writeElements(meshDens);
}