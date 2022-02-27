#include "MeshHbeam3D.h"
#include "Mesher.h"


MeshHbeam3D::MeshHbeam3D(
	const Hbeam3Dsection& section				/*!H-beam section definition*/,
	double				  length				/*!Length of beam*/,
	int					  nNodesTopFlangeWidth	/*!Number of nodes on top flange in Z-direction (width)*/,
	int					  nNodesBeamLength		/*!Number of nodes along beam length*/)
	: MeshPart3D()
{
	setInput(section, length, nNodesTopFlangeWidth, nNodesBeamLength);
}
void MeshHbeam3D::setInput(
	const Hbeam3Dsection& _section				/*!H-beam section definition*/,
	double				  _length				/*!Length of beam*/,
	int					  nNodesTopFlangeWidth	/*!Number of nodes on top flange in Z-direction (width)*/,
	int					  _nNodesBeamLength		/*!Number of nodes along beam length*/)
{
	section = _section;
	length = _length;
	nNodesBeamLength = _nNodesBeamLength;
	elDens = Hbeam3DsectionElementDistribution(section, nNodesTopFlangeWidth);
}

/*
	- Create top flange by extruding face x 3
	- Create bot flange by extruding face x 3
	- Create web by extruding face x 3
	- Connect top of web to bot of top flange
	- Connect bot of web to top of bot flange

*/
void MeshHbeam3D::writeNodes() {
	
	MeshCsys csysTopFlange(this->csys);
	MeshCsys csysBotFlange(&csysTopFlange);
	MeshCsys csysWeb(&csysTopFlange);

	csysBotFlange.pos.z -= (section.heightWeb() + section.thkBot);
	if(!section.isCbeam()){
		csysBotFlange.pos.x += (section.widthTop - section.widthBot)/2.0;
	}
	
	double elsizeWeb_z = section.heightWeb() / elDens.web_h;
	csysWeb.pos.z -= (section.heightWeb() - elsizeWeb_z);
	if (!section.isCbeam()) {
		csysWeb.pos.x += (section.widthTop / 2.0 - section.thkWeb / 2.0);
	}

	topFlange = Mesh3D_volumeExtrusion(
		MeshDensity2D(nNodesBeamLength + 1, elDens.topFl_h + 1), glm::dvec2(length, section.thkTop));
	topFlange.setCsys(csysTopFlange);
	if(!section.isCbeam()){
		topFlange.extrudeYZface(section.widthTopFlOuter(), elDens.topFl_w_o);
	}
	topFlange.extrudeYZface(section.thkWeb, elDens.web_w);
	topFlange.extrudeYZface(section.widthTopFlOuter(), elDens.topFl_w_o);

	botFlange = Mesh3D_volumeExtrusion(
		MeshDensity2D(nNodesBeamLength + 1, elDens.botFl_h + 1), glm::dvec2(length, section.thkBot));
	botFlange.setCsys(csysBotFlange);
	if (!section.isCbeam()) {
		botFlange.extrudeYZface(section.widthBotFlOuter(), elDens.botFl_w_o);
	}
	botFlange.extrudeYZface(section.thkWeb, elDens.web_w);
	botFlange.extrudeYZface(section.widthBotFlOuter(), elDens.botFl_w_o);
	
	web = Mesh3D_volumeExtrusion(
		MeshDensity2D(nNodesBeamLength + 1, elDens.web_h - 1), glm::dvec2(length, section.heightWeb() - 2. * elsizeWeb_z));
	web.setCsys(csysWeb);
	web.extrudeYZface(section.thkWeb, elDens.web_w);

	topFlange.writeNodes();
	botFlange.writeNodes();
	web.writeNodes();
}
void MeshHbeam3D::writeElements() {
	topFlange.writeElements();
	botFlange.writeElements();
	web.writeElements();

	NodeIterator2D faceTF_bot =  topFlange.getFace(section.isCbeam() ? 0 : 1, 4).nodeIter;
	NodeIterator2D faceWeb_top =  web.getFace(0, 5).nodeIter;	
	RowMesher3D::writeElements(&faceTF_bot, &faceWeb_top);

	NodeIterator2D faceBF_top = botFlange.getFace(section.isCbeam() ? 0 : 1, 5).nodeIter;
	NodeIterator2D faceWeb_bot = web.getFace(0, 4).nodeIter;
	RowMesher3D::writeElements(&faceBF_top, &faceWeb_bot);
}

/*!
	Uses MeshHbeam3D to define a C-beam
*/
MeshCbeam3D::MeshCbeam3D(
	const Cbeam3Dsection& section,
	double				  length,
	int					  nNodesTopFlangeWidth,
	int					  nNodesBeamLength)
{
	hbeam = MeshHbeam3D(section, length, nNodesTopFlangeWidth, nNodesBeamLength);
}
void MeshCbeam3D::setInput(const Cbeam3Dsection& section, double length, int nNodesTopFlangeWidth, int nNodesBeamLength){
	hbeam.setInput(section, length, nNodesTopFlangeWidth, nNodesBeamLength);
}

void MeshCbeam3D::writeNodes() {
	hbeam.writeNodes();
}
void MeshCbeam3D::writeElements() {
	hbeam.writeElements();
}