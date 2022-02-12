#include "math_utilities.h"
#include "glm/gtc/constants.hpp"

const double GLMPI = glm::pi<double>();
const double GLM2PI = glm::pi<double>()*2.0;
const glm::dvec3 X_DIR = glm::dvec3(1.0, 0.0, 0.0);
const glm::dvec3 Y_DIR = glm::dvec3(0.0, 1.0, 0.0);
const glm::dvec3 Z_DIR = glm::dvec3(0.0, 0.0, 1.0);
const glm::dvec3 NULL_DIR = glm::dvec3(0.0, 0.0, 0.0);
const glm::dvec3 NULL_POS = NULL_DIR;
const glm::mat3x3 UNIT_MAT_3x3 = glm::dmat3x3(1.0);

double pow2(double val) {
	return std::pow(val, 2.0);
}
int twoPow(int exponent) {
	return (int)std::pow((double)2, (double)exponent);
}

//https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
glm::dmat3x3 makeCsysMatrix(const glm::dvec3& _rotAxis, double angle) {

	glm::dvec3 rotAxis = glm::normalize(_rotAxis);
	
	double cosA = glm::cos(angle);
	double sinA = glm::sin(angle);
	double rx = rotAxis.x;
	double ry = rotAxis.y;
	double rz = rotAxis.z;
	double rxx = rx * rx;
	double ryy = ry * ry;
	double rzz = rz * rz;
	double rxy = rx * ry;
	double rxz = rx * rz;
	double ryz = ry * rz;

	glm::mat3x3 rotMat(
		cosA + rxx * (1. - cosA),			rxy * (1. - cosA) - rz * sinA,		rxz * (1. - cosA) + ry * sinA,
		rxy * (1. - cosA) + rz * sinA,		cosA + ryy * (1. - cosA),			ryz*(1. - cosA) - rx * sinA,
		rxz * (1. - cosA) - ry * sinA,		ryz*(1. - cosA) + rx * sinA,		cosA + rzz * (1. - cosA)
	);
	return rotMat;
}

glm::dmat3x3 makeCsysMatrix(const glm::dvec3& dirX, const glm::dvec3& pXY) {

	glm::dvec3 xAxis = glm::normalize(dirX);
	glm::dvec3 yAxis = glm::normalize(pXY);
	glm::dvec3 zAxis = glm::cross(xAxis, yAxis);
	zAxis = glm::normalize(zAxis);

	yAxis = glm::cross(zAxis, xAxis);
	yAxis = glm::normalize(yAxis);

	glm::dmat3x3 rotMat(xAxis, yAxis, zAxis);	
	return rotMat;

}






double calcArcIncrement(double startAng, double endAng, int nnodes) {
	double dang;

	bool fullCircle = false;
	if (endAng < 0.0 && startAng < 0.0)
		fullCircle = true;

	if (startAng < 0.0) {
		startAng = 0.0;
	}
	if (endAng < 0.0){
		endAng = 2.0*glm::pi<double>();
	}

	dang = (endAng - startAng);
	if (!fullCircle){
		dang /= (double)(nnodes - 1);
	}
	else{
		dang /= (double)nnodes;
	}

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

glm::dvec2 coordsOnEllipseXY(double angle, double rad1, double rad2) {
	return glm::dvec2(rad1 * glm::sin(angle), rad2 * glm::cos(angle));
}

glm::dvec2 coordsOnCircleXY(double angle, double rad) {
	return rad * glm::dvec2(glm::sin(angle), glm::cos(angle));
}