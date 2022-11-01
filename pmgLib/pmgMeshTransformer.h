#pragma once
#include <glm/glm.hpp>

namespace pmg {
	class MeshTransformer {
	public:
		glm::dvec3 getTransformedCoords(const glm::dvec3& coords) const {
			return coords;
		}
	};

}
