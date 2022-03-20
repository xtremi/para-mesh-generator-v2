#include "EllipseMesher.h"
#include "LineMesher.h"

std::vector<glm::dvec2> EllipseMesher::default_empty_coord_vec = std::vector<glm::dvec2>();


void EllipseMesher::writeNodes(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity1D&	meshDens,
	const EllipseRadius&	radius,
	const ArcAngles& 		arcAngles,
	direction				rotAxis)
{
	getOrWriteCoords(MesherMode::write, pos, csys, default_empty_coord_vec, meshDens, radius, arcAngles, rotAxis);
}

void EllipseMesher::writeNodesQ(
	const glm::dvec3&		pos,
	MeshCsys&				csys,
	const MeshDensity1D&	meshDens,
	const EllipseRadius&	radius,
	double					startAng,
	double					dAng,
	direction				rotAxis)
{
	getOrWriteCoordsQ(MesherMode::write, pos, csys, default_empty_coord_vec, meshDens, radius, startAng, dAng, rotAxis);

}

void EllipseMesher::getLocalCoords(
	std::vector<glm::dvec2>& coords,
	const MeshDensity1D&	 meshDens,
	const EllipseRadius&	 radius,
	const ArcAngles& 		 arcAngles)
{
	MeshCsys csys;
	getOrWriteCoords(MesherMode::get, glm::dvec3(0.0), csys, coords, meshDens, radius, arcAngles, direction::z);
}

void EllipseMesher::getLocalCoordsQ(
	std::vector<glm::dvec2>& coords,
	const MeshDensity1D&	 meshDens,
	const EllipseRadius&	 radius,
	double					 startAng,
	double					 dAng)
{
	MeshCsys csys;
	getOrWriteCoordsQ(MesherMode::get, glm::dvec3(0.0), csys, coords, meshDens, radius, startAng, dAng, direction::z);
}

void EllipseMesher::getOrWriteCoords(
	MesherMode				 mode,
	const glm::dvec3&		 pos,
	MeshCsys&				 csys,
	std::vector<glm::dvec2>& coords,
	const MeshDensity1D&	 meshDens,
	const EllipseRadius&	 radius,
	const ArcAngles& 		 arcAngles,
	direction				 rotAxis)
{
	double dang = arcAngles.angStep(meshDens.nnodes);
	getOrWriteCoordsQ(mode, pos, csys, coords, meshDens, radius, arcAngles.start, dang, rotAxis);
}

void EllipseMesher::getOrWriteCoordsQ(
	MesherMode				 mode,
	const glm::dvec3&		 pos,
	MeshCsys&				 csys,
	std::vector<glm::dvec2>& coords,
	const MeshDensity1D&	 meshDens,
	const EllipseRadius&	 radius,
	double					 startAng,
	double					 dAng,
	direction				 rotAxis)
{
	int firstNode = writer->getNextNodeID();
	
	glm::dvec3 curCoord(pos);
	double ang = startAng;

	for (int i = 0; i < meshDens.nnodes; i++) {
		curCoord = coordsOnEllipse(ang, radius.rad1, radius.rad2, rotAxis);
		if(mode == MesherMode::write){
			writer->writeNode(curCoord, glm::dvec3(0.0), nullptr, &csys); //check this 04.09.2021
		}
		else {
			coords[i] = curCoord;
		}
		ang += dAng;
	}


	Mesher::nodeID1 = firstNode;
}

void EllipseMesher::writeElements(const MeshDensity1D& meshDens) {
	LineMesher::writeElements(meshDens);
}
