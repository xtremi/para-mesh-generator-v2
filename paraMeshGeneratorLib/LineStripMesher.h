#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class LineStripMesher : private Mesher {

public:
	static void writeNodes(
		const glm::dvec3&			  pos,
		MeshCsys&					  csys,
		const LineStrip&			  lineStrip,
		const MeshDensity1DlineStrip& meshDens);

	static void writeElements(const MeshDensity1DlineStrip& meshDens);
};