#include "Shapes.h"

bool limitArcAngles(ArcAngles& arcAngles, double& dang, int nnodes) {
	return limitArcAngles(arcAngles.start, arcAngles.end, dang, nnodes);
}

double Rectangle::perimeter() {
	return 2.0 * (size.x + size.y);
}
void Rectangle::elementsPerSides(int nNodesPerimeter, int& nElWidthX, int& nElHeightY) {
	double recWfactor = size.x / perimeter();
	nElWidthX = (int)(recWfactor * (double)nNodesPerimeter);
	nElHeightY = (nNodesPerimeter - 2 * nElWidthX) / 2;
}

glm::dvec2 Rectangle::getCornerCoord(int corner) {
	switch (corner)
	{
	case 1: return  0.5*size * glm::dvec2(-1.0, 1.0); break;
	case 2: return  0.5*size * glm::dvec2(1.0, 1.0); break;
	case 3: return  0.5*size * glm::dvec2(1.0, -1.0); break;
	default: return 0.5*size * glm::dvec2(-1.0, -1.0); break;
	}
}

double EllipseRadius::perimeter() const {
	double h = pow2(rad1 - rad2) / pow2(rad1 + rad2);

	double d = 10 + std::sqrt(4.0 - 3 * h);
	double A = 1 + 3 * h / d;

	double per = GLMPI * (rad1 + rad2)*A;
	return per;
}

void ArcAngles::setFullCircle() {
	start = 0.0;
	end = GLM2PI;
	m_fullCircle = true;
}

double ArcAngles::angStep(int nnodes) const {
	double dang = (end - start);
	if (!m_fullCircle)
		dang /= (double)(nnodes - 1);
	else
		dang /= (double)nnodes;
	return dang;
}

const glm::dvec2 Rectangle::recDirs[4] = {
	glm::dvec2(1.0, 0.0),
	glm::dvec2(0.0, -1.0),
	glm::dvec2(-1.0, 0.0),
	glm::dvec2(0.0, 1.0) };

glm::dvec2 RecTubeSize::calculateOuterSize(int nNodesEdge, int nPerimeterNodeLayers) {
	return inner + 2.* (double)(nPerimeterNodeLayers - 1)*inner / (double)nNodesEdge;
}
glm::dvec2 RecTubeSize::calculateOuterSize(int nNodesWidth, int nNodesHeight, int nPerimeterNodeLayers) {
	glm::dvec2 outerSize;
	outerSize.x = inner.x + 2.* (double)(nPerimeterNodeLayers - 1)*inner.x / (double)nNodesWidth;
	outerSize.y = inner.y + 2.* (double)(nPerimeterNodeLayers - 1)*inner.y / (double)nNodesHeight;
	return outerSize;
}
void RecTubeSize::setOuterSize(int nNodesEdge, int nPerimeterNodeLayers) {
	outer = calculateOuterSize(nNodesEdge, nPerimeterNodeLayers);
}
void RecTubeSize::setOuterSize(int nNodesWidth, int nNodesHeight, int nPerimeterNodeLayers) {
	outer = calculateOuterSize(nNodesWidth, nNodesHeight, nPerimeterNodeLayers);
}