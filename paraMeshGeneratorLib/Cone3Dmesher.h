#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class MesherInput {
public:
	glm::dvec3 pos;
};

struct NodeVec2D { 
	int dir1, dir2;
	int circ() { return dir1; }
	int norm() { return dir2; }
};
struct NodeVec3D : public NodeVec2D { 
	int dir3; 
	int axis() { return dir3; }
	int refDir() { return dir3; }
};

struct MeshSize2D {
	NodeVec2D nodes;
	int nElDir1(bool closedLoop = false) { return closedLoop ? nodes.dir1 - 1 : nodes.dir1; }
	int nElDir2(bool closedLoop = false) { return closedLoop ? nodes.dir2 - 1 : nodes.dir2; }
	int nElCirc(bool closedLoop = false) { return nElDir1(closedLoop); }
	int nElNorm(bool closedLoop = false) { return nElDir2(closedLoop); }
};

struct MeshSize3D {
	NodeVec3D nodes;
	int nElDir1(bool closedLoop = false) { return closedLoop ? nodes.dir1 - 1 : nodes.dir1; }
	int nElDir2(bool closedLoop = false) { return closedLoop ? nodes.dir2 - 1 : nodes.dir2; }
	int nElDir3(bool closedLoop = false) { return closedLoop ? nodes.dir3 - 1 : nodes.dir3; }
	int nElCirc(bool closedLoop = false) { return nElDir1(closedLoop); }
	int nElNorm(bool closedLoop = false) { return nElDir2(closedLoop); }
	int nElAxis(bool closedLoop = false) { return nElDir3(closedLoop); }
	int nElRefDir(bool closedLoop = false) { return nElDir3(closedLoop); }
};



struct Pipe2Dradius {
	double inner, outer;
};
struct Pipe3Dradius {
	Pipe2Dradius start, end;
};
struct ArcAngles {
	double start, end;
};

class MesherInputCone : public MesherInput {
public:
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
		const glm::dvec3&	spos,
		double				radiusStartOuter,
		double				radiusEndOuter,
		double				radiusStartInner,
		double				radiusEndInner,
		double				startAng,
		double				endAng,
		double				height,
		const glm::ivec3&	nnodes,
		direction			rotaxis,
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