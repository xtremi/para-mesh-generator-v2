#pragma once
#include "FeaWrite.h"
#include "glm/gtc/constants.hpp"
#include "fmt/core.h"
#include "fmt/format.h"

const glm::dvec3 X_DIR = glm::dvec3(1.0, 0.0, 0.0);
const glm::dvec3 Y_DIR = glm::dvec3(0.0, 1.0, 0.0);
const glm::dvec3 Z_DIR = glm::dvec3(0.0, 0.0, 1.0);

void coordTransform1(glm::dvec3* coords, const std::vector<double>& params) {
	coords->z = coords->x*coords->x;
}

//https://stackoverflow.com/questions/39440390/deforming-plane-mesh-to-sphere
void coordTransformSpherify(glm::dvec3* coords, const std::vector<double>& params) {

}

FEAwriter::~FEAwriter() {	
	close();
}

void FEAwriter::writeComment(const std::string& msg) {
#ifdef TO_BUFFER
	buffer += msg + "\n";
	processWriteBuffer();
#else
	*file << str;
#endif
}


void FEAwriter::close() {
	dumpBuffer();
	file->close();
}


void FEAwriter::processWriteBuffer() {
	nWritesToBuffer++;
	if (nWritesToBuffer >= maxWritesToBuffer) {
		nWritesToBuffer = 0;
		dumpBuffer();
	}
}

void FEAwriter::dumpBuffer() {
	//buffer.seekg(0, std::ios::end);
	//int buffersize = buffer.tellg();
	//buffer.seekg(0, std::ios::beg);
	int buffersize = buffer.size();
	int buffercap = buffer.capacity();

	//std::cout << "dumpBuffer() " << (double)buffersize/1000.0 << " Kb" << "(" << (double)buffercap / 1000.0 << " Kb" << ")" << std::endl;
	//*file << buffer.str();
	*file << buffer;
	buffer.clear();
}

/*
convenient functions
*/
std::string FEAwriter::vectorToString(int* vec, int size, char delim) {
	std::string str;
	for (int i = 0; i < size; i++) {
		str.push_back(delim);
		str += (fmt::format_int(vec[i]).str());
	}
	return str;
}

void FEAwriter::writeVectorCommaDelimeted(int* vec, int size) {
	std::string str;
	for (int i = 0; i < size; i++) {
		//str += (", " + std::to_string(vec[i]));
		str += (", " + fmt::format_int(vec[i]).str());
	}

#ifdef TO_BUFFER
		buffer += str;
#else
		*file << str;
#endif
}

void FEAwriter::writeVectorSpaceDelimeted(int* vec, int size) {
	std::string str;
	for (int i = 0; i < size; i++) {
		str += (" " + fmt::format_int(vec[i]).str());
	}

#ifdef TO_BUFFER
	buffer += str;
#else
	*file << str;
#endif
}

void FEAwriter::write2nodedBeam(int n[4]) {
	write2nodedBeam(elementID++, n);
}
void FEAwriter::write4nodedShell(int n[4]) {
	write4nodedShell(elementID++, n);
}
void FEAwriter::write8nodedHexa(int n[8]) {
	write8nodedHexa(elementID++, n);
}

void FEAwriter::writeNode(const glm::dvec3& c, const glm::dvec3& transl, glm::dmat3x3* csys) {
	
	glm::dvec3 newCoords = c;	
	
	//Local transform:
	if (currentCoordTransformer)
		currentCoordTransformer(&newCoords, &currentCoordTransformParams);
	
	//Local rotation:
	if (csys)
		newCoords = newCoords * (*csys);

	//Translate:
	newCoords += transl;
	
	writeNode(nodeID++, newCoords);
}

void FEAwriter::writeNode(int niD, const glm::dvec3& c, const glm::dvec3& transl, glm::dmat3x3* csys) {
	glm::dvec3 newCoords = c;

	//Local transform:
	if (currentCoordTransformer)
		currentCoordTransformer(&newCoords, &currentCoordTransformParams);

	//Local rotation:
	if (csys)
		newCoords = newCoords * (*csys);

	//Translate:
	newCoords += transl;

	writeNode(niD, newCoords);
}

/*********************************/
/*    NASTRAN                    */
/*********************************/
void NastranFEAwriter::writeComment(const std::string& msg) {
	FEAwriter::writeComment("$" + msg);
}

void NastranFEAwriter::writeNode(int nodeID, const glm::dvec3& c) {
	static const std::string GRID_FMT_FORMAT = "GRID, {:d},,{:.5f},{:.5f},{:.5f}\n";
	std::string str = fmt::format(GRID_FMT_FORMAT, nodeID, c.x, c.y, c.z);	
#ifdef TO_BUFFER
	buffer += str;
	processWriteBuffer();
#else
	*file << str;
#endif
	
}
void NastranFEAwriter::write2nodedBeam(int elID, int n[2]) {
	std::string str = "CBAR," + fmt::format_int(elID).str() + ",";
	str += vectorToString(n, 2, ',');
	str += "\n";
#ifdef TO_BUFFER
	buffer += str;
	processWriteBuffer();
#else
	*file << str;
#endif
}

void NastranFEAwriter::write4nodedShell(int elID, int n[4]) {
	std::string str = "CQUAD4," + fmt::format_int(elID).str() + ",";
	str += vectorToString(n, 4, ',');
	str += "\n";
#ifdef TO_BUFFER
	buffer += str;
	processWriteBuffer();
#else
	*file << str;
#endif
}
void NastranFEAwriter::write8nodedHexa(int elID, int n[8]) {

	std::string str = "CHEXA, " + fmt::format_int(elID).str() + ",";
	for (int j = 0; j < 8; j++) {
		if (j == 6)
			str += ",\n,";
		else
			str += ", ";
		str += std::to_string(n[j]);
	}
	str += "\n";

#ifdef TO_BUFFER
	buffer += str;
	processWriteBuffer();
#else
	*file << str;
#endif

}


/*********************************/
/*    ABAQUS                     */
/*********************************/
void AbaqusFEAwriter::writeNode(int nodeID, const glm::dvec3& c) {
	(*file) << " " << nodeID << ", " << c.x << ", " << c.y << ", " << c.z << "\n";
}
void AbaqusFEAwriter::write2nodedBeam(int elID, int n[2]) {

}
void AbaqusFEAwriter::write4nodedShell(int elID, int n[4]) {
	(*file) << elID;
	writeVectorCommaDelimeted(n, 4);
	(*file) << "\n";
}
void AbaqusFEAwriter::write8nodedHexa(int elID, int n[8]) {
	(*file) << elID;
	writeVectorCommaDelimeted(n, 8);
	(*file) << "\n";
}

void AbaqusFEAwriter::writePreNodes(int numberOfNodes) {
	(*file) << "*Node\n";
}
void AbaqusFEAwriter::writePreElements(int numberOfElements) {
	if (eldim == Dim::DIM2D)
		(*file) << "*Element, type=S4R\n";
	else
		(*file) << "*Element, type=C3D8R\n";
}

/*********************************/
/*    ANSYS                      */
/*********************************/
void AnsysFEAwriter::writeNode(int nodeID, const glm::dvec3& c) {
	(*file) << "n, " << nodeID << ", " << c.x << ", " << c.y << ", " << c.z << "\n";
}
void AnsysFEAwriter::write2nodedBeam(int elID, int n[2]) {

}
void AnsysFEAwriter::write4nodedShell(int elID, int n[4]) {
	(*file) << "en, " << elID;
	writeVectorCommaDelimeted(n, 4);
	(*file) << "\n";
}
void AnsysFEAwriter::write8nodedHexa(int elID, int n[8]) {
	(*file) << "en, " << elID;
	writeVectorCommaDelimeted(n, 8);
	(*file) << "\n";
}

void AnsysFEAwriter::writePreNodes(int numberOfNodes){
	(*file) << "/prep7\n";
}
void AnsysFEAwriter::writePreElements(int numberOfElements){
	if (eldim == Dim::DIM2D)
		(*file) << "et, 1, SHELL181\nsectype, 1, SHELL,, S3D\nsecnum, 1\ntype, 1\n";
	else
		(*file) << "et, 1, SOLID185\ntype, 1\n";
}


/*********************************/
/*    VTK                        */
/*********************************/
void VTKFEAwriter::writeNode(int nodeID, const glm::dvec3& c) {
	(*file) << c.x << " " << c.y << " " << c.z << "\n";
}

void VTKFEAwriter::write2nodedBeam(int elID, int n[2]) {

}
void VTKFEAwriter::write4nodedShell(int elID, int n[4]) {
	writeVectorSpaceDelimeted(n, 4);
	(*file) << "\n";
}
void VTKFEAwriter::write8nodedHexa(int elID, int n[8]) {
	writeVectorSpaceDelimeted(n, 8);
	(*file) << "\n";
}
void VTKFEAwriter::writePreNodes(int numberOfNodes) {}
void VTKFEAwriter::writePreElements(int numberOfElements) {}

/*********************************/
/*    FEAwriter general          */
/*********************************/

//Will be moved out of FEAwrite:
/********************************************************
	Elements row:
********************************************************/
int FEAwriter::writeElementRow(
	const std::vector<int>& bl,
	const std::vector<int>& br,
	const std::vector<int>& tr,
	const std::vector<int>& tl,
	int elStartID) {

	int n[8];
	int id = elStartID;
	for (int i = 0; i < bl.size() - 1; i++) {
		n[0] = bl[i];
		n[1] = br[i];
		n[2] = br[i + 1];
		n[3] = bl[i + 1];
		n[4] = tl[i];
		n[5] = tr[i];
		n[6] = tr[i + 1];
		n[7] = tl[i + 1];
		write8nodedHexa(id++, n);
	}
	return id;
}

int FEAwriter::writeElementRow(
	NodeIterator* nodesSide1,
	NodeIterator* nodesSide2,
	int elementID1
)
{
	int n[4];
	n[0] = nodesSide1->first();
	n[1] = nodesSide2->first();
	n[2] = nodesSide2->next();
	n[3] = nodesSide1->next();

	int elID = elementID1;

	while (n[1] > 0 && n[2] > 0) {
		write4nodedShell(elID++, n);
		//	0-------1 
		//	|       |
		//	3-------2
		n[0] = n[3];
		n[1] = n[2];
		n[3] = nodesSide1->next();
		n[2] = nodesSide2->next();
	}
	return elID;
}


/********************************************************
	Node line:
********************************************************/
/*int FEAwriter::writeNodesLineXq(const glm::dvec3& spos, double dx, int nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesLineQ(spos, dx, nnodes, nodeIDstart, direction::x, csys);
}
int FEAwriter::writeNodesLineYq(const glm::dvec3& spos, double dy, int nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesLineQ(spos, dy, nnodes, nodeIDstart, direction::y, csys);
}
int FEAwriter::writeNodesLineZq(const glm::dvec3& spos, double dz, int nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesLineQ(spos, dz, nnodes, nodeIDstart, direction::z, csys);
}
int FEAwriter::writeNodesLineX(const glm::dvec3&	spos, double length, int nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesLine(spos, length, nnodes, nodeIDstart, direction::x, csys);
}
int FEAwriter::writeNodesLineY(const glm::dvec3&	spos, double length, int nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesLine(spos, length, nnodes, nodeIDstart, direction::y, csys);
}
int FEAwriter::writeNodesLineZ(const glm::dvec3&	spos, double length, int nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesLine(spos, length, nnodes, nodeIDstart, direction::z, csys);
}

int FEAwriter::writeNodesLine(const glm::dvec3& spos, double length, int nnodes, int nodeIDstart,
	direction dir, glm::dmat3x3* csys)
{
	double ds = length / (double)(nnodes - 1);
	return writeNodesLineQ(spos, ds, nnodes, nodeIDstart, dir, csys);
}

int FEAwriter::writeNodesLine(const glm::dvec3& spos, const glm::dvec3& sposEnd, int nnodes, int nodeIDstart,
	glm::dmat3x3* csys) 
{
	glm::dvec3 ds = (sposEnd - spos) / (double)(nnodes - 1);
	return writeNodesLineQ(spos, ds, nnodes, nodeIDstart, csys);
}

int FEAwriter::writeNodesLineQ(
	const glm::dvec3&	spos,
	double				ds, 
	int					nnodes, 
	int					nodeIDstart,
	direction			dir,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords = spos;
	int nodeID = nodeIDstart;
	for (int i = 0; i < nnodes; i++) {
		//writeNode(nodeID++, coords, csys);		
		coords[(size_t)dir] += ds;
	}
	return nodeID;
}

int FEAwriter::writeNodesLineQ(
	const glm::dvec3&	spos,
	const glm::dvec3&	ds,			
	int					nnodes,
	int					nodeIDstart,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords = spos;
	int nodeID = nodeIDstart;
	for (int i = 0; i < nnodes; i++) {
		//writeNode(nodeID++, coords, csys);
		coords += ds;
	}
	return nodeID;
}

int FEAwriter::writeNodesLineQ_nth(
	const glm::dvec3&	spos,
	double				ds,
	int					nnodes,
	int					nodeIDstart,
	int					skipNth,
	direction			dir,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords = spos;
	int nodeID = nodeIDstart;
	for (int i = 0; i < nnodes; i++) {
		glm::dvec3 coords = spos;
		if (i%skipNth) {
			//writeNode(nodeID++, coords, csys);
		}
		coords[(size_t)dir] += ds;
	}
	return nodeID;
}

int FEAwriter::writeNodesLineQ_nth(
	const glm::dvec3&	spos,
	const glm::dvec3&	ds,
	int					nnodes,
	int					nodeIDstart,
	int					skipNth,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords = spos;
	int nodeID = nodeIDstart;
	for (int i = 0; i < nnodes; i++) {
		if(i%skipNth){
			//writeNode(nodeID++, coords, csys);
		}
		coords += ds;
	}
	return nodeID;
}
*/
/********************************************************
	Node circle (arc):
********************************************************/
/*
int FEAwriter::writeNodesCircularX(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, int	nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCircular(centerPos, radius, startAng, endAng, nnodes, nodeIDstart, direction::x, csys);
}
int FEAwriter::writeNodesCircularY(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, int	nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCircular(centerPos, radius, startAng, endAng, nnodes, nodeIDstart, direction::y, csys);
}
int FEAwriter::writeNodesCircularZ(const glm::dvec3& centerPos, double radius, double startAng, double endAng,
	int nnodes, int	nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCircular(centerPos, radius, startAng, endAng, nnodes, nodeIDstart, direction::z, csys);
}
int FEAwriter::writeNodesCircularXq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, int	nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCircularQ(centerPos, radius, startAng, dAng, nnodes, nodeIDstart, direction::x, csys);
}
int FEAwriter::writeNodesCircularYq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, int	nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCircularQ(centerPos, radius, startAng, dAng, nnodes, nodeIDstart, direction::y, csys);
}
int FEAwriter::writeNodesCircularZq(const glm::dvec3& centerPos, double radius, double startAng, double dAng,
	int nnodes, int	nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCircularQ(centerPos, radius, startAng, dAng, nnodes, nodeIDstart, direction::z, csys);
}

int FEAwriter::writeNodesCircular(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				endAng,
	int					nnodes,
	int					nodeIDstart,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	double dang = 0.0;
	limitArcAngles(startAng, endAng, dang, nnodes);
	return writeNodesCircularQ(centerPos, radius, startAng, dang, nnodes, nodeIDstart, rotAxis, csys);
}

int FEAwriter::writeNodesCircularQ(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				dAng,
	int					nnodes,
	int					nodeIDstart,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords;
	double ang = startAng;
	int nodeID = nodeIDstart;
	for (int i = 0; i < nnodes; i++) {

		switch (rotAxis)
		{
		case direction::x:
			coords = glm::dvec3(0.0, radius * glm::sin(ang), radius * glm::cos(ang)) + centerPos;
			break;
		case direction::y:
			coords = glm::dvec3(radius * glm::sin(ang), 0.0, radius * glm::cos(ang)) + centerPos;
			break;
		case direction::z:
			coords = glm::dvec3(radius * glm::sin(ang), radius * glm::cos(ang), 0.0) + centerPos;
			break;
		default:
			break;
		}		
		//writeNode(nodeID++, coords, csys);		
		ang += dAng;
	}
	return nodeID;
}

int FEAwriter::writeNodesCircularQ_nth(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				dAng,
	int					nnodes,
	int					nodeIDstart,
	int					skipNth,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords;
	double ang = startAng;
	int nodeID = nodeIDstart;
	for (int i = 0; i < nnodes; i++) {

		if(i%skipNth){

			switch (rotAxis)
			{
			case direction::x:
				coords = glm::dvec3(0.0, radius * glm::sin(ang), radius * glm::cos(ang)) + centerPos;
				break;
			case direction::y:
				coords = glm::dvec3(radius * glm::sin(ang), 0.0, radius * glm::cos(ang)) + centerPos;
				break;
			case direction::z:
				coords = glm::dvec3(radius * glm::sin(ang), radius * glm::cos(ang), 0.0) + centerPos;
				break;
			default:
				break;
			}
			//writeNode(nodeID++, coords, csys);
		}
		ang += dAng;
	}
	return nodeID;
}

int FEAwriter::writeNodesCircular_nth(
	const glm::dvec3&	centerPos,
	double				radius,
	double				startAng,
	double				endAng,
	int					nnodes,
	int					nodeIDstart,
	int					skipNth,
	direction			rotAxis,
	glm::dmat3x3*		csys)
{
	double dang = 0.0;
	limitArcAngles(startAng, endAng, dang, nnodes);
	return writeNodesCircularQ_nth(centerPos, radius, startAng, dang, nnodes, nodeIDstart, skipNth, rotAxis, csys);
}
*/
/********************************************************
	Elements line:
********************************************************/
/*int FEAwriter::writeElementsLine(
	int		nnodes,
	int		nodeID1,
	int		elementID1,
	bool	closedLoop
)
{
	int n[2];
	int c = nodeID1;
	int elID = elementID1;

	int nEl = closedLoop ? nnodes : nnodes - 1;

	for (int i = 0; i < nEl; i++) {
		n[0] = c++;
		n[1] = n[0] + 1;

		if (closedLoop && i == (nEl - 1)) {
			n[1] -= nnodes;
		}
		write2nodedBeam(elID++, n);
	}
	return elID;
}
*/
/********************************************************
	Nodes plane:
********************************************************/
/*int FEAwriter::writeNodesPlaneQ(
	const glm::dvec3&	spos,
	const glm::dvec2&	dp,
	const glm::ivec2&	nnodes,
	int					nodeIDstart,
	plane				pln,
	glm::dmat3x3*		csys)
{
	glm::dvec3 coords = spos;
	int nodeID = nodeIDstart;
	for (int i2 = 0; i2 < nnodes.y; i2++)
	{
		switch (pln){
		case plane::xy:
			nodeID = writeNodesLineXq(coords, dp.x, nnodes.x, nodeID, csys);
			coords.y += dp.y;
			break;
		case plane::xz:
			nodeID = writeNodesLineXq(coords, dp.x, nnodes.x, nodeID, csys);
			coords.z += dp.y;
			break;
		case plane::yz:
			nodeID = writeNodesLineYq(coords, dp.x, nnodes.x, nodeID, csys);
			coords.z += dp.y;
			break;
		default:
			break;
		}
	}
	return nodeID;
}

int FEAwriter::writeNodesPlane(
	const glm::dvec3&	spos,
	const glm::dvec2&	size,
	const glm::ivec2&	nnodes,
	int					nodeIDstart,
	plane				pln,
	glm::dmat3x3*		csys)
{
	glm::dvec2 dsize = glm::dvec2(size.x / (double)(nnodes.x - 1), size.y / (double)(nnodes.y - 1));
	return writeNodesPlaneQ(spos, dsize, nnodes, nodeIDstart, pln, csys);
}

int FEAwriter::writeNodesPlaneXZq(const glm::dvec3&	spos, const glm::dvec2&	dxz, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesPlaneQ(spos, dxz, nnodes, nodeIDstart, plane::xz, csys);
}
int FEAwriter::writeNodesPlaneXYq(const glm::dvec3&	spos, const glm::dvec2&	dxy, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesPlaneQ(spos, dxy, nnodes, nodeIDstart, plane::xy, csys);
}
int FEAwriter::writeNodesPlaneYZq(const glm::dvec3&	spos, const glm::dvec2&	dyz, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesPlaneQ(spos, dyz, nnodes, nodeIDstart, plane::yz, csys);
}
int FEAwriter::writeNodesPlaneXZ(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesPlane(spos, size, nnodes, nodeIDstart, plane::xz, csys);
}
int FEAwriter::writeNodesPlaneXY(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesPlane(spos, size, nnodes, nodeIDstart, plane::xy, csys);
}
int FEAwriter::writeNodesPlaneYZ(const glm::dvec3& spos, const glm::dvec2& size, const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys) {
	return writeNodesPlane(spos, size, nnodes, nodeIDstart, plane::yz, csys);
}
*/
/*
	 x_______________x_______________x                 -
	 |               |               |				   |
	 |				 |				 |				   |
	 |				 |				 |				   |  4el
	 |				 |				 |				   |
	 |				 |				 |                 |
	 x_______________x_______________x				   -
	 |\              |              /|				   |  2elL
	 |  \ 			 |		      /	 |				   |
	 |	   \ x_______x_______x  / 	 |				   -
	 |       |       |       |		 |				   |  2elL
	 |	     |       |       |       |                 |
	 x_______x_______x_______x_______x                 -        ^
	 |       |       |       |       |				   |  2elL  |
	 |       |       |       |       |				   |		|
t0   x_______x_______x_______x_______x  row t (top)	   -		|
	 | \     |     / |  \    |     / |  elRow t		   |  1elL  |
	 |m0 x___x___x/	 |   x___x___x	 |  row m (mid)	   -     	|  L
	 |   |   |   |   |   |   |   |   |	elRow m		   |  1elL	|		  ^
b0   x___x___x___x___x___x___x___x___x  row b (bot)	   -		|		  |
	 |   |   |   |   |   |   |   |   |	elRow b		   |  1elL  |         | X
	 x___x___x___x___x___x___x___x___x ref2			   -	    -         -
	 |   |   |   |   |   |   |   |   |
	 x___x___x___x___x___x___x___x___x ref1


	------------------------------------------------------
   | First row starts at x = elSize
   |------------------------------------------------------
   | 			nRefinements --->
   |  L =  elL * (3 + 6 + 12 + 24 + ...)
   |  L = 3elL * (1 + 2 +  4 +  8 + ...)
   |  L = 3elL * sum(2^k) (k = 0 -> nRefinements - 1)
   |
   |  L =  3elL * (1 - 2^nRef) / (1 - 2)
   |  L = -3elL * (1 - 2^nRef)
   |
   |   L = 3elL * (2^nRef - 1)
   | elL = L / (4 * (2^nRef - 1))
   |
   |------------------------------------------------------
   | First row starts at distance = 0.0
   |------------------------------------------------------
   |
   | 			nRefinements --->
   |  L =   elL * ((3-1) + 6 + 12 + 24 + ...)
   |  L =   elL * (6 + 12 + 24 + ...) + 2elL
   |  L =  6elL * sum(2^k) (k = 0 -> nRefinements - 2) + 2elL
   |  L =  6elL * (2^(nRef-1) - 1) + 2elL
   |  L =  6elL * 2^(nRef-1) - 6elL + 2elL
   |  L =  6elL * 2^(nRef-1) - 4elL
   |  L =   elL * (6*2^(nRef-1) - 4)
   |
   |
   |   L = elL * (6 * 2^(nRef-1) - 4)   vs   L =  3elL * (2^nRef - 1)
   | elL =   L / (6 * 2^(nRef-1) - 4)   vs elL = L / (4 * (2^nRef - 1))
   |
   |------------------------------------------------------

*/
/*
int FEAwriter::writeNodesPlane_ref(
	const glm::dvec3& spos,
	const glm::dvec2& size,
	int				  nNodesEdge,
	int               nRefinements,
	int               nodeIDstart,
	bool			  startWithOffset,
	plane			  pln,
	glm::dmat3x3*     csys)
{
	int nodeID = nodeIDstart;
	glm::dvec3 coords(spos);

	int		currentNodesPerRow = nNodesEdge;
	int		currentRefFactor = 1;
	int		currentRefinement = 0;

	double elSizeX = initialRefElSize2D(size.x, nRefinements, startWithOffset);

	glm::dvec2 curElSize(elSizeX, size.y / (double)(currentNodesPerRow - 1)); //start with square elements (??)

	direction edgeDirection, refinementDirection;
	getPlaneDirections(pln, refinementDirection, edgeDirection);

	if (startWithOffset) {
		coords[(size_t)refinementDirection] += curElSize.x;
	}

	while (currentRefinement < nRefinements) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x
		nodeID = writeNodesLineQ(coords, curElSize.y, currentNodesPerRow, nodeID, edgeDirection);
		coords[(size_t)edgeDirection] = spos.y;
		coords[(size_t)refinementDirection] += curElSize.x;


		//row m:  |  x--x--x  |  x--x--x  |
		nodeID = writeNodesLineQ_nth(coords, curElSize.y, currentNodesPerRow, nodeID, 4, edgeDirection, csys);
		coords[(size_t)refinementDirection] += curElSize.x;
		coords[(size_t)edgeDirection] = spos[(size_t)edgeDirection];

		//Refine
		currentRefFactor *= 2;
		// [9 nodes / 8 elements] -> [5 nodes / 4 elements] -> [3 nodes / 2 elements]
		currentNodesPerRow = (nNodesEdge - 1) / currentRefFactor + 1;
		curElSize.y *= 2.0;

		//row t:
		nodeID = writeNodesLineQ(coords, curElSize.y, currentNodesPerRow, nodeID, edgeDirection, csys);
		curElSize.x *= 2.0;
		coords[(size_t)refinementDirection] += curElSize.x;
	}
	return nodeID;

}


int FEAwriter::writeNodesPlaneXY_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements,
	int nodeIDstart, bool startWithOffset, glm::dmat3x3* csys)
{
	return writeNodesPlane_ref(spos, size, nNodesEdge, nRefinements, nodeIDstart, startWithOffset, plane::xy, csys);
}
int FEAwriter::writeNodesPlaneXZ_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements,
	int nodeIDstart, bool startWithOffset, glm::dmat3x3* csys)
{
	return writeNodesPlane_ref(spos, size, nNodesEdge, nRefinements, nodeIDstart, startWithOffset, plane::xz, csys);
}
int FEAwriter::writeNodesPlaneYZ_ref(const glm::dvec3& spos, const glm::dvec2& size, int nNodesEdge, int nRefinements,
	int nodeIDstart, bool startWithOffset, glm::dmat3x3* csys)
{
	return writeNodesPlane_ref(spos, size, nNodesEdge, nRefinements, nodeIDstart, startWithOffset, plane::yz, csys);
}
*/


/********************************************************
	Node cone:
********************************************************/
/*
	Cone around X,Y or Z-axis
	nnodes.x - along circumferance
	nndoes.y - along height
*/
/*int FEAwriter::writeNodesCone(
	const glm::dvec3&	centerPos,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				endAng,
	double				height,
	const glm::ivec2&	nnodes,
	int					nodeIDstart,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	double dang = 0.0;
	bool fullCircle = limitArcAngles(startAng, endAng, dang, nnodes.x);

	double dH = height / (double)(nnodes.y - 1);
	double dR = (radiusEnd - radiusStart)/ (double)(nnodes.y - 1);

	int			nodeID = nodeIDstart;
	glm::dvec3  coords = centerPos;
	double		radius = radiusStart;

	for (int i = 0; i < nnodes.y; i++) {

		nodeID = writeNodesCircularQ(coords, radius, startAng, dang, nnodes.x, nodeID, rotaxis, csys);
		coords[(size_t)rotaxis] += dH;
		radius += dR;
	}
	return nodeID;
}

int FEAwriter::writeNodesConeX(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCone(centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, nodeIDstart, direction::x, csys);
}
int FEAwriter::writeNodesConeY(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCone(centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, nodeIDstart, direction::y, csys);
}
int FEAwriter::writeNodesConeZ(const glm::dvec3& centerPos, double radiusStart, double radiusEnd, double startAng, double endAng, double height,
	const glm::ivec2& nnodes, int nodeIDstart, glm::dmat3x3* csys)
{
	return writeNodesCone(centerPos, radiusStart, radiusEnd, startAng, endAng, height, nnodes, nodeIDstart, direction::z, csys);
}
*/
/*

	In the code X-dir refers to refinement direction (along height)
	Y-dir refers to around the perimeter
*/
/*
int FEAwriter::writeNodesCone_ref(
	const glm::dvec3&	centerPos,
	int					nNodesEdge,
	int					nRefinements,
	double				radiusStart,
	double				radiusEnd,
	double				startAng,
	double				endAng,
	double				height,
	int					nodeIDstart,
	direction			rotaxis,
	glm::dmat3x3*		csys)
{
	
	int		nodeID		= nodeIDstart;
	bool	fullCircle	= (startAng < 0.0) && (endAng < 0.0);

	double	dR = radiusEnd - radiusStart;
	
	int		nElAroundStart	= fullCircle ? nNodesEdge : nNodesEdge - 1;
	double	arcLengthStart	= 2.0 * glm::pi<double>() * radiusStart;
	double	coneLength		= std::sqrt(std::pow(dR, 2.0) + std::pow(height, 2.0));
	double	elSizeX			= initialRefElSize2D(coneLength, nRefinements, false);
	
	glm::dvec2	curElSize(elSizeX, arcLengthStart / (double)(nElAroundStart));
	int			currentNodesPerArc	= nNodesEdge;
	int			currentRefFactor	= 1;
	int			currentRefinement	= 0;	
	double		currentRadius		= radiusStart;
	double		currentConeLength	= 0.0;
	double		currentDh			= 0.0;

	glm::dvec3 coords(centerPos);

	while (currentRefinement < nRefinements) {
		currentRefinement++;

		//row b: x--x--x--x--x--x--x--x--x
		nodeID = writeNodesCircular(coords, currentRadius, startAng, endAng, currentNodesPerArc, nodeID, rotaxis, csys);
		currentConeLength	+= curElSize.x;
		currentRadius		= radiusStart + dR * (currentConeLength / coneLength);
		currentDh			= (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;

		//row m:  |  x--x--x  |  x--x--x  |
		nodeID = writeNodesCircular_nth(coords, currentRadius, startAng, endAng, currentNodesPerArc, nodeID, 4, rotaxis, csys);
		currentConeLength += curElSize.x;
		currentRadius = radiusStart + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;

		//Refine
		currentRefFactor *= 2;
		// [9 nodes / 8 elements] -> [5 nodes / 4 elements] -> [3 nodes / 2 elements]
		if (!fullCircle)
			currentNodesPerArc = (nNodesEdge - 1) / currentRefFactor + 1;
		else
			currentNodesPerArc = nNodesEdge / currentRefFactor;

		curElSize.y *= 2.0;

		//row t:
		nodeID = writeNodesCircular(coords, currentRadius, startAng, endAng, currentNodesPerArc, nodeID, rotaxis, csys);
		curElSize.x *= 2.0;
		currentConeLength += curElSize.x;
		currentRadius = radiusStart + dR * (currentConeLength / coneLength);
		currentDh = (height / coneLength)*curElSize.x;
		coords[(size_t)rotaxis] += currentDh;

		
	}
	return nodeID;
}
*/



/********************************************************
	Elements plane:
********************************************************/

/*

  5x_10x_15x___x  nnodes.x = 5 (nElx = 4)
   |   |   |   |  nnodes.y = 4 (nEly = 3)
  4x__9x_14x___x
   |   |   |   |
  3x__8x_13x___x
   |   |   |   |    ^
  2x__7x_12x___x    |
   |   |   |   |    | 
  1x__6x_11x___x    | Xdir

  
  IF closed loop

  5x_10x_15x___x    nnodes.x = 5 (nElx = 5)
   |   |   |   |    nnodes.y = 4 (nEly = 3)
  4x__9x_14x___x
   |   |   |   |
  3x__8x_13x___x
   |   |   |   |
  2x__7x_12x___x
   |   |   |   |
  1x__6x_11x___x
   |   |   |   |
  5x_10x_15x___x
*/
/*
int FEAwriter::writeElementsPlane(
	glm::ivec2			nnodes,
	int					nodeID1,
	int					elementID1,
	bool				closedLoop
)
{
	int n[4];
	int c = nodeID1;
	int elID = elementID1;

	int nEly = nnodes.y - 1;
	int nElx = closedLoop ? nnodes.x : nnodes.x - 1;

	for (int iy = 0; iy < nEly; iy++) {
		for (int ix = 0; ix < nElx; ix++) {					
			n[0] = c++;					
			n[1] = n[0] + 1;
			n[2] = n[1] + nnodes.x;	
			n[3] = n[2] - 1;

			if (closedLoop && ix == (nElx - 1)) {
				n[1] -= nnodes.x;
				n[2] -= nnodes.x;
			}

			write4nodedShell(elID++, n);
		}
		if(!closedLoop) c++;
	}
	return elID;
}
*/
/*
 nElements = 3
 nodeIncr  = 4
 nodeID1_side1 = 1
 nodeID2_side2 = 100
 elementID = 1

 side1   |   side2

	x1------x100
	|   e1  |
	x5------x104
	|   e2  |
	x9------x108
	|    e3 |
	x13-----x112

*/

/*
	 x_______________x_______________x                 -
	 |               |               |				   |
	 |				 |				 |				   |
	 |				 |				 |				   |  4el
	 |				 |				 |				   |
	 |				 |				 |                 |
	 x_______________x_______________x				   -
	 |\              |              /|				   |  2elL
	 |  \ 			 |		      /	 |  elRow t        |
	 |	   \ x_______x_______x  / 	 |				   -
	 |       |       |       |		 |				   |  2elL
	 |	     |       |       |       |  elRow m        |
	 x_______x_______x_______x_______x                 -
	 |       |       |       |       |				   |  2elL
	 |       |       |       |       |	elRow b		   |
t0   x_______x_______x_______x_______x             	   -
	 | \     |     / |  \    |     / |  elRow t		   |  1elL
	 |m0 x___x___x/	 |   x___x___x	 |                 -
	 |   |   |   |   |   |   |   |   |	elRow m		   |  1elL
b0   x___x___x___x___x___x___x___x___x  	           -
										elRow b		   |  1elL
*/
/*int FEAwriter::writeElementsPlane_ref(
	int		nNodesY,
	int		nRefinements,
	int		elementID1,
	int		nodeID1,
	bool	closedLoop)
{
	int currentRefFactor = 1;
	int	currentNodesPerRow = nNodesY;
	int currentElementPerRow = closedLoop ? nNodesY : nNodesY - 1;

	int elID = elementID1;
	int c = nodeID1;
	int n[4];

	for (int r = 0; r < nRefinements; r++) {


		if (r != 0) {
			//elRow b
			for (int i = 0; i < currentElementPerRow; i++) {
				n[0] = c++;									//  3x------x2
				n[1] = n[0] + 1;							//   |      |
				n[2] = n[1] + currentNodesPerRow;			//   |      |
				n[3] = n[2] - 1;							//  0x------x1
				
				if (closedLoop && i == (currentElementPerRow - 1)) {
					n[1] -= currentNodesPerRow;
					n[2] -= currentNodesPerRow;
 				}
				write4nodedShell(elID++, n);
			}
			if(!closedLoop) c++;
		}
		
		
		//elRow m + t 
		int nnodesRowM = 3 * currentElementPerRow / 4;
		currentRefFactor *= 2;
		int nnodesRowT;

		if (!closedLoop)
			nnodesRowT = (nNodesY - 1) / currentRefFactor + 1;
		else
			nnodesRowT = (nNodesY) / currentRefFactor;

		for (int i = 0; i < currentElementPerRow; i += 4) {

			int b0 = c;
			int m0 = b0 + currentNodesPerRow - i / 4;
			int t0 = m0 + nnodesRowM - i / 4;

			int b[5] = { b0, b0 + 1, b0 + 2, b0 + 3 , b0 + 4 };
			int m[3] = { m0, m0 + 1, m0 + 2 };
			int t[3] = { t0, t0 + 1, t0 + 2 };


			if (closedLoop && i == (currentElementPerRow - 4)) {
				b[4] -= (currentNodesPerRow );
				t[2] -= (nnodesRowT);
			}

			std::vector<int*> elNodes;                                      //  t0       t1     t2
			int n1[4] = { b[0], b[1], m[0], t[0] }; elNodes.push_back(n1);  //   x_______x_______x
			int n2[4] = { b[1], b[2], m[1], m[0] }; elNodes.push_back(n2);  //   | \     |     / |
			int n3[4] = { b[2], b[3], m[2], m[1] }; elNodes.push_back(n3);  //   | m0x_m1x___x/m2|
			int n4[4] = { b[3], b[4], t[2], m[2] }; elNodes.push_back(n4);  //   |   |   |   |   |
			int n5[4] = { m[0], m[1], t[1], t[0] }; elNodes.push_back(n5);  //   x___x___x___x___x
			int n6[4] = { m[1], m[2], t[2], t[1] }; elNodes.push_back(n6);  //  b0   b1  b2  b3  b4

	
			for (int i = 0; i < 6; i++){
				write4nodedShell(elID++, elNodes[i]);
			}

			c += 4;
		}
		
		currentNodesPerRow = nnodesRowT;
		currentElementPerRow = closedLoop ? currentNodesPerRow : currentNodesPerRow - 1;

		c += (nnodesRowM + 1);
		if (closedLoop) c--;
	}
	return elID;
}
*/





