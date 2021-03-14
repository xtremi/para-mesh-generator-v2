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
		ConeMesher::writeNodes(writer, coords, radiusInner, radiusOuter, startAng, endAng, 0.0,
			glm::ivec2(nnodes.x, nnodes.y), rotaxis, csys);

		
		coords[(size_t)rotaxis] += dH;
		radiusInner += dRi;
		radiusOuter += dRo;
	}

	nodeID1 = firstNodeID;


}

#ifdef TEMP_DISABLE
void Cone3Dmesher::writeNodes_ref(
	FEAwriter*			writer,
	const glm::dvec3&	spos,
	const glm::ivec2&	nNodes12,
	int					nRefinements,
	double				radiusStartOuter,
	double				radiusEndOuter,
	double				radiusStartInner,
	double				radiusEndInner,
	double				startAng,
	double				endAng,
	double				height,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	int firstNodeID = writer->getNextNodeID();

	bool	fullCircle = (startAng < 0.0) && (endAng < 0.0);
	
	double  dRi = (radiusEndInner - radiusStartInner);
	double  dRo = (radiusEndOuter - radiusStartOuter);
	int		nElAroundStart = fullCircle ? nNodes12.x : nNodes12.x - 1;
	double	arcLengthStart = 2.0 * glm::pi<double>() * radiusStart;
	double	coneLength = std::sqrt(std::pow(dR, 2.0) + std::pow(height, 2.0));
	double	elSizeX = initialRefinementElementSize(coneLength, nRefinements, false);


	glm::ivec2	currentNodesPerFace(nnodes12);	//.x around - .y outwards
	glm::dvec2	currentElSize12(size.x / (double)(nNodesFace.x - 1), size.y / (double)(nNodesFace.y - 1));
	double		currentElSize3 = initialRefinementElementSize(size.z, nRefinements, startWithOffset) / 2.0; //this is not correct	

	int		currentRefFactor1 = 1;
	int		currentRefFactor2 = 1;
	int		currentRefinement = 0;

	glm::dvec3 coords(spos);

	while (currentRefinement < nRefinements) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x
		//draw disk nodes
		

		//row m1:  |  x--x--x  |  x--x--x  | (dir1)

		//Refine dir1

		//row m2:  x-----x-----x-----x-----x
		//draw disk nodes

		//row m3:  |  x--x--x  |  x--x--x  |

		//Refine dir2

		//row t:  x-----x-----x-----x-----x (dir2)
		//draw disk nodes
	}

}

void Cone3Dmesher::writeElements_ref(
	FEAwriter*	writer,
	glm::ivec2	nnodes12,
	int			nRefinements,
	bool		closedLoop) 
{
	CuboidMesher::writeElementsRef(writer, nnodes12, nRefinements, closedLoop);
}
#endif
void Cone3Dmesher::writeElements(
	FEAwriter*	writer,
	glm::ivec3	nnodes,
	bool		closedLoop)
{
	CuboidMesher::writeElements(writer, nnodes, closedLoop);
}