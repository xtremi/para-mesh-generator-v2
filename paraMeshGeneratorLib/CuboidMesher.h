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

	static void writeNodesXYZq(const MeshCsys& spos, const MeshDensity3D& meshDens,const glm::dvec3& dxyz);
	static void writeNodesXYZ(const MeshCsys& spos, const MeshDensity3D& meshDens, const glm::dvec3& size);

	static void writeElements(const MeshDensity3D& meshDens);


};


class CuboidMesherRef : private Mesher
{
public:
	static void writeNodes(
		const MeshCsys&			spos,
		const MeshDensity3Dref&	meshDens,
		const glm::dvec3&		size,		
		bool					startWithOffset,
		plane					pln);

	static void writeElements(const MeshDensity3Dref& meshDens);

private:

	struct RefShapeData {
		const MeshDensity3Dref* meshDens;
		plane					pln;
		direction				refDir;		
	};
	struct RefLayerData {
		MeshCsys				curPos;
		glm::dvec2				curElSize;
		double					curElSizeRefDir;
	};

	static void writeNodes_layerB(
		MeshCsys&				curPos,
		const MeshDensity2D&	meshDensD12,
		const glm::dvec2&		elSize,
		double					elSizeRefDir,
		plane					pln,
		direction				refDir);
	static void writeNodes_layerM1(
		MeshCsys&				curPos,
		const MeshDensity2D&	meshDensD12,
		const glm::dvec2&		elSize,
		double					elSizeRefDir,
		plane					pln,
		direction				refDir);
	static void writeNodes_layerM2(
		MeshCsys&				curPos,
		const MeshDensity2D&	meshDensD12,
		glm::dvec2&				elSize,
		double					elSizeRefDir,
		plane					pln,
		direction				refDir);
	static void writeNodes_layerM3(
		MeshCsys&				curPos,
		const MeshDensity2D&	meshDensD12,
		const glm::dvec2&		elSize,
		double					elSizeRefDir,
		plane					pln,
		direction				refDir);
	static void writeNodes_layerT(
		MeshCsys&				curPos,
		const MeshDensity2D&	meshDensD12,
		glm::dvec2&				elSize,
		double&					elSizeRefDir,
		plane					pln,
		direction				refDir);

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