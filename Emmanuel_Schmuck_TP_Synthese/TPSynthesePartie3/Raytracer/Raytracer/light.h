#pragma once

#include "utils.h"

class PointLight {

	Vec3 position;
	Vec3 color;
	double intensity;
	double radius;
	double fallof;

public:

	PointLight() {};
	PointLight(Vec3 pos, Vec3 col, double i, double rad, double f) { position = pos; color = col; intensity = i; radius = rad; fallof = f; };

	Vec3 getPosition() { return position; };
	Vec3 getColor() { return color; };
	double getIntensity() { return intensity; };
	double getRadius() { return radius; };
	double getFallof() { return fallof; };

};