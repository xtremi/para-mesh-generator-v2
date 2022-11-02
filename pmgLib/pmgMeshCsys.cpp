#include "pmgMeshCsys.h"
using namespace pmg;

void MeshCsys::setParentCsys(MeshCsys* _parentCsys) {
	parentCsys = _parentCsys;
	update();
}


void MeshCsys::update(bool updateParents) {
	if(updateParents){
		if(parentCsys) parentCsys->update(updateParents);
	}
	l2g_R = parentCsys ? parentCsys->l2g_R * rot : rot;
	l2g_T = parentCsys ? parentCsys->l2g_T + parentCsys->l2g_R * pos : pos;
}


glm::vec3 MeshCsys::getGlobalCoords(const glm::dvec3& localCoords) const {
	glm::dvec3 glcoords(localCoords);
	glcoords = l2g_R * glcoords;
	glcoords += l2g_T;
	return glcoords;
}


/*

std::vector<MeshCsys*> MeshCsys::getAllParentCsys() {
	std::vector<MeshCsys*> parents;
	MeshCsys* curCsys = this;
	while (curCsys) {
		parents.insert(parents.begin(), curCsys);
		curCsys = curCsys->parentCsys;
	}
	return parents;
}

void MeshCsys::updateParents() {
	std::vector<MeshCsys*> parents = getAllParentCsys();
	for (MeshCsys* csys : parents) {
		csys->update();
	}
}

*/