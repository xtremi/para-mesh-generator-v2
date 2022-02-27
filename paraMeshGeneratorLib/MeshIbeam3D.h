#pragma once
#include "MeshPart.h"

class MeshIbeam3D : public MeshPart3D
{
public:
	MeshIbeam3D() : MeshPart3D(){}

	void writeNodes();
	void writeElements();
};

