#pragma once

#include "vec.h"


class ray {

	vec3 origin;
	vec3 direction;

public:

	// constructors
	ray() { origin = vec3(); direction = vec3(); };
	ray(vec3 o, vec3 d) { origin = o; direction = d.normalize(); };

	// get, set
	vec3 getOrigin() { return origin; };
	vec3 getDirection() { return direction; };
	void setOrigin(vec3 o) { origin = o; };
	void setDirection(vec3 d) { direction = d.normalize(); };
	void set(vec3 o, vec3 d) { origin = o; direction = d.normalize(); };

	// get 3D point (vec3) at distance d (float) from ray origin following ray direction
	vec3 getPosAtDistance(float d) { vec3 v = origin + direction*d; return v; };
	
};