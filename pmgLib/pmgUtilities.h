#pragma once
#include <vector>
#include <glm/vec3.hpp>

namespace pmg{

typedef std::vector<double>		VecD;
typedef std::vector<int>		VecI;
typedef std::vector<glm::dvec3> VecGLM3d;

int stdVecSum(const VecI& vec);

}