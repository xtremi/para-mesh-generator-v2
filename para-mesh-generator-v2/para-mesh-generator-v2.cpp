#include "PlaneMesher.h"
#include "ConeMesher.h"
#include "ArgumentParser/ArgumentParser.h"

using namespace ArgParse;



int main(int argc, char* argv[]) {

	ArgumentParser argParser;
	argParser.options.argCaseInsensitive = true;
	argParser.readInput(argc, argv);
	

	std::string mesherType = argParser.getArg("-m");

	glm::dvec3 csysX, csysY, pos;
	glm::dvec2 size;
	glm::ivec2 nNodes;
	argParser.getArg("-csysx",  csysX,	X_DIR);
	argParser.getArg("-csysy",  csysY,	Y_DIR);
	argParser.getArg("-pos",    pos,	NULL_POS);
	argParser.getArg("-nnodes", nNodes,	glm::ivec2(10,10));
	argParser.getArg("-size",	size,	glm::ivec2(10.0,10.0));



	if (mesherType == "planemesher") {
		
	}
	


	return 0;
}