#pragma once
#include <glm/glm.hpp>
#include "ParaMeshGenCommon.h"

extern const double GLMPI;
extern const double GLM2PI;


glm::dmat3x3 makeCsysMatrix(const glm::dvec3& dirX, const glm::dvec3& pXY);

glm::dvec3 coordsOnCircle(double angle, double radius, direction rotAxis);

double initialRefElSize2D(double totalMeshLength, int nRefinements, bool skipFirstRowHeight);
double initialRefElSize3D(double totalMeshLength, int nRefinements, bool skipFirstRowHeight);

double pow2(double val);

bool limitArcAngles(double& startAng, double& endAng, double& dang, int nnodes);
//bool limitArcAngles(ArcAngles& arcAngles, double& dang, int nnodes);
double calcArcIncrement(double startAng, double endAng, int nnodes);