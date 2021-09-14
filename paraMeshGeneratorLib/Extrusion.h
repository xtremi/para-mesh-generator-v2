#pragma once
#include "NodeIterator.h"

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
	int			  extrusionIndex = 0;
	double		  length;				// ExtrusionType::line
	double		  radius, endAngle;		// ExtrusionType::arc
	int			  nElements;
private: 
	bool _isStart;
};


/*

	Extrusion of edge (result in new face)

	isStart = true

		nElements
	|---|---|---|---|
		---Edg1-->
	 x---x---x---x---o1
 |	 x---x---x---x---x  |
Edg0 x---x---x---x---x Edg2
 |	 x---x---x---x---x  |
 v	 x---x---x---x---o2  v
		---Edg3-->

	<-----length----->
	----> Extrusion

	isStart = false

		nElements
	|---|---|---|---|
		---Edg1-->
	 |--|x---x---x---o1
 |	 |-e5x---x---x---x  |
Edg0 |--|x---x---x---x Edg2
 |	 |--|x---x---x---x  |
 v	 |--|x---x---x---o2  v
		v  --Edg3-->
		 
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

	/*!		
		edge[0] is the edge same edge as edge[2] of the previous extrusion
		edge[5] is the first edge with nodes belonging to this extrusion
	*/
	MeshEdge edges[5];

	int endCornerNode1();
	int endCornerNode2();

	void setNodeOffset(int nOffs);
	void addToFirstNodeID(int n);
protected:
	void initEdges(int nnodeEdge1, int firstNodeID, MeshExtrusion* previousExtrusion);
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
