#pragma once
#include "Mesher.h"
#include "FeaWrite.h"
#include "MeshDensity.h"




/*
              x----x
	        / |     \
	x-----x   |      \
	|     |  |        \
	x-----x  |         \
	       \ |          \
		     x-----------x
*/
class QuadStripMesher : private Mesher
{
public:
		static void writeNodes(
		const glm::dvec3&			  pos,
		MeshCsys&					  csys,
		const MeshDensity2DquadStrip& meshDens,
		const QuadStrip&			  quadStrip);
	static void writeNodes2(
		const glm::dvec3&			  pos,
		MeshCsys&					  csys,
		const MeshDensity2DquadStrip& meshDens,
		const QuadStrip&			  quadStrip);

	static void writeElements(const MeshDensity2DquadStrip& meshDens);
	static void writeElements2(const MeshDensity2DquadStrip& meshDens);
};
