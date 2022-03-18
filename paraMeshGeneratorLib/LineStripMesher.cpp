#include "LineStripMesher.h"
#include "LineMesher.h"

void LineStripMesher::writeNodes(
	const glm::dvec3&			  pos,
	MeshCsys&					  csys,
	const LineStrip&			  lineStrip,
	const MeshDensity1DlineStrip& meshDens)
{
	MESHER_NODE_WRITE_START
	int nLineStrips = lineStrip.nLines();
	glm::dvec3 p1, p2;

	glm::dvec3 endOffset(0.);

	for (int i = 0; i < nLineStrips; i++) {
		lineStrip.getLineEndPoints(i, p1, p2);
		if (meshDens.closedLoop) {
			if (i == (nLineStrips - 1)) {
				endOffset = (p2 - p1) / (double)meshDens.segmentMeshDens[i].nEl();
			}
		}
		LineMesher::writeNodes(pos + p1, csys, meshDens.segmentMeshDens[i], pos + p2 - endOffset);
	}
	MESHER_NODE_WRITE_END
}

void LineStripMesher::writeElements(const MeshDensity1DlineStrip& meshDens) {
	LineMesher::writeElements(meshDens);
}