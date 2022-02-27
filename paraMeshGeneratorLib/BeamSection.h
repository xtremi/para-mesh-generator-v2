#pragma once

class Hbeam3Dsection {
public:
	Hbeam3Dsection() {}
	Hbeam3Dsection(
		double height,
		double width,
		double thkWeb,
		double thkFlange);

	Hbeam3Dsection(
		double _height,
		double _widthTop,
		double _widthBot,
		double _thkWeb,
		double thkFlangeTop,
		double thkFlangeBot);

	double height;	 /*total height*/
	double widthTop; /*Width top flange*/
	double widthBot; /*Width bottom flange*/
	double thkWeb;   /*Thk. web*/
	double thkTop;   /*Thk. flange top*/
	double thkBot;   /*Thk. flange bot*/

	virtual double widthTopFlOuter() const;
	virtual double widthBotFlOuter() const;
	double heightWeb() const;

	bool isCbeam() const {
		return _isCbeam;
	}

protected:
	bool _isCbeam = false;
};

class Cbeam3Dsection : public Hbeam3Dsection
{
public:
	Cbeam3Dsection() {
		_isCbeam = true;
	}
	Cbeam3Dsection(
		double height,
		double width,
		double thkWeb,
		double thkFlange);
	Cbeam3Dsection(
		double _height,
		double _widthTop,
		double _widthBot,
		double _thkWeb,
		double thkFlangeTop,
		double thkFlangeBot);

	double widthTopFlOuter() const;
	double widthBotFlOuter() const;
};

class Hbeam3DsectionElementDistribution {
public:
	Hbeam3DsectionElementDistribution() {}
	Hbeam3DsectionElementDistribution(
		const Hbeam3Dsection& geometry,
		int					  nNodesTopFlangeWidth);


	int topFl_w_o; /*Elements along top flange outer width*/
	int topFl_h;   /*Elements along top flange height*/
	int botFl_w_o; /*Elements along bot flange outer width*/
	int botFl_h;   /*Elements along bot flange height*/
	int web_w;	   /*Elements along web width*/
	int web_h;	   /*Elements along web height*/

protected:
	bool isCbeam = false;
};
