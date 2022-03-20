#include "QuadStripMesher.h"
#include "PlaneMesher.h"
#include "LineMesher.h"
#include "LineStripMesher.h"

/*!
	Writes nodes along line strips
*/
void QuadStripMesher::writeNodes(
	const glm::dvec3&			  pos,
	MeshCsys&					  csys,
	const MeshDensity2DquadStrip& meshDens,
	const QuadStrip&			  quadStrip) 
{
	MESHER_NODE_WRITE_START
	if (quadStrip.nQuads() != meshDens.nQuads()) {
		throw("QuadStripMesher::writeNodes unequal number of quads");
	}
	MeshDensity1DlineStrip meshDensLine(meshDens.getMeshDensDir1());
		
	LineStrip line1 = quadStrip.getLineStripBot();
	LineStrip line2 = quadStrip.getLineStripTop();
	LineStrip current(line1);
	std::vector<glm::dvec3> lineDispl;
	for (int i = 0; i < line1.nPoints(); i++) {
		glm::dvec3 distVec = line2.points[i] - line1.points[i];
		lineDispl.push_back(distVec / (double)meshDens.dir2());
	}

	for (int i = 0; i < meshDens.dir2(); i++) {
		LineStripMesher::writeNodes(pos, csys, current, meshDensLine);
		for (int i = 0; i < current.nPoints(); i++) {
			current.points[i] += lineDispl[i];
		}
	}


		
	MESHER_NODE_WRITE_END
}


/*!
	Writes quad by quad
		Not correct - overlapping nodes
*/
/*void QuadStripMesher::writeNodes2(
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
}*/


void QuadStripMesher::writeElements(const MeshDensity2DquadStrip& meshDens)
{
	PlaneMesher::writeElements(meshDens);
}

/*!
	Not correct - overlapping nodes
*/
/*void QuadStripMesher::writeElements2(const MeshDensity2DquadStrip& meshDens)
{
	int nQuads = meshDens.nQuads();
	MeshDensity2D quadMeshDens;
	for (int i = 0; i < nQuads; i++) {
		quadMeshDens = meshDens.meshDensQ(i);
		PlaneMesher::writeElements(quadMeshDens);
		Mesher::nodeID1 += quadMeshDens.nNodes();
	}

}*/