#include "math_utilities.h"
#include "glm/gtc/constants.hpp"


glm::dmat3x3 makeCsysMatrix(const glm::dvec3& dirX, const glm::dvec3& pXY) {

	glm::dvec3 xAxis = glm::normalize(dirX);
	glm::dvec3 yAxis = glm::normalize(pXY);
	glm::dvec3 zAxis = glm::cross(xAxis, yAxis);
	zAxis = glm::normalize(zAxis);

	yAxis = glm::cross(zAxis, xAxis);
	yAxis = glm::normalize(yAxis);

	//glm::dmat3x3 rotMat(xAxis, yAxis, zAxis);	
	glm::dmat3x3 rotMat(
		xAxis[0], yAxis[0], zAxis[0],
		xAxis[1], yAxis[1], zAxis[1],
		xAxis[2], yAxis[2], zAxis[2]);

	return rotMat;

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


/*

_______________________________________
				   |
skipFirstRowHeight |skipFirstRowHeight
	= false		   |    = true
___________________|___________________

		 x = 0.0  ^	1elL
					1elL ^ x = 0.0
					1elL
					2elL
					2elL
					2elL
					4elL
					4elL
		 x = L  v	4elL  v x = L
*/
double initialRefinementElementSize(double totalMeshLength, int nRefinements, bool skipFirstRowHeight) {

	double denom;
	if (!skipFirstRowHeight)
		denom = 6.0 * std::pow(2.0, (double)(nRefinements - 1)) - 4.0;
	else
		denom = 3.0 * (std::pow(2.0, (double)nRefinements) - 1.0);
	return totalMeshLength / denom;
}

double calcArcIncrement(double startAng, double endAng, int nnodes) {
	double dang;

	bool fullCircle = false;
	if (endAng < 0.0 && startAng < 0.0)
		fullCircle = true;

	if (startAng < 0.0)
		startAng = 0.0;
	if (endAng < 0.0)
		endAng = 2.0*glm::pi<double>();

	dang = (endAng - startAng);
	if (!fullCircle)
		dang /= (double)(nnodes - 1);
	else
		dang /= (double)nnodes;

	return dang;
}

bool limitArcAngles(double& startAng, double& endAng, double& dang, int nnodes) {
	bool fullCircle = false;
	if (endAng < 0.0 && startAng < 0.0)
		fullCircle = true;

	if (startAng < 0.0)
		startAng = 0.0;
	if (endAng < 0.0)
		endAng = 2.0*glm::pi<double>();

	dang = (endAng - startAng);
	if (!fullCircle)
		dang /= (double)(nnodes - 1);
	else
		dang /= (double)nnodes;

	return fullCircle;
}