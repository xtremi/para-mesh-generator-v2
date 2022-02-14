#pragma once
#include "Mesher.h"
#include "FeaWrite.h"


#ifdef TO_BE_REMOVED_MAYBE
class MesherInputCone3D : public MesherInput {
public:
	MesherInputCone3D(){}
	MesherInputCone3D(const glm::dvec3& pos, const NodeVec3D& _nodes, const Pipe3Dradius& _radius, const ArcAngles& _angle, double _height, direction _axis)
		: MesherInput(pos), meshDens{ meshDens3D(_nodes) }, radius{_radius}, angle{_angle}, height{_height}, axis{_axis}{}

	meshDens3D		meshDens;
	Pipe3Dradius	radius;
	ArcAngles		angle;
	double			height;	
	direction		axis;
};
#endif



class Cone3Dmesher : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity3D&	meshDens,
		const Pipe3Dradius&		radius,
		const ArcAngles&		angle,
		double					height,
		direction				axis);

	static void writeElements(const MeshDensity3D& meshDens);
};

class Cone3DmesherRef : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity3Dref& meshDens,
		const Pipe3Dradius&		radius,
		const ArcAngles&		angle,
		double					height,
		bool					startWithOffset,
		direction				rotaxis);

	static void writeElements(const MeshDensity3Dref& meshDens);

private:
	struct RefShapeData {
		const MeshDensity3Dref* meshDens;
		const Pipe3Dradius*		radius;
		const ArcAngles*		angle;
		direction				rotAxis;
		double					height;
		double					coneLengthOuter;
		double					coneLengthInner;
		MeshCsys*				csys;
	};
	struct RefLayerData {
		glm::dvec3				curPos;
		double					curConeLength;
		Disk2Dradius			curRadius;
		double					curElSizeRefDir;
	};

	static void incrementConeStep(const RefShapeData& rsData, RefLayerData& rlData);
	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM1(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM2(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM3(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
};

class Cone3DmesherRef2 : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&		pos,
		MeshCsys&				csys,
		const MeshDensity3Dref& meshDens,
		const Pipe3Dradius&		radius,
		const ArcAngles&		angle,
		double					height,
		bool					startWithOffset,
		direction				rotaxis);

	static void writeElements(const MeshDensity3Dref& meshDens);

private:
	struct RefShapeData {
		const MeshDensity3Dref* meshDens;
		const Pipe3Dradius*		radius;
		const ArcAngles*		angle;
		direction				rotAxis;
		double					height;
		double					coneLengthOuter;
		double					coneLengthInner;
		MeshCsys* csys;
	};
	struct RefLayerData {
		glm::dvec3				curPos;
		double					curAngle;
		double					curAngleStep;
		glm::dvec3				curConeLineStep;
		glm::dvec3				curLineStartPos;
	};

	static void updateLayerData(const RefShapeData& rsData, RefLayerData& rlData, int RefLayer);
	static void writeNodes_layerB(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM1(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM2(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerM3(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
	static void writeNodes_layerT(const RefShapeData& rsData, RefLayerData& rlData, int refLayer);
};