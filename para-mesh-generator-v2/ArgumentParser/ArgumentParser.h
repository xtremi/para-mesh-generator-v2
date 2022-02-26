#pragma once
#include <string>
#include <map>
#include <vector>
#ifdef USING_GLM
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#endif

namespace ArgParse{

	namespace StringManip{

		extern const std::string WHITESPACE;

		std::string toUpperCase(const std::string& str);
		std::string toLowerCase(const std::string& str);
		std::string	leftTrim(const std::string &s);
		std::string	rightTrim(const std::string &s);
		std::string	trim(const std::string &s);
		
		std::vector<std::string> stringToStdvector(const std::string &s, char delim, bool trimStrings);

		template<typename T>
		bool					toReal(const std::string& str, T& value) {
			try {
				value = std::stod(str);
			}
			catch (...) {
				value = 0.0;
				return false;
			}
			return true;
		}

		template<typename T>
		bool					toInteger(const std::string& str, T& value) {
			try {
				value = std::stoi(str);
			}
			catch (...) {
				value = 0;
				return false;
			}
			return true;
		}
		
		template <typename T>
		std::vector<T>			stringVectorToRealVector(const std::vector<std::string>& strVec, T defaultValue) {
			std::vector<T> realVec;
			T val = 0.0;
			for (std::string str : strVec) {
				if (!toReal(str, val))
					val = defaultValue;
				realVec.push_back(val);
			}
			return realVec;
		}
		
		template <typename T>
		std::vector<T>			stringVectorToIntVector(const std::vector<std::string>& strVec, T defaultValue = 0) {
			std::vector<T> intVec;
			for (std::string str : strVec) {
				T val = 0;
				if (!toInteger(str, val))
					val = defaultValue;
				intVec.push_back(val);
			}
			return intVec;
		}

		template <typename T>
		std::vector<T>			stringToRealVector(const std::string& str, char delim, T defaultValue = 0.0) {
			std::vector<std::string> strvec = stringToStdvector(str, delim, true);
			return stringVectorToRealVector<T>(strvec, defaultValue);
		}
		
		template <typename T>
		T stringToGlmVec(const std::string& str, char delim, double defaultValue = 0.0) {
			std::vector<std::string> strvec = stringToStdvector(str, delim, true);
			std::vector<double> realvec = stringVectorToRealVector<double>(strvec, defaultValue);

			T val; 
			for(int i = 0; i < val.length(); i++){
				if (realvec.size() > i) {
					val[i] = realvec[i];
				}
			}
			return val;
		}

		template <typename T>
		std::vector<T>			 stringToIntVector(const std::string& str, char delim, T defaultValue = 0) {
			std::vector<std::string> strvec = stringToStdvector(str, delim, true);
			return stringVectorToIntVector<T>(strvec, defaultValue);
		}

	}

class ArgumentParser {
public:
	ArgumentParser() {}

	int			readInput(int argc, char* argv[]);
	bool		hasArg(const std::string& arg);
	std::string getArg(const std::string& arg);
	

	struct Options {
		bool	argCaseInsensitive = false;
		int		minArguments = 0;
		int		notEnoughArgErrorCode = 1;

	};
	Options options;

protected:
	std::map<std::string, std::string> args;

	virtual int processArgs() { return 0; }	

public:

	//template <typename T>
	//bool getArg(const std::string& arg, T& value, const T& defaultValue) {
	//	return false;
	//}

	bool getArg(const std::string& arg, std::string& value, const std::string& defaultValue = "") {
		value = getArg(arg);
		if (value.empty()) {
			value = defaultValue;
			return false;
		}
		return true;
	}

	bool getArg(const std::string& arg, double& value, const double& defaultValue) {
		std::string strVal;
		if (!getArg(arg, strVal, "")) {
			value = defaultValue;
			return false;
		}
		return StringManip::toReal<double>(strVal, value);
	}

	bool getArg(const std::string& arg, int& value, const int& defaultValue) {
		std::string strVal;
		if (!getArg(arg, strVal, "")) {
			value = defaultValue;
			return false;
		}
		return StringManip::toInteger<int>(strVal, value);
	}

	bool getArg(const std::string& arg, glm::dvec2& value, const glm::dvec2& defaultValue) {
		std::string strVal;
		if (!getArg(arg, strVal, "")) {
			value = defaultValue;
			return false;
		}
		value = StringManip::stringToGlmVec<glm::dvec2>(strVal, ',', 0.0);
		return true;
	}

	bool getArg(const std::string& arg, glm::dvec3& value, const glm::dvec3& defaultValue) {
		std::string strVal;
		if (!getArg(arg, strVal, "")) {
			value = defaultValue;
			return false;
		}
		value = StringManip::stringToGlmVec<glm::dvec3>(strVal, ',', 0.0);
		return true;
	}

	bool getArg(const std::string& arg, glm::ivec2& value, const glm::dvec2& defaultValue) {
		std::string strVal;
		if (!getArg(arg, strVal, "")) {
			value = defaultValue;
			return false;
		}
		value = StringManip::stringToGlmVec<glm::ivec2>(strVal, ',', 0.0);
		return true;
	}

	bool getArg(const std::string& arg, glm::ivec3& value, const glm::dvec3& defaultValue) {
		std::string strVal;
		if (!getArg(arg, strVal, "")) {
			value = defaultValue;
			return false;
		}
		value = StringManip::stringToGlmVec<glm::ivec3>(strVal, ',', 0.0);
		return true;
	}

};


}