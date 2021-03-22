#pragma once
#include "FeaWrite.h"

class Mesher {
public:
//	FEAwriter* getWriter() { return writer; }
	static void setFEAwriter(FEAwriter* feaWriter) { writer = feaWriter; }
protected:
	static int nodeID1;	
	static FEAwriter* writer;
};
