#pragma once
#include "pmgMath.h"
#include "pmgUtilities.h"
#include "pmgShapes.h"

namespace pmg{

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
	Path() {}
	glm::dvec3 positionI(int i, int imax, bool closedLoop = false) const;
	glm::dvec3 tangentI(int i, int imax, bool closedLoop = false) const;
	virtual glm::dvec3 position(double pathPercentage) const = 0;
	virtual glm::dvec3 tangent(double pathPercentage) const = 0;
	virtual double length() const = 0;

	static double pathFactor(int i, int imax);
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
	enum class pos_or_tangent { position, tangent };

	VecGLM3d getPathCoordsOrTangents(
		pos_or_tangent	posOrTangent,
		int				totalNodes,
		bool			closedLoop = false,
		VecI*			nodesPerSegment = nullptr) const;

};

class PathAxis : public Path {
public:
	PathAxis(direction dir, double _length) : pathDirection{ dir }, pathLength{ _length }{}
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
	double length() const { return pathLength; }
protected:
	direction pathDirection;
	double pathLength = 1.0;
};

class PathLinear : public Path {
public:
	PathLinear(const glm::dvec3& dir, double _length);
	PathLinear(const glm::dvec3& vector);

	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
	double length() const { return pathLength; }

protected:
	glm::dvec3 pathDirection;
	double pathLength = 1.0;
};

class PathSine : public Path {
public:
	PathSine(const glm::dvec3& dirX, const glm::dvec3& dirY, double _length, double amplitude, double waveLength);
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
	double length() const;

protected:
	glm::dvec3 directionX, directionY;
	double pathLength = 1.0;
	double amplitude = 1.0;
	double omega = 1.0;
};


class PathArc : public Path, public ArcAngles {
public:
	PathArc(
		double			  rad,
		const glm::dvec3& normal,
		const glm::dvec3& pXZ,
		double			  startAng = -1.,
		double			  endAng = -1.);
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
	double length() const;

protected:
	glm::dvec3 directionX, directionY;
	double radius = 1.0;
};

class PathLineStrip : public Path {
public:
	PathLineStrip(const std::vector<glm::dvec3>& _points);
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
	double length() const;

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


class PathComposite : public Path {
public:
	PathComposite(const std::vector<Path*>& _paths);
	glm::dvec3 position(double pathPercentage) const;
	glm::dvec3 tangent(double pathPercentage) const;
	double length() const;

	bool hasCornerNodes() const { return true; }
	VecD getCornerPathFactors() const;

protected:
	std::vector<Path*>   paths;
	struct PathFactor {
		double start; double end; double size() const { return end - start; }
	};
	std::vector<PathFactor> pathFactors;
};


}