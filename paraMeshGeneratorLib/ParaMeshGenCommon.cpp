#include "ParaMeshGenCommon.h"
#include <glm/gtc/constants.hpp>
#include "math_utilities.h"

direction getPlaneNormal(plane pln) {
	switch (pln) {
	case plane::xy:
		return direction::z; break;
	case plane::xz:
		return direction::y; break;
	case plane::yz:
	default:
		return direction::x; break;
	}
}

plane getPlaneFromNormal(direction normal) {
	switch (normal)
	{
	case direction::x: return plane::yz; break;
	case direction::y: return plane::xz; break;
	case direction::z: 
	default:
		return plane::xy; break;
	}
}

void getPlaneDirections(plane pln, direction& d1, direction& d2){
	
	switch (pln) {
	case plane::xy:
		d1 = direction::x;
		d2 = direction::y;
		break;
	case plane::xz:
		d1 = direction::x;
		d2 = direction::z;
		break;
	case plane::yz:
		d1 = direction::y;
		d2 = direction::z;
		break;
	default:
		break;
	}
}

glm::dvec3 coordsOnEllipse(double angle, double rad1, double rad2, direction rotAxis){
	glm::dvec2 locCoords = coordsOnEllipseXY(angle, rad1, rad2);
	switch (rotAxis)
	{
	case direction::x:
		return glm::dvec3(0.0, locCoords.x, locCoords.y);
		break;
	case direction::y:
		return glm::dvec3(locCoords.x, 0.0, locCoords.y);
		break;
	case direction::z:
		return glm::dvec3(locCoords.x, locCoords.y, 0.0);;
		break;
	default:
		return glm::dvec3(0.0);
		break;
	}
}


glm::dvec3 coordsOnCircle(double angle, double radius, direction rotAxis) {
	glm::dvec2 locCoords = coordsOnCircleXY(angle, radius);
	switch (rotAxis)
	{
	case direction::x:
		return glm::dvec3(0.0, locCoords.x, locCoords.y);
		break;
	case direction::y:
		return glm::dvec3(locCoords.x, 0.0, locCoords.y);
		break;
	case direction::z:
		return glm::dvec3(locCoords.x, locCoords.y, 0.0);;
		break;
	default:
		return glm::dvec3(0.0);
		break;
	}
}

