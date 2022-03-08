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
	int pointIndex = 0;
	for (const Segment& segment : segments) {
		
		if (pathPercentage < segment.endFactor) {

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

