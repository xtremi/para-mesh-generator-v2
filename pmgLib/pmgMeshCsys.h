#pragma once
#include <glm/glm.hpp>

namespace pmg{

class MeshCsys {
public:
	glm::vec3 getGlobalCoords(const glm::dvec3& localCoords) const {
		return localCoords;
	}
};

}