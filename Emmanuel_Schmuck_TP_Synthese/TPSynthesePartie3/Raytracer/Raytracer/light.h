#pragma once

#include "utils.h"

class pointLight {

	vec3 position;
	vec3 color;
	double intensity;
	double radius;

public:

	pointLight() {};
	pointLight(vec3 pos, vec3 col, double i, double rad) { position = pos; color = col; intensity = i; radius = rad; };

	vec3 getPosition() { return position; };
	vec3 getColor() { return color; };
	double getIntensity() { return intensity; };
	double getRadius() { return radius; };


};