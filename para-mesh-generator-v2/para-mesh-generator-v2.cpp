#include "PlaneMesher.h"
#include "ConeMesher.h"
#include "ArgumentParser/ArgumentParser.h"

using namespace ArgParse;



int main(int argc, char* argv[]) {
	std::cout << "start" << std::endl;
	ArgumentParser argParser;
	argParser.options.argCaseInsensitive = true;
	argParser.readInput(argc, argv);
	

	std::string mesherType = argParser.getArg("-m");
	std::string outPath, format;
	glm::dvec3 csysX, csysY, pos;
	glm::dvec2 size2D, radius2D, angles;
	glm::ivec2 nNodes2D;
	glm::ivec3 nNodes3D;
	double height;
	int nref = 3;
	
	argParser.getArg("-o", outPath);
	argParser.getArg("-f",			format,		"nas");
	argParser.getArg("-csysx",		csysX,		X_DIR);
	argParser.getArg("-csysy",		csysY,		Y_DIR);
	argParser.getArg("-pos",		pos,		NULL_POS);
	argParser.getArg("-nnodes2D",	nNodes2D,	glm::ivec2(10,10));
	argParser.getArg("-nnodes3D",	nNodes3D,	glm::ivec3(10,10,10));
	argParser.getArg("-size2D",		size2D,		glm::dvec2(10.0,10.0));
	argParser.getArg("-rad2D",		radius2D,	glm::dvec2(5.0,10.0));
	argParser.getArg("-angles",		angles,		glm::dvec2(0.0, GLMPI * 1.7));
	argParser.getArg("-height",		height,		10.0);	
	argParser.getArg("-nref",		nref,		3);	

	if (outPath.empty()) {
		return 1;
	}
	std::ofstream file;
	file.open(outPath);
	if (!file.is_open()) {
		return 2;
	}


	glm::dmat3x3 orientation = makeCsysMatrix(csysX, csysY);
	MeshCsys glcsys(pos, &orientation);

	FEAwriter* writer = nullptr;
	if (format == "nas") {
		writer = new NastranFEAwriter(&file);
		Mesher::setFEAwriter(writer);
	}
	else {
		return 3;
	}

	if (mesherType == "planemesher") {
		std::cout << "planemesher" << std::endl;
		PlaneMesher::writeNodes(NULL_POS, glcsys, MeshDensity2D(nNodes2D.x, nNodes2D.y), size2D, plane::xy);
		PlaneMesher::writeElements(MeshDensity2D(nNodes2D.x, nNodes2D.y));
	}
	else if (mesherType == "conemesher") {
		ConeMesher::writeNodes(NULL_POS, glcsys, MeshDensity2D(nNodes2D.x, nNodes2D.y), 
			Cone2Dradius(radius2D.x, radius2D.y), ArcAngles(angles.x, angles.y), height, direction::x);
		ConeMesher::writeElements(MeshDensity2D(nNodes2D.x, nNodes2D.y));
	}
	else {
		return 4;
	}
	
	writer->close();
	std::cout << "end" << std::endl;
	return 0;
}