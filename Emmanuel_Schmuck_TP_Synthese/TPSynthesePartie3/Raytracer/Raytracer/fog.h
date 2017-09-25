#pragma once

#include "vec.h"

class Fog {

public:

	Vec3 color;
	double exponent;
	double maxOpacity;
	double smoothDistance;
	double startDistance;

	Fog() {};
	Fog(Vec3 col, double exp, double maxopa, double smoothdist, double startdist) {
		exponent = exp;
		maxOpacity = maxopa;
		smoothDistance = smoothdist;
		startDistance = startdist;
		color = col;
	};


};