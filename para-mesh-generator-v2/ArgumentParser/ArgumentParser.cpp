#include "ArgumentParser.h"
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

const std::string ArgParse::StringManip::WHITESPACE = " \n\r\t\f\v";

bool ArgParse::ArgumentParser::hasArg(const std::string& arg) {
	return (args.find(arg) != args.end());
}

std::string ArgParse::ArgumentParser::getArg(const std::string& arg) {
	
	auto it = options.argCaseInsensitive ? args.find(StringManip::toLowerCase(arg)) : args.find(arg);
	if (it != args.end()) {
		return it->second;
	}
	return "";
}

int ArgParse::ArgumentParser::readInput(int argc, char* argv[]) {

	if (argc < (options.minArguments + 1)) {
		return options.notEnoughArgErrorCode;
	}

	std::string currentArg;
	std::string currentArgValue;
	for (int i = 1; i < argc; i++) {

		currentArg = argv[i];
		if (currentArg[0] == '-') {
			i++;
			if (i < argc + 1) {
				currentArgValue = argv[i];
				if(options.argCaseInsensitive){
					currentArgValue = StringManip::toLowerCase(currentArgValue);
				}				
				args[StringManip::toLowerCase(currentArg)] = currentArgValue;
			}
		}
		else {
			args[currentArg] = "";
		}
	}

	return processArgs();
}


std::string ArgParse::StringManip::toUpperCase(const std::string& str)
{
	std::string strL = str;
	std::transform(strL.begin(), strL.end(), strL.begin(), [](unsigned char c) { return std::toupper(c); });
	return strL;
}
std::string ArgParse::StringManip::toLowerCase(const std::string& str)
{
	std::string strL = str;
	std::transform(strL.begin(), strL.end(), strL.begin(), [](unsigned char c) { return std::tolower(c); });
	return strL;
}

/*template<typename T>
bool ArgParse::StringManip::toReal(const std::string& str, T& value) {
	try {
		value = std::stod(str);
	}
	catch (...) {
		value = 0.0;
		return false;
	}
	return true;
}*/

/*template<typename T>
bool ArgParse::StringManip::toInteger(const std::string& str, T& value) {
	try {
		value = std::stoi(str);
	}
	catch (...) {
		value = 0;
		return false;
	}
	return true;
}*/

std::vector<std::string> ArgParse::StringManip::stringToStdvector(const std::string &s, char delim, bool trimStrings) {
	std::stringstream		 ss(s);
	std::string				 item;
	std::vector<std::string> elems;

	while (std::getline(ss, item, delim)) {
		if (trimStrings)
			item = trim(item);
		elems.push_back(item);

	}
	return elems;
}

/*template <typename T>
std::vector<T> ArgParse::StringManip::stringVectorToRealVector(const std::vector<std::string>& strVec, T defaultValue) {
	std::vector<T> realVec;
	T val = 0.0;
	for (std::string str : strVec) {
		if (!toReal(str, val))
			val = defaultValue;
		realVec.push_back(val);
	}
	return realVec;
}*/

/*template <typename T>
std::vector<T> ArgParse::StringManip::stringVectorToIntVector(const std::vector<std::string>& strVec, T defaultValue) {
	std::vector<T> intVec;
	for (std::string str : strVec) {
		T val = 0;
		if (!toInteger(str, val))
			val = defaultValue;
		intVec.push_back(val);
	}
	return intVec;
}*/

/*template <typename T>
std::vector<T> ArgParse::StringManip::stringToRealVector(const std::string& str, char delim, T defaultValue) {
	std::vector<std::string> strvec = stringToStdvector(str, delim, true);
	return stringVectorToRealVector<T>(strvec, defaultValue);
}*/

/*template <typename T>
std::vector<T> ArgParse::StringManip::stringToIntVector(const std::string& str, char delim, T defaultValue) {
	std::vector<std::string> strvec = stringToStdvector(str, delim, true);
	return stringVectorToIntVector<T>(strvec, defaultValue);
}*/


std::string ArgParse::StringManip::leftTrim(const std::string &s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string ArgParse::StringManip::rightTrim(const std::string &s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string ArgParse::StringManip::trim(const std::string &s) {
	return rightTrim(leftTrim(s));
}