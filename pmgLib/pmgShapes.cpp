#include "pmgShapes.h"
#include "pmgMath.h"
using namespace pmg;

void ArcAngles::setFullCircle() {
	start = 0.0;
	end = GLM2PI;
	m_fullCircle = true;
}

double ArcAngles::angStep(int nnodes) const {
	double dang = (end - start);
	if (!m_fullCircle){
		dang /= (double)(nnodes - 1);
	}
	else{
		dang /= (double)nnodes;
	}
	return dang;
}