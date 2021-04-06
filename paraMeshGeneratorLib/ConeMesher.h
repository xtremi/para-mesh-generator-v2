#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class ConeMesher : private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,		
		const MeshDensity2D&	meshDens,
		const Cone2Dradius&		radius,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);

	static void writeNodesX(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height);
	static void writeNodesY(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height);
	static void writeNodesZ(const MeshCsys& spos, const MeshDensity2D& meshDens, const Cone2Dradius& radius, const ArcAngles& angle, double height);

	static void writeElements(const MeshDensity2D& meshDens);


	static void writeNodes_nthLine(
		const MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		const Cone2Dradius&	 radius,
		double				 startAng,
		double				 dang,
		double				 height,
		int					 skipNth,
		direction			 rotaxis);

	static void writeNodes_nthArc(
		const MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		const Cone2Dradius&	 radius,
		double				 startAng,
		double				 dang,
		double				 height,
		int					 skipNth,
		direction			 rotaxis);
};

class ConeMesherRef : private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity2Dref&	meshDens,
		const Cone2Dradius&		radius,
		const ArcAngles&		angle,
		double					height,
		bool					startWithOffset,
		direction				rotaxis);

	static void writeElements(const MeshDensity2Dref& meshDens);

private:
	static void incrementConeStep(
		MeshCsys&			curPos,
		double&				curConeLength,
		double&				curRadius,
		double				coneLength,
		double				elSizeRefDir,
		double				height,
		const Cone2Dradius& radius,
		direction			rotAxis);

	static void writeNodes_refLayerB(
		MeshCsys&			curPos,
		int					nodesPerRow,
		const glm::dvec2&	elSize,
		const ArcAngles&	angle,
		const Cone2Dradius& radius,
		double				coneLength,
		double				height,
		double&				curRadius,
		double&				curConeLength,
		direction			rotAxis);

	static void writeNodes_refLayerM(
		MeshCsys&			curPos,
		int					nodesPerRow,
		const glm::dvec2&	elSize,
		const ArcAngles&	angle,
		const Cone2Dradius& radius,
		double				coneLength,
		double				height,
		double&				curRadius,
		double&				curConeLength,
		direction			rotAxis);

	static void writeNodes_refLayerT(
		MeshCsys&			curPos,
		int					nodesPerRow,
		glm::dvec2&			elSize,
		const ArcAngles&	angle,
		const Cone2Dradius& radius,
		double				coneLength,
		double				height,
		double&				curRadius,
		double&				curConeLength,
		direction			rotAxis);
};