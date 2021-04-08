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

	struct RefShapeData{		
		const MeshDensity2Dref* meshDens;
		const Cone2Dradius*		radius;
		const ArcAngles*		angle;
		direction				rotAxis;
		double					height;
		double					coneLength;
	};
	struct RefLayerData{
		MeshCsys				curPos;
		double					curConeLength;
		double					curRadius;
		glm::dvec2				curElSize;
	};

/*	struct WriteRefLayerInput {
		MeshCsys				curPos;
		const MeshDensity2Dref* meshDens;
		const Cone2Dradius*		radius;
		const ArcAngles*		angle;
		direction				rotAxis;
		double					height;
		double					coneLength;
		double					curConeLength;
		double					curRadius;
		glm::dvec2				curElSize;
	};*/

	static void incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData);
	static void writeNodes_refLayerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_refLayerM(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_refLayerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
};