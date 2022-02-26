#include "PlaneMesher.h"
#include "ConeMesher.h"
#include "CuboidMesher.h"
#include "Cone3Dmesher.h"

#include "ArgumentParser/ArgumentParser.h"
#include "glm/gtx/string_cast.hpp"

using namespace ArgParse;

struct Input {
	std::string			outPath;	// -o
	std::string			format;		// -f
	std::string			mesherType;	// -m
	MeshCsys			csys;		// -csysX, -csysY, -pos
	MeshDensity2D		md2D;		// -nNodes2D
	MeshDensity3D		md3D;		// -nNodes3D
	MeshDensity2Dref	md2Dref;	// -nNodes2D, -nref
	MeshDensity3Dref	md3Dref;	// -nNodes3D, -nref
	glm::dvec2			size2D;		// -size2D
	glm::dvec3			size3D;		// -size3D
	ArcAngles			arcAngels;	// -angles
	Cone2Dradius		cone2Drad;	// -radius2D
	Pipe3Dradius		pipe3Drad;	// -radius3D
	double				height;		// -height
	int					nref;		// -nref
	static Input get(int argc, char* argv[]);

	std::string toStr();

private:
	glm::dmat3x3 orientation;
};

int main(int argc, char* argv[]) {
	std::cout << "PMG start" << std::endl;
	Input input = Input::get(argc, argv);
	std::cout << "PMG input: " << std::endl;
	std::cout << input.toStr() << std::endl;
	
	if (input.outPath.empty()) {
		return 1;
	}
	std::ofstream file;
	file.open(input.outPath);
	if (!file.is_open()) {
		return 2;
	}

	FEAwriter* writer = nullptr;
	if (input.format == "nas") {
		writer = new NastranFEAwriter(&file);
		Mesher::setFEAwriter(writer);
	}
	else {
		return 3;
	}

	std::cout << "Starting mesher type: " << input.mesherType << std::endl;
	if (input.mesherType == "planemesher") {
		PlaneMesher::writeNodes(NULL_POS, input.csys, input.md2D, input.size2D, plane::xy);
		PlaneMesher::writeElements(input.md2D);
	}
	else if (input.mesherType == "planemesherref") {
		PlaneMesherRef::writeNodes(NULL_POS, input.csys, input.md2Dref, input.size2D, false, plane::xy);
		PlaneMesherRef::writeElements(input.md2Dref);
	}
	else if (input.mesherType == "cuboidmesher") {
		CuboidMesher::writeNodes(NULL_POS, input.csys, input.md3D, input.size3D, plane::xy);
		CuboidMesher::writeElements(input.md3D);
	}
	else if (input.mesherType == "cuboidmesherref") {
		CuboidMesherRef::writeNodes(NULL_POS, input.csys, input.md3Dref, input.size3D, false, plane::xy);
		CuboidMesherRef::writeElements(input.md3Dref);
	}
	else if (input.mesherType == "planemesherref") {
		PlaneMesherRef::writeNodes(NULL_POS, input.csys, input.md2Dref, input.size2D, false, plane::xy);
		PlaneMesherRef::writeElements(input.md2Dref);
	}
	else if (input.mesherType == "conemesher") {
		ConeMesher::writeNodes(NULL_POS, input.csys, input.md2D, 
			input.cone2Drad, input.arcAngels, input.height, direction::x);
		ConeMesher::writeElements(input.md2D);
	}
	else if (input.mesherType == "conemesherref") {
		ConeMesherRef::writeNodes(NULL_POS, input.csys, input.md2Dref,
			input.cone2Drad, input.arcAngels, input.height, false, direction::x);
		ConeMesherRef::writeElements(input.md2Dref);
	}
	else if (input.mesherType == "cone3dmesher") {
		Cone3Dmesher::writeNodes(NULL_POS, input.csys, input.md3D,
			input.pipe3Drad, input.arcAngels, input.height, direction::x);
		Cone3Dmesher::writeElements(input.md3D);
	}
	else if (input.mesherType == "cone3dmesherref") {
		Cone3DmesherRef::writeNodes(NULL_POS, input.csys, input.md3Dref,
			input.pipe3Drad, input.arcAngels, input.height, false, direction::x);
		Cone3DmesherRef::writeElements(input.md3Dref);
	}
	else if (input.mesherType == "cone3dmesherref2") {
		Cone3DmesherRef2::writeNodes(NULL_POS, input.csys, input.md3Dref,
			input.pipe3Drad, input.arcAngels, input.height, false, direction::x);
		Cone3DmesherRef2::writeElements(input.md3Dref);
	}
	else {
		std::cout << "PMG error: Mesher type " << input.mesherType << " does not exist." << std::endl;
		delete writer;
		return 4;
	}
	
	writer->close();
	delete writer;

	std::cout << "PMG end" << std::endl;
	return 0;
}


Input Input::get(int argc, char* argv[]) {
	ArgumentParser argParser;
	argParser.options.argCaseInsensitive = true;
	argParser.readInput(argc, argv);

	Input input;
	glm::dvec3 csysX, csysY, pos;
	double	   nNodes1D;
	glm::ivec2 nNodes2D;
	glm::ivec3 nNodes3D;
	glm::dvec2 angles;
	glm::dvec2 rad2D;
	glm::dvec4 rad3D;

	argParser.getArg("-m",			input.mesherType);
	argParser.getArg("-o",			input.outPath);
	argParser.getArg("-f",			input.format,	"nas");
	argParser.getArg("-csysx",		csysX,			X_DIR);
	argParser.getArg("-csysy",		csysY,			Y_DIR);
	argParser.getArg("-pos",		pos,			NULL_POS);
	argParser.getArg("-nnodes1D",	nNodes1D,		10);
	argParser.getArg("-nnodes2D",	nNodes2D,		glm::ivec2(10, 10));
	argParser.getArg("-nnodes3D",	nNodes3D,		glm::ivec3(10, 10, 10));
	argParser.getArg("-size2D",		input.size2D,	glm::dvec2(10.0, 10.0));
	argParser.getArg("-size3D",		input.size3D,	glm::dvec3(10.0, 10.0, 10.));
	argParser.getArg("-rad2D",		rad2D,			glm::dvec2(5.0, 10.0));
	argParser.getArg("-rad3D",		rad3D,			glm::dvec4(5.0, 10.0, 2.5, 5.0));
	argParser.getArg("-angles",		angles,			glm::dvec2(0.0, GLMPI * 1.7));
	argParser.getArg("-height",		input.height,	10.0);
	argParser.getArg("-nref",		input.nref,		3);


	input.orientation = makeCsysMatrix(csysX, csysY);
	input.csys		= MeshCsys(pos, &input.orientation);
	input.md2D		= MeshDensity2D(nNodes2D.x, nNodes2D.y);
	input.md3D		= MeshDensity3D(nNodes3D.x, nNodes3D.y, nNodes3D.z);
	input.md2Dref	= MeshDensity2Dref(input.nref, nNodes1D);
	input.md3Dref	= MeshDensity3Dref(input.nref, nNodes2D.y, nNodes2D.y);
	input.arcAngels = ArcAngles(angles.x, angles.y);
	input.cone2Drad = Cone2Dradius(rad2D.x, rad2D.y);
	input.pipe3Drad = Pipe3Dradius(rad3D.x, rad3D.y, rad3D.z, rad3D.w);

	return input;
}

std::string Input::toStr() {

	std::string str = "";
	str += "\nOutput path        : " + outPath;
	str += "\nFormat             : " + format;
	str += "\ncsys.pos           : " + glm::to_string(csys.pos);
	str += "\ncsys.x             : " + glm::to_string(csys.dirX());
	str += "\ncsys.y             : " + glm::to_string(csys.dirY());
	str += "\ncsys.z             : " + glm::to_string(csys.dirZ());
	str += "\nMesh. dens. 2D     : " + std::to_string(md2D.dir1()) + ", " + std::to_string(md2D.dir2());
	str += "\nMesh. dens. 3D     : " + std::to_string(md3D.dir1()) + ", " + std::to_string(md3D.dir2()) + ", " + std::to_string(md3D.dir3());
	str += "\nMesh. dens. 2D ref : " + std::to_string(md2Dref.dir2()) + ", [" + std::to_string(md2Dref.nRefs()) + "]";
	str += "\nMesh. dens. 3D ref : " + std::to_string(md3Dref.dir2()) + ", " + std::to_string(md3Dref.dir3()) + ", [" + std::to_string(md3Dref.nRefs()) + "]";
	str += "\nSize2D             : " + glm::to_string(size2D);
	str += "\nSize3D             : " + glm::to_string(size3D);
	str += "\nCone rad. 2D       : " + std::to_string(cone2Drad.start()) + "," + std::to_string(cone2Drad.end());
	str += "\nPipe rad. 3D       : " + std::to_string(pipe3Drad.start.inner()) + "," + std::to_string(pipe3Drad.start.outer()) + ", " + std::to_string(pipe3Drad.end.inner()) + "," + std::to_string(pipe3Drad.end.outer());
	str += "\nAngles             : " + std::to_string(arcAngels.start) + "," + std::to_string(arcAngels.end);
	str += "\nHeight             : " + std::to_string(height);
	str += "\nN.ref.             : " + std::to_string(nref);

	return str;
}