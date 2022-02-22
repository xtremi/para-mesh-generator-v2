#include <iostream>
#include <fstream>
#include <string>

static const int         DEFAULT_INPUT_1 = 5;
static const std::string DEFAULT_INPUT_2 = "sample_text";
static const std::string DEFAULT_INPUT_3 = "output.txt";

int toInt(const std::string& str, int valueIfFailed);

int main(int argc, char* argv[])
{
	std::cout << "web_cpp_app entry" << std::endl;

	int			input1 = DEFAULT_INPUT_1;
	std::string input2 = DEFAULT_INPUT_2;
	std::string input3 = DEFAULT_INPUT_3;

	if (argc > 1) {
		input1 = toInt(argv[1], DEFAULT_INPUT_1);
	}
	if (argc > 2) {
		input2 = std::string(argv[2]);
	}
   if (argc > 3) {
      input3 = std::string(argv[3]);
   }
			
	std::ofstream file;
	file.open(input3);
	if (!file.is_open())
		return 1;
	std::cout << input3 << " opened." << std::endl;
	for (int i = 0; i < input1; i++)
		file << input2 << "\n";
	
	std::cout << "\tData written." << std::endl;

	file.close();
	std::cout << input3 << " closed." << std::endl;


	std::cout << "web_cpp_app exit" << std::endl;
	return 0;
}

int toInt(const std::string& str, int valueIfFailed) {
	int i = 0;
	try {
		i = std::stoi(str);
	}
	catch (...) {
		i = valueIfFailed;
	}
	return i;
}