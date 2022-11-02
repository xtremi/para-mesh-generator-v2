#include "pmgPath.h"
#include <algorithm> // std::min_element

using namespace pmg;

/*!
	This is currently used for PathLineStrip (but could have more use later. TBD.)

	Given a set of normalized locations (\p requiredNodeLocations) (along a path for example), between 0.0 and 1.0:

  [0.0]  0.15       0.4   0.5                   1.0
	x--N1-x----N2----x--N3-x---------N4----------x
		NOTE: The location 0.0 should not be included in \p requiredNodeLocations


	and a number of nodes \totalNodes, this function calculates how many nodes
	should be between each locations (segments), and what the spacing between each
	nodes within each segment should be, for correctly distributing the nodes
	as evenly as possibly along the path (path of segment).

	The returned size of \p nodesPerSegment and \p nodeSpacingPerSegment vectors
	are the same size as \p requiredLocations
*/
bool pmg::calculateNodeSpacing(
	int			totalNodes				/*!Total number of nodes*/,
	const VecD& requiredLocations		/*!Required locations [0.0 -> 1.0] where a nodes is required*/,
	VecI&		nodesPerSegment			/*!Calculated number of nodes between each req. locations (segment)*/,
	VecD&		nodeSpacingPerSegemnt	/*!Spacing between nodes for segment*/,
	bool		closedLoop				/*!Will account for a closed loop*/)
{
	if (requiredLocations.size() == 0) {
		return true;
	}

	double dpath = 0.;				//estimated spacing between nodes based on rest of path length
	int	   elCount = 0;				//element counter 
	double curPathLoc = 0.0;		//the current location (0.0 -> 1.0) at the iteration
	double endLoc = 1.0;			//the normalized end location of the path
	double remainder = 0.0;			//remainer of rounding of flooring or rounding the number of elements along a segment
	int	   nElseg;					//number of elements on segment at the iteration
	int	   totalElements = totalNodes - 1;

	if (closedLoop) {
		endLoc -= (1.0 / (totalElements + 1));	//if closeLoop we stop a step a before
	}

	for (int j = 0; j < requiredLocations.size(); j++) {

		double pathRemaining = (endLoc - curPathLoc);
		int    elementsRemaining = totalElements - elCount;

		dpath = pathRemaining / (double)elementsRemaining;			//Approximate the path step size

		double segmentLength = requiredLocations[j] - curPathLoc;

		/*Switch between rounding and flooring the number of elements based on remainer from previous iteration*/
		if (remainder <= 0.0) {
			nElseg = (int)std::round(segmentLength / dpath);				//number of element that fit on segment with dpath size
		}
		else {
			nElseg = (int)(segmentLength / dpath);
		}
		remainder = segmentLength / dpath - (double)nElseg;


		if (nElseg == 0) nElseg = 1; //cannot have 0 elements

		double actualDpath = segmentLength / (double)nElseg; //actual normalized dpath with nElSeg on segment

		curPathLoc += (double)nElseg * actualDpath;
		elCount += nElseg;
		nodesPerSegment.push_back(nElseg);
		nodeSpacingPerSegemnt.push_back(actualDpath);
	}
	/*
		If there is a remaining node to add (so that total is equal to the required total)
		this will find the segment that has the largest node spacing,
		add a node and calculate the new spacing.
	*/
	int numberOfAdditonalNodes = totalNodes - elCount - 1;
	for (int i = 0; i < numberOfAdditonalNodes; i++) {
		auto it = std::max_element(nodeSpacingPerSegemnt.begin(), nodeSpacingPerSegemnt.end());
		size_t index = it - nodeSpacingPerSegemnt.begin();
		double segmentSize = nodeSpacingPerSegemnt[index] * (double)nodesPerSegment[index];
		double newSpacing = segmentSize / ((double)nodesPerSegment[index] + 1);
		nodesPerSegment[index]++;
		nodeSpacingPerSegemnt[index] = newSpacing;
		elCount++;
	}
	if (!closedLoop) {
		nodesPerSegment[nodesPerSegment.size() - 1] += 1;
	}
	if (closedLoop) totalElements++;	//there is actually one more element if closeLoop
	if (elCount != totalElements) {
		int j = 0;
		//throw("Not expected number of nodes in calculateNodeSpacing()");
	}

	return true;
}

/*!
	Returns a vector of the coordinates or tangents of each
	node along a path.
*/
VecGLM3d Path::getPathCoordsOrTangents(
	pos_or_tangent	posOrTangent,
	int				totalNodes,
	bool			closedLoop,
	VecI*			nodesPerSegmentOut) const
{
	VecGLM3d result;
	if (hasCornerNodes()) {
		VecD pathCorner = getCornerPathFactors();
		VecI nodesPerSegment;
		VecD nodeSpacing;
		calculateNodeSpacing(totalNodes, pathCorner, nodesPerSegment, nodeSpacing, closedLoop);

		double currentPathLoc = 0.0;
		for (int j = 0; j < nodesPerSegment.size(); j++) {
			for (int i = 0; i < nodesPerSegment[j]; i++) {
				if (posOrTangent == pos_or_tangent::position) {
					result.push_back(position(currentPathLoc));
				}
				else {
					result.push_back(tangent(currentPathLoc));
				}
				currentPathLoc += nodeSpacing[j];
			}
		}
		if (nodesPerSegmentOut) {
			*nodesPerSegmentOut = nodesPerSegment;
		}
	}
	else {
		for (int i = 0; i < totalNodes; i++) {
			if (posOrTangent == pos_or_tangent::position) {
				result.push_back(positionI(i, closedLoop ? totalNodes + 1 : totalNodes));
			}
			else {
				result.push_back(tangentI(i, closedLoop ? totalNodes + 1 : totalNodes));
			}
		}
	}

	return result;
}

/*!
	A path defines the positions along some function.
	The positions along the path can be retrieved from path::position.
	If a path has "corners" (for example when being a line strip), or has certain locations that are important,
	the normalized positions can be retreived by path::getCornerPathFactors().

	When distributing nodes along a path, we may want to be sure nodes are located at these corners.
	This function will return a VecGLM3d of locations along the path, and make sure there
	are nodes at the corner locations if the path has said corners. Between corners, nodes are evenly distributed.
	If the path does not have corner nodes, the nodes are distributed evenly.

	See also calculateNodeSpacing() which is used here to calculate the spacing between nodes for each segments
	between corners.
*/
VecGLM3d Path::getPathCoordinates(
	int			totalNodes			/*!The total number of nodes along the path*/,
	bool		closedLoop			/*!True if this the path is a closedLoop*/,
	VecI*		nodesPerSegmentOut	/*![out][optional] if passed, the nodes per segment will be set to this vector*/) const
{
	return getPathCoordsOrTangents(pos_or_tangent::position, totalNodes, closedLoop, nodesPerSegmentOut);
}

VecGLM3d Path::getPathTangents(
	int			totalNodes			/*!The total number of nodes along the path*/,
	bool		closedLoop			/*!True if this the path is a closedLoop*/,
	VecI*		nodesPerSegmentOut	/*![out][optional] if passed, the nodes per segment will be set to this vector*/) const
{
	return getPathCoordsOrTangents(pos_or_tangent::tangent, totalNodes, closedLoop, nodesPerSegmentOut);
}

/*!
	See also the other getPathCoordinates() for more details.
	In this function, if the Path has corner nodes, node coordinates at these
	corners will be included in the returned coordinate vector.
	Between each corner coordinates, the number of nodes are determined by \p nodesPerSegment.
	Thus, the distance between nodes are not necessarily evenly spaced.

	If there are not corner nodes, and nodesPerSegment is empty. \p numberOfNodes will
	be used.
*/
VecGLM3d Path::getPathCoordinates(
	const VecI& nodesPerSegment,
	int			numberOfNodes,
	bool		closedLoop) const
{
	VecGLM3d coordinates;
	if (hasCornerNodes()) {
		VecD corners = getCornerPathFactors();
		if (corners.size() != nodesPerSegment.size()) {
			throw("Number of corners not equal number of segments in getPathCoordinates()");
		}

		double currentLoc = 0.0, dloc;
		for (int i = 0; i < corners.size(); i++) {
			dloc = (corners[i] - currentLoc) / (double)(nodesPerSegment[i]);
			for (int j = 0; j < nodesPerSegment[i]; j++) {
				coordinates.push_back(position(currentLoc));
				currentLoc += dloc;
			}
		}
	}
	else {
		int totalNodes = stdVecSum(nodesPerSegment);
		if (totalNodes == 0) {
			totalNodes = numberOfNodes;
		}
		for (int i = 0; i < totalNodes; i++) {
			coordinates.push_back(positionI(i, closedLoop ? totalNodes + 1 : totalNodes));
		}
	}

	return coordinates;
}


double Path::pathFactor(int i, int imax) const {
	return (double)i / (double)(imax - 1);
}

glm::dvec3 Path::positionI(int i, int imax, bool closedLoop) const {
	return position(pathFactor(i, closedLoop ? imax + 1 : imax));
}
glm::dvec3 Path::tangentI(int i, int imax, bool closedLoop) const {
	return tangent(pathFactor(i, closedLoop ? imax + 1 : imax));
}

/*PathAxis*/
glm::dvec3 PathAxis::position(double percentage) const {
	glm::dvec3 pos(0.);
	pos[(size_t)pathDirection] = percentage * pathLength;
	return pos;
}
glm::dvec3 PathAxis::tangent(double pathPercentage) const {
	switch (pathDirection)
	{
	case direction::x: return X_DIR;
	case direction::y: return Y_DIR;
	case direction::z: return Z_DIR;
	default: throw("invalid direction in PathAxis::tangent"); break;
	}
}

/*PathLinear*/
PathLinear::PathLinear(const glm::dvec3& dir, double _length)
	: pathDirection{ glm::normalize(dir) }, pathLength{ _length }{}
PathLinear::PathLinear(const glm::dvec3& vector)
	: pathDirection{ glm::normalize(vector) }, pathLength{ glm::length(vector) }{}
glm::dvec3 PathLinear::position(double pathPercentage) const {
	return pathDirection * pathLength * pathPercentage;
}
glm::dvec3 PathLinear::tangent(double pathPercentage) const {
	return pathDirection;
}

/*PathSine*/
PathSine::PathSine(const glm::dvec3& dirX, const glm::dvec3& dirY, double _length, double amplitude, double waveLength)
	: directionX{ dirX }, directionY{ dirY }, pathLength{ _length }, amplitude{ amplitude }
{
	omega = GLM2PI / waveLength;
	glm::dvec3 dirZ = glm::normalize(glm::cross(directionX, directionY));
	directionY = glm::normalize(glm::cross(dirZ, directionX));
}

glm::dvec3 PathSine::position(double percentage) const {
	double xValue = percentage * pathLength;
	double sineValue = amplitude * glm::sin(omega * xValue);
	glm::dvec3 pos(0.);
	pos = directionX * xValue + directionY * sineValue;
	return pos;
}
glm::dvec3 PathSine::tangent(double percentage) const {
	double xValue = percentage * pathLength;
	double sineTangentValue = omega * amplitude * glm::cos(omega * xValue);

	glm::dvec3 t(0.);
	t = glm::normalize(directionX + directionY * sineTangentValue);
	return t;
}
/*really bad approximation*/
double PathSine::length() const {
	double waveLength = GLM2PI / omega;
	double nWaves = pathLength / waveLength;
	return 2. * amplitude * nWaves + pathLength;
}


/*PathCircular*/
PathArc::PathArc(
	double			  rad		/*!Circle radius*/,
	const glm::dvec3& normal	/*!Circle normal*/,
	const glm::dvec3& pXZ		/*!Point on XZ plane (Z is the normal)*/,
	double			  startAng	/*!Start angle (if startAng and endAng is negative, its a full circle)*/,
	double			  endAng	/*!End angle (if startAng and endAng is negative, its a full circle)*/)
	: radius{ rad }
{
	//xyzxyz
	glm::dvec3 directionZ = glm::normalize(normal);
	directionY = glm::normalize(glm::cross(normal, glm::normalize(pXZ)));
	directionX = glm::normalize(glm::cross(directionY, normal));

	start = startAng;
	end = endAng;
	m_fullCircle = false;
	if (start < 0.0 && end < 0.0) {
		setFullCircle();
	}
	if (start < 0.0) start = 0.0;
	if (end < 0.0) end = GLM2PI;
}

glm::dvec3 PathArc::position(double pathPercentage) const {
	double angle = pathPercentage * this->angleSize();
	return coordsOnCircleQ(angle, radius, directionX, directionY);
}
glm::dvec3 PathArc::tangent(double pathPercentage) const {
	double angle = pathPercentage * GLM2PI;
	return tangentOnCircleQ(angle, radius, directionX, directionY);
}
double PathArc::length() const {
	return radius * std::abs(end - start);
}



/*PathLineStrip*/
PathLineStrip::PathLineStrip(const std::vector<glm::dvec3>& _points)
	: points{ _points }
{
	totalLength = 0.;
	Segment currentsegment;
	for (int i = 1; i < points.size(); i++) {
		currentsegment.length = glm::distance(points[i], points[i - 1]);
		currentsegment.direction = glm::normalize(points[i] - points[i - 1]);
		totalLength += currentsegment.length;
		segments.push_back(currentsegment);
	}

	double lengthAtSegmentStart = 0.;
	for (Segment& segment : segments) {
		segment.segmentFactor = segment.length / totalLength;
		segment.startFactor = lengthAtSegmentStart / totalLength;
		lengthAtSegmentStart += segment.length;
		segment.endFactor = lengthAtSegmentStart / totalLength;
	}

}

glm::dvec3 PathLineStrip::position(double pathPercentage) const
{
	if (pathPercentage < 0.0) pathPercentage = 0.0;
	if (pathPercentage > 1.0) pathPercentage = 1.0;

	int pointIndex = 0;
	for (const Segment& segment : segments) {

		if (pathPercentage <= segment.endFactor) {

			double factorOfCurrentPath = pathPercentage - segment.startFactor;
			factorOfCurrentPath /= segment.segmentFactor;

			glm::dvec3 segmentStart = points[pointIndex];
			glm::dvec3 pos = segmentStart + segment.direction * segment.length * factorOfCurrentPath;
			return pos;
			break;
		}
		pointIndex++;
	}
}
glm::dvec3 PathLineStrip::tangent(double pathPercentage) const
{
	if (pathPercentage < 0.0) pathPercentage = 0.0;
	if (pathPercentage > 1.0) pathPercentage = 1.0;

	int pointIndex = 0;
	for (const Segment& segment : segments) {

		if (pathPercentage <= segment.endFactor) {
			return segment.direction;
		}
		pointIndex++;
	}
}
double PathLineStrip::length() const {
	return totalLength;
}



VecD PathLineStrip::getCornerPathFactors() const {
	VecD cornerDist;
	for (int i = 1; i < segments.size(); i++) {
		cornerDist.push_back(segments[i].startFactor);
	}
	cornerDist.push_back(1.0);
	return cornerDist;
}


PathComposite::PathComposite(const std::vector<Path*>& _paths) {
	paths = _paths;
	double totalLength = length();

	double currentPathStartFactor = 0.;
	double currentLength = 0.;

	for (const Path* path : paths) {
		PathFactor pathPart;
		pathPart.start = currentPathStartFactor;

		double pathLength = path->length();
		currentLength += pathLength;
		currentPathStartFactor = (currentLength / totalLength);
		pathPart.end = currentPathStartFactor;

		pathFactors.push_back(pathPart);
	}
}
glm::dvec3 PathComposite::position(double pathPercentage) const {
	if (pathPercentage < 0.0) pathPercentage = 0.0;
	if (pathPercentage > 1.0) pathPercentage = 1.0;

	int pathIndex = 0;
	glm::dvec3 currentPosition(0.);
	for (const PathFactor& pathFactor : pathFactors) {
		currentPosition -= paths[pathIndex]->position(0.); //For handling circles that have their position defined by center
		if (pathPercentage <= pathFactor.end) {
			double factorOfCurrentPath = pathPercentage - pathFactor.start;
			factorOfCurrentPath /= pathFactor.size();
			return paths[pathIndex]->position(factorOfCurrentPath) + currentPosition;
			break;
		}
		currentPosition += paths[pathIndex]->position(1.);

		pathIndex++;
	}
}
glm::dvec3 PathComposite::tangent(double pathPercentage) const {
	if (pathPercentage < 0.0) pathPercentage = 0.0;
	if (pathPercentage > 1.0) pathPercentage = 1.0;

	int pathIndex = 0;
	for (const PathFactor& pathFactor : pathFactors) {
		if (pathPercentage <= pathFactor.end) {
			double factorOfCurrentPath = pathPercentage - pathFactor.start;
			factorOfCurrentPath /= pathFactor.size();
			return paths[pathIndex]->tangent(factorOfCurrentPath);
			break;
		}
		pathIndex++;
	}
}

VecD PathComposite::getCornerPathFactors() const {
	VecD cornerPathFactors;
	for (const PathFactor& pathFactor : pathFactors) {
		cornerPathFactors.push_back(pathFactor.end);
	}
	return cornerPathFactors;
}
double PathComposite::length() const {
	double totalLength = 0.;
	for (const Path* path : paths) {
		totalLength += path->length();
	}
	return totalLength;
}





