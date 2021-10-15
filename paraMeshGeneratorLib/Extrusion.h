#pragma once
#include "ParaMeshGenCommon.h"
#include "MeshDensity.h"
#include "MeshCsys.h"
#include  <memory>

enum class ExtrusionType {
	line,
	arc
};


class ExtrudeStepData{
public:	
	glm::dvec3 pos;
	MeshCsys   csys;
	double	   startSpace;
	double	   arcAngle;
};

class ExtrudeEdgeStepData : public ExtrudeStepData{
public:
	glm::dvec2 dxy;
	double lengthY;
};

class ExtrudeFaceStepData : public ExtrudeStepData {
public:
	int		   nNodesEdgeX;
	glm::dvec3 dxyz;
	glm::dvec2 sizeYZ;
};



class MeshEdge {
public:
	MeshEdge() {}
	MeshEdge(std::shared_ptr<NodeIterator1D> _nodeIter) {
		nodeIter = _nodeIter;
	}
	
	std::shared_ptr<NodeIterator1D> nodeIter;
	int startNode() { return nodeIter->first(); }
	int endNode() { return nodeIter->last(); }
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
	MeshExtrusion(int _nElements, MeshExtrusion* previousExtrusion = nullptr);	
	virtual int nNodes();
	bool isStart() { return _isStart; }
	virtual double spacing() {
		return 0.;
	}

	virtual void writeNodes(ExtrudeStepData* curStepData){}
	virtual void writeElements(){}

protected: 
	int nElements;	//in extrusion direction
	bool _isStart;
};

/*
	MeshExtrusion can inherit this for line extrusion properties
*/
class MeshExtrusion_linearProp {
protected:
	MeshExtrusion_linearProp(double _length) : length{ _length } { }
	double length;
};

/*
	MeshExtrusion can inherit this for arc extrusion properties
*/
class MeshExtrusion_arcProp {
protected:
	MeshExtrusion_arcProp(double _radius, double _endAngle) : radius{_radius}, endAngle{_endAngle}{}
	double radius, endAngle;
};

/*
	MeshExtrusion can inherit this for ref extrusion properties
*/
class MeshExtrusion_refProp {
protected:
	MeshExtrusion_refProp(int _nRef) : nRef{_nRef}{}
	int nRef;
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
		int	   _nElements, 
		int	   nnodeEdge1, 
		int	   firstNodeID, 
		MeshEdgeExtrusion* previousExtrusion = nullptr);

	/*!		
		edge[0] is the edge same edge as edge[2] of the previous extrusion
		edge[5] is the first edge with nodes belonging to this extrusion
	*/	
	MeshEdge edges[5];
	virtual int endCornerNode1();
	virtual int endCornerNode2();

	virtual void setNodeOffset(int nOffs);
	virtual void addToFirstNodeID(int n);	
};

class MeshEdgeExtrusion_noRef : public MeshEdgeExtrusion{
public:
	MeshEdgeExtrusion_noRef(
		int	   _nElements,
		int	   nnodeEdge1,
		int	   firstNodeID,
		MeshEdgeExtrusion* previousExtrusion = nullptr);

	virtual void writeElements();

protected:
	MeshDensity2D meshDens;
	
	void initEdges(int nnodeEdge1, int firstNodeID, MeshEdgeExtrusion* previousExtrusion);
};

class MeshEdgeExtrusion_ref : public MeshEdgeExtrusion, public MeshExtrusion_refProp {
public:
	MeshEdgeExtrusion_ref(
		int	   _nRef,
		int	   nnodeEdge1,
		int	   firstNodeID,
		MeshEdgeExtrusion* previousExtrusion = nullptr);

	virtual void writeElements();

	int nNodesEdge2afterRefinement() {
		return meshDens.dir2ref();
	}

protected:
	MeshDensity2Dref meshDens;
	void initEdges(int nnodeEdge1, int firstNodeID, MeshEdgeExtrusion* previousExtrusion);
};

class MeshEdgeExtrusionLinear : public MeshEdgeExtrusion_noRef, public MeshExtrusion_linearProp {
public:
	MeshEdgeExtrusionLinear(
		double _length,
		int	   _nElements,
		int	   nnodeEdge1,
		int	   firstNodeID,
		MeshEdgeExtrusion* previousExtrusion = nullptr);

	double spacing();
	void writeNodes(ExtrudeStepData* curStepData);
};

class MeshEdgeExtrusionArc : public MeshEdgeExtrusion_noRef, public MeshExtrusion_arcProp {
public:
	MeshEdgeExtrusionArc(
		double _radius,
		double _endAngle,
		int	   _nElements,
		int	   nnodeEdge1,
		int	   firstNodeID,
		MeshEdgeExtrusion* previousExtrusion = nullptr);

	double spacing();
	void writeNodes(ExtrudeStepData* curStepData);
};

class MeshEdgeExtrusionLinearRef : public MeshEdgeExtrusion_ref, public MeshExtrusion_linearProp {
public:
	MeshEdgeExtrusionLinearRef(
		double _length,
		int	   _nRef,
		int	   nnodeEdge1,
		int	   firstNodeID,
		MeshEdgeExtrusion* previousExtrusion = nullptr);

	double spacing();
	void writeNodes(ExtrudeStepData* curStepData);
};
class MeshEdgeExtrusionArcRef : public MeshEdgeExtrusion_ref, public MeshExtrusion_arcProp {
	MeshEdgeExtrusionArcRef(
		double _radius,
		double _endAngle,
		int	   _nRef,
		int	   nnodeEdge1,
		int	   firstNodeID,
		MeshEdgeExtrusion* previousExtrusion = nullptr);

	double spacing();
	void writeNodes(ExtrudeStepData* curStepData);
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
		int					 _nElements,
		const MeshDensity2D& face0nodes, 
		int					 firstNodeID,
		MeshFaceExtrusion*	 previousExtrusion = nullptr);


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
	NodeIterator1D endEdgeIterators[4];	
};


class MeshFaceExtrusion_noRef : public MeshFaceExtrusion {
public:
	MeshFaceExtrusion_noRef(
		int	   _nElements,
		const  MeshDensity2D& face0nodes,
		int	   firstNodeID,
		MeshFaceExtrusion* previousExtrusion = nullptr);

	virtual void writeElements();

protected:
	MeshDensity3D meshDens;
	void initFaces(const MeshDensity2D& face0nodes, int firstNodeID, MeshFaceExtrusion* previousExtrusion);
};


class MeshFaceExtrusionLinear : public MeshFaceExtrusion_noRef, public MeshExtrusion_linearProp {
public:
	MeshFaceExtrusionLinear(
		double _length,
		int	   _nElements,
		const MeshDensity2D& face0nodes,
		int	   firstNodeID,
		MeshFaceExtrusion* previousExtrusion = nullptr);

	double spacing();
	void writeNodes(ExtrudeStepData* curStepData);
};


class MeshFaceExtrusionArc : public MeshFaceExtrusion_noRef, public MeshExtrusion_arcProp {
public:
	MeshFaceExtrusionArc(
		double				 _radiusInner,
		double				 _endAngle,
		int					 _nElements,
		const MeshDensity2D& face0nodes,
		int					 firstNodeID,
		MeshFaceExtrusion*		 previousExtrusion = nullptr);

	double spacing();
	void writeNodes(ExtrudeStepData* curStepData);

};

