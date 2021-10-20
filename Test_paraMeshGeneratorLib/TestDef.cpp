#include "TestDef.h"
#include <fstream>

const std::string TEST_OUTPUT_FOLDER = "..\\test_output\\";
const std::string GMSH_EXE_PATH = "D:\\Program Files\\Gmsh\\gmsh-4.7.1-Windows64\\gmsh.exe";


void TestDef::run() {
	
	//Log:
	std::cout << std::right;
	std::cout << "Test: ";
	std::cout << "[" << std::setw(6) << std::to_string(id) << "] ";
	std::cout << "[" << std::setw(30) << category << "] ";
	std::cout << std::setw(42) << std::left << testName;
	startTimer();
	
	//Do test:
	std::string meshFilePath = TEST_OUTPUT_FOLDER + fileName + ".dat";
	
	std::string caughtErrorMessage = "";
	int testRes = false;
	try{
		testRes = function(meshFilePath);
	}
	catch (const char* errmsg) {
		testRes = false;
		caughtErrorMessage = errmsg;
	}

	//Log:
	std::string timeStr = durationString(durationMS());
	std::string resultString = testRes ? "FAIL" : "PASS";
	std::cout << resultString + " " + timeStr << std::endl;
	if(!caughtErrorMessage.empty()){
		std::cout << "\t" << "Caught error message: " << caughtErrorMessage << std::endl;
	}

	//GMSH bat script (for visualization):
	createGMSHbatScript();
}

void TestDef::createGMSHbatScript() {

	std::string batFileName = TEST_OUTPUT_FOLDER + fileName + ".bat";
	std::ofstream batFile;
	batFile.open(batFileName);
	if (!batFile.is_open()) return;

	batFile << "\"" << GMSH_EXE_PATH << "\"" << " -open " << fileName + ".dat";

	batFile.close();

}

void TestDef::startTimer() {
	time_start = std::chrono::high_resolution_clock::now();
}
int TestDef::durationMS() {
	std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - time_start);
	return t.count();
}

std::string TestDef::durationString(int ms) {

	std::string msStr = "";
	if (ms > 60000) {
		int min = ms / (60000);
		float sec = (float)(ms - min * 60000) / 1000.0;
		std::ostringstream ss; ss.precision(1);
		ss << min << "min" << std::fixed << sec << "s";
		msStr = ss.str();
	}
	else if (ms > 1000) {
		std::ostringstream ss; ss.precision(2);
		ss << std::fixed << (float)ms / 1000.0 << "s";
		msStr = ss.str();
	}
	else
		msStr = std::to_string(ms) + "ms";


	msStr.insert(0, 12 - msStr.size(), ' ');
	return ("[" + msStr + "]");
}
