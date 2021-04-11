#include "EllipseMesher.h"
#include "LineMesher.h"

std::vector<glm::dvec2> EllipseMesher::default_empty_coord_vec = std::vector<glm::dvec2>();


void EllipseMesher::writeNodes(
	const MeshCsys&			spos,
	int						nnodes,
	const EllipseRadius&	radius,
	const ArcAngles& 		arcAngles,
	direction				rotAxis)
{
	getOrWriteCoords(MesherMode::write, spos, default_empty_coord_vec, nnodes, radius, arcAngles, rotAxis);
}

void EllipseMesher::writeNodesQ(
	const MeshCsys&			spos,
	int						nnodes,
	const EllipseRadius&	radius,
	double					startAng,
	double					dAng,
	direction				rotAxis)
{
	getOrWriteCoordsQ(MesherMode::write, spos, default_empty_coord_vec, nnodes, radius, startAng, dAng, rotAxis);

}

void EllipseMesher::getLocalCoords(
	std::vector<glm::dvec2>& coords,
	int						 nnodes,
	const EllipseRadius&	 radius,
	const ArcAngles& 		 arcAngles)
{
	getOrWriteCoords(MesherMode::get, MeshCsys(), coords, nnodes, radius, arcAngles, direction::z);
}

void EllipseMesher::getLocalCoordsQ(
	std::vector<glm::dvec2>& coords,
	int						 nnodes,
	const EllipseRadius&	 radius,
	double					 startAng,
	double					 dAng)
{
	getOrWriteCoordsQ(MesherMode::get, MeshCsys(), coords, nnodes, radius, startAng, dAng, direction::z);
}

void EllipseMesher::getOrWriteCoords(
	MesherMode				 mode,
	const MeshCsys&			 spos,
	std::vector<glm::dvec2>& coords,
	int						 nnodes,
	const EllipseRadius&	 radius,
	const ArcAngles& 		 arcAngles,
	direction				 rotAxis)
{
	double dang = arcAngles.angStep(nnodes);
	getOrWriteCoordsQ(mode, spos, coords, nnodes, radius, arcAngles.start, dang, rotAxis);
}

void EllipseMesher::getOrWriteCoordsQ(
	MesherMode				 mode,
	const MeshCsys&			 spos,
	std::vector<glm::dvec2>& coords,
	int						 nnodes,
	const EllipseRadius&	 radius,
	double					 startAng,
	double					 dAng,
	direction				 rotAxis)
{
	int firstNode = writer->getNextNodeID();
	
	glm::dvec3 curCoord;
	double ang = startAng;

	for (int i = 0; i < nnodes; i++) {
		curCoord = coordsOnEllipse(ang, radius.rad1, radius.rad2, rotAxis);
		if(mode == MesherMode::write){
			writer->writeNode(curCoord, spos.pos, spos.csys);
		}
		else {
			coords[i] = curCoord;
		}
		ang += dAng;
	}


	Mesher::nodeID1 = firstNode;
}

void EllipseMesher::writeElements(int nnodes, bool closedLoop) {
	LineMesher::writeElementsLine(nnodes, closedLoop);
}
