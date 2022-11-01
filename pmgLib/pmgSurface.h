#pragma once
#include "pmgPath.h"
#include <memory>
#define pmgptr std::shared_ptr


namespace pmg {

	class Surface {
	private:
	};

	/*!
		Defines a surface either:
		1) Between two non-closed paths
			- Each end points of the paths are then connected by straight lines
		2) Between two closed loops
	*/
	class BoundedSurface : public Surface {
	private:
		pmgptr<pmg::Path> outer, inner;
		bool closedLoop = false;
	};

	class ExtrudedSurface {
	private:
		BoundedSurface surface;
		pmgptr<pmg::Path> extrudePath;
	};

}