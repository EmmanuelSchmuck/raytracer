#pragma once


#include "utils.h"


class sphere {

	vec3 center;
	double radius;
	vec3 color;
	double reflectance;

public:

	// constructors
	sphere() { center = vec3(0, 0, 0); radius = 1; color = vec3(0, 0, 0); reflectance = 0; };
	sphere(vec3 c, double r, vec3 col, double refl) { center = c; radius = r; color = col; reflectance = refl; };

	// get, set
	vec3 getCenter() { return center; };
	double getRadius() { return radius; };
	vec3 getColor() { return color; };
	double getReflectance() { return reflectance; };
	void setCenter(vec3 c) { center = c; };
	void setRadius(double r) { radius = r; };
	void setColor(vec3 col) { color = col; };
	void set(vec3 c, double r) { center = c; radius = r; };

	vec3 getNormalAtPos(vec3 pos) { return (pos - center).normalize(); };

	// intersect between a ray and this sphere
	bool intersect(ray r, double &t) {
		bool result = false;

		double delta, a, b, c, t1, t2;

		a = 1.0;
		b = 2.0 * (r.getDirection().dot(r.getOrigin()-center));
		c = (r.getOrigin()-center).normSquared() - radius*radius;

		delta = b*b - 4.0 * a*c;

		t1 = (-b + sqrt(delta)) / (2 * a);
		t2 = (-b - sqrt(delta)) / (2 * a);


		if (delta >= 0 && (t1>0 || t2>0)) {

			result = true;

			if (t2 > 0) { t = t2; }
			else if (t1 > 0) { t = t1; }
			
		}

		

		return result;
	
	};

	ray getReflectedRay(ray r, vec3 impact) {

		vec3 normal = getNormalAtPos(impact);

		//cout << normal.getX() << " " << normal.getY() << " " << normal.getZ() << endl;

		vec3 reflection = normal * -2 * r.getDirection().dot(normal) + r.getDirection();

		//cout << reflection.getX() << " " << reflection.getY() << " " << reflection.getZ() << endl;

		impact = impact + normal * 0.001;

		return ray(impact, reflection);

	};



};