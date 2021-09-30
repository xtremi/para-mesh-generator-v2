#include "MeshCsys.h"

void MeshCsys::reset() {
	hasRotToGlobal = csys ? true : false;
	local2globalT = pos; //csys ? (*csys) * pos : pos;
	local2globalR = csys ? (*csys) : glm::dmat3x3(1.0);
}

glm::dvec3 MeshCsys::getGlobalCoords(const glm::dvec3& localCoords) const {
	glm::dvec3 glcoords(localCoords);
	glcoords = local2globalR * glcoords;
	glcoords += local2globalT;
	return glcoords;
}

/*
	Go through parentCsys up to the last and inserts them in a std::vector
	last parent (global csys) is the first item in the vector
	(includes it self!)
*/
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
	parents[0]->reset();
	for (size_t i = 0; i < parents.size() - 1; i++) {
		parents[i + 1]->setParentCsys(parents[i]);
	}
}

void MeshCsys::update() {
	if (parentCsys) setParentCsys(parentCsys);
	else {
		if (csys) local2globalR = *csys;
	}
}

void MeshCsys::setParentCsys(MeshCsys* _parentCsys) {
	parentCsys = _parentCsys;
	if (parentCsys->hasRotToGlobal) {
		hasRotToGlobal = true;
	}

	if (csys && parentCsys->hasRotToGlobal) {
		local2globalR = _parentCsys->local2globalR * (*csys);
	}
	else if (!csys && parentCsys->hasRotToGlobal) {
		local2globalR = _parentCsys->local2globalR;
	}
	else if (csys) {
		local2globalR = *csys;
	}

	if (parentCsys->hasRotToGlobal) {
		local2globalT = _parentCsys->local2globalT + _parentCsys->local2globalR*pos;
	}
	else {
		local2globalT = _parentCsys->local2globalT + pos;
	}
}
