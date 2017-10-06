#pragma once

#include "utils.h"
#include "ray.h"
#include "materials.h"

//===================================================================
// Abstract base class for all primitives (sphere, plane, disk etc...)
class Primitive {

protected:

	string type = "none";
	int index;
	Vec3 center;
	double radius;
	Material material;
	Color albedo;
	bool invertedNormals = false;

public:

	Vec3 bounds[2];

	Primitive() {};

	int getIndex() { return index; }
	void setCenter(Vec3 c) { center = c; };
	double getRadius() { return radius; }

	Vec3 getCenter() { return center; };
	Color getAlbedo() { return albedo; };
	Material getMat() { return material; };
	string getType() { return type; }

	virtual Vec3 getNormalAtPos(Vec3 pos) = 0;
	virtual bool intersect(Ray r, double &t) = 0;
	virtual Vec3 getRandomPointOnSurface() = 0;

	// get the reflection of ray r at point impact
	Ray getReflectedRay(const Ray &r, const Vec3 &impact) {

		// normal of the surface at point 'impact'
		Vec3 normal = getNormalAtPos(impact);

		// reflection direction
		Vec3 reflection = normal * -2 * r.getDirection().dot(normal) + r.getDirection();

		// random offset for the roughness (blur the reflection)
		Vec3 offset = Vec3(1 - 2 * rng.rand(), 1 - 2 * rng.rand(), 1 - 2 * rng.rand());
		offset = offset * 1 * material.roughness;

		// make sure the new ray origin is not inside the object
		//Vec3 origin = impact + normal * 0.001;

		reflection = Vec3::lerp(reflection, randomDirectionInsideUnitHemisphere_CosineLobeAroundNormal(1, normal),material.roughness);

		//return Ray(origin, (reflection + offset).normalize(), r.getEnergy(), r.isInside());
		return Ray(impact,  reflection.normalize(), r.getEnergy(), r.isInside());


	};

	// get the refraction of ray r at point impact
	Ray getRefractedRay(Ray r, Vec3 impact) {

		// normal of the surface at point 'impact'
		Vec3 normal = r.isInside() ? getNormalAtPos(impact) *-1 : getNormalAtPos(impact);

		double n1 = r.isInside() ? material.refractionIndex : 1.00;
		double n2 = r.isInside() ? 1.00 : material.refractionIndex;

		Vec3 incident = r.getDirection();

		float iCrossN = incident.cross(normal).norm();
		float iDotN = incident.dot(normal);
		float theta = (float)asin((n1 / n2) * iCrossN);
		if (theta != theta) {
			return Ray(impact, incident, r.getEnergy(), !r.isInside());
		}
		float alpha = (float)(iCrossN / (tan(theta)) * sign(iDotN) - iDotN);

		// refraction direction
		Vec3 refracted = incident + normal*alpha;

		// make sure the new ray origin is on the right side of the object
		//Vec3 origin = impact + normal * 0.001;

		double energy = r.getEnergy();

		return Ray(impact, refracted, energy, !r.isInside());

	};

	bool isInside(Primitive* p) {

		
		/*
		double xmin = bounds[0].x, pxmin = p->bounds[0].x;
		double ymin = bounds[0].y, pymin = p->bounds[0].y;
		double zmin = bounds[0].z, pzmin = p->bounds[0].z;
		double xmax = bounds[1].x, pxmax = p->bounds[1].x;
		double ymax = bounds[1].y, pymax = p->bounds[1].y;
		double zmax = bounds[1].z, pzmax = p->bounds[1].z;

		if (type == "plane") { return false; }

		if (xmin < pxmin || ymin < pymin || zmin < pzmin) { return false; }
		if (xmax > pxmax || ymax > pymax || zmax > pzmax) { return false; }

		return true;
		*/
		
		//cout << p->bounds[0] << " " << p->bounds[1] << endl;
		if (type == "plane") { return false; }
		if (center.x < p->bounds[0].x || center.y < p->bounds[0].y || center.z < p->bounds[0].z) { return false; }
		if (center.x > p->bounds[1].x || center.y > p->bounds[1].y || center.z > p->bounds[1].z) { return false; }
		return true;
		

	}

};

class Box : public Primitive {


public:

	
	double sizeX, sizeY, sizeZ;
	Vec3 faceXp, faceXm, faceYp, faceYm, faceZp, faceZm;

	Box() {
		type = "box";
	};
	Box(int i, Vec3 c, double w, Color alb, Material mat) {
		center = c; material = mat; index = i; albedo = alb / 255; type = "box";
		bounds[0] = center+Vec3(1,1,1)*-w / 2;
		bounds[1] = center + Vec3(1, 1, 1)*+w / 2;
		radius = w / 2;
		update();
	};

	Box(int i, Vec3 c, double sx, double sy, double sz, Color alb, Material mat) {
		center = c; material = mat; index = i; albedo = alb / 255; type = "box";
		bounds[0] = center + Vec3(sx, sy, sz) / 2;
		bounds[1] = center + Vec3(sx, sy, sz) / 2;
		radius = (sx+sy+sz) / 6;
		update();
	};


	void update() {

		sizeX = bounds[1].x - bounds[0].x;
		sizeY = bounds[1].y - bounds[0].y;
		sizeZ = bounds[1].z - bounds[0].z;

		faceXp = center + Vec3(1, 0, 0)*sizeX / 2;
		faceXm = center - Vec3(1, 0, 0)*sizeX / 2;
		faceYp = center + Vec3(0, 1, 0)*sizeY / 2;
		faceYm = center - Vec3(0, 1, 0)*sizeY / 2;
		faceZp = center + Vec3(0, 0, 1)*sizeZ / 2;
		faceZm = center - Vec3(0, 0, 1)*sizeZ / 2;

		//cout << faceZp-center << endl;


	
	}

	Vec3 getNormalAtPos(Vec3 pos) {

		Vec3 fp, n;

		double eps=0.0001;
		//Vec3 n = (pos - center).normalize();

		fp = pos - faceXp;
		n = Vec3(1, 0, 0);
		if (fp.normalize().dot(n) < eps && fp.normalize().dot(n) > -eps) { return n; }
		fp = pos - faceXm;
		n = Vec3(-1, 0, 0);
		if (fp.normalize().dot(n) < eps && fp.normalize().dot(n) > -eps) { return n; }
		fp = pos - faceYp;
		n = Vec3(0, 1, 0);
		if (fp.normalize().dot(n) < eps && fp.normalize().dot(n) > -eps) { return n; }
		fp = pos - faceYm;
		n = Vec3(0, -1, 0);
		if (fp.normalize().dot(n) < eps && fp.normalize().dot(n) > -eps) { return n; }
		fp = pos - faceZp;
		n = Vec3(0, 0, 1);
		if (fp.normalize().dot(n) < eps && fp.normalize().dot(n) > -eps) { return n; }
		fp = pos - faceZm;
		n = Vec3(0, 0, -1);
		if (fp.normalize().dot(n) < eps && fp.normalize().dot(n) > -eps) { return n; }

		return Vec3(0,0,0);
	};

	Vec3 getRandomPointOnSurface() {
		Vec3 pos;

		double r1 = rng.rand();
		double r2 = rng.rand();

		pos.x = center.x ;
		pos.y = center.y ;
		pos.z = center.z ;
		//cout << pos << endl;
		return pos + getNormalAtPos(pos)*0.01;
	};

	// intersect between a ray and this primitive
	bool intersect(Ray r, double &t) {

		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (bounds[r.sign[0]].x - r.getOrigin().x) * r.invdir.x;
		tmax = (bounds[1 - r.sign[0]].x - r.getOrigin().x) * r.invdir.x;
		tymin = (bounds[r.sign[1]].y - r.getOrigin().y) * r.invdir.y;
		tymax = (bounds[1 - r.sign[1]].y - r.getOrigin().y) * r.invdir.y;

		if ((tmin > tymax) || (tymin > tmax))
			return false;
		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (bounds[r.sign[2]].z - r.getOrigin().z) * r.invdir.z;
		tzmax = (bounds[1 - r.sign[2]].z - r.getOrigin().z) * r.invdir.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		if (tmin > 0 || tmax > 0) {
			if (tmin > 0) { t = tmin; }
			else if (tmax > 0) { t = tmax; }

			return true;
		}
		return false;
	

	};





};

//===================================================================
class Sphere : public Primitive {

	double radius;

public:

	// constructors
	Sphere() { type = "sphere"; };
	Sphere(int i, Vec3 c, double r, Color alb, Material mat) {
		center = c; radius = r; material = mat; index = i; albedo = alb / 255; type = "sphere";
		bounds[0] = center - Vec3(radius,radius,radius)*1;
		bounds[1] = center + Vec3(radius, radius, radius)*1;
	};
	Sphere(int i, Vec3 c, double r, Color alb, Material mat, bool invertNormals) {
		center = c; radius = r; material = mat; index = i; albedo = alb / 255; invertedNormals = invertedNormals; type = "sphere";
		bounds[0] = center - Vec3(radius, radius, radius)*1;
		bounds[1] = center + Vec3(radius, radius, radius)*1;
	};

	Vec3 getNormalAtPos(Vec3 pos) {
		return (invertedNormals)? (pos - center).normalize()*-1 : (pos - center).normalize();
	};

	Vec3 getRandomPointOnSurface() {
		Vec3 pos;
		

		double r1 = rng.rand();
		double r2 = rng.rand();

		pos.x = center.x + 2 * radius*cos(2 * 3.1415*r1)*sqrt(r2*(1 - r2));
		pos.y = center.y + 2 * radius*sin(2 * 3.1415*r1)*sqrt(r2*(1 - r2));
		pos.z = center.z + radius*(1 - 2*r2);
		//cout << pos << endl;
		return pos+getNormalAtPos(pos)*0.01;
	};

	// intersect between a ray and this primitive
	bool intersect(Ray r, double &t) {
		bool result = false;

		double delta, a, b, c, t1, t2, t0;

		a = 1.0;
		b = 2.0 * (r.getDirection().dot(r.getOrigin() - center));
		c = (r.getOrigin() - center).normSquared() - radius*radius;

		delta = b*b - 4.0 * a*c;

		t1 = (-b + sqrt(delta)) / (2 * a);
		t2 = (-b - sqrt(delta)) / (2 * a);
		t0 = -b / (2 * a);


		if (delta > 0 && (t1 > 0 || t2 > 0)) {

			result = true;

			if (t2 > 0) { t = t2; }
			else if (t1 > 0) { t = t1; }

		}

		return result;

	};

};

//===================================================================
class Plane : public Primitive {

	Vec3 normal;

public:

	// constructors
	Plane() { type = "plane"; };
	Plane(int i, Vec3 c, Vec3 n, Color alb, Material mat) {
		center = c; normal = n.normalize(); material = mat; index = i; albedo = alb / 255; type = "plane";
	};


	Vec3 getNormalAtPos(Vec3 pos) {
		return normal.normalize();
	};

	Vec3 getRandomPointOnSurface() {
		return center;
	};

	// intersect between a ray and this primitive
	bool intersect(Ray r, double &t) {

		Vec3 p0 = center;
		Vec3 l0 = r.getOrigin();
		Vec3 n = normal*-1;
		Vec3 l = r.getDirection();

		float denom = n.dot(l);
		if (denom > 1e-6) {

			Vec3 p0l0 = p0 - l0;
			t = p0l0.dot(n) / denom;
			return (t >= 0);
		}
		return false;
	}
};


//===================================================================
class Disk : public Primitive {

	Vec3 normal;
	double radius;

public:

	// constructors
	Disk() { type = "disk"; };
	Disk(int i, Vec3 c, Vec3 n, double r, Color alb, Material mat) {
		center = c; normal = n.normalize(); material = mat; index = i; albedo = alb / 255; radius = r; type = "disk";
		bounds[0] = center - Vec3(radius, radius, radius);
		bounds[1] = center + Vec3(radius, radius, radius);
	};

	Vec3 getNormalAtPos(Vec3 pos) {
		return normal.normalize();
	};

	Vec3 getRandomPointOnSurface() {
		return center;
	};

	// intersect between a ray and this primitive
	bool intersect(Ray r, double &t) {

		Vec3 p0 = center;
		Vec3 l0 = r.getOrigin();
		Vec3 n = normal*-1;
		Vec3 l = r.getDirection();


		float denom = n.dot(l);
		if (denom > 1e-6) {
			Vec3 p0l0 = p0 - l0;
			t = p0l0.dot(n) / denom;
			double distFromCenter = (p0 - r.getPosAtDistance(t)).norm();
			return (t >= 0 && distFromCenter < radius);
		}

		return false;

	};

};


