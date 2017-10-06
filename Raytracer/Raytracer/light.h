#pragma once

#include "utils.h"

//===================================================================
// Class : PointLight
// position (of the light in the scene)
// color (of the light) NOT IN USE
// intensity : decrease with inverse power law of the distance, using "fallof" as exponent
// fallof : the exponent of the inverse power law (2 for realistic light)
// radius : used to smooth shadows (mimic a realistic light with a size)
class PointLight {

	Vec3 position;
	Color color;
	Color intensity;
	double radius;
	double fallof;

public:

	PointLight() {};
	PointLight(Vec3 pos, Color col, Color i, double rad, double f) { position = pos; color = col; intensity = i; radius = rad; fallof = f; };

	Vec3 getPosition() { return position; };
	Color getColor() { return color; };
	Color getIntensity() { return intensity; };
	double getRadius() { return radius; };
	double getFallof() { return fallof; };

};