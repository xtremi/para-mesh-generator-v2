#pragma once
#include "Mesher.h"
#include "FeaWrite.h"



class MesherInputCone3D : public MesherInput {
public:
	MesherInputCone3D(){}
	MesherInputCone3D(const glm::dvec3& pos, const NodeVec3D& _nodes, const Pipe3Dradius& _radius, const ArcAngles& _angle, double _height, direction _axis)
		: MesherInput(pos), meshSize{ MeshSize3D(_nodes) }, radius{_radius}, angle{_angle}, height{_height}, axis{_axis}{}

	MeshSize3D		meshSize;
	Pipe3Dradius	radius;
	ArcAngles		angle;
	double			height;	
	direction		axis;
};

class Cone3Dmesher : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		MesherInputCone3D&	meshInp,
		glm::dmat3x3*		csys = nullptr);



	static void writeElements(
		FEAwriter*	writer,
		glm::ivec3	nnodes,
		bool		closedLoop);



};

class Cone3DmesherRef : private Mesher
{
public:
	static void writeNodes(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes12,
		int					nRefinements,
		double				radiusStartOuter,
		double				radiusEndOuter,
		double				radiusStartInner,
		double				radiusEndInner,
		double				startAng,
		double				endAng,
		double				height,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(
		FEAwriter*	writer,
		glm::ivec2	nnodes12,
		int			nRefinements,
		bool		closedLoop);

private:
	static void writeNodes_refLayerM1(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes12,
		double				radiusInner,
		double				radiusOuter,
		double				startAng,
		double				dang,
		int					skipNth,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);

	static void writeNodes_refLayerM2(
		FEAwriter*			writer,
		const glm::dvec3&	spos,
		const glm::ivec2&	nnodes12,
		double				radiusInner,
		double				radiusOuter,
		double				startAng,
		double				dang,
		int					skipNth,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);
};