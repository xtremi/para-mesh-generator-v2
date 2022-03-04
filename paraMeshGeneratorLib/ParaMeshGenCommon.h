#pragma once
#include <glm/glm.hpp>

enum class Dim { DIM1D, DIM2D, DIM3D };
enum class direction{x,y,z, custom};
enum class plane {xy, xz, yz, custom};

void getPlaneDirections(plane pln, direction& d1, direction& d2);
direction getPlaneNormal(plane pln);
plane getPlaneFromNormal(direction normal);
glm::dvec3 coordsOnEllipse(double angle, double rad1, double rad2, direction rotAxis);
glm::dvec3 coordsOnCircle(double angle, double radius, direction rotAxis);


glm::dvec3 coordsOnCircle(double angle, double radius, 
	const glm::dvec3& normal, const glm::dvec3& dirX);
glm::dvec3 coordsOnCircleQ(double angle, double radius,
	const glm::dvec3& dirX, const glm::dvec3& dirY);
