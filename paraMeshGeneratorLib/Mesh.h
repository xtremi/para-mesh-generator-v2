#pragma once
#include "math_utilities.h"
#include "ParaMeshGenCommon.h"
#include "FeaWrite.h"
#include "Geometry.h"
#include "glm/gtc/constants.hpp"

enum class ExtrusionType { 
	line, 
	arc 
};


/*
	   ----> dirX
	   	 
  |	   x    x    x    x    x 
  |	   
dirY   x    x    x    x    x
  |	   
  V	   x    x    x    x    x
	 
	 

*/
/*
class NodeIterator2D : public NodeIterator {
public:
	int next() {	

		if (currentIterIndexY == nNodesY) {
			if (currentIterIndexX == nNodesX) {

			}
		}

		if (currentIterIndexX == nNodesX && currentIterIndexY == nNodesY)
			return 0;
		return currentNodeID;
	}
private:
	void reset() {
		currentIterIndexX = 0;
		currentIterIndexY = 0;
	}
	int currentIterIndexX, currentIterIndexY;
	int nNodesX, nNodesY;
	int nodeIncrX, nodeIncrY;	
};
*/
class MeshEdge {
public:
	MeshEdge(){}
	MeshEdge(size_t _edgeID, NodeIterator1D _nodeIter) {
		edgeID = _edgeID; nodeIter = _nodeIter;
	}
	size_t edgeID = 0;
	NodeIterator1D nodeIter;
};
//
//class MeshFace {
//	size_t faceID;
//	NodeIterator1D nodeIter;
//};

/*

	Extrusion of "anything" (edge or face)

	    nElements
	|---|---|---|---|
	x
	x 
	x----> Extrusion 
	x
	x
	<-----length----->
*/
class MeshExtrusion {
public:
	MeshExtrusion(double _length, int _nElements, bool _isStart = false) {
		length			= _length; 
		nElements		= _nElements;
		extrusionType	= ExtrusionType::line;
		isStart = _isStart;
	}
	MeshExtrusion(double _radius, double _endAngle, int _nElements, bool _isStart = false) {
		radius = _radius;
		endAngle = _endAngle;
		nElements = _nElements;
		extrusionType = ExtrusionType::arc;
		isStart = _isStart;
		//length = 2.0*glm::pi<double>()*radius * (endAngle/ 2.0*glm::pi<double>());
		length = radius * endAngle;
	}


	double spacing() { 
		if(extrusionType == ExtrusionType::line)
			return length / (double)(nElements); 
		else if (extrusionType == ExtrusionType::arc)
			return endAngle / (double)(nElements);
	}

	//Number of nodes along extrusion
	int nNodes() { return isStart ? (nElements + 1) : nElements; }

	ExtrusionType extrusionType;
	double		  length;				// ExtrusionType::line
	double		  radius, endAngle;		// ExtrusionType::arc
	int			  nElements;
	bool		  isStart = false;
};


/*

	Extrusion of edge (result in new face)

		nElements
	|---|---|---|---|
	    ---Edg1-->
	 x---x---x---x---x  
 |	 x---x---x---x---x  |  
Edg0 x---x---x---x---x Edg2 
 |	 x---x---x---x---x  |
 v	 x---x---x---x---x  v
        ---Edg3-->

	<-----length----->
	----> Extrusion

*/
class MeshEdgeExtrusion : public MeshExtrusion {
public:
	MeshEdgeExtrusion(double _length, int _nElements, int nnodeEdge1, int firstNodeID, bool _isStart = false) 
		: MeshExtrusion(_length, _nElements, _isStart)
	{
		initEdges(nnodeEdge1, firstNodeID);
	}	

	MeshEdgeExtrusion(double _radius, double _endAngle, int _nElements, int nnodeEdge1, int firstNodeID, bool _isStart = false)
		: MeshExtrusion(_radius, _endAngle, _nElements, _isStart)
	{
		initEdges(nnodeEdge1, firstNodeID);
	}
	
	MeshEdge edges[4];
	//This offset allows to reuse the extrusion with different nodeIDs
	//The offset should normally be equal to the firstNodeID of its Mesh
	void setNodeOffset(int nOffs) {
		for (MeshEdge& edg : edges)
			edg.nodeIter.nodeIDoffset = nOffs;
	}
	
	void addToFirstNodeID(int n) {
		for (MeshEdge& edg : edges)
			edg.nodeIter.firstNodeID += n;
	}

/*

isStart extrusion:

nElements = 3
nNodes    = 4
x1  x2  x3  x4      
x5  x6  x7  x8      

 ------------------

normal extrusion:

nElements = 3
nNodes    = 3
-   x1  x2  x3
-   x5  x6  x7


*/
	void initEdges(int nnodeEdge1, int firstNodeID) {

		int nnodesExtr = isStart ? nElements + 1: nElements;

		/*					EdgeID			  firstNodeID						nNodes      nodeIncr    */
		edges[0] = MeshEdge(0, NodeIterator1D(0,								nnodeEdge1, nnodesExtr));
		edges[1] = MeshEdge(1, NodeIterator1D(0,								nnodesExtr, 1));
		edges[2] = MeshEdge(2, NodeIterator1D(nnodesExtr - 1,					nnodeEdge1, nnodesExtr));
		edges[3] = MeshEdge(3, NodeIterator1D(((nnodeEdge1 - 1) * nnodesExtr),	nnodesExtr, 1));
		addToFirstNodeID(firstNodeID);

	//extrusionsXdir[0].edges[0] = MeshEdge(0, NodeIterator1D(0, nNodesY, _nNodesXY.x));
	//extrusionsXdir[0].edges[1] = MeshEdge(1, NodeIterator1D(0, nNodesY, 1));
	//extrusionsXdir[0].edges[2] = MeshEdge(2, NodeIterator1D(_nNodesXY.x - 1, nNodesY, _nNodesXY.x));
	//extrusionsXdir[0].edges[3] = MeshEdge(3, NodeIterator1D(((_nNodesXY.x - 1) * nNodesY), nNodesY, 1));

	}

};

/*

         _________e8________
        |\                  |\
        | \                 | \
        |  \e5              |  \e7
        |   \            e12|   \
      e9|    \              |    \
        |     \_______e6__________\
        |     |             |     |    ^ 
        | _ _ | _ _ e4_ _ _ |     |	   |
         \    |              \    |e11 |        ^ 
          \   |e10            \e3 |	   | Length | Extrusion 
         e1\  |                \  |	   |        |
        	\ |                 \ |	   |
        	 \|__________________\|	   v
                       e2

 

*/
class MeshFaceExtrusion : public MeshExtrusion {
	MeshFaceExtrusion(double _length, int _nElements) : MeshExtrusion(_length, _nElements) {}
	//MeshEdge edges[12];
};

class Mesh {
public:
	Mesh() {
		csys.pos = glm::dvec3(0.0);
	}
public:
	virtual void writeNodes() = 0;
	virtual void writeElements() = 0;
	int numberOfNodes() { return nNodes; }
	int numberOfElements() { return nElements; }

	void setPosition(const glm::dvec3& _pos) { csys.pos = _pos; }
	void setRotationMatrix(glm::dmat3x3* _rotMat) { csys.csys = _rotMat; }
	void setCsys(MeshCsys& _csys) { csys = _csys; }

protected:
	virtual void calculateNumberOfNodes() = 0;
	virtual void calculateNumberOfElements() = 0;

	Dim elementDim;
	MeshCsys csys;

	int nNodes;
	int nElements;

	size_t iteratedEdgeID;
	size_t iteratedFaceID;
};


class MeshPrimitive : public Mesh {};


class MeshPrimitive2D : public MeshPrimitive{};


class MeshRec2D : public MeshPrimitive2D {

public:
	MeshRec2D() {}

	void initRectangle(const glm::dvec2& _sizeXY, const glm::ivec2& _nNodesXY);

	void extrudeYedge(double length, int nElements);
	void extrudeYedgeArc(double endAng, double radius, int nElements);

	virtual void writeNodes();
	virtual void writeElements();


	MeshEdge getEdge(int section, int edgeIndex) {
		if (section < extrusionsXdir.size() && edgeIndex < 4) {
			return extrusionsXdir[section].edges[edgeIndex];
		}
		return MeshEdge();
	}

protected:
	std::vector<MeshEdgeExtrusion> extrusionsXdir;

	void calculateNumberOfNodes();
	void calculateNumberOfElements();
	void calculateNumberOfNodesX();

	double lengthY;
	int	   nNodesY;
	int	   nNodesX;
};


//class MeshTube2D : public MeshPrimitive2D , public MeshExtrusion {};
//class MeshExtrudedRec: public MeshRec2D {};
//class MeshExtrudedTube : public MeshTube2D, public MeshExtrusion {};
//class MeshObject : public Mesh {};
