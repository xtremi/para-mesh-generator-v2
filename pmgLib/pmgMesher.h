#pragma once
#include "pmgMeshWriter.h"
#include "pmgMesh.h"

namespace pmg{




class Mesher {
public:
	void setFEAwriter(pmg::MeshWriter* _writer) {
		writer = _writer;
	}

	void write(Mesh1D& mesh);
	void write(Mesh2D& mesh);
	void write(Mesh3D& mesh);

	void write(const Mesh1D& mesh1, const Mesh1D mesh2);
	void write(const Mesh2D& mesh1, const Mesh2D mesh2, int edge1, int edge2);
	void write(const Mesh3D& mesh1, const Mesh3D mesh2, int face1, int face2);

private:
	MeshWriter* writer;
};


void getPathToPathData(
	const Path& innerPath,
	const Path& outerPath,
	const MeshDensity2D& meshDens,
	VecGLM3d& innerCoords,
	VecGLM3d& outDirections,
	VecD& distances,
	const glm::dvec3& outerPathTranslation = NULL_POS);



}