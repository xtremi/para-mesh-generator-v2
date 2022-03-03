#pragma once
#include "math_utilities.h"
#include <vector>




class LineStrip {
public:
	LineStrip() {}
	LineStrip(const std::vector<glm::dvec3>& points);

	int nLines() const;
	int nPoints() const;
	bool getLineEndPoints(int line_i, glm::dvec3& p1, glm::dvec3& p2) const;
	void addPoint(const glm::dvec3& p);

	std::vector<glm::dvec3> points;
};

class Rectangle {
public:
	Rectangle(const glm::dvec2& _size) : size{ _size } {}
	Rectangle() {}

	double perimeter();
	void elementsPerSides(int nNodesPerimeter, int& nElWidthX, int& nElHeightY);

	glm::dvec2 getCornerCoord(int corner);
	glm::dvec2 size;

	static const glm::dvec2 recDirs[4];
};

class Quad {
public:
	Quad(const glm::dvec3& c1, const glm::dvec3& c2, const glm::dvec3& c3, const glm::dvec3& c4);
	Quad() {}

	glm::dvec3 dir12() const;
	glm::dvec3 dir14() const;
	glm::dvec3 dir43() const;
	glm::dvec3 c1() const;
	glm::dvec3 c2() const;
	glm::dvec3 c3() const;
	glm::dvec3 c4() const;
	glm::dvec3 corner(int i) const;
	glm::dvec3 corners[4];
};

class QuadStrip {
public:
	struct QuadStripSection {
		glm::dvec3 p1, p2;
	};

	QuadStrip() {}
	QuadStrip(const std::vector<QuadStripSection>& _sectionPoints) : sectionPoints{ _sectionPoints } {}
	void addSection(const QuadStripSection& section);
	int nQuads() const;
	Quad quad(int i) const;
	LineStrip getLineStripBot() const;
	LineStrip getLineStripTop() const;
	

private:
	std::vector<QuadStripSection> sectionPoints;
};


class EllipseRadius {
public:
	EllipseRadius() {}
	EllipseRadius(double _rad1, double _rad2) : rad1{ _rad1 }, rad2{ _rad2 } {}
	double rad1, rad2;

	double perimeter() const;
};



class Pipe2Dradius {
public:
	Pipe2Dradius() {}
	Pipe2Dradius(double _rad1, double _rad2) : radi{ glm::dvec2(_rad1, _rad2) } {}

	double rad1() const { return radi.x; };
	double rad2() const { return radi.y; };
	double dR() const { return radi.y - radi.x; }
	void setRad1(double r) { radi.x = r; }
	void setRad2(double r) { radi.y = r; }
	void incrRad1(double dr) { radi.x += dr; }
	void incrRad2(double dr) { radi.y += dr; }
	glm::dvec2 radi;
};

class Disk2Dradius : public Pipe2Dradius {
public:
	Disk2Dradius() {}
	Disk2Dradius(const Pipe2Dradius& pipe2drad) { radi = pipe2drad.radi; }
	Disk2Dradius(double _inner, double _outer) : Pipe2Dradius(_inner, _outer) {}
	double inner() const { return radi.x; }
	double outer() const { return radi.y; }
	void setInner(double r) { setRad1(r); }
	void setOuter(double r) { setRad2(r); }
	void incrInner(double dr) { incrRad1(dr); }
	void incrOuter(double dr) { incrRad2(dr); }
};

class Cone2Dradius : public Pipe2Dradius {
public:
	Cone2Dradius() {}
	Cone2Dradius(const Pipe2Dradius& pipe2drad) { radi = pipe2drad.radi; }
	Cone2Dradius(double _start, double _end) : Pipe2Dradius(_start, _end) {}
	double start() const { return radi.x; }
	double end() const { return radi.y; }
	void setStart(double r) { setRad1(r); }
	void setEnd(double r) { setRad2(r); }
	void incrStart(double dr) { incrRad1(dr); }
	void incrEnd(double dr) { incrRad2(dr); }
};

class Pipe3Dradius {
public:
	Pipe3Dradius() {}
	Pipe3Dradius(double startInner, double startOuter, double endInner, double endOuter)
		: start{ Disk2Dradius(startInner, startOuter) }, end{ Disk2Dradius(endInner, endOuter) } {}

	Disk2Dradius start, end;
	double dRi() const { return end.inner() - start.inner(); }
	double dRo() const { return end.outer() - start.outer(); }
};


class ArcAngles {
public:
	ArcAngles(double _start, double _end) : start{ _start }, end{ _end }{
		m_fullCircle = false;
	}
	ArcAngles() {
		setFullCircle();
	}

	double start, end;
	double angleSize(bool absolute = true) const { 
		return absolute ? std::abs(end - start) : end - start;
	}
	bool fullCircle() const { return m_fullCircle; }

	void setFullCircle();
	double angStep(int nnodes) const;

	void setStart(double rad) { start = rad; m_fullCircle = false; }
	void setEnd(double rad) { end = rad; m_fullCircle = false; }
private:
	bool m_fullCircle;
};

class RecTubeSize {
public:
	RecTubeSize() {}
	RecTubeSize(const glm::dvec2& _inner, const glm::dvec2& _outer = glm::dvec2()) : inner{ _inner }, outer{ _outer } {}

	glm::dvec2 inner;
	glm::dvec2 outer;

	glm::dvec2 dsize() const { return outer - inner; }

	glm::dvec2 calculateOuterSize(int nNodesEdge, int nPerimeterNodeLayers);
	glm::dvec2 calculateOuterSize(int nNodesWidth, int nNodesHeight, int nPerimeterNodeLayers);
	void setOuterSize(int nNodesEdge, int nPerimeterNodeLayers);
	void setOuterSize(int nNodesWidth, int nNodesHeight, int nPerimeterNodeLayers);
};




bool limitArcAngles(ArcAngles& arcAngles, double& dang, int nnodes);