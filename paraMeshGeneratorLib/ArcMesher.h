#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


class ArcMesher : private Mesher {
public:
	static void writeNodesCircular(
		const MeshCsys&		spos,
		int					nnodes,
		double				radius,
		const ArcAngles& 	arcAngles,
		direction			rotAxis);

	static void writeNodesCircularQ(
		const MeshCsys&		spos,
		int					nnodes,
		double				radius,
		double				startAng,
		double				dAng,		
		direction			rotAxis);

	static void writeNodesCircularX(const MeshCsys& spos, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularY(const MeshCsys& spos, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularZ(const MeshCsys& spos, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularXq(const MeshCsys& spos, int nnodes, double radius, double startAng, double dAng);
	static void writeNodesCircularYq(const MeshCsys& spos, int nnodes, double radius, double startAng, double dAng);
	static void writeNodesCircularZq(const MeshCsys& spos, int nnodes, double radius, double startAng, double dAng);

	static void writeNodesCircular_nth(
		const MeshCsys&		spos,
		int					nnodes,
		double				radius,
		const ArcAngles& 	arcAngles,
		int					skipNth,
		direction			rotAxis);

	static void writeNodesCircularQ_nth(
		const MeshCsys&		spos,
		int					nnodes,
		double				radius,
		double				startAng,
		double				dAng,
		int					skipNth,
		direction			rotAxis);

	static void writeElementsLine(int nnodes, bool closedLoop = false);
};

