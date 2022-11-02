#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace pmg{

class MeshCsys {
public:
	MeshCsys(
		const glm::dvec3& _pos = glm::dvec3(0.f),
		const glm::dmat3& _rotM = glm::dmat3(1.f)) 
		: pos{ _pos }, rot{ _rotM } 
	{
		update();
	}

	MeshCsys(
		MeshCsys* parent, 
		const glm::dvec3& _pos = glm::dvec3(0.0), 
		const glm::dmat3& _rotM = glm::dmat3(1.f))
		: MeshCsys(_pos, _rotM)
	{
		setParentCsys(parent);
	}

	void update(bool updateParents = false);
	void setParentCsys(MeshCsys* _parentCsys);

	glm::vec3 getGlobalCoords(const glm::dvec3& localCoords) const;

	glm::dvec3 pos;
	glm::dmat3 rot;

	glm::dvec3 l2g_T;
	glm::dmat3 l2g_R;

	MeshCsys* parentCsys = nullptr;


	/*void updateParents();
std::vector<MeshCsys*> getAllParentCsys();*/
};

}