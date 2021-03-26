#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class CuboidMesher : private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity3D&	meshDens,
		const glm::dvec3&		size,		
		plane					pln);

	static void writeNodesQ(
		const MeshCsys&			spos,
		const MeshDensity3D&	meshDens,
		const glm::dvec3&		dp,	
		plane					pln);

	static void writeNodesXYZq(const MeshCsys& spos, const MeshDensity3D& meshDens,const glm::dvec3& dxz);
	static void writeNodesXYZ(const MeshCsys& spos, const MeshDensity3D& meshDens, const glm::dvec3& size);

	static void writeElements(const MeshDensity3D& meshDens);


};


class CuboidMesherRef : private Mesher
{
public:
	static void writeNodes(
		const glm::dvec3&	spos,
		const glm::dvec3&	size,
		const glm::ivec2&	nNodesFace,
		int					nRefinements,
		bool				startWithOffset,
		plane				pln,
		glm::dmat3x3*		csys = nullptr);

	static void writeElements(
		glm::ivec2	nNodesFace,
		int			nRefinements,
		bool		closedLoop);

private:
	static void writeElements_rows_bm1m2(
		const glm::ivec2&	currentNodes12,
		glm::ivec2&			nextNodes12,
		int&				firstNodeBrow,
		int&				firstNodeM1row,
		int&				firstNodeM2row,
		bool				closedLoop);

	static void writeElements_rows_m2m3t(
		const glm::ivec2&	currentNodes12,
		glm::ivec2&			nextNodes12,
		int&				firstNodeM2row,
		int&				firstNodeM3row,
		int&				firstNodeTrow,
		bool				closedLoop);
};