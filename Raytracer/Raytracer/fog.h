#pragma once

#include "vec.h"

//===================================================================
// Class : Fog
// blends objects with a color following a power law of the scene depth
// exponent : the exponent of the power law (1 is linear)
// maxOpacity : will be reached if depth >= smoothDistance + startDistance
// smoothDistance : distance over which opacity increases following the power law
// startDistance : fog has no effect if distance < startDistance
class Fog {

public:

	Color color;
	double exponent;
	double maxOpacity;
	double smoothDistance;
	double startDistance;

	Fog() {};
	Fog(Color col, double exp, double maxopa, double smoothdist, double startdist) {
		exponent = exp;
		maxOpacity = maxopa;
		smoothDistance = smoothdist;
		startDistance = startdist;
		color = col;
	};


};