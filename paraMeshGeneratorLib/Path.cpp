#include "Path.h"

double Path::pathFactor(int i, int imax) const {
	return (double)i / (double)(imax - 1);
}

glm::dvec3 Path::position(int i, int imax) const {
	return position(pathFactor(i, imax));
}

glm::dvec3 PathAxis::position(double percentage) const {
	glm::dvec3 pos(0.);
	pos[(size_t)pathDirection] = percentage * length;
	return pos;
}



glm::dvec3 PathSine::position(double percentage) const {

	double xValue = percentage * length;
	double sineValue = amplitude * glm::sin(omega * xValue);
	glm::dvec3 pos(0.);
	pos = directionX * xValue + directionY * sineValue;
	return pos;
}


glm::dvec3 PathCircular::position(double pathPercentage) const {
	double angle = pathPercentage * GLM2PI;
	return coordsOnCircleQ(angle, radius, directionX, directionY);
}
