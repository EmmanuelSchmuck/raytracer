#pragma once

#include "vec.h"
#include "sphere.h"


class Ray {

	Vec3 origin;
	Vec3 direction;
	double energy;
	bool inside;
	int containerIndex;
	double alpha;

public:

	// constructors
	Ray() { alpha = 0; };
	Ray(Vec3 o, Vec3 d, double e, bool _inside) { origin = o; direction = d.normalize(); energy = e; inside = _inside; containerIndex = -1; alpha = 0; };

	// get, set
	Vec3 getOrigin() { return origin; };
	Vec3 getDirection() { return direction; };
	double getEnergy() { return energy; };
	bool isInside() { return inside; };
	void setOrigin(Vec3 o) { origin = o; };
	void setEnergy(double e) { energy = e; };
	void setDirection(Vec3 d) { direction = d.normalize(); };
	void setContainerIndex(int i) { containerIndex = i; };
	int getContainerIndex() { return containerIndex; };
	void setAlpha(double a) { alpha = a; };
	double getAlpha() { return alpha; };
	void set(Vec3 o, Vec3 d, double e, bool _inside) { origin = o; direction = d.normalize(); energy = e; inside = _inside; };

	// get 3D point (Vec3) at distance d (float) from ray origin following ray direction
	Vec3 getPosAtDistance(float d) { Vec3 v = origin + direction*d; return v; };
	
};