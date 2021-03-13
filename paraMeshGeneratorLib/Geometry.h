#pragma once
#include "glm/glm.hpp"

class Geometry {};

class RecGeometry2D : public Geometry {
protected:
	glm::dvec2 size;
};
class RecGeometry3D : public RecGeometry2D {
protected:
	double height;
};

class CircularGeometry2D : public Geometry {
protected:
	double radius;
};
class CircularGeometry3D : public CircularGeometry2D {
protected:
	double height;
};

class PipeGeometry2D : public CircularGeometry2D {
protected:
	double innerRadius;
};
class PipeGeometry3D : public PipeGeometry2D {
protected:
	double height;
};