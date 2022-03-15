#pragma once
#include "math_utilities.h"
#include "ParaMeshGenCommon.h"
#include "general_utilities.h"
#include <vector>

class Path;
bool calculateNodeSpacing(
	int			totalNodes,
	const VecD& requiredLocations, 
	VecI&		nodesPerSegment, 
	VecD&		nodeSpacingPerSegemnt,
	bool		closedLoop = false);



class Path
{
public:
	Path(){}
	glm::dvec3 position(int i, int imax) const;
	glm::dvec3 tangent(int i, int imax) const;
	virtual glm::dvec3 position(double pathPercentage) const = 0;
	virtual glm::dvec3 tangent(double pathPercentage) const = 0;

	double pathFactor(int i, int imax) const;
	virtual bool hasCornerNodes() const { return false; }
	virtual VecD getCornerPathFactors() const { return VecD(); }

	VecGLM3d getPathCoordinates(
		int			totalNodes,
		bool		closedLoop = false,
		VecI*		nodesPerSegment = nullptr) const;

	VecGLM3d getPathTangents(
		int			totalNodes,
		bool		closedLoop = false,
		VecI*		nodesPerSegment = nullptr) const;

	VecGLM3d getPathCoordinates(
		const VecI& nodesPerSegment,
		int			numberOfNodes,
		bool		closedLoops) const;

private:
	enum class pos_or_tangent{position, tangent};

	VecGLM3d getPathCoordsOrTangents(
		pos_or_tangent	posOrTangent,
		int				totalNodes,
		bool			closedLoop = false,
		VecI*			nodesPerSegment = nullptr) const;

};

class PathAxis : public Path{
public:	
	PathAxis(direction dir, double _length) : pathDirection{ dir }, length{ _length }{}
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
protected:
	direction pathDirection;
	double length = 1.0;
};

class PathLinear : public Path {
public:
	PathLinear(const glm::dvec3& dir, double _length) : pathDirection{ glm::normalize(dir) }, length{ _length }{}
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
protected:
	glm::dvec3 pathDirection;
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
	glm::dvec3 tangent(double pathPercentage) const;

protected:
	glm::dvec3 directionX, directionY;
	double length = 1.0;
	double amplitude = 1.0;
	double omega = 1.0;
};


class PathCircular : public Path {
public:
	PathCircular(double _radius, const glm::dvec3& normal, const glm::dvec3& pXZ)
		: radius{ _radius }
	{	
		glm::dvec3 directionZ = glm::normalize(normal);
		directionY = glm::normalize(glm::cross(normal, glm::normalize(pXZ)));
		directionX = glm::normalize(glm::cross(normal, directionY));
	}
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
protected:
	glm::dvec3 directionX, directionY;
	double radius = 1.0;
};

class PathLineStrip : public Path {
public:
	PathLineStrip(const std::vector<glm::dvec3>& _points);
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;

	bool hasCornerNodes() const { return true; }
	VecD getCornerPathFactors() const;

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

