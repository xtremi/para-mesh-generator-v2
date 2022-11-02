#pragma once
#include "pmgMeshCsys.h"
#include "pmgMeshTransformer.h"
#include "pmgMeshDensity.h"
#include "pmgPath.h"
#include "pmgSurface.h"



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

}