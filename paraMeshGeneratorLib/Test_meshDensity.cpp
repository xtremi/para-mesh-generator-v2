#include "Test_meshDensity.h"
#include "PlaneMesher.h"
#include "CuboidMesher.h"
#include "Extrusion.h"
#include "RefinementCalculations.h"
#include "Test_convinient_functions.h"


std::vector<int> getNodeIteratorResult(NodeIterator& nit) {
	std::vector<int> res;
	for (int nid = nit.first(); nid; nid = nit.next()) {
		res.push_back(nid);
	}
	return res;
}

std::vector<std::vector<int>> getNodeIteratorResult_4(NodeIterator2D& nit) {
	std::vector<std::vector<int>> res;
	int n1, n2, n3, n4;
	for (int b = nit.first4(n1, n2, n3, n4); b; b = nit.next4(n1, n2, n3, n4)) {
		res.push_back(std::vector<int>({ n1,n2,n3,n4 }));
	}
	return res;
}

int meshDensity2DcornerNodes(const std::string& fileName){
	TEST_START2

	MeshDensity2D meshDens(3, 4);
	PlaneMesher::writeNodesXY(pos, glCsys, meshDens, glm::dvec2(5.0));
	PlaneMesher::writeElements(meshDens);

	int firstNodeID = 1;
	std::vector<int> expectedNodeIDs({1,3,12,10});
	for (int i = 0; i < 4; i++) {
		if ((firstNodeID + meshDens.cornerNode(i)) != expectedNodeIDs[i]) {
			return 1;
		}
	}

	TEST_END
}

int meshDensity2DrefCornerNodes(const std::string& fileName) {

	MeshDensity2Dref meshDens(3, 17);
	
	int firstNodeID = 1;
	std::vector<int> expectedNodeIDs({ 1,67,69,17 });
	for (int i = 0; i < 4; i++) {
		if ((firstNodeID + meshDens.cornerNode(i)) != expectedNodeIDs[i]) {
			return 1;
		}
	}
	return 0;
}



int meshDensity2DnodeIterator(const std::string& fileName) {

	MeshDensity2D meshDens(3, 4);
	std::vector<std::vector<int>> expectedNodeIDs({
		{1,4,7, 10},
		{1,2,3},
		{3,6,9,12},
		{10,11,12}
	});

	std::vector<std::vector<int>> actualNodeIDs;
	for (int i = 0; i < 4; i++) {
		NodeIterator1D iter = meshDens.edgeNodeIterator(i, 1);
		actualNodeIDs.push_back(getNodeIteratorResult(iter));
	};

	if (!equalVecVectors(actualNodeIDs, expectedNodeIDs)) {
		return 1;
	}
	return 0;
}

int meshDensity2DnodeIteratorPreNodes(const std::string& fileName) {
	MeshDensity2D meshDens(3, 4);
	int preNodes[4] = { 100, 33, 44, 660 };
	std::vector<std::vector<int>> expectedNodeIDs({
		{preNodes[0], 1,4,7, 10},
		{preNodes[1], 1,2,3},
		{preNodes[2], 3,6,9,12},
		{preNodes[3], 10,11,12}
		});

	std::vector<std::vector<int>> actualNodeIDs;
	for (int i = 0; i < 4; i++) {
		NodeIterator1D iter = meshDens.edgeNodeIterator(i, 1, preNodes[i]);
		actualNodeIDs.push_back(getNodeIteratorResult(iter));
	};

	if (!equalVecVectors(actualNodeIDs, expectedNodeIDs)) {
		return 1;
	}
	return 0;
}

int meshDensity2DrefNodeIterator(const std::string& fileName) {

	TEST_START2
	MeshDensity2Dref meshDens1(3, 17);
	MeshDensity2Dref meshDens2(3, 33);

	PlaneMesherRef::writeNodes(pos, glCsys, meshDens1, glm::dvec2(8.0, 5.0), false, plane::xy);
	PlaneMesherRef::writeElements(meshDens1);

	pos.x += 10.;
	PlaneMesherRef::writeNodes(pos, glCsys, meshDens2, glm::dvec2(16.0, 10.0), false, plane::xy);
	PlaneMesherRef::writeElements(meshDens2);
	writer.close();

	std::vector<std::vector<int>> actualNodeIDs;
	std::vector<std::vector<int>> expectedNodeIDs({
		{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
		{1,30,39,54,59,67},
		{67,68,69},
		{17,38,47,58,63,69}
	});	

	NodeIterator1D iter = meshDens1.edgeNodeIterator(0, 1);
	actualNodeIDs.push_back(getNodeIteratorResult(iter));
	NodeIterator1Dref iterRef = meshDens1.edgeNodeIteratorRefDir(1, 1);
	actualNodeIDs.push_back(getNodeIteratorResult(iterRef));
	iter = meshDens1.edgeNodeIterator(2, 1);
	actualNodeIDs.push_back(getNodeIteratorResult(iter));
	iterRef = meshDens1.edgeNodeIteratorRefDir(3, 1);
	actualNodeIDs.push_back(getNodeIteratorResult(iterRef));
	if (!equalVecVectors(actualNodeIDs, expectedNodeIDs)) {
		return 1;
	}

	expectedNodeIDs = {
		{70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102},
		{70,127,144,173,182,197},
		{197,198,199,200,201},
		{102,143,160,181,190,201}
	};
	actualNodeIDs.clear();

	iter = meshDens2.edgeNodeIterator(0, 70);
	actualNodeIDs.push_back(getNodeIteratorResult(iter));
	iterRef = meshDens2.edgeNodeIteratorRefDir(1, 70);
	actualNodeIDs.push_back(getNodeIteratorResult(iterRef));
	iter = meshDens2.edgeNodeIterator(2, 70);
	actualNodeIDs.push_back(getNodeIteratorResult(iter));
	iterRef = meshDens2.edgeNodeIteratorRefDir(3, 70);
	actualNodeIDs.push_back(getNodeIteratorResult(iterRef));
	if (!equalVecVectors(actualNodeIDs, expectedNodeIDs)) {
		return 1;
	}

	return 0;
}


int meshDensity3DnodeIterator(const std::string& fileName) {

	MeshDensity3D meshDens(3, 4, 5);

	std::vector<std::vector<int>> expectedNodeIDs;
	std::vector<std::vector<int>> actualNodeIDs;

	expectedNodeIDs = std::vector<std::vector<int>>({
		{1,4,7,10,	13,16,19,22, 25,28,31,34, 37,40,43,46, 49,52,55,58},
		{1,2,3, 13,14,15, 25,26,27, 37,38,39, 49,50,51},
		{3,6,9,12, 15,18,21,24, 27,30,33,36, 39,42,45,48, 51,54,57,60},
		{10,11,12, 22,23,24, 34,35,36, 46,47,48, 58,59,60},
		{1,2,3, 4,5,6, 7,8,9, 10,11,12},
		{49,50,51, 52,53,54, 55,56,57, 58,59,60},
		});
	
	for (int i = 0; i < 6; i++) {
		NodeIterator2D iter = meshDens.faceNodeIterator(i, 1);
		actualNodeIDs.push_back(getNodeIteratorResult(iter));
	}

	if (!equalVecVectors(actualNodeIDs, expectedNodeIDs)) {
		return 1;
	}

	return 0;
}

int meshDensity3DnodeIteratorPreNodes(const std::string& fileName) {
	MeshDensity3D meshDens(3, 4, 5);

	std::vector<std::vector<int>> expectedNodeIDs;
	std::vector<std::vector<int>> actualNodeIDs;

	//should be prenodes values per row not all in the beginning
	expectedNodeIDs = std::vector<std::vector<int>>({
		{100,1,4,7,10,		101,13,16,19,22,	102,25,28,31,34,	103,37,40,43,46,	104,49,52,55,58},
		{210,1,2,3,			220,13,14,15,		230,25,26,27,		240,37,38,39,		250,49,50,51},
		{1,3,6,9,12,		11,15,18,21,24,		21,27,30,33,36,		31,39,42,45,48,		41,51,54,57,60},
		{88,10,11,12,		90,22,23,24,		92,34,35,36,		94,46,47,48,		96,58,59,60},
		{100,1,2,3,			101,4,5,6,			102,7,8,9,			103,10,11,12},
		{100,49,50,51,		99,52,53,54,		98,55,56,57,		97,58,59,60},
		});

	std::vector<NodeIterator1D> preNodesIter({
		NodeIterator1D(100,		5, 1),
		NodeIterator1D(210,		5, 10),
		NodeIterator1D(1,		5, 10),
		NodeIterator1D(88,		5, 2),
		NodeIterator1D(100,		4, 1),
		NodeIterator1D(100,		4, -1),
	});

	for (int i = 0; i < 6; i++) {
		NodeIterator2D iter = meshDens.faceNodeIterator(i, 1, preNodesIter[i]);
		actualNodeIDs.push_back(getNodeIteratorResult(iter));
	}

	if (!equalVecVectors(actualNodeIDs, expectedNodeIDs)) {
		return 1;
	}

	return 0;
}


int meshDensity3DcornerNodes(const std::string& fileName) {
	TEST_START2

	MeshDensity3D meshDens(3, 4, 5);
	CuboidMesher::writeNodesXYZ(pos, glCsys, meshDens, glm::dvec3(5.0));
	CuboidMesher::writeElements(meshDens);

	int firstNodeID = 1;
	std::vector<int> expectedNodeIDs({ 1,49,58,10,3,51,60, 12 });
	for (int i = 0; i < 8; i++) {
		if ((firstNodeID + meshDens.cornerNode(i)) != expectedNodeIDs[i]) {
			return 1;
		}
	}


	TEST_END

}

int nodeIterator1D(const std::string& fileName) {

	std::vector<int> result;
	std::vector<int> expectedResult;
	
	NodeIterator1D it1(1, 4, 1);
	result = getNodeIteratorResult(it1);
	expectedResult = {1,2,3,4};
	if (!equalVectors(result, expectedResult)) return 1;

	NodeIterator1D it2(1, 4, 1, 9);
	result = getNodeIteratorResult(it2);
	expectedResult = { 9, 1, 2, 3, 4 };
	if (!equalVectors(result, expectedResult)) return 1;

	NodeIterator1D it3(2, 4, 2);
	result = getNodeIteratorResult(it3);
	expectedResult = { 2, 4, 6, 8 };
	if (!equalVectors(result, expectedResult)) return 1;

	NodeIterator1D it4(99, 3, 100, 1000);
	result = getNodeIteratorResult(it4);
	expectedResult = { 1000, 99, 199, 299 };
	if (!equalVectors(result, expectedResult)) return 1;
	
	return 0;
}



int nodeIterator1Dm(const std::string& fileName) {

	std::vector<int> result;
	std::vector<int> expectedResult;

	
	NodeIterator1D it1(1, 4, 1);		//1,2,3,4	
	NodeIterator1D it2(8, 2, 2, 9);		//9,8,10	
	NodeIterator1D it3(100, 3, 3, 900);	//900,100,103,106

	expectedResult = { 1,2,3,4, 9,8,10, 900,100,103,106 };
	NodeIterator1Dm itm1(std::vector<NodeIterator1D>({ it1,it2,it3 }), false);
	result = getNodeIteratorResult(itm1);
	if (!equalVectors(result, expectedResult)) return 1;

	it1 = NodeIterator1D(10, 4, 1);			//10,11,12,13	
	it2 = NodeIterator1D(8, 2, 2, 13);		//13,8,10	
	it3 = NodeIterator1D(100, 3, 3, 10);	//10,100,103,106


	expectedResult = { 10,11,12,13,		8,10,	100,103,106 };
	itm1 = NodeIterator1Dm(std::vector<NodeIterator1D>({ it1,it2,it3 }), true);
	result = getNodeIteratorResult(itm1);
	if (!equalVectors(result, expectedResult)) return 1;

	return 0;
}

/*

	NOT COMPLETED
*/
int nodeIterator1Dref(const std::string& fileName) {
	TEST_START2

	glm::dvec2 size(20.0, 17.0);
	int nRef = 3;
	int nNodesDir2 = refinement::minNodesEdge0_2d(nRef);
	MeshDensity2Dref meshDens(nRef, nNodesDir2);

	PlaneMesherRef::writeNodes(pos, glCsys, meshDens, size, true, plane::xy);
	PlaneMesherRef::writeElements(meshDens);

	/*edge3*/
	std::vector<int> result;
	std::vector<int> expectedResult({ 17,38,47,58,63,69 });

	NodeIterator1Dref it1(17, nRef, 16, NodeIterator1Dref::Type::edge3);
	result = getNodeIteratorResult(it1);
	if (!equalVectors(result, expectedResult)) return 1;
	
	if (it1.get(0) != 17) return 1;
	if (it1.get(1) != 38) return 1;
	if (it1.get(5) != 69) return 1;
	if (it1.last() != 69) return 1;

	/*edge1*/
	expectedResult = std::vector<int>({ 1,30,39,54,59,67 });
	NodeIterator1Dref it2(1, nRef, 16, NodeIterator1Dref::Type::edge1);
	result = getNodeIteratorResult(it2);
	if (!equalVectors(result, expectedResult)) return 1;
	
	if (it2.get(0) != 1) return 1;
	if (it2.get(2) != 39) return 1;
	if (it2.get(5) != 67) return 1;
	if (it2.last() != 67) return 1;

	/*edge3 with preNode*/
	expectedResult = std::vector<int>({ 100, 1,30,39,54,59,67 });
	NodeIterator1Dref it3(1, nRef, 16, NodeIterator1Dref::Type::edge1, 100);
	result = getNodeIteratorResult(it3);
	if (!equalVectors(result, expectedResult)) return 1;

	/*edge1 with preNode*/
	expectedResult = std::vector<int>({1000, 1,30,39,54,59,67 });
	NodeIterator1Dref it4(1, nRef, 16, NodeIterator1Dref::Type::edge1, 1000);
	result = getNodeIteratorResult(it4);
	if (!equalVectors(result, expectedResult)) return 1;

	TEST_END
}

int nodeIterator2D(const std::string& fileName) {

	std::vector<int> result;
	std::vector<int> expectedResult;

	//From NodeIterator2D example 1
	NodeIterator2D it1(1, 5, 3, 25, 5);
	result = getNodeIteratorResult(it1);
	expectedResult = { 1,26,51,76,101,6,31,56,81,106,11,36,61,86,111 };
	if (!equalVectors(result, expectedResult)) return 1;

	//From NodeIterator2D example 2
	NodeIterator1D preNodesIt(101, 3, 1);
	NodeIterator2D it2(1, 3, 3, 1, 12, preNodesIt);
	result = getNodeIteratorResult(it2);
	expectedResult = { 101,1,2,3, 102,13,14,15, 103,25,26,27 };
	if (!equalVectors(result, expectedResult)) return 1;

	return 0;
}

int nodeIterator2D_4(const std::string& fileName) {

	std::vector<std::vector<int>> result;
	std::vector<std::vector<int>> expectedResult;

	/*
	::::::::::::::::::::::::::::::::::::::::::::
	From NodeIterator2D example 1
	::::::::::::::::::::::::::::::::::::::::::::
		nNodesX = 5, nNodesY = 3
			   	  		  
	incrY = 5
	  ^	  11x   36x   61x   86x  111x
	  |       
	dirY   6x   31x   56x   81x  106x
	  |	   
	  |    1x   26x   51x   76x  101x

		----> dirX incrX = 25
	
	
	*/
	NodeIterator2D it1(1, 5, 3, 25, 5);
	result = getNodeIteratorResult_4(it1);
	expectedResult = { 
		{1, 26, 31, 6}, {26, 51, 56, 31}, {51, 76, 81, 56}, {76, 101, 106, 81}, 
		{6, 31, 36, 11}, {31, 56, 61, 36}, {56, 81, 86, 61}, {81, 106, 111, 86}
	};

	if (!equalVecVectors(result, expectedResult)) return 1;

	//From NodeIterator2D example 2
	//NodeIterator1D preNodesIt(101, 3, 1);
	//NodeIterator2D it2(1, 3, 3, 1, 12, preNodesIt);
	//result = getNodeIteratorResult(it2);
	//expectedResult = { 101,1,2,3, 102,13,14,15, 103,25,26,27 };
	//if (!equalVectors(result, expectedResult)) return 1;

	return 0;
}

int nodeIterator2Dref(const std::string& fileName) {
	TEST_START2

	glm::dvec3 size(15.0, 10.0, 40.);
	int nRef = 2;
	int nNodesDir12 = 9;
	MeshDensity3Dref meshDens(nRef, nNodesDir12, nNodesDir12, false);

	CuboidMesherRef::writeNodes(pos, glCsys, meshDens, size, true, plane::yz);
	CuboidMesherRef::writeElements(meshDens);

	//Face 1
	std::vector<std::vector<int>> resFace1;
	std::vector<std::vector<int>> expFace1 = {
		{1,136,141,10}, {10,141,146,19},
		{19,146,151,28}, {28,151,156,37},
		{37,156,161,46}, {46,161,166,55},
		{55,166,171,64}, {64,171,176,73},

		{136,211,181,141},{181,211,216,186},{141,181,186,146},
		{146,186,191,151},{186,216,221,191},{151,191,221,156},

		{156,221,196,161},{196,221,226,201},{161,196,201,166},
		{166,201,206,171},{201,226,231,206},{171,206,231,176},

		{211,236,241,216},{216,241,246,221},{221,246,251,226},{226,251,256,231},

		{236,276,279,241},{241,279,282,246},{246,282,285,251},{251,285,288,256},

		{276,300,291,279},{291,300,303,294},{279,291,294,282},
		{282,294,297,285},{294,303,306,297},{285,297,306,288}
	};

	//Face 3
	std::vector<std::vector<int>> resFace3;
	std::vector<std::vector<int>> expFace3 = {
		{9,140,145,18}, {18,145,150,27},
		{27,150,155,36}, {36,155,160,45},
		{45,160,165,54}, {54,165,170,63},
		{63,170,175,72}, {72,175,180,81}, //0-7

		{140,215,185,145},{185,215,220,190},{145,185,190,150},
		{150,190,195,155},{190,220,225,195},{155,195,225,160}, //8-13

		{160,225,200,165},{200,225,230,205},{165,200,205,170},
		{170,205,210,175},{205,230,235,210},{175,210,235,180},//14-18
		
		{215,240,245,220},{220,245,250,225},{225,250,255,230},{230,255,260,235}, //20-24

		{240,278,281,245},{245,281,284,250},{250,284,287,255},{255,287,290,260}, //25-28

		{278,302,293,281},{293,302,305,296},{281,293,296,284},
		{284,296,299,287},{296,305,308,299},{287,299,308,290}//29-34
	};

	//Face 4
	std::vector<std::vector<int>> resFace4;
	std::vector<std::vector<int>> expFace4 = {
		{1,136,82,2},{82,136,137,91},{2,82,91,3},
		{3,91,100,4},{91,137,138,100},{4,100,138,5}, //0-5

		{5,138,109,6},{109,138,139,118},{6,109,118,7},
		{7,118,127,8},{118,139,140,127},{8,127,140,9},//6-11

		{136,211,212,137},{137,212,213,138},{138,213,214,139},{139,214,215,140},	//12-15

		{211,236,237,212},{212,237,238,213},{213,238,239,214},{214,239,240,215}, //16-19

		{236,276,261,237},{261,276,277,266},{237,261,266,238},
		{238,266,271,239},{266,277,278,271},{239,271,278,240},//20-25

		{276,300,301,277},{277,301,302,278}, //26-27

	};

	//Face 5
	std::vector<std::vector<int>> resFace5;
	std::vector<std::vector<int>> expFace5 = {
		{73,176,90,74},{90,176,177,99},{74,90,99,75},
		{75,99,108,76},{99,177,178,108},{76,108,178,77}, //0-5

		{77,178,117,78},{117,178,179,126},{78,117,126,79},
		{79,126,135,80},{126,179,180,135},{80,135,180,81},//6-11

		{176,231,232,177},{177,232,233,178},{178,233,234,179},{179,234,235,180},//12-15

		{231,256,257,232},{232,257,258,233},{233,258,259,234},{234,259,260,235},//16-19

		{256,288,265,257},{265,288,289,270},{257,265,270,258},
		{258,270,275,259},{270,289,290,275},{259,275,290,260},//20-25

		{288,306,307,289},{289,307,308,290}, //26-27

	};

	//Face1
	NodeIterator2Dref it1(1, nNodesDir12, nNodesDir12, nRef, NodeIterator2Dref::Type::face1);
	resFace1 = getNodeIteratorResult_4(it1);
	if (!equalVecVectors(resFace1, expFace1))
		return 1;

	//Face3
	NodeIterator2Dref it3(1, nNodesDir12, nNodesDir12, nRef, NodeIterator2Dref::Type::face3);
	resFace3 = getNodeIteratorResult_4(it3);
	if (!equalVecVectors(resFace3, expFace3))
		return 1;
	
	//Face5
	NodeIterator2Dref it5(1, nNodesDir12, nNodesDir12, nRef, NodeIterator2Dref::Type::face5);
	resFace5 = getNodeIteratorResult_4(it5);
	if (!equalVecVectors(resFace5, expFace5))
		return 1;

	//Face4
	NodeIterator2Dref it4(1, nNodesDir12, nNodesDir12, nRef, NodeIterator2Dref::Type::face4);
	resFace4 = getNodeIteratorResult_4(it4);
	if (!equalVecVectors(resFace4, expFace4))
		return 1;


	TEST_END
}

/*
	Partly similar to test nodeIterator2Dref()
*/
int meshDensity3DnodeIteratorRef(const std::string& fileName) {

	TEST_START2

	glm::dvec3 size(15.0, 10.0, 40.);
	int nRef = 2;
	int nNodesDir12 = 9;
	MeshDensity3Dref meshDens(nRef, nNodesDir12, nNodesDir12, false);

	//Face 1
	std::vector<std::vector<int>> expFace1 = {
		{1,136,141,10}, {10,141,146,19},
		{19,146,151,28}, {28,151,156,37},
		{37,156,161,46}, {46,161,166,55},
		{55,166,171,64}, {64,171,176,73},

		{136,211,181,141},{181,211,216,186},{141,181,186,146},
		{146,186,191,151},{186,216,221,191},{151,191,221,156},

		{156,221,196,161},{196,221,226,201},{161,196,201,166},
		{166,201,206,171},{201,226,231,206},{171,206,231,176},

		{211,236,241,216},{216,241,246,221},
		{221,246,251,226},{226,251,256,231},

		{236,276,279,241},{241,279,282,246},
		{246,282,285,251},{251,285,288,256},

		{276,300,291,279},{291,300,303,294},{279,291,294,282},
		{282,294,297,285},{294,303,306,297},{285,297,306,288}
	};

	//Face 3
	std::vector<std::vector<int>> expFace3 = {
		{9,140,145,18}, {18,145,150,27},
		{27,150,155,36}, {36,155,160,45},
		{45,160,165,54}, {54,165,170,63},
		{63,170,175,72}, {72,175,180,81}, //0-7

		{140,215,185,145},{185,215,220,190},{145,185,190,150},
		{150,190,195,155},{190,220,225,195},{155,195,225,160}, //8-13

		{160,225,200,165},{200,225,230,205},{165,200,205,170},
		{170,205,210,175},{205,230,235,210},{175,210,235,180},//14-18

		{215,240,245,220},{220,245,250,225}, //20-24
		{225,250,255,230},{230,255,260,235},

		{240,278,281,245},{245,281,284,250},
		{250,284,287,255},{255,287,290,260}, //25-28

		{278,302,293,281},{293,302,305,296},{281,293,296,284},
		{284,296,299,287},{296,305,308,299},{287,299,308,290}//29-34
	};

	//Face0:
	std::vector<int> expFace0 = linearlyOrderedVector(1, 81, 1);
	std::vector<int> expFace2 = linearlyOrderedVector(300, 308, 1);


	std::vector<std::vector<int>> resFace1, resFace3, resFace4, resFace5;
	std::vector<int> resFace0, resFace2;

	//Face 0
	NodeIterator2D it0(meshDens.faceNodeIterator(0, 1));
	resFace0 = getNodeIteratorResult(it0);
	if (!equalVectors(resFace0, expFace0))
		return 1;

	//Face1
	NodeIterator2Dref it1(meshDens.faceNodeIteratorRefDir(1, 1));
	resFace1 = getNodeIteratorResult_4(it1);
	if (!equalVecVectors(resFace1, expFace1))
		return 1;

	//Face 2
	NodeIterator2D it2(meshDens.faceNodeIterator(2, 1));
	resFace2 = getNodeIteratorResult(it2);
	if (!equalVectors(resFace2, expFace2))
		return 1;

	//Face3
	NodeIterator2Dref it3(meshDens.faceNodeIteratorRefDir(3, 1));
	resFace3 = getNodeIteratorResult_4(it3);
	if (!equalVecVectors(resFace3, expFace3))
		return 1;



	return 0;

}


int nodeIterator2Dm(const std::string& fileName) {
	return 1;
}




int meshEdgeExtrusion(const std::string& fileName) {

	double length = 10.0;

	/*
		3-->
^	9--10--11--12--------17--------18--25--26--27   ^
|	|   |   |   |        |          |   |   |   |   |
0	5---6---7---8--------15--------16--22--23--24   2
	|   |   |   |        |          |   |   |   |
	1---2---3---4--------13--------14--19--20--21
	   1-->

	*/
	std::vector<std::vector<int>> expectedEdges;
	std::vector<std::vector<int>> resultEdges;

	//First extrusion:
	MeshEdgeExtrusionLinear edgeExtr1(length, 3, 3, 1);
	expectedEdges = std::vector<std::vector<int>>({
		{1,5,9},
		{1,2,3,4},
		{4,8,12},
		{9,10,11,12},
		{1,5,9}
		});

	resultEdges = std::vector<std::vector<int>>({
		getNodeIteratorResult(*edgeExtr1.edges[0].nodeIter),
		getNodeIteratorResult(*edgeExtr1.edges[1].nodeIter),
		getNodeIteratorResult(*edgeExtr1.edges[2].nodeIter),
		getNodeIteratorResult(*edgeExtr1.edges[3].nodeIter),
		getNodeIteratorResult(*edgeExtr1.edges[4].nodeIter)
		});

	for (int i = 0; i < 5; i++) {
		if (!equalVectors(expectedEdges[i], resultEdges[i])) return 1;
	}

	//Second extrusion:
	MeshEdgeExtrusionLinear edgeExtr2(length, 2, 3, 13, &edgeExtr1);
	expectedEdges = std::vector<std::vector<int>>({
		{4, 8, 12},
		{4, 13, 14},
		{14,16,18},
		{12, 17, 18},
		{13, 15, 17}
		});

	resultEdges = std::vector<std::vector<int>>({
		getNodeIteratorResult(*edgeExtr2.edges[0].nodeIter),
		getNodeIteratorResult(*edgeExtr2.edges[1].nodeIter),
		getNodeIteratorResult(*edgeExtr2.edges[2].nodeIter),
		getNodeIteratorResult(*edgeExtr2.edges[3].nodeIter),
		getNodeIteratorResult(*edgeExtr2.edges[4].nodeIter)
		});

	for (int i = 0; i < 5; i++) {
		if (!equalVectors(expectedEdges[i], resultEdges[i])) return 1;
	}

	//Third extrusion:
	MeshEdgeExtrusionLinear edgeExtr3(length, 3, 3, 19, &edgeExtr2);
	expectedEdges = std::vector<std::vector<int>>({
		{14, 16, 18},
		{14, 19, 20, 21},
		{21, 24, 27},
		{18, 25, 26, 27},
		{19, 22, 25}
		});

	resultEdges = std::vector<std::vector<int>>({
		getNodeIteratorResult(*edgeExtr3.edges[0].nodeIter),
		getNodeIteratorResult(*edgeExtr3.edges[1].nodeIter),
		getNodeIteratorResult(*edgeExtr3.edges[2].nodeIter),
		getNodeIteratorResult(*edgeExtr3.edges[3].nodeIter),
		getNodeIteratorResult(*edgeExtr3.edges[4].nodeIter),
		});

	for (int i = 0; i < 5; i++) {
		if (!equalVectors(expectedEdges[i], resultEdges[i])) return 1;
	}



	return 0;
}



int meshFaceExtrusion(const std::string& fileName) {

	double length = 10.0;

	TEST_START2
		MeshDensity3D meshDens(3, 4, 3);
	MeshDensity2D meshDensFace(meshDens.dir2(), meshDens.dir3());
	CuboidMesher::writeNodes(NULL_POS, glCsys, meshDens, glm::dvec3(5.0, 7.0, 5.0), plane::xy);
	CuboidMesher::writeElements(meshDens);

	std::vector<std::vector<int>> expectedFaces;
	std::vector<std::vector<int>> resultFaces;

	//First extrusion:
	MeshFaceExtrusionLinear faceExtr1(length, 2, meshDensFace, 1);

	expectedFaces = std::vector<std::vector<int>>({
		{1,4,7,10,	13,16,19,22, 25,28,31,34},
		{1,2,3, 13,14,15, 25,26,27},
		{3,6,9,12, 15,18,21,24, 27,30,33,36},
		{10,11,12, 22,23,24, 34,35,36},
		{1,2,3, 4,5,6, 7,8,9, 10,11,12},
		{25,26,27, 28,29,30, 31,32,33, 34,35,36}
		});
	resultFaces.clear();
	for (int i = 0; i < 6; i++) {
		resultFaces.push_back(getNodeIteratorResult(faceExtr1.faces[i].nodeIter));
	}

	for (int i = 0; i < 6; i++) {
		if (!equalVectors(expectedFaces[i], resultFaces[i])) return 1;
	}

	//Second extrusion:	
	MeshFaceExtrusionLinear faceExtr2(length, 2, meshDensFace, 37, &faceExtr1);
	expectedFaces = std::vector<std::vector<int>>({
		{3,6,9,12, 15,18,21,24, 27,30,33,36},
		{3,37,38, 15,45,46, 27,53,54},
		{38,40,42,44, 46,48,50,52, 54,56,58,60},
		{12,43,44, 24,51,52, 36,59,60},
		{3,37,38, 6,39,40, 9,41,42, 12,43,44},
		{27,53,54, 30,55,56, 33,57,58, 36,59,60},

		{37,39,41,43, 45,47,49,51, 53,55,57,59},
		});
	resultFaces.clear();
	for (int i = 0; i < 7; i++) {
		resultFaces.push_back(getNodeIteratorResult(faceExtr2.faces[i].nodeIter));
	}

	for (int i = 0; i < 7; i++) {
		if (!equalVectors(expectedFaces[i], resultFaces[i])) return 1;
	}

	//third extrusion:
	MeshFaceExtrusionLinear faceExtr3(length, 2, meshDensFace, 61, &faceExtr2);
	expectedFaces = std::vector<std::vector<int>>({
		{38,40,42,44, 46,48,50,52, 54,56,58,60},
		{38,61,62, 46,69,70, 54,77,78},
		{62,64,66,68, 70,72,74,76, 78,80,82,84},
		{44,67,68, 52,75,76, 60,83,84},
		{38,61,62, 40,63,64, 42,65,66, 44,67,68},
		{54,77,78, 56,79,80, 58,81,82, 60,83,84},

		{61,63,65,67, 69,71,73,75, 77,79,81,83},
		});
	resultFaces.clear();
	for (int i = 0; i < 7; i++) {
		resultFaces.push_back(getNodeIteratorResult(faceExtr3.faces[i].nodeIter));
	}

	for (int i = 0; i < 7; i++) {
		if (!equalVectors(expectedFaces[i], resultFaces[i])) return 1;
	}

	return 0;
}