#pragma once
#include <cmath>

namespace pmg{

class ArcAngles {
public:
	ArcAngles(double _start, double _end) : start{ _start }, end{ _end }{
		m_fullCircle = false;
	}
	ArcAngles() {
		setFullCircle();
	}

	double start, end;

public:
	void setFullCircle();
	double angStep(int nnodes) const;
	double angleSize(bool absolute = true) const {
		return absolute ? std::abs(end - start) : end - start;
	}
	bool fullCircle() const { 
		return m_fullCircle; 
	}
	void setStart(double rad) { 
		start = rad; 
		m_fullCircle = false; 
	}
	void setEnd(double rad) { 
		end = rad; 
		m_fullCircle = false; 
	}
	
protected:
	bool m_fullCircle;
};

}