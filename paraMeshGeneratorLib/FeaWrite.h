#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "string_utilities.h"
#include "MeshCsys.h"
#include "ParaMeshGenCommon.h"


#define TO_BUFFER

//typedef void(*coordTransform)(glm::dvec3*, const std::vector<double>&);
void coordTransform1(glm::dvec3* coords, std::vector<double>*);

class FEAwriter {
	
public:
	FEAwriter(std::ofstream* _file) { 
		file = _file; 
		buffer.reserve(maxWritesToBuffer * 75);
	}
	~FEAwriter();
	void close();
	
	virtual void writeComment(const std::string& msg);
	void writeNode(const glm::dvec3& c, const glm::dvec3& transl, glm::dmat3x3* csys, const MeshCsys* meshCSYS = nullptr);
	void writeNode(int nodeID, const glm::dvec3& c, const glm::dvec3& transl, glm::dmat3x3* csys);

	void write2nodedBeam(int n[2]);
	void write4nodedShell(int n[4]);
	void write8nodedHexa(int n[8]);
	
	virtual void writeNode(int nodeID, const glm::dvec3& c) = 0;
	virtual void write2nodedBeam(int elID, int n[2]) = 0;
	virtual void write4nodedShell(int elID, int n[4]) = 0;
	virtual void write8nodedHexa(int elID, int n[8]) = 0;
	void writePreNodes(int numberOfNodes = -1){}
	void writePostNodes(){}
	void writePreElements(int numberOfElements = -1){}
	void writePostElements(){}

	int getNextElementID() { return elementID; }
	int getNextNodeID() { return nodeID; }
	//void setNextNodeID(int id) { nodeID = id; }
	//void setNextElementID(int id) { elementID = id; }

	void setCurrentElementPropertyID(int id) {
		currentPropertyID = id;
	}

	void setCoordTransformer(void(*trsfFunc)(glm::dvec3*, std::vector<double>*)) {
		currentCoordTransformer = trsfFunc;
	}
	void unsetCoordTransformer() {
		currentCoordTransformer = nullptr;
	}
	void setCurrentCoordTransformParams(const std::vector<double>& coordTrsfParams){
		currentCoordTransformParams = coordTrsfParams;
	}

protected:
	int nodeID		      = 1;
	int elementID	      = 1;
	int currentPropertyID = -1;

	std::vector<double> currentCoordTransformParams;
	void(*currentCoordTransformer)(glm::dvec3*, std::vector<double>*) = nullptr;
	
protected:
	std::ofstream* file = NULL;
	Dim eldim;

	//std::stringstream buffer;
	std::string buffer;
	int nWritesToBuffer = 0;
	int maxWritesToBuffer = (int)1e5;
	void processWriteBuffer();
	void dumpBuffer();

	void writeVectorCommaDelimeted(int* vec, int size);
	void writeVectorSpaceDelimeted(int* vec, int size);
	std::string vectorToString(int* vec, int size, char delim);


//Functions below will be moved out of FEAwriter:
public:
/********************************************************
	Elements row:
********************************************************/

	int writeElementRow(
		const std::vector<int>& bl,
		const std::vector<int>& br,
		const std::vector<int>& tr,
		const std::vector<int>& tl,
		int elStartID);

};


class NastranFEAwriter : public FEAwriter {
public:
	NastranFEAwriter(std::ofstream* _file) : FEAwriter(_file) {};
	void writeNode(int nodeID, const glm::dvec3& c);
	void write2nodedBeam(int elID, int n[2]);
	void write4nodedShell(int elID, int n[4]);
	void write8nodedHexa(int elID, int n[8]);

	void writeComment(const std::string& msg);
};

class AbaqusFEAwriter : public FEAwriter {
public:
	AbaqusFEAwriter(std::ofstream* _file) : FEAwriter(_file) {}
	void writeNode(int nodeID, const glm::dvec3& c);
	void write2nodedBeam(int elID, int n[2]);
	void write4nodedShell(int elID, int n[4]);
	void write8nodedHexa(int elID, int n[8]);

	void writePreNodes(int numberOfNodes = -1);
	void writePreElements(int numberOfNodes = -1);
};

class AnsysFEAwriter : public FEAwriter {
public:
	AnsysFEAwriter(std::ofstream* _file) : FEAwriter(_file) {};
	void writeNode(int nodeID, const glm::dvec3& c);
	void write2nodedBeam(int elID, int n[2]);
	void write4nodedShell(int elID, int n[4]);
	void write8nodedHexa(int elID, int n[8]);

	void writePreNodes(int numberOfNodes = -1);
	void writePreElements(int numberOfNodes = -1);	
};

class VTKFEAwriter : public FEAwriter {
public:
	VTKFEAwriter(std::ofstream* _file) : FEAwriter(_file) {};
	void writeNode(int nodeID, const glm::dvec3& c);
	void write2nodedBeam(int elID, int n[2]);
	void write4nodedShell(int elID, int n[4]);
	void write8nodedHexa(int elID, int n[8]);

	void writePreNodes(int numberOfNodes = -1);
	void writePreElements(int numberOfNodes = -1);
};


/********************************************************
	Node line:
********************************************************/
/*	int writeNodesQ(
		const glm::dvec3&	spos,
		double				dx,
		int					nrows,
		int					nodeIDstart,
		direction			dir,
		glm::dmat3x3*		csys = nullptr);

	int writeNodesQ(
		const glm::dvec3&	spos,
		const glm::dvec3&	ds,
		int					nnodes,
		int					nodeIDstart,
		glm::dmat3x3*		csys = nullptr);

	int writeNodes(const glm::dvec3& spos, double length, int nrows, int nodeIDstart,
		direction dir = direction::x, glm::dmat3x3* csys = nullptr);

	int writeNodes(const glm::dvec3& spos, const glm::dvec3& sposEnd, int nnodes, int nodeIDstart,
		glm::dmat3x3* csys = nullptr);

	int writeNodesXq(const glm::dvec3& spos, double dx, int nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesYq(const glm::dvec3& spos, double dx, int nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesZq(const glm::dvec3& spos, double dx, int nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesX(const glm::dvec3&	spos, double length, int nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesY(const glm::dvec3&	spos, double length, int nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesZ(const glm::dvec3&	spos, double length, int nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);

	int writeNodesQ_nth(
		const glm::dvec3&	spos,
		double				dx,
		int					nrows,
		int					nodeIDstart,
		int					skipNth,
		direction			dir,
		glm::dmat3x3*		csys = nullptr);

	int writeNodesQ_nth(
		const glm::dvec3&	spos,
		const glm::dvec3&	ds,
		int					nnodes,
		int					nodeIDstart,
		int					skipNth,
		glm::dmat3x3*		csys = nullptr);
*/
/********************************************************
	Node circle (arc):
********************************************************/
/*	int writeNodesCircular(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				endAng,
		int					nnodes,
		int					nodeIDstart,
		direction			rotAxis = direction::y,
		glm::dmat3x3*		csys = nullptr);

	int writeNodesCircularQ(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				dAng,
		int					nnodes,
		int					nodeIDstart,
		direction			rotAxis = direction::y,
		glm::dmat3x3*		csys = nullptr);

	int writeNodesCircularX(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
		int nnodes, int	nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesCircularY(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
		int nnodes, int	nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesCircularZ(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
		int nnodes, int	nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesCircularXq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
		int nnodes, int	nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesCircularYq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
		int nnodes, int	nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesCircularZq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
		int nnodes, int	nodeIDstart, glm::dmat3x3* csys = nullptr);

	int writeNodesCircular_nth(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				endAng,
		int					nnodes,
		int					nodeIDstart,
		int					skipNth,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);

	int writeNodesCircularQ_nth(
		const glm::dvec3&	centerPos,
		double				radius,
		double				startAng,
		double				dAng,
		int					nnodes,
		int					nodeIDstart,
		int					skipNth,
		direction			rotAxis,
		glm::dmat3x3*		csys = nullptr);
*/
/********************************************************
	Elements line:
********************************************************/
/*	int writeElements(
		int		nnodes,
		int		nodeID1,
		int		elementID1,
		bool	closedLoop = false
	);
*/
/********************************************************
	Nodes plane:
********************************************************/
/*	int writeNodes(
		const glm::dvec3&	spos,
		const glm::dvec2&	dp,
		const glm::ivec2&	nnodes,
		int					nodeIDstart,
		plane				pln = plane::xy,
		glm::dmat3x3*		csys = nullptr);

	int writeNodesQ(
		const glm::dvec3&	spos,
		const glm::dvec2&	size,
		const glm::ivec2&	nnodes,
		int					nodeIDstart,
		plane				pln = plane::xy,
		glm::dmat3x3*		csys = nullptr);

	int writeNodesXZq(const glm::dvec3&	spos, const glm::dvec2&	dxz, const glm::ivec2& nnodes, int nodeIDstart,	glm::dmat3x3* csys = nullptr);
	int writeNodesXYq(const glm::dvec3&	spos, const glm::dvec2&	dxy, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesYZq(const glm::dvec3&	spos, const glm::dvec2&	dyz, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesXZ(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesXY(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesYZ(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);

	int writeNodes_ref(
		const glm::dvec3& spos,
		const glm::dvec2& size,
		int				  nNodesEdge,
		int               nRefinements,
		int               nodeIDstart,
		bool			  startWithOffset,
		plane			  pln = plane::xy,
		glm::dmat3x3*     csys = nullptr);

	int writeNodesXY_ref(const glm::dvec3& spos,	const glm::dvec2& size,	int	nNodesEdge, int nRefinements,
		int nodeIDstart, bool startWithOffset, glm::dmat3x3* csys = nullptr);
	int writeNodesXZ_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements,
		int nodeIDstart, bool startWithOffset, glm::dmat3x3* csys = nullptr);
	int writeNodesYZ_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements,
		int nodeIDstart, bool startWithOffset, glm::dmat3x3* csys = nullptr);
*/

/********************************************************
	Node cone:
********************************************************/
/*	int writeNodesCone(
		const glm::dvec3&	centerPos,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				endAng,
		double				height,
		const glm::ivec2&	nnodes,
		int					nodeIDstart,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr
	);

	int writeNodesConeX(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesConeY(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
		const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);
	int writeNodesConeZ(const glm::dvec3& centerPos,	double radiusStart, double radiusEnd,double startAng, double endAng, double height,
		const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys = nullptr);

	int writeNodesCone_ref(
		const glm::dvec3&	centerPos,
		int					nNodesEdge,
		int					nRefinements,
		double				radiusStart,
		double				radiusEnd,
		double				startAng,
		double				endAng,
		double				height,
		int					nodeIDstart,
		direction			rotaxis,
		glm::dmat3x3*		csys = nullptr);
*/





/********************************************************
	Elements plane:
********************************************************/
/*	int writeElements(
		glm::ivec2			nnodes,
		int					nodeID1,
		int					elementID1,
		bool				closedLoop = false
	);

	int writeElements_ref(
		int					nNodesY,
		int					nRefinements,
		int					elementID1,
		int					nodeID1,
		bool				closedLoop = false);
*/



