#pragma once
#include "pmgPath.h"
#include "pmgMeshCsys.h"
#include "pmgMeshTransformer.h"
#include "pmgMeshDensity.h"
#include "pmgSurface.h"
#include "pmgMeshWriter.h"
#include <iostream>

namespace pmg{

	static const pmg::MeshCsys globalCsys;
	static const pmg::MeshTransformer noTransformer;

	class Mesh {
	public:
		int		  firstNodeID;
		int		  firstElementID;
		const pmg::MeshCsys* csys = &globalCsys;
		const pmg::MeshTransformer* transformer = &noTransformer;
	};

	class Mesh1D : public Mesh {
	public:
		pmg::MeshDensity1D meshDensity;
		pmg::Path* path;
	};
	class Mesh2D : public Mesh {
	public:
		pmg::MeshDensity2D meshDensity;
		pmg::Surface* surface;
	};
	class Mesh3D : public Mesh {
	public:
		pmg::MeshDensity3D	  meshDensity;
		pmg::ExtrudedSurface* volume;
	};

enum class node_skip { none = 0, first = 1, last = -1, first_and_last = -2, every_2 = 2, every_3 = 3, every_4 = 4, every_5 = 5, every_6 = 6 };
bool skip(int i, int last, node_skip nskip);



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


void Mesher::write(Mesh1D& mesh) {
	mesh.firstNodeID = writer->nextNodeID();
	mesh.firstElementID = writer->nextElementID();

	for (int i = 0; i < mesh.meshDensity.nNodes(); i++) {
		glm::dvec3 pos = mesh.path->positionI(i, mesh.meshDensity.nNodes(), mesh.meshDensity.closedLoop);
		writer->writeNode(pos, *mesh.csys, *mesh.transformer);
	}

	int nodeIDs[2] = { mesh.firstNodeID, mesh.firstNodeID + 1 };
	for (int i = 0; i < mesh.meshDensity.nElements(); i++) {
		writer->write2nodedBeam(nodeIDs);
		nodeIDs[0]++; nodeIDs[1]++;
		if (mesh.meshDensity.closedLoop && i == mesh.meshDensity.nElements() - 2) {
			nodeIDs[1] = mesh.firstNodeID;
		}
	}

}

}