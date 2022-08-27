#pragma once
#include <fstream>
#include <glm/glm.hpp>

namespace pmg{

class MeshTransformer {
public:
	glm::dvec3 getTransformedCoords(const glm::dvec3& coords) const {
		return coords;
	}
};

class MeshCsys {
public:
	glm::vec3 getGlobalCoords(const glm::dvec3& localCoords) const {
		return localCoords;
	}
};

class MeshWriter {
public:
	MeshWriter(const std::string& filePath);
	bool open();
	void close();

	void writeNode(const glm::dvec3& localCoords, const MeshCsys& meshCsys, const MeshTransformer& meshTransf);
	void write2nodedBeam(int n[2]);
	void write4nodedShell(int n[4]);
	void write8nodedHexa(int n[8]);

	virtual void writeNode(int nodeID, const glm::dvec3& globalCoords);
	virtual void write2nodedBeam(int elID, int n[2]);
	virtual void write4nodedShell(int elID, int n[4]);
	virtual void write8nodedHexa(int elID, int n[8]);


	int nextNodeID() { return nodeID; }
	int nextElementID() { return elementID; }

protected:
	std::ofstream file;
	std::string buffer;
	std::string filePath;
	int nodeID = 1;
	int elementID = 1;
	int currentPropertyID = -1;

	int nWritesToBuffer = 0;
	int maxWritesToBuffer = (int)1e5;
	void processWriteBuffer();
	void dumpBuffer();

	void writeVectorCommaDelimeted(int* vec, int size);
	void writeVectorSpaceDelimeted(int* vec, int size);
	std::string vectorToString(int* vec, int size, char delim);
};

class NastranWriter : public MeshWriter {
public:
	NastranWriter(const std::string& filePath) : MeshWriter(filePath){}

protected:
	void writeNode(int nodeID, const glm::dvec3& globalCoords);
};

}