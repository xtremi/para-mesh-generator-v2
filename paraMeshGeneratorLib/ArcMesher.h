#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


class ArcMesher : private Mesher {
public:
	static void writeNodesCircular(
		MeshCsys&		spos,
		int					nnodes,
		double				radius,
		const ArcAngles& 	arcAngles,
		direction			rotAxis);

	static void writeNodesCircularQ(
		MeshCsys&		spos,
		int					nnodes,
		double				radius,
		double				startAng,
		double				dAng,		
		direction			rotAxis);

	static void writeNodesCircularX(MeshCsys& spos, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularY(MeshCsys& spos, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularZ(MeshCsys& spos, int nnodes, double radius, const ArcAngles& arcAngles);
	static void writeNodesCircularXq(MeshCsys& spos, int nnodes, double radius, double startAng, double dAng);
	static void writeNodesCircularYq(MeshCsys& spos, int nnodes, double radius, double startAng, double dAng);
	static void writeNodesCircularZq(MeshCsys& spos, int nnodes, double radius, double startAng, double dAng);

	static void writeNodesCircular_nth(
		MeshCsys&		spos,
		int					nnodes,
		double				radius,
		const ArcAngles& 	arcAngles,
		int					skipNth,
		direction			rotAxis);

	static void writeNodesCircularQ_nth(
		MeshCsys&		spos,
		int					nnodes,
		double				radius,
		double				startAng,
		double				dAng,
		int					skipNth,
		direction			rotAxis);

	static void writeElementsLine(int nnodes, bool closedLoop = false);
};

