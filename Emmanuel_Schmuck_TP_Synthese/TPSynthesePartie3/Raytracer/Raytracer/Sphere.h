#pragma once

#include "utils.h"
#include "ray.h"


class Sphere {

	int index;
	Vec3 center;
	double radius;
	Vec3 albedo;
	double reflectance;
	double specularPower;
	double specularConstant;
	double transparency;
	double refractionIndex = 1.4;

public:

	// constructors
	Sphere() { center = Vec3(0, 0, 0); radius = 1; albedo = Vec3(0, 0, 0); reflectance = 0; specularPower = 0; specularConstant = 0; transparency = 0; };
	Sphere(int i,Vec3 c, double r, Vec3 col, double refl, double specp, double specc, double trans) {
		center = c; radius = r; albedo = col; reflectance = refl; specularPower = specp, specularConstant = specc; transparency = trans; index = i;
	};

	// get, set
	Vec3 getCenter() { return center; };
	double getRadius() { return radius; };
	Vec3 getAlbedo() { return albedo; };
	double getReflectance() { return reflectance; };
	double getSpecularPower() { return specularPower; };
	double getSpecularConstant() { return specularConstant; };
	double getTransparency() { return transparency; };
	int getIndex() { return index; }
	void setCenter(Vec3 c) { center = c; };
	void setRadius(double r) { radius = r; };
	void setAlbedo(Vec3 col) { albedo = col; };
	void set(Vec3 c, double r) { center = c; radius = r; };



	Vec3 getNormalAtPos(Vec3 pos) {
		return (pos - center).normalize();
	};

	// intersect between a ray and this Sphere
	bool intersect(Ray r, double &t) {
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

	Ray getReflectedRay(Ray r, Vec3 impact) {

		Vec3 normal = getNormalAtPos(impact);

		//cout << normal.getX() << " " << normal.getY() << " " << normal.getZ() << endl;

		Vec3 reflection = normal * -2 * r.getDirection().dot(normal) + r.getDirection();

		//Vec3 offset = randomGaussVector(transparency);

		//offset.set(0, 0, 0);

		//reflection = (reflection * offset).normalize();

		//cout << reflection.getX() << " " << reflection.getY() << " " << reflection.getZ() << endl;

		impact = impact + normal * 0.001;

		return Ray(impact, reflection, r.getEnergy(), r.isInside());

	};

	Ray getRefractedRay(Ray r, Vec3 impact) {

		Vec3 normal = r.isInside() ? getNormalAtPos(impact) *-1 : getNormalAtPos(impact);

		//Vec3 normal = getNormalAtPos(impact);

		double n1 = r.isInside() ? refractionIndex : 1.00;
		double n2 = r.isInside() ? 1.00 : refractionIndex;

		Vec3 incident = r.getDirection();

		double costheta1 = incident.dot(normal);
		double sintheta2 = (n1 / n2)*sqrt(1 - costheta1*costheta1);

		Vec3 refracted = incident*(n1 / n2) + normal * (costheta1*(n1 / n2)-sqrt(1-sintheta2*sintheta2));

		Vec3 offset = Vec3(1 - 2 * (double)rand() / RAND_MAX, 1 - 2 * (double)rand() / RAND_MAX, 1 - 2 * (double)rand() / RAND_MAX) * 0.005;

		

		//Vec3 refracted = normal.cross((normal*-1).cross(incident))*(n1 / n2)-normal*sqrt(1-(n1/n2)*(n1/n2)*normal.cross(incident).dot(normal.cross(incident)));

		
		impact = impact + normal * 0.001 ;

		double energy = (costheta1*(n1 / n2) - sqrt(1 - sintheta2*sintheta2) < 0) ? 0 : r.getEnergy();

		refracted = r.getDirection();
		energy = r.getEnergy();

		return Ray(impact, refracted+offset, energy, !r.isInside());

	};



};

