#pragma once
#include "ParaMeshGenCommon.h"
#include "FeaWrite.h"
#include "Geometry.h"
#include "Extrusion.h"
#include <memory>

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
	Dim elementDim;
	MeshCsys csys;

	int firstNodeID, lastNodeID;
	int firstElementID, lastElementID;
	int nNodes;
	int nElements;
};

class MeshPart : public Mesh {};

class MeshPart1D : public MeshPart {
public:
	MeshPart1D() : MeshPart() {
		elementDim = Dim::DIM1D;
	}
};
class MeshPart2D : public MeshPart {
public:
	MeshPart2D() : MeshPart() {
		elementDim = Dim::DIM2D;
	}
};
class MeshPart3D : public MeshPart {
public:
	MeshPart3D() : MeshPart() {
		elementDim = Dim::DIM3D;
	}
};
/*
	Not implemented
*/
class MeshRec2D : public MeshPart2D {
public:
	MeshRec2D() : MeshPart2D(){}

private:
	RecGeometry2D	geo;
	MeshDensity2D	meshDensity;
};
