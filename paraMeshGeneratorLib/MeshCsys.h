#pragma once
#include "math_utilities.h"
#include <vector>

/*
	Defines a mesh position and orientation:
	- pos : position of mesh
	- csys: 3x3 rotation matrix (if null, not used)
*/
struct MeshCsys {
	MeshCsys() : pos{ glm::dvec3(0.0) } {
		reset();
	}
	MeshCsys(const glm::dvec3& _pos, glm::dmat3x3* _csys = nullptr) : pos{ _pos }, csys{ _csys } {
		reset();
	}
	MeshCsys(MeshCsys* parent, const glm::dvec3& _pos = glm::dvec3(0.0), glm::dmat3x3* _csys = nullptr)
		: MeshCsys(_pos, _csys) {
		setParentCsys(parent);
	}

	void reset();
	/*
		Moves along its own axes
		(Changing pos directly moves the CSYS along the axes of parent CSYS)
	*/
	void moveInLocalCsys(const glm::dvec3& transl) {
		pos += (*csys) * transl;
	}

	glm::dvec3		pos;
	glm::dmat3x3*	csys = nullptr;

	glm::dvec3		local2globalT;
	glm::dmat3x3	local2globalR;
	bool			hasRotToGlobal = false;

	glm::dvec3		dirX() const { return csys ? (*csys)[0] : X_DIR; }
	glm::dvec3		dirY() const { return csys ? (*csys)[1] : Y_DIR; }
	glm::dvec3		dirZ() const { return csys ? (*csys)[2] : Z_DIR; }

	glm::dvec3		dirXg() const { return local2globalR[0]; }
	glm::dvec3		dirYg() const { return local2globalR[1]; }
	glm::dvec3		dirZg() const { return local2globalR[2]; }

	glm::dvec3 getGlobalCoords(const glm::dvec3& localCoords) const;

	/*
		Go through parentCsys up to the last and inserts them in a std::vector
		last parent (global csys) is the first item in the vector
		(includes it self!)
	*/
	std::vector<MeshCsys*> getAllParentCsys();
	void updateParents();
	void update();
	void setParentCsys(MeshCsys* _parentCsys);

private:
	MeshCsys* parentCsys = nullptr;
};
