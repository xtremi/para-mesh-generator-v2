#pragma once
#include "pmgPath.h"
#include <memory>
#define pmgptr std::shared_ptr


namespace pmg {
	class Surface;

	class SurfaceData {
	protected:
		SurfaceData(int _ixmax, int _iymax, bool _closedLoop) 
			: ixmax{_ixmax}, iymax{_iymax}, closedLoop{_closedLoop}	{}
		friend class Surface;
		int ixmax, iymax;
		bool closedLoop;
	};

	class Surface {
	public:
		virtual glm::dvec3 positionI(int ix, int iy, SurfaceData* surfaceData) const;
		virtual glm::dvec3 position(double pathPercentageX, double pathPercentageY, SurfaceData* surfaceData) const {
			return glm::dvec3(0.);
		};

		virtual SurfaceData* init(int ixmax, int iymax, bool closedLoop = false) { 
			return new SurfaceData(ixmax, iymax, closedLoop);
		}
		virtual void cleanUp(SurfaceData* surfaceData) {};
	};



	class BoundedSurface;
	class BoundedSurfaceData : private SurfaceData {
	protected:
		BoundedSurfaceData(int _ixmax, int _iymax, bool _closedLoop) : SurfaceData(_ixmax, _iymax, _closedLoop){}
		friend class BoundedSurface;
		VecGLM3d innerCoords, outDirs;
		VecD distances;
	};

	/*!
		Defines a surface either:
		1) Between two non-closed paths
			- Each end points of the paths are then connected by straight lines
		2) Between two closed loops
	*/
	class BoundedSurface : public Surface {
	public:
		pmgptr<pmg::Path> outer, inner;
		glm::dvec3 outerTranslation;
		bool closedLoop = false;

		SurfaceData* init(int ixmax, int iymax, bool closedLoop = false);
		virtual void cleanUp(SurfaceData* surfaceData);

		virtual glm::dvec3 positionI(int ix, int iy, SurfaceData* surfaceData) const;
	};

	/*
		Base class for function based surfaces
		Doesn't really need a specialization of SurfaceData
	*/
	class MathFunctionSurface : public Surface {
		virtual glm::dvec3 position(double pathPercentageX, double pathPercentageY, SurfaceData* surfaceData) const = 0;
	};

	class SinCosWavesSurface : public MathFunctionSurface {
	public:
		SinCosWavesSurface(double _ampX, double _ampY, double _waveLengthX, double _waveLengthY, double _sizeX, double _sizeY) :
			ampX{ _ampX }, ampY{ _ampY }, waveLengthX{ _waveLengthX }, waveLengthY{ _waveLengthY }, sizeX{ _sizeX }, sizeY{ _sizeY }{}
		glm::dvec3 position(double pathPercentageX, double pathPercentageY, SurfaceData* surfaceData) const;
	public:
		double ampX, ampY;
		double waveLengthX, waveLengthY;
		double sizeX, sizeY;
	};

	/*!
		Defines a volume of an extruded surface along a path 
	*/
	class ExtrudedSurface {
	public:
		BoundedSurface surface;
		pmgptr<pmg::Path> extrudePath;
	};


	void getPathToPathData(
		const Path& innerPath,
		const Path& outerPath,
		int	  nNodesPathDir,
		bool  closedLoopPathDir,
		VecGLM3d& innerCoords,
		VecGLM3d& outDirections,
		VecD& distances,
		const glm::dvec3& outerPathTranslation = NULL_POS);

}