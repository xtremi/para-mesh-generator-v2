#include "pmgMeshWriter.h"
#include "fmt/core.h"
#include "fmt/format.h"
using namespace pmg;

MeshWriter::MeshWriter(const std::string& _filePath) : filePath{ _filePath } 
{}

bool MeshWriter::open() {
	file.open(filePath);
	return file.is_open();
}
void MeshWriter::close() {
	dumpBuffer();
	file.close();
}

void MeshWriter::writeNode(
	const glm::dvec3&	   c, 
	const MeshCsys&		   meshCsys,
	const MeshTransformer& meshTransformer)
{
	glm::dvec3 newCoords = c;
	newCoords = meshTransformer.getTransformedCoords(newCoords);
	newCoords = meshCsys.getGlobalCoords(newCoords);
	writeNode(nodeID++, newCoords);
}


void MeshWriter::write2nodedBeam(int n[2]){
	write2nodedBeam(elementID++, n);
}

void MeshWriter::write4nodedShell(int n[4]) {
	write4nodedShell(elementID++, n);
}

void MeshWriter::write8nodedHexa(int n[8]) {
	write8nodedHexa(elementID++, n);
}

void MeshWriter::write2nodedBeam(int elID, int n[2]) {
	static const std::string FMT_FORMAT = "B2 {:d},{:d},{:d}\n";
	std::string str = fmt::format(FMT_FORMAT, elID, n[0], n[1]);
	buffer += str;
	processWriteBuffer();
}

void MeshWriter::write4nodedShell(int elID, int n[4]) {
	static const std::string FMT_FORMAT = "S4 {:d},{:d},{:d},{:d},{:d}\n";
	std::string str = fmt::format(FMT_FORMAT, elID, n[0], n[1], n[2], n[3], n[4]);
	buffer += str;
	processWriteBuffer();
}

void MeshWriter::write8nodedHexa(int elID, int n[8]) {
	static const std::string FMT_FORMAT = "H8 {:d},{:d},{:d},{:d},{:d},{:d},{:d},{:d},{:d}\n";
	std::string str = fmt::format(FMT_FORMAT, elID, n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7]);
	buffer += str;
	processWriteBuffer();
}


void MeshWriter::writeNode(
	int				  nodeID,
	const glm::dvec3& c)
{
	static const std::string GRID_FMT_FORMAT = "N {:d},{:.5f},{:.5f},{:.5f}\n";
	std::string str = fmt::format(GRID_FMT_FORMAT, nodeID, c.x, c.y, c.z);
	buffer += str;
	processWriteBuffer();
}

void NastranWriter::writeNode(
	int				  nodeID,
	const glm::dvec3& c)
{
	static const std::string GRID_FMT_FORMAT = "GRID, {:d},,{:.5f},{:.5f},{:.5f}\n";
	std::string str = fmt::format(GRID_FMT_FORMAT, nodeID, c.x, c.y, c.z);
	buffer += str;
	processWriteBuffer();
}


void NastranWriter::write2nodedBeam(int elID, int n[2]) {
	static const std::string FMT_FORMAT = "CBAR, {:d}, 1, {:d}, {:d}\n";
	std::string str = fmt::format(FMT_FORMAT, elID, n[0], n[1]);
	buffer += str;
	processWriteBuffer();
}

void NastranWriter::write4nodedShell(int elID, int n[4]) {
	static const std::string FMT_FORMAT = "CQUAD4, {:d}, {:d}, {:d}, {:d}, {:d}, {:d}\n";
	std::string str = fmt::format(FMT_FORMAT, elID, currentPropertyID,
		n[0], n[1], n[2], n[3]);
	buffer += str;
	processWriteBuffer();
}

void NastranWriter::write8nodedHexa(int elID, int n[8]) {


}

void MeshWriter::processWriteBuffer() {
	nWritesToBuffer++;
	if (nWritesToBuffer >= maxWritesToBuffer) {
		nWritesToBuffer = 0;
		dumpBuffer();
	}
}

void MeshWriter::dumpBuffer() {
	file << buffer;
	buffer.clear();
}

void MeshWriter::writeVectorCommaDelimeted(int* vec, int size) {
	std::string str;
	for (int i = 0; i < size; i++) {
		str += (", " + fmt::format_int(vec[i]).str());
	}
	buffer += str;
}

void MeshWriter::writeVectorSpaceDelimeted(int* vec, int size) {
	std::string str;
	for (int i = 0; i < size; i++) {
		str += (" " + fmt::format_int(vec[i]).str());
	}
	buffer += str;
}

std::string MeshWriter::vectorToString(int* vec, int size, char delim) {
	std::string str;
	for (int i = 0; i < size; i++) {
		str.push_back(delim);
		str += (fmt::format_int(vec[i]).str());
	}
	return str;
}