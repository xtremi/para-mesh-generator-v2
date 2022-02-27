#pragma once
#include "MeshPart.h"
#include "MeshPartExtrusion.h"
#include "BeamSection.h"


/*!
                 ^ Y  
	             : 
	x-------------------------x
	|            :            |    top flange
	x---------x-----x---------x
	          |  :  |
	          |  :  |
	          |  :  |
     - - - - -| -|- | - - - - -> X
	          |  :  |
	          |  :  |
	          |  :  |
	   x------x-----x------x
	   |         :         |    bot flange
	   x-------------------x
	             :
*/
class MeshHbeam3D : public MeshPart3D
{
public:
	MeshHbeam3D() : MeshPart3D(){}
	MeshHbeam3D(
		const Hbeam3Dsection& section, 
		double				  length, 
		int					  nNodesTopFlangeWidth,
		int					  nNodesBeamLength);
	void setInput(const Hbeam3Dsection& section, double length, int nNodesTopFlangeWidth, int nNodesBeamLength);

	void writeNodes();
	void writeElements();

private:
	Mesh3D_volumeExtrusion topFlange, web, botFlange;
	Hbeam3Dsection					  section;
	Hbeam3DsectionElementDistribution elDens;
	int nNodesBeamLength;
	double length;
};

class MeshCbeam3D : public MeshPart3D {
public:
	MeshCbeam3D() : MeshPart3D() {}
	MeshCbeam3D(
		const Cbeam3Dsection& section,
		double				  length,
		int					  nNodesTopFlangeWidth,
		int					  nNodesBeamLength);
	void setInput(const Cbeam3Dsection& section, double length, int nNodesTopFlangeWidth, int nNodesBeamLength);

	void writeNodes();
	void writeElements();

private:
	MeshHbeam3D hbeam;
};

