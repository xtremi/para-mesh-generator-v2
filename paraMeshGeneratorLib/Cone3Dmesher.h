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
		const MeshCsys&			spos,
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
		const MeshCsys&			spos,
		const MeshDensity3Dref& meshDens,
		const Pipe3Dradius&		radius,
		const ArcAngles&		angle,
		double					height,
		direction				rotaxis);

	static void writeElements(const MeshDensity3Dref& meshDens);

private:
	static void writeNodes_refLayerM1(
		const MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		Disk2Dradius&		 radius,
		double				 startAng,
		double				 dang,
		int					 skipNth,
		direction			 rotaxis);

	static void writeNodes_refLayerM2(
		const MeshCsys&		 spos,
		const MeshDensity2D& meshDens,
		Disk2Dradius&		 radius,
		double				 startAng,
		double				 dang,
		int					 skipNth,
		direction			 rotaxis);
};