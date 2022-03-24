#pragma once
#include <string>

#ifdef _DEBUG
#define SPEED_TEST_AMOUNT 1e5
#else
#define SPEED_TEST_AMOUNT 1e8
#endif

int speedTestAddition(const std::string& fileName);
int speedTestMultiplication(const std::string& fileName);
int speedTestVec3addition(const std::string& fileName);
int speedTestMat3Vec3multiplication(const std::string& fileName);
int speedTestWriteCubes(const std::string& fileName);
int speedTestWriteRotatedCubes(const std::string& fileName);
int speedTestWriteNodesAndElements(const std::string& fileName);
int speedTestWriteLines(const std::string& fileName);
int speedTestWritePathLineStrip(const std::string& fileName);