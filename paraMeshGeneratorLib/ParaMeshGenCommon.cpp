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

bool limitArcAngles(ArcAngles& arcAngles, double& dang, int nnodes) {
	return limitArcAngles(arcAngles.start, arcAngles.end, dang, nnodes);
}


void ArcAngles::setFullCircle(){
	start = 0.0;
	end = GLM2PI;
	m_fullCircle = true;
}

double ArcAngles::angStep(int nnodes) const {
	double dang = (end - start);
	if(!m_fullCircle)
		dang /= (double)(nnodes - 1);
	else
		dang /= (double)nnodes;
	return dang;
}

const glm::dvec2 Rectangle::recDirs[4] = {
	glm::dvec2(1.0, 0.0),
	glm::dvec2(0.0, -1.0),
	glm::dvec2(-1.0, 0.0),
	glm::dvec2(0.0, 1.0) };