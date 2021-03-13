#include "Cone3Dmesher.h"
#include "ConeMesher.h"
#include "CuboidMesher.h"

void Cone3Dmesher::writeNodes(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	double				radiusStartOuter,
	double				radiusEndOuter,
	double				radiusStartInner,
	double				radiusEndInner,
	double				startAng,
	double				endAng,
	double				height,
	const glm::ivec3&	nnodes,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{

	int firstNodeID = writer->getNextNodeID();

	double dang = 0.0;
	//bool fullCircle = limitArcAngles(startAng, endAng, dang, nnodes.x);

	double dH = height / (double)(nnodes.z - 1);
	double dRi = (radiusEndInner - radiusStartInner) / (double)(nnodes.z - 1);
	double dRo = (radiusEndOuter - radiusStartOuter) / (double)(nnodes.z - 1);

	glm::dvec3  coords = spos;
	double		radiusInner = radiusStartInner;
	double		radiusOuter = radiusStartOuter;

	for (int i = 0; i < nnodes.z; i++) {
	  //DiskMesher::
		ConeMesher::writeNodesCone(writer, coords, radiusInner, radiusOuter, startAng, endAng, 0.0,
			glm::ivec2(nnodes.x, nnodes.y), rotaxis, csys);

		
		coords[(size_t)rotaxis] += dH;
		radiusInner += dRi;
		radiusOuter += dRo;
	}

	nodeID1 = firstNodeID;


}


void Cone3Dmesher::writeElements(
	FEAwriter*	writer,
	glm::ivec3	nnodes,
	bool		closedLoop)
{
	CuboidMesher::writeElements(writer, nnodes, closedLoop);
}