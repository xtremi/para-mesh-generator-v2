#pragma once
#include "FeaWrite.h"

#define MESHER_NODE_WRITE_START int firstNode = writer->getNextNodeID(); MeshCsys curPos(spos);
#define MESHER_NODE_WRITE_END Mesher::nodeID1 = firstNode;

enum class MesherMode {write, get};

class Mesher {
public:
//	FEAwriter* getWriter() { return writer; }
	static void setFEAwriter(FEAwriter* feaWriter) { writer = feaWriter; }
protected:
	static int nodeID1;	
	static FEAwriter* writer;
};

