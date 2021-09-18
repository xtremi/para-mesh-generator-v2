#pragma once
#include "ParaMeshGenCommon.h"
#include "FeaWrite.h"
#include "Geometry.h"
#include "Extrusion.h"


class Mesh {
public:
	Mesh() {
		csys.pos = glm::dvec3(0.0);
	}
public:
	virtual void writeNodes() = 0;
	virtual void writeElements() = 0;
	int numberOfNodes() { return nNodes; }
	int numberOfElements() { return nElements; }

	void setPosition(const glm::dvec3& _pos) { csys.pos = _pos; }
	void setRotationMatrix(glm::dmat3x3* _rotMat) { csys.csys = _rotMat; }
	void setCsys(MeshCsys& _csys) { csys = _csys; }

protected:
	virtual void calculateNumberOfNodes() = 0;
	virtual void calculateNumberOfElements() = 0;

	Dim elementDim;
	MeshCsys csys;

	int firstNodeID, lastNodeID;
	int firstElementID, lastElementID;
	int nNodes;
	int nElements;

	size_t iteratedEdgeID;
};


class MeshPrimitive : public Mesh {};


class MeshPrimitive1D : public MeshPrimitive{
public:
	MeshPrimitive1D() : MeshPrimitive() {
		elementDim = Dim::DIM1D;
	}
};
class MeshPrimitive2D : public MeshPrimitive{
public:
	MeshPrimitive2D() : MeshPrimitive() {
		elementDim = Dim::DIM2D;
	}
};
class MeshPrimitive3D : public MeshPrimitive{
public:
	MeshPrimitive3D() : MeshPrimitive() {
		elementDim = Dim::DIM3D;
	}
};
/*
	Not implemented
*/
class MeshRec2D : public MeshPrimitive2D {
public:
	MeshRec2D() : MeshPrimitive2D(){}

private:
	RecGeometry2D	geo;
	MeshDensity2D	meshDensity;
};

class Mesh2D_planeExtrusion : public MeshPrimitive2D {
public:
	Mesh2D_planeExtrusion(int nElementsY, double lengthY);
	Mesh2D_planeExtrusion();

	void setStartEdgeY(int nElementsY, double lengthY);

	void extrudeYedge(double length, int nElements);
	void extrudeYedgeArc(double endAng, double radius, int nElements);

	virtual void writeNodes();
	virtual void writeElements();


	MeshEdge	 getEdge(int section, int edgeIndex);
	MeshEdge_ext getExtrudedEdge(int edgeIndex);

private:
	struct ExtrudeStepData {
		glm::dvec3 pos;
		MeshCsys   csys;
		double	   startSpace;
		double	   arcAngle;
		int		   nNodesEdgeX;
		glm::dvec2 dxy;

	};

protected:
	std::vector<MeshEdgeExtrusion> extrusionsXdir;
	void setNodeOffsetOnMeshEdgeExtrusions(int nodeIDoffset);

	void writeNodesExtrudeLine(ExtrudeStepData& current, MeshExtrusion& curExtr);
	void writeNodesExtrudeArc(ExtrudeStepData& current, MeshExtrusion& curExtr);

	void calculateNumberOfNodes();
	void calculateNumberOfElements();
	void calculateNumberOfNodesX();

	double lengthY;
	int	   nNodesY;
	int	   nNodesX;
};


//class MeshTube2D : public MeshPrimitive2D , public MeshExtrusion {};
//class MeshExtrudedRec: public MeshRec2D {};
//class MeshExtrudedTube : public MeshTube2D, public MeshExtrusion {};
//class MeshObject : public Mesh {};
