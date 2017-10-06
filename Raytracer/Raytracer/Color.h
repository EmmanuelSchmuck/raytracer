#pragma once

#include "vec.h"




struct Color {

	double r;
	double g;
	double b;
	Color(double _r, double _g, double _b) { r = _r, g = _g, b = _b; };
	Color() {};

	// vector lerp
	static Color lerp(Color v1, Color v2, double alpha) {
		Color v;
		v.r = ((1 - alpha)*v1.r + alpha*v2.r);
		v.g = ((1 - alpha)*v1.g + alpha*v2.g);
		v.b = ((1 - alpha)*v1.b + alpha*v2.b);
		return v;
	};

	double norm() const { return sqrt(r*r + g*g + b*b); };

	void operator = (const Color v) { r = v.r; g = v.g; b = v.b; };
	const Color operator + (const Color v) const { Color u(r + v.r, g + v.g, b + v.b); return u; };
	const Color operator - (const Color v) const { Color u(r - v.r, g - v.g, b - v.b); return u; };
	const Color operator * (const Color v) const { Color u(r * v.r, g * v.g, b * v.b); return u; };
	const Color operator / (const Color v) const { Color u(r / v.r, g / v.g, b / v.b); return u; };

	// scalar multiplg and divide
	const Color operator *  (const double f) const { Color u(r*f, g*f, b*f); return u; };
	const Color operator / (const double f) const { Color u(r / f, g / f, b / f); return u; };

	// normalize the vector and returns a clone
	Color normalize() const { double n = norm(); Color u(r / n, g / n, b / n); return u; };

	void set(double _r, double _g, double _b) { r = _r; g = _g; b = _b; };

};


// crayola color palette
Color fuschia(181, 51, 137);
Color blue(31, 117, 254);
Color green(28, 172, 120);
Color yellow(253, 255, 0);
Color orange(255, 136, 100);
Color white(255, 255, 255);
Color black(0, 0, 0);
Color lightGrey(200, 200, 200);
Color darkGrey(100, 100, 100);
Color red(237, 10, 63);