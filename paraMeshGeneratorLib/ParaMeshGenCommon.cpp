#include "ParaMeshGenCommon.h"



direction getPlaneNormal(plane pln) {
	switch (pln) {
	case plane::xy:
		return direction::z; break;
	case plane::xz:
		return direction::y; break;
	case plane::yz:
	default:
		return direction::x; break;
	}
}

void getPlaneDirections(plane pln, direction& d1, direction& d2){
	
	switch (pln) {
	case plane::xy:
		d1 = direction::x;
		d2 = direction::y;
		break;
	case plane::xz:
		d1 = direction::x;
		d2 = direction::z;
		break;
	case plane::yz:
		d1 = direction::y;
		d2 = direction::z;
		break;
	default:
		break;
	}
}