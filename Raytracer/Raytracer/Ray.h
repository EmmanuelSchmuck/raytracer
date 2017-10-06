#pragma once

#include "vec.h"
#include "primitives.h"

//=========================================================
// Class : Ray
// a ray with origin and direction
// also contains "energy" (pseudo realistic) that will decrease after each reflection or refraction
// bool isInside is used to keep track of whether the ray is inside or outside a primitive
// containerIndex and alpha are not used
class Ray {

	Vec3 origin;
	Vec3 direction;
	double energy;
	bool inside;
	int containerIndex;
	double alpha;

public:

	Vec3 invdir;
	int sign[3]; 

	// constructors
	Ray() { alpha = 0;
	origin = Vec3(0, 0, 0); direction = Vec3(0,0,0); energy = 1; inside = false; containerIndex = -1; alpha = 0;
	invdir.set(1 / direction.x, 1 / direction.y, 1 / direction.z);
	sign[0] = (invdir.x < 0);
	sign[1] = (invdir.y < 0);
	sign[2] = (invdir.z < 0);
	
	};

	Ray(Vec3 o, Vec3 d, double e, bool _inside) {
		origin = o; direction = d.normalize(); energy = e; inside = _inside; containerIndex = -1; alpha = 0;
		invdir.set(1/direction.x,1/direction.y,1/direction.z);
		sign[0] = (invdir.x < 0);
		sign[1] = (invdir.y < 0);
		sign[2] = (invdir.z < 0);
	};

	// get, set
	Vec3 getOrigin() const { return origin; };
	Vec3 getDirection() const { return direction; };
	double getEnergy() const { return energy; };
	bool isInside() const { return inside; };
	void setOrigin(Vec3 o) { origin = o; };
	void setEnergy(double e) { energy = e; };
	void setDirection(Vec3 d) { direction = d.normalize(); };
	void setContainerIndex(int i) { containerIndex = i; };
	int getContainerIndex() { return containerIndex; };
	void setAlpha(double a) { alpha = a; };
	double getAlpha() { return alpha; };
	void set(Vec3 o, Vec3 d, double e, bool _inside) {
		origin = o; direction = d.normalize(); energy = e; inside = _inside;
		invdir.set(1 / direction.x, 1 / direction.y, 1 / direction.z);
		sign[0] = (invdir.x < 0);
		sign[1] = (invdir.y < 0);
		sign[2] = (invdir.z < 0);
	
	};

	// get 3D point (Vec3) at distance d (float) from ray origin following ray direction
	Vec3 getPosAtDistance(float d) { Vec3 v = origin + direction*d; return v; };

};

