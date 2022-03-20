#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


class ArcMesher : private Mesher {
public:

	static void writeNodes(
		const glm::dvec3&	 pos,
		MeshCsys&			 csys,
		const MeshDensity1D& meshDens,
		double				 radius,
		const ArcAngles&	 angle,
		const glm::dvec3&	 xdir,
		const glm::dvec3&	 ydir);

	static void writeNodesCircular(
		const glm::dvec3&	 pos,
		MeshCsys&			 csys,
		const MeshDensity1D& meshDens,
		double				 radius,
		const ArcAngles& 	 arcAngles,
		direction			 rotAxis);

	static void writeNodesCircularQ(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		const MeshDensity1D& meshDens,
		double				radius,
		double				startAng,
		double				dAng,		
		direction			rotAxis);

	static void writeNodesCircularX(const glm::dvec3& pos, MeshCsys& csys,  const MeshDensity1D& meshDens, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularY(const glm::dvec3& pos, MeshCsys& csys,  const MeshDensity1D& meshDens, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularZ(const glm::dvec3& pos, MeshCsys& csys,  const MeshDensity1D& meshDens, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularXq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double radius, double startAng, double dAng);
	static void writeNodesCircularYq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double radius, double startAng, double dAng);
	static void writeNodesCircularZq(const glm::dvec3& pos, MeshCsys& csys, const MeshDensity1D& meshDens, double radius, double startAng, double dAng);

	static void writeNodesCircular_nth(
		const glm::dvec3&	 pos,
		MeshCsys&			 csys,
		const MeshDensity1D& meshDens,
		double				 radius,
		const ArcAngles& 	 arcAngles,
		direction			 rotAxis);

	static void writeNodesCircularQ_nth(
		const glm::dvec3&	 pos,
		MeshCsys&			 csys,
		const MeshDensity1D& meshDens,
		double				 radius,
		double				 startAng,
		double				 dAng,
		direction			 rotAxis);

	static void writeNodes(
		const glm::dvec3&	pos,
		MeshCsys&			csys,
		const MeshDensity1D& meshDens,
		double				radius,
		const glm::dvec3&	p1,
		const glm::dvec3&	p2,
		const glm::dvec3&   normal);

	static void writeElements(const MeshDensity1D& meshDens);
};

