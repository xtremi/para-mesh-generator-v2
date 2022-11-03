#pragma once
#include "pmgPath.h"
#include <memory>
#define pmgptr std::shared_ptr


namespace pmg {

	class Surface {
	public:
		virtual glm::dvec3 positionI(int ix, int iy, int ixmax, int iymax, bool closedLoop = false) const;
		virtual glm::dvec3 position(double pathPercentageX, double pathPercentageY) const {
			return glm::dvec3(0.);
		};

		virtual void cleanUp() {};
	};

	glm::dvec3 Surface::positionI(int ix, int iy, int ixmax, int iymax, bool closedLoop = false) const 
	{
		return position(Path::pathFactor(ix, closedLoop ? ixmax + 1 : ixmax), Path::pathFactor(iy, iymax));
	}


	/*!
		Defines a surface either:
		1) Between two non-closed paths
			- Each end points of the paths are then connected by straight lines
		2) Between two closed loops
	*/
	class BoundedSurface : public Surface {
	public:
		pmgptr<pmg::Path> outer, inner;
		bool closedLoop = false;

		void cleanUp();

		virtual glm::dvec3 positionI(int ix, int iy, int ixmax, int iymax, bool closedLoop = false) const;

	protected:
		VecGLM3d innerCoords, outDirs;
		VecD distances;
	};

	void BoundedSurface::cleanUp() {
		innerCoords.clear();
		outDirs.clear();
		distances.clear();
	}

	glm::dvec3 BoundedSurface::positionI(int ix, int iy, int ixmax, int iymax, bool closedLoop) const {
		
		getPathToPathData(inner, outer, meshDens, innerCoords, outDirs, distances, outerPathTranslation);

	}


	/*!
		Defines a volume of an extruded surface along a path 
	*/
	class ExtrudedSurface {
	public:
		BoundedSurface surface;
		pmgptr<pmg::Path> extrudePath;
	};

}