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

/*
	returns val^2
*/
double pow2(double val) {
	return std::pow(val, 2.0);
}
/*!
	returns 2^exponent
*/
int twoPow(int exponent) {
	return (int)std::pow((double)2, (double)exponent);
}

/*!
	Creates rotation matrix that defines a rotation around 
	the axis \p _rotAxis with the given \p angle.

	Based on: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
*/
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

/*!
	Creates a rotation matrix that defines a coordinate system
	with x-axis \p dirX and with \p pXY laying on the xy-plane.
*/
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

/*!
	Calculates the angle increment from one node to the next
	on an arc or circle between angle = \p startAng and 
	angle = \p endAng with \p nnodes nodes.

	If \p endAng and \p startAng are negative, the circle is considered full.
*/
double calcArcIncrement(double startAng, double endAng, int nnodes) {
	double dang;

	bool fullCircle = false;
	if (endAng < 0.0 && startAng < 0.0){
		fullCircle = true;
	}

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


/*!
	??
*/
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
	//return glm::dvec2(rad1 * glm::sin(angle), rad2 * glm::cos(angle));
	return glm::dvec2(rad1 * glm::cos(angle), rad2 * glm::sin(angle));
}

glm::dvec2 coordsOnCircleXY(double angle, double rad) {
	//return rad * glm::dvec2(glm::sin(angle), glm::cos(angle));
	return rad * glm::dvec2(glm::cos(angle), glm::sin(angle));
}


/*
	Circle in 3D given two points and normal
	https://math.stackexchange.com/a/2375120/464816
	
	Circle center given two points and normal
	https://stackoverflow.com/questions/69098266/calculate-circle-center-3d-from-2-point-arc-angle-and-plane-normal

	Circle center in 2D given two points and radius 
	https://stackoverflow.com/a/4914148/4572356

	Point on circle in 3D (given normal, radius and angle)
	https://math.stackexchange.com/a/73242/464816


*/


/*!
	Returns the center of an arc going through p1 and p2 given a radius.
	This results in two possible results.
	One solution is given with passing positive radius, the other by
	passing a negative radius.

	This is on the xy-plane. Center.z is set to 0.0.

	Based on https://stackoverflow.com/a/4914148/4572356
*/
glm::dvec3 circleCenterXY(const glm::dvec3& p1, const glm::dvec3& p2, double radius) {

	double q = glm::distance(p1, p2);
	glm::dvec3 p3 = (p1 + p2) / 2.0;		//Point in the center of p1 and p2

	double d = radius < 0. ? -1. : 1.;
	radius = std::abs(radius);

	glm::dvec3 center;
	center.x = p3.x + d * std::sqrt(pow2(radius) + pow2(q / 2.)) * (p1.y - p2.y) / q;
	center.y = p3.y + d * std::sqrt(pow2(radius) + pow2(q / 2.)) * (p2.x - p1.x) / q;
	center.z = 0.0;
	return center;
}

/*!
	Calculates and returs the center of a circle defined by
	an arc passing through p1 and p2, and the given radius, 
	on the plane with the given normal.

	Based on: https://stackoverflow.com/questions/69098266/calculate-circle-center-3d-from-2-point-arc-angle-and-plane-normal
*/
glm::dvec3 circleCenter(const glm::dvec3& p1, const glm::dvec3& p2, const glm::dvec3 normal, double radius) {

	/*
	    P2                
		x                 
		| \               dirP1P2  = direction from P1 to P2
		|   \             distP1P2 = distance from P1 to P2
		M----C (center)	  dirMC    = direction from M to C
		|   /			  distMC   = distance from  M to C
		| / 
		x
       P1	
	*/

	glm::dvec3 dirP1P2, dirMC;
	double distP1P2, distMC;

	dirP1P2	 = glm::normalize(p2 - p1);
	distP1P2 = glm::distance(p1, p2);
	dirMC	 = glm::normalize(glm::cross(normal, dirP1P2));
	distMC	 = std::sqrt(pow2(radius) - pow2(distP1P2 / 2.0));

	glm::dvec3 M = (p1 + p2) / 2.;
	glm::dvec3 center = M + distMC * dirMC;
	return center;
}


/*!
	Given a circle with center \p center returns the angle in the circle
	to the point \p p.
	This assume the x-axis is 0 deg 
	(Offset passed is added to resulting angle.)
*/
double angleOfPointOnCircle(const glm::dvec3& p, const glm::dvec3& center, double offset) {
	return std::atan2(p.y - center.y, p.x - center.x) + offset;
}


/*!
	Given an the normal and x-axis of a circle, calculates the coord
	on the circle at an angle.

	normal and dirX are normalized

	Based on: https://math.stackexchange.com/a/73242/464816
*/
glm::dvec3 coordsOnCircle(double angle, double radius,
	const glm::dvec3& normal, const glm::dvec3& dirX)
{
	glm::dvec3 dx = glm::normalize(dirX);
	glm::dvec3 n  = glm::normalize(n);
	glm::dvec3 dy = glm::cross(n, dx);
	dy = glm::normalize(dy);
	return coordsOnCircleQ(angle, radius, dx, dy);
}
/*!
	Similar to coordsOnCircle() but can be used when dirX and dirY are known.
	dirX and dirY are assumed to already be normalized

*/
glm::dvec3 coordsOnCircleQ(double angle, double radius,
	const glm::dvec3& dirX, const glm::dvec3& dirY)
{
	return radius * glm::cos(angle) * dirX + radius * glm::sin(angle) * dirY;
}
