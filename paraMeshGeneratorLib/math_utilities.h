#pragma once
#include <glm/glm.hpp>

extern const double GLMPI;
extern const double GLM2PI;
extern const glm::dvec3 X_DIR;
extern const glm::dvec3 Y_DIR;
extern const glm::dvec3 Z_DIR;
extern const glm::dvec3 NULL_DIR;
extern const glm::dvec3 NULL_POS;
extern const glm::mat3x3 UNIT_MAT_3x3;



glm::dmat3x3 makeCsysMatrix(const glm::dvec3& dirX, const glm::dvec3& pXY);
glm::dmat3x3 makeCsysMatrix(const glm::dvec3& rotAxis, double angle);



/*
	returns val^2
*/
double pow2(double val);
/*
	returns 2^exponent
*/
int twoPow(int exponent);


bool limitArcAngles(double& startAng, double& endAng, double& dang, int nnodes);

double calcArcIncrement(double startAng, double endAng, int nnodes);

glm::dvec2 coordsOnEllipseXY(double angle, double rad1, double rad2);
glm::dvec2 coordsOnCircleXY(double angle, double rad);