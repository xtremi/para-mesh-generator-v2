#pragma once
#include "FeaWrite.h"

#define MESHER_NODE_WRITE_START int firstNode = writer->getNextNodeID(); MeshCsys curPos(spos);
#define MESHER_NODE_WRITE_END Mesher::nodeID1 = firstNode;

enum class MesherMode {write, get};

class Mesher {
public:
	static FEAwriter* getWriter() { return writer; }
	static void setFEAwriter(FEAwriter* feaWriter) { writer = feaWriter; }
	static void setNodeID1(int id) { nodeID1 = id; }
protected:
	static int nodeID1;	
	static FEAwriter* writer;
};

class RowMesher2D : private Mesher{
public:
	static void writeElements(NodeIterator1D* nodeIt1, NodeIterator1D* nodeIt2);
};