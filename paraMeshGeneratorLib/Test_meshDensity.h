#pragma once
#include <string>
#include <vector>

int meshDensity2DcornerNodes(const std::string& fileName);
int meshDensity2DrefCornerNodes(const std::string& fileName);
int meshDensity3DcornerNodes(const std::string& fileName);
int meshDensity2DnodeIterator(const std::string& fileName);
int meshDensity2DnodeIteratorPreNodes(const std::string& fileName);
int meshDensity2DrefNodeIterator(const std::string& fileName);
int meshDensity3DnodeIterator(const std::string& fileName);
int meshDensity3DnodeIteratorPreNodes(const std::string& fileName);
int meshDensity3DnodeIteratorRef(const std::string& fileName);

int nodeIterator1D(const std::string& fileName);
int nodeIterator1Dref(const std::string& fileName);
int nodeIterator1Dm(const std::string& fileName);
int nodeIterator2D(const std::string& fileName);
int nodeIterator2D_4(const std::string& fileName);
int nodeIterator2Dref(const std::string& fileName);
int nodeIterator2Dm(const std::string& fileName);

int meshEdgeExtrusion(const std::string& fileName);
int meshFaceExtrusion(const std::string& fileName);

std::vector<int> getNodeIteratorResult(NodeIterator& nit);
std::vector<std::vector<int>> getNodeIteratorResult_4(NodeIterator2D& nit);

