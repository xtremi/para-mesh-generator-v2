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
class QuadStrip3Dmesher : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&			  pos,
		MeshCsys&					  csys,
		const MeshDensity3DquadStrip& meshDens,
		const QuadStrip3D&			  quadStrip);
	static void writeElements(const MeshDensity3DquadStrip& meshDens);
};
