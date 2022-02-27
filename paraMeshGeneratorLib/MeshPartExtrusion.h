#pragma once
#include "MeshPart.h"

class Mesh2D_planeExtrusion : public MeshPart2D {
public:
	Mesh2D_planeExtrusion(int nElementsY, double lengthY);
	Mesh2D_planeExtrusion();

	void setStartEdgeY(int nElementsY, double lengthY);

	void extrudeYedge(double length, int nElements);
	void extrudeYedgeRef(double length, int nRef);
	void extrudeYedgeArc(double endAng, double radius, int nElements);
	void extrudeYedgeArcRef(double endAng, double radius, int nRef);

	virtual void writeNodes();
	virtual void writeElements();


	MeshEdge getEdge(int section, int edgeIndex);
	MeshEdge getExtrudedEdge(int edgeIndex);

protected:
	std::vector<std::shared_ptr<MeshEdgeExtrusion>> extrusionsXdir;
	void setNodeOffsetOnMeshEdgeExtrusions(int nodeIDoffset);

	int	   nNodesEdgeY;
	double lengthY;
};


class Mesh3D_volumeExtrusion : public MeshPart3D {
public:
	Mesh3D_volumeExtrusion(const MeshDensity2D& meshDensYZface, const glm::dvec2& sizeYZ);
	Mesh3D_volumeExtrusion();

	void setStartFaceYZ(const MeshDensity2D& meshDensYZface, const glm::dvec2& sizeYZ);

	void extrudeYZface(double length, int nElements);
	void extrudeYZfaceRef(double length, int nRef);
	void extrudeYZfaceArc(double endAng, double radiusInner, int nElements);
	void extrudeYZfaceArcRef(double endAng, double radiusInner, int nRef);

	virtual void writeNodes();
	virtual void writeElements();

	MeshFace getFace(int section, int faceIndex);


protected:
	std::vector<std::shared_ptr<MeshFaceExtrusion>> extrusionsXdir;
	void setNodeOffsetOnMeshEdgeExtrusions(int nodeIDoffset);

	glm::dvec2 sizeYZ;
	MeshDensity3D meshDensity; //of the face being extruded
};

