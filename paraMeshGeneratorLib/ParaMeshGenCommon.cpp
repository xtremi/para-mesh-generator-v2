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

glm::dvec3 coordsOnCircle(double angle, double radius, direction rotAxis) {

	switch (rotAxis)
	{
	case direction::x:
		return glm::dvec3(0.0, radius * glm::sin(angle), radius * glm::cos(angle));
		break;
	case direction::y:
		return glm::dvec3(radius * glm::sin(angle), 0.0, radius * glm::cos(angle));
		break;
	case direction::z:
		return glm::dvec3(radius * glm::sin(angle), radius * glm::cos(angle), 0.0);;
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

