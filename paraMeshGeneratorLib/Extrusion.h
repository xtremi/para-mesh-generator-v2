#pragma once
#include "ParaMeshGenCommon.h"
#include "MeshDensity.h"


enum class ExtrusionType {
	line,
	arc
};

class MeshEdge {
public:
	MeshEdge() {}
	MeshEdge(NodeIterator1D _nodeIter) {
		nodeIter = _nodeIter;
	}
	
	NodeIterator1D nodeIter;
	int startNode() { return nodeIter.first(); }
	int endNode() { return nodeIter.last(); }
};

class MeshEdge_ext {
public:
	MeshEdge_ext() {}
	MeshEdge_ext(NodeIterator1Dm _nodeIter) {
		nodeIter = _nodeIter;
	}
	NodeIterator1Dm nodeIter;	
};

class MeshFace {
public:
	MeshFace() {}
	MeshFace(NodeIterator2D _nodeIter) {
		nodeIter = _nodeIter;
	}
	NodeIterator2D nodeIter;

};

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
	MeshExtrusion(double _length, int _nElements, MeshExtrusion* previousExtrusion = nullptr);
	MeshExtrusion(double _radius, double _endAngle, int _nElements, MeshExtrusion* previousExtrusion = nullptr);
	double spacing();
	int nNodes();

	bool isStart() { return _isStart; }
	ExtrusionType extrusionType;
	double		  length;				// ExtrusionType::line
	double		  radius, endAngle;		// ExtrusionType::arc
	int			  nElements;
private: 
	bool _isStart;
};


/*

	Extrusion of edge (result in new face)

	-----------------------------------------
		isStart = true
	-----------------------------------------

	 <-----length---->

		nElements
	|---|---|---|---|
		---Edg3-->
 ^	 x---x---x---x---o2 ^
 |	 x---x---x---x---x  |
Edg0 x---x---x---x---x Edg2  |----> Extrusion
 |	 x---x---x---x---x  |
 	 x---x---x---x---o1  
		---Edg1-->
	 |-----------|
	 MeshDensity2D meshDens covers this part

	
	
	-----------------------------------------
		isStart = false
	-----------------------------------------

		nElements
	 |---|---|---|---|

		^  --Edg3-->
 ^   |--|x---x---x---o2 ^
 |	 |-e5x---x---x---x  |
Edg0 |--|x---x---x---x Edg2  |----> Extrusion
 |	 |--|x---x---x---x  |
 	 |--|x---x---x---o1  
		  --Edg1-->

		 |-----------|
		  MeshDensity2D meshDens covers this part

	Edge 0 is the edge containing the nodes at the end of the previous extrusion
	Edge 5 is the first row of nodes (same direction of edge 0) belonging to
	this extrusion

	o1 / o2 - endCornerNode1 / endCornerNode2

*/
class MeshEdgeExtrusion : public MeshExtrusion {
public:
	MeshEdgeExtrusion(
		double _length, 
		int	   _nElements, 
		int	   nnodeEdge1, 
		int	   firstNodeID, 
		MeshExtrusion* previousExtrusion = nullptr);
	MeshEdgeExtrusion(
		double _radius, 
		double _endAngle, 
		int	   _nElements, 
		int	   nnodeEdge1, 
		int	   firstNodeID, 
		MeshExtrusion* previousExtrusion = nullptr);

	/*
		The mesh density of the nodes of these extrusion.
		This means from edge0 is not included.
	*/
	MeshDensity2D meshDens;

	/*!		
		edge[0] is the edge same edge as edge[2] of the previous extrusion
		edge[5] is the first edge with nodes belonging to this extrusion
	*/	
	MeshEdge edges[5];
	MeshEdge getEdge(int edgeID);
	int endCornerNode1();
	int endCornerNode2();

	void setNodeOffset(int nOffs);
	void addToFirstNodeID(int n);
protected:
	void initEdges(int nnodeEdge1, int firstNodeID, MeshExtrusion* previousExtrusion);
};

/*          F3 (back)

		 _________e10_______
		|\                  |\
		| \                 | \
		|  \e2   F5(top)    |  \e6
		|   \             e7|   \
	  e3|    \              |    \
		|     \_______e9__________\
 F0->   |     |             |     |  <-F2
(F6)	| _ _ | _ _ e11 _ _ |     |	    -----> extrude dir (x)
		 \    |              \    |e5 
		  \   |e1  F1(front)  \e4 |	   
		 e0\  |                \  |	   
			\ |                 \ |	   
			 \|__________________\|	   
			c0	  ^   e8
                  |
				 F4 (bottom)
			   
			   <----------------->
			        length

*/
class MeshFaceExtrusion : public MeshExtrusion {
public:
	MeshFaceExtrusion(
		double				 _length,
		int					 _nElements,
		const MeshDensity2D& face0nodes, 
		int					 firstNodeID,
		MeshExtrusion*		 previousExtrusion = nullptr);
	MeshFaceExtrusion(		 
		double				 _radiusInner,
		double				 _endAngle,
		int					 _nElements,
		const MeshDensity2D& face0nodes,
		int					 firstNodeID,
		MeshExtrusion*		 previousExtrusion = nullptr);


	/*
		The mesh density of the nodes of this extrusion.
		This means from face0 is not included.
	*/
	MeshDensity3D meshDens;
	/*!
		face[0] is the face same face as face[5] of the previous extrusion
		face[6] is the first face with nodes belonging to this extrusion
	*/
	MeshFace faces[7];
	NodeIterator1D getEndEdgeIterator0();
	NodeIterator1D getEndEdgeIterator1();
	NodeIterator1D getEndEdgeIterator2();
	NodeIterator1D getEndEdgeIterator3();

	void setNodeOffset(int nOffs);
	void addToFirstNodeID(int n);

protected:
	void initFaces(const MeshDensity2D& face1nodes, int firstNodeID, MeshExtrusion* previousExtrusion);
	NodeIterator1D endEdgeIterators[4];

	
};
