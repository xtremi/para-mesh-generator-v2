#include "pmgSurface.h"
using namespace pmg;

glm::dvec3 Surface::positionI(int ix, int iy, SurfaceData* sd) const {
	return position(
		Path::pathFactor(ix, sd->closedLoop ? sd->ixmax + 1 : sd->ixmax),
		Path::pathFactor(iy, sd->iymax),
		sd);
}

SurfaceData* BoundedSurface::init(int ixmax, int iymax, bool closedLoop) {
	BoundedSurfaceData* bsd = new BoundedSurfaceData(ixmax, iymax, closedLoop);
	getPathToPathData(*inner, *outer, 
		bsd->ixmax, bsd->closedLoop, 
		bsd->innerCoords, bsd->outDirs, bsd->distances, 
		outerTranslation);
	return bsd;
}

void BoundedSurface::cleanUp(SurfaceData* sd) {
	((BoundedSurfaceData*)sd)->distances.clear();
	((BoundedSurfaceData*)sd)->outDirs.clear();
	((BoundedSurfaceData*)sd)->innerCoords.clear();
	delete sd;
}

glm::dvec3 BoundedSurface::positionI(int ix, int iy, SurfaceData* sd) const {
	BoundedSurfaceData* bsd = (BoundedSurfaceData*)sd;
	double distanceFactor = Path::pathFactor(iy, bsd->iymax);
	return bsd->innerCoords[ix] + distanceFactor * bsd->distances[ix] * bsd->outDirs[ix];
}


glm::dvec3 SinCosWavesSurface::position(double pathPercentageX, double pathPercentageY, SurfaceData* surfaceData) const {
	glm::dvec3 coords(
		-sizeX / 2. + sizeX * pathPercentageX,
		-sizeY / 2. + sizeY * pathPercentageY,
		0.0);
	
	double omegaX = GLM2PI / waveLengthX;
	double omegaY = GLM2PI / waveLengthY;
	coords.z = 
		ampX * glm::sin(omegaX * coords.x) +
		ampY * glm::cos(omegaY * coords.y);
	return coords;
}

void pmg::getPathToPathData(
	const Path& innerPath					/*![in] The inner path*/,
	const Path& outerPath					/*![in] The outer path*/,
	int	  nNodesPathDir						/*![in] number of nodes along the paths*/,
	bool  closedLoopPathDir					/*![in] if the paths defines a closed loop*/,
	VecGLM3d& innerCoords					/*![out] The coordinates along innerPath*/,
	VecGLM3d& outDirections					/*![out] The directions from innerPath to outerPath*/,
	VecD& distances							/*![out] The distances between the coords on innerPath and outerPath*/,
	const glm::dvec3& outerPathTranslation	/*![in][optional] Is added to the coordinates of outerPath*/)
{
	VecI nodesPerSegmentInner;
	VecGLM3d outerCoords;
	innerCoords = innerPath.getPathCoordinates(nNodesPathDir, closedLoopPathDir, &nodesPerSegmentInner);
	outerCoords = outerPath.getPathCoordinates(nodesPerSegmentInner, nNodesPathDir, closedLoopPathDir);

	for (int i = 0; i < nNodesPathDir; i++) {
		outerCoords[i] += outerPathTranslation;
		outDirections.push_back(glm::normalize(outerCoords[i] - innerCoords[i]));
		distances.push_back(glm::distance(outerCoords[i], innerCoords[i]));
	}
}

//SurfaceData* MathFunctionSurface::init(int ixmax, int iymax, bool closedLoop) {
//	return Surface::init(ixmax, iymax, closedLoop);
//}
//void MathFunctionSurface::cleanUp(SurfaceData* surfaceData) {
//	Surface::cleanUp(surfaceData);
//}

