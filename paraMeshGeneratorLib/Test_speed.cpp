#include "Test_speed.h"
#include "LineMesher.h"
#include "PathMesher.h"
#include "CuboidMesher.h"
#include "Test_convinient_functions.h"



/*!
Not real test. Intended for checking time of multiple addition operations.
*/
int speedTestAddition(const std::string& fileName) {
	double numbers[10] = { 1, 2., 232.,2342., 1231., 434., 345345., 234., 454., 4. };

	int index = 0;
	double res = 0.;
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = numbers[index] + numbers[index + 1];
		index++;
	}
	return 0;
}
/*!
Not real test. Intended for checking time of multiple multiplication operations.
*/
int speedTestMultiplication(const std::string& fileName) {
	double numbers[10] = { 1, 2., 232.,2342., 1231., 434., 345345., 234., 454., 4. };

	int index = 0;
	double res = 0.;
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = numbers[index] * numbers[index + 1];
		index++;
	}
	return 0;
}
/*!
Not real test. Intended for checking time of multiple glm::vec3 addition operations.
*/
int speedTestVec3addition(const std::string& fileName) {
	glm::dvec3 vecs[10] = {
		glm::dvec3(223.,311.,345.), glm::dvec3(24.,3343.,523423.), glm::dvec3(2334.,2343.,576.),
		glm::dvec3(2133.,3.,5434.), glm::dvec3(2342.,3776.,56.),	glm::dvec3(2.,3.,5.),
		glm::dvec3(25.,38.,58888.),	glm::dvec3(1112.,322.,511.), glm::dvec3(22342.,376.,5777.),
		glm::dvec3(23.,33.,35.)
	};

	int index = 0;
	glm::dvec3 res(0.0);
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = vecs[index] + vecs[index + 1];
		index++;
	}
	return 0;
}
/*!
Not real test. Intended for checking time of multiple glm::dvec3 x glm::dmat3x3 operations.
*/
int speedTestMat3Vec3multiplication(const std::string& fileName) {
	glm::dvec3 vecs[10] = {
		glm::dvec3(223.,311.,345.), glm::dvec3(24.,3343.,523423.), glm::dvec3(2334.,2343.,576.),
		glm::dvec3(2133.,3.,5434.), glm::dvec3(2342.,3776.,56.),	glm::dvec3(2.,3.,5.),
		glm::dvec3(25.,38.,58888.),	glm::dvec3(1112.,322.,511.), glm::dvec3(22342.,376.,5777.),
		glm::dvec3(23.,33.,35.)
	};
	glm::dmat3x3 mats[10] = {
		makeCsysMatrix(vecs[1], GLM2PI * 0.1), makeCsysMatrix(vecs[7], GLM2PI * 0.1),
		makeCsysMatrix(vecs[0], GLM2PI * 3.1), makeCsysMatrix(vecs[8], GLM2PI * 2.1),
		makeCsysMatrix(vecs[4], GLM2PI * 56.1), makeCsysMatrix(vecs[3], GLM2PI * 0.21),
		makeCsysMatrix(vecs[2], GLM2PI * 2.1), makeCsysMatrix(vecs[5], GLM2PI * 0.11),
		makeCsysMatrix(vecs[0], GLM2PI * 1.1), makeCsysMatrix(vecs[9], GLM2PI * 0.91)
	};

	int index = 0;
	glm::dvec3 res(0.0);
	for (int i = 0; i < SPEED_TEST_AMOUNT; i++) {
		if (index == 9) index = 0;
		res = mats[index + 1] * vecs[index];
		index++;
	}
	return 0;

}


static const int nCubesSpeedTest = 50;
static const int nNodesCubeSpeedTest = 30;
/*!
Speed test for writing mulitple cubes.
 \p nCubesSpeedTest number of cubes, \p nNodesCubeSpeedTest number of nodes per cube edge.
*/
int speedTestWriteCubes(const std::string& fileName) {
	TEST_START

	int n = nNodesCubeSpeedTest;

	for (int i = 0; i < nCubesSpeedTest; i++) {
		CuboidMesher::writeNodes(NULL_POS, pos, MeshDensity3D(n, n, n), glm::dvec3(0.5, 0.5, 2.0), plane::xy);
		CuboidMesher::writeElements(MeshDensity3D(n, n, n));
	}

	TEST_END
}
int speedTestWriteRotatedCubes(const std::string& fileName) {
	TEST_START

		double R = 5.;
	glm::dvec3 v(1., 1., 1.);
	v = glm::normalize(v);

	glm::dmat3x3 rotMF2 = makeCsysMatrix(v, GLMPI / 2.);

	MeshCsys csysF0(10. * Z_DIR);
	MeshCsys csysF1(R * Z_DIR, &rotMF2);
	MeshCsys csysF2(-R * Z_DIR);
	csysF1.setParentCsys(&csysF0);
	csysF2.setParentCsys(&csysF1);
	csysF2.updateParents();
	writeXYZlines(csysF0, 2.0, 5);
	writeXYZlines(csysF1, 2.0, 5);
	writeXYZlines(csysF2, 2.0, 5);

	int n = nNodesCubeSpeedTest;

	for (int i = 0; i < nCubesSpeedTest; i++) {
		double ang = GLMPI * (double)i / 20.;
		rotMF2 = makeCsysMatrix(v, ang);
		csysF1.pos += 1.45 * v;
		csysF2.updateParents();

		CuboidMesher::writeNodes(NULL_POS, csysF2, MeshDensity3D(n, n, n), glm::dvec3(0.5, 0.5, 2.0), plane::xy);
		CuboidMesher::writeElements(MeshDensity3D(n, n, n));
	}

	TEST_END
}

int nLines = 10;
int nNodesPerLine = 10000;
int speedTestWriteLines(const std::string& fileName) {
	TEST_START2

	for (int i = 0; i < nLines; i++) {
		LineMesher::writeNodesX(pos, glCsys, nNodesPerLine, 10.);
		LineMesher::writeElements(nNodesPerLine);
		pos.z += 1.0;
	}


	TEST_END
}
int speedTestWritePathLineStrip(const std::string& fileName) {
	TEST_START2
		PathLineStrip pathLS(VecGLM3d({
			glm::dvec3(0.,0.,0.), glm::dvec3(1.,1.,0.5), glm::dvec3(2., 0.5, 0.5), glm::dvec3(2., 3., 0.)
			}));

	for (int i = 0; i < nLines; i++) {
		PathMesher::writeNodes(pos, glCsys, nNodesPerLine, pathLS, pathLS.getCornerPathFactors());
		PathMesher::writeElements(nNodesPerLine);
		pos.z += 1.0;
	}

	TEST_END
}


int speedTestWriteNodesAndElements(const std::string& fileName) {
	TEST_START
		int nNodes = nNodesCubeSpeedTest * nNodesCubeSpeedTest * nNodesCubeSpeedTest * nCubesSpeedTest;
	int nElements = (nNodesCubeSpeedTest - 1) * (nNodesCubeSpeedTest - 1) * (nNodesCubeSpeedTest - 1) * nCubesSpeedTest;

	for (int i = 0; i < nNodes; i++) {
		writer.writeNode(i + 1, glm::dvec3(12.23, 23.2, 6.));
	}
	int nodeIDs[4] = { 1,2,3,4 };
	for (int i = 0; i < nElements; i++) {
		writer.write4nodedShell(i + 1, nodeIDs);
	}
	TEST_END
}