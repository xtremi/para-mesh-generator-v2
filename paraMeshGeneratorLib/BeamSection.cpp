#include "BeamSection.h"

Hbeam3Dsection::Hbeam3Dsection(
	double height,
	double width,
	double thkWeb,
	double thkFlange) : Hbeam3Dsection(height, width, width, thkWeb, thkFlange, thkFlange) {}

Hbeam3Dsection::Hbeam3Dsection(
	double _height,
	double _widthTop,
	double _widthBot,
	double _thkWeb,
	double thkFlangeTop,
	double thkFlangeBot) :
	height{ _height }, widthTop{ _widthTop }, widthBot{ _widthBot },
	thkWeb{ _thkWeb }, thkTop{ thkFlangeTop }, thkBot{ thkFlangeBot }
{}


/*!Width of outer part of top flange*/
double Hbeam3Dsection::widthTopFlOuter() const {
	return 0.5 * (widthTop - thkWeb);
}
/*!Width of outer part of bot flange*/
double Hbeam3Dsection::widthBotFlOuter() const {
	return 0.5 * (widthBot - thkWeb);
}
/*!Height of web without flange thickness*/
double Hbeam3Dsection::heightWeb() const {
	return height - thkTop - thkBot;
}

/*!Width of outer part of top flange*/
double Cbeam3Dsection::widthTopFlOuter() const {
	return widthTop - thkWeb;
}
/*!Width of outer part of bot flange*/
double Cbeam3Dsection::widthBotFlOuter() const {
	return widthBot - thkWeb;
}

Cbeam3Dsection::Cbeam3Dsection(
	double height,
	double width,
	double thkWeb,
	double thkFlange) : Hbeam3Dsection(height, width, thkWeb, thkFlange)
{
	_isCbeam = true;
}
Cbeam3Dsection::Cbeam3Dsection(
	double _height,
	double _widthTop,
	double _widthBot,
	double _thkWeb,
	double thkFlangeTop,
	double thkFlangeBot) : Hbeam3Dsection(_height, widthTop, widthBot, _thkWeb, thkFlangeTop, thkFlangeBot)
{
	_isCbeam = true;
}

/*
	int topFl_w_o, topFl_w_i, topFl_h;
	int botFl_w_o, botFl_w_i, botFl_h;
	int web_w, web_h;


	x---------x-----x---------x
	|  outer  |inner|outer    | for top and bot flange
	x---------x-----x---------x

*/
Hbeam3DsectionElementDistribution::Hbeam3DsectionElementDistribution(
	const Hbeam3Dsection& geometry,
	int					  nNodesTopFlangeWidth)
{
	int nElTopFlangeWidth = nNodesTopFlangeWidth - 1;
	int nElBotFlangeWidth = (int)((geometry.widthBot / geometry.widthTop) * (double)nElTopFlangeWidth);
	topFl_h = (int)((geometry.thkTop / geometry.widthTop) * (double)nElTopFlangeWidth);
	botFl_h = (int)((geometry.thkBot / geometry.widthTop) * (double)nElTopFlangeWidth);
	web_h = (int)((geometry.heightWeb() / geometry.widthTop) * (double)nElTopFlangeWidth);

	double topFl_w_i_factor = geometry.thkWeb / geometry.widthTop;
	double topFl_w_o_factor = 1.0 - topFl_w_i_factor;
	if (!geometry.isCbeam()) {
		topFl_w_o_factor /= 2.0;

	}

	topFl_w_o = (int)(topFl_w_o_factor * (double)nElTopFlangeWidth);
	web_w	  = nElTopFlangeWidth -  topFl_w_o;
	if (!geometry.isCbeam()) {
		web_w -= topFl_w_o;
	}
	botFl_w_o = nElBotFlangeWidth - web_w;
	if (!geometry.isCbeam()) {
		botFl_w_o /= 2;
	}
}
