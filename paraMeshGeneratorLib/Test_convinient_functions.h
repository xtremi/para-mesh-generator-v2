#include "MeshCsys.h"
#include "MeshDensity.h"
#include "FeaWrite.h"
#include "Mesher.h"

/*
	Tests should be defined in some class and these functions should
	be defined there.
*/
/*************************************************************************/
#define TEST_START std::ofstream file; \
file.open(fileName); \
if (!file.is_open()) return 1; \
NastranFEAwriter writer(&file); \
Mesher::setFEAwriter(&writer); \
MeshCsys pos(glm::dvec3(0.0));

#define TEST_START2 std::ofstream file; \
file.open(fileName); \
if (!file.is_open()) return 1; \
NastranFEAwriter writer(&file); \
Mesher::setFEAwriter(&writer); \
MeshCsys glCsys(glm::dvec3(0.0)); \
glm::dvec3 pos(NULL_POS);

#define TEST_END writer.close(); \
return 0;

void writeXYZlines(MeshCsys& csys, double length, int nNodes);
void writeXYZplanes(MeshCsys & csys, const MeshDensity2D & meshDens, const glm::dvec2 & size);

std::vector<int> linearlyOrderedVector(int iStart, int iEnd, int iStep);
void writeDebugBeamElements(FEAwriter * w, int firstNode, int lastNode);


template<typename T>
bool equalVectors(const std::vector<T>&vec1, const std::vector<T>&vec2) {
	if (vec1.size() != vec2.size()) return false;
	for (int i = 0; i < vec1.size(); i++) {
		if (vec1[i] != vec2[i]) return false;
	}
	return true;
}

template<typename T>
bool equalVecVectors(const std::vector<std::vector<T>>&vec1, std::vector<std::vector<T>>&vec2) {
	if (vec1.size() != vec2.size()) {
		return false;
	}
	for (int i = 0; i < vec1.size(); i++) {
		if (!equalVectors(vec1[i], vec2[i])) {
			return false;
		}
	}
	return true;
}