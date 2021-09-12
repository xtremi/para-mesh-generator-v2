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
protected:
	size_t iteratedFaceID;
};
class MeshPrimitive3D : public MeshPrimitive{
public:
	MeshPrimitive3D() : MeshPrimitive() {
		elementDim = Dim::DIM3D;
	}
	size_t iteratedFaceID;
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

class MeshRec2D_ext : public MeshPrimitive2D {
public:
	MeshRec2D_ext() : MeshPrimitive2D() {}

	void initRectangle(const glm::dvec2& _sizeXY, const glm::ivec2& _nNodesXY);

	void extrudeYedge(double length, int nElements);
	void extrudeYedgeArc(double endAng, double radius, int nElements);

	virtual void writeNodes();
	virtual void writeElements();


	MeshEdge	 getEdge(int section, int edgeIndex);
	MeshEdge_ext getExtrudedEdge(int edgeIndex);

protected:
	std::vector<MeshEdgeExtrusion> extrusionsXdir;

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
