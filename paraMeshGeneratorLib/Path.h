#pragma once
#include "math_utilities.h"
#include "ParaMeshGenCommon.h"
#include "general_utilities.h"
#include <vector>

class Path
{
public:
	Path(){}
	glm::dvec3 position(int i, int imax) const;
	virtual glm::dvec3 position(double pathPercentage) const = 0;

	double pathFactor(int i, int imax) const;
};

class PathAxis : public Path{
public:	
	PathAxis(direction dir, double _length) : pathDirection{ dir }, length{ _length }{}
	glm::dvec3 position(double pathPercentage) const;
protected:
	direction pathDirection;
	double length = 1.0;
};

class PathSine : public Path {
public:
	PathSine(const glm::dvec3& dirX, const glm::dvec3& dirY, double _length, double amplitude, double waveLength) 
		: directionX{ dirX }, directionY{ dirY }, length{ _length }, amplitude{ amplitude }
	{
		omega = GLM2PI / waveLength;
		glm::dvec3 dirZ = glm::normalize(glm::cross(directionX, directionY));
		directionY = glm::normalize(glm::cross(dirZ, directionX));
	}
	glm::dvec3 position(double pathPercentage) const;
protected:
	glm::dvec3 directionX, directionY;
	double length = 1.0;
	double amplitude = 1.0;
	double omega = 1.0;
};


class PathCircular : public Path {
public:
	PathCircular(double _radius, const glm::dvec3& normal, const glm::dvec3& dirX)
		: radius{ _radius }, directionX{ dirX }
	{	
		directionY = glm::normalize(glm::cross(normal, directionX));
		directionX = glm::normalize(glm::cross(normal, directionY));
	}
	glm::dvec3 position(double pathPercentage) const;
protected:
	glm::dvec3 directionX, directionY;
	double radius = 1.0;
};

class PathLineStrip : public Path {
public:
	PathLineStrip(const std::vector<glm::dvec3>& _points);
	glm::dvec3 position(double pathPercentage) const;

	VecGLM3d getCornerDistances();

protected:
	struct Segment {
		glm::dvec3 direction;		/*directions of segment*/
		double	   startFactor;		/*percentage of path at start point*/
		double	   endFactor;		/*percentage of path at end point*/
		double	   segmentFactor;	/*normalized segment size*/
		double	   length;			/*segment length*/
	};

	std::vector<glm::dvec3> points;					/*coordinates of points on line-strip*/
	std::vector<Segment>    segments;
	double					totalLength;
};

