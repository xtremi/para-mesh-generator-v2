#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


class ArcMesher : private Mesher {
public:
	static void writeNodesCircular(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nnodes,
		double				radius,
		const ArcAngles& 	arcAngles,
		direction			rotAxis);

	static void writeNodesCircularQ(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nnodes,
		double				radius,
		double				startAng,
		double				dAng,		
		direction			rotAxis);

	static void writeNodesCircularX(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularY(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularZ(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularXq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, double startAng, double dAng);
	static void writeNodesCircularYq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, double startAng, double dAng);
	static void writeNodesCircularZq(const glm::dvec3& pos, MeshCsys& csys, int nnodes, double radius, double startAng, double dAng);

	static void writeNodesCircular_nth(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nnodes,
		double				radius,
		const ArcAngles& 	arcAngles,
		int					skipNth,
		direction			rotAxis);

	static void writeNodesCircularQ_nth(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		int					nnodes,
		double				radius,
		double				startAng,
		double				dAng,
		int					skipNth,
		direction			rotAxis);

	static void writeElementsLine(int nnodes, bool closedLoop = false);
};

