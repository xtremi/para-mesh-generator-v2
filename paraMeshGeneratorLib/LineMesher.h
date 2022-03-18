#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


class LineMesher : private Mesher {

public:

	static void writeNodesQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		const MeshDensity1D&    meshDens,
		double					ds,		
		direction				dir);

	static void writeNodesQ(
		const glm::dvec3&		pos,
		MeshCsys&				spos,
		const MeshDensity1D&    meshDens,
		const glm::dvec3&		ds);

	static void writeNodes(const std::vector<glm::dvec2>& xycoords, double zcoord);


	static void writeNodes(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length, direction dir);
	static void writeNodes(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, const glm::dvec3& sposEnd);
	static void writeNodesXq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double dx);
	static void writeNodesYq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double dy);
	static void writeNodesZq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double dz);
	static void writeNodesX(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length);
	static void writeNodesY(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length);
	static void writeNodesZ(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double length);

	static void writeElements(const MeshDensity1D& meshDens);

	static NodeIterator1D nodeIterator(int nnodes);
};

