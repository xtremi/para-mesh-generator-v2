#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <ctime>   
#include <sstream>

extern const std::string TEST_OUTPUT_FOLDER;
extern const std::string GMSH_EXE_PATH;

typedef int(*testFunction)(const std::string& fileName);

class TestDef
{

public:
	TestDef(
		int					_id,		
		const std::string&	_testName, 
		const std::string&  _category,
		testFunction		_function) 
	{
		id = _id;
		testName = _testName; function = _function;
		category = _category;
		fileName = std::to_string(id) + "_" + testName;
	}
	int		     id;
	std::string  testName, category;
	testFunction function;
	std::string  fileName;

	void run();

private:
	std::chrono::time_point<std::chrono::steady_clock> time_start, time_end;
	void		startTimer();
	int			durationMS();
	std::string durationString(int ms);


	void createGMSHbatScript();

};
