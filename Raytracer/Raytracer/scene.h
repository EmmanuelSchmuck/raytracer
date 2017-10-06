#pragma once

#include "primitives.h"
#include "light.h"
#include "fog.h"
#include "ray.h"
#include "vec.h"
#include "boundingbox.h"

//===================================================================
// holds data from the intersect of a ray with a primitive
class IntersectData {

public:

	Vec3 position;
	Vec3 normal;
	Primitive* hit;
	double distance;
	bool success;

	IntersectData() {};
	IntersectData(Vec3 p, Vec3 n, double d, bool s, Primitive* _hit) { position = p; normal = n; distance = d, success = s; hit = _hit; };

};

//===================================================================
// the scene contains primitives (sphere, disk, plane...), lights, fog, etc
class Scene {

	Vec3 cameraPosition;
	vector<Primitive*> primitives;
	vector<PointLight> lights;
	double ambientIntensity;
	double environmentLightingIntensity = 0;
	Color environmentLightingColor = Color(0, 0, 0);
	Fog fog;
	



public:

	BoundingBox* boundingBox;
	int maxObjectPerBox = 64;
	int numberOfBoxes = 0;
	int total_content = 0;

	//constructors
	Scene() { };
	Scene(Vec3 campos) {
		cameraPosition = campos;
		boundingBox = new BoundingBox(campos-Vec3(1,1,1)*500,campos+Vec3(1,1,1)*500);
	};

	//get, set
	Vec3 getCameraPosition() { return cameraPosition; };
	void setCameraPosition(Vec3 pos) { cameraPosition = pos; };
	double getAmbientIntensity() { return ambientIntensity; };
	void setAmbientIntensity(double i) { ambientIntensity = i; };
	Primitive* getSphere(int i) { return primitives[i]; };
	PointLight getLight(int i) { return lights[i]; };
	Fog getFog() { return fog; }

	void computeBoundingBox() {

		boundingBox->content=primitives;
		
		splitFullBox(boundingBox);

		cout << endl << "=== scene initialized ===" << endl;
		cout << "final number of boxes : " << numberOfBoxes << endl;
		cout << "final content total : " << total_content <<" (+ "<<boundingBox->unboundedObjects<<" unbounded objects)"<< endl;

	}


	void splitFullBox(BoundingBox* b) {

		if (b->content.size()-b->unboundedObjects <= maxObjectPerBox) {
			b->resizeToFitContentBounds();
			//cout << "objects in box "<<b<<" : "<< b->content.size() << endl;
			total_content += b->content.size();
			numberOfBoxes++;

			return;
		}

		b->resizeToFitContentBounds();

		Vec3 diag = (b->bounds[1] - b->bounds[0]) / 2;
		Vec3 c = b->getCenter();
		double x = b->sizeX/2;
		double y = b->sizeY/2;
		double z = b->sizeZ/2;
		

		b->addChildBox(c + Vec3(0,0,0), c + Vec3(0, 0, 0) + diag);
		b->addChildBox(c + Vec3(-x, 0, 0), c + Vec3(-x, 0, 0) + diag);
		b->addChildBox(c + Vec3(0, -y, 0), c + Vec3(0, -y, 0) + diag);
		b->addChildBox(c + Vec3(0, 0, -z), c + Vec3(0, 0, -z) + diag);
		b->addChildBox(c + Vec3(-x, -y, 0), c + Vec3(-x, -y, 0) + diag);
		b->addChildBox(c + Vec3(0, -y, -z), c + Vec3(0, -y, -z) + diag);
		b->addChildBox(c + Vec3(-x, 0, -z), c + Vec3(-x, 0, -z) + diag);
		b->addChildBox(c + Vec3(-x, -y, -z), c + Vec3(-x, -y, -z) + diag);

		b->putContentInSubBoxes();

		b->removeEmptySubBoxes();

		for (int j = 0; j < b->childBox.size(); j++) {
			splitFullBox(b->childBox[j]);
		}


	}



	//add a Sphere to the Scene
	void addSphere(Vec3 pos, double radius, Color alb, Material mat) {

		Primitive* p = new Sphere(primitives.size(), pos, radius, alb, mat);
		primitives.push_back(p);

	};

	//add a Sphere to the Scene
	void addSphere(Vec3 pos, double radius, Color alb, Material mat, bool invertNormals) {

		Primitive* p = new Sphere(primitives.size(), pos, radius, alb, mat, invertNormals);
		primitives.push_back(p);

	};

	//add a Box to the Scene
	void addBox(Vec3 pos, double width, Color alb, Material mat) {

		Primitive* p = new Box(primitives.size(), pos, width, alb, mat);
		primitives.push_back(p);

	};

	//add a Plane to the Scene
	void addPlane(Vec3 pos, Vec3 normal, Color alb, Material mat) {

		Primitive* p = new Plane(primitives.size(), pos, normal, alb, mat);
		primitives.push_back(p);

	};

	//add a Disk to the Scene
	void addDisk(Vec3 pos, Vec3 normal, double r, Color alb, Material mat) {
		Primitive* p = new Disk(primitives.size(), pos, normal, r, alb, mat);
		primitives.push_back(p);
	};

	//add a PointLight to the Scene
	void addLight(Vec3 pos, Color color, Color intensity, double radius, double fallof) {
		PointLight l(pos, color, intensity, radius, fallof);
		lights.push_back(l);
	};

	//setup fog values (color, exponent, max opacity, smooth distance, start distance)
	void setFog(Color col, double exp, double maxopa, double smoothdist, double startdist) {
		fog = Fog(col, exp, maxopa, smoothdist, startdist);
	};

	void setEnvironmentLighting(Color c, double i) {
		environmentLightingColor = c;
		environmentLightingIntensity = i;
	}
	double getEnvironmentLightingIntensity() { return environmentLightingIntensity; }
	Color getEnvironmentLightingColor() { return environmentLightingColor; }


	IntersectData intersectBoundingBox(BoundingBox *b, Ray r) {

		IntersectData data, data_temp;
		Primitive* sphereHit = NULL;
		double t = 999999;
		bool success = false;
		double t_temp = 0;


		for (int i = 0; i < b->content.size(); i++) {
			if (b->content[i]->intersect(r, t_temp)) {
				if (t_temp < t && t_temp > 0.001) {
					t = t_temp;
					sphereHit = b->content[i];
					success = true;
				}
			}
		}

		for (int j = 0; j < b->childBox.size(); j++) {
			if (b->childBox[j]->intersect(r, t_temp)) {
				data_temp = intersectBoundingBox(b->childBox[j], r);
				if (data_temp.distance < t) {
					t = data_temp.distance;
					sphereHit = data_temp.hit;
					success = true;
				}
			}

		}

		data.success = success;
		data.distance = t;
		data.hit = sphereHit;

		return data;

	}


	// === DEPRECATED ===
	//intersect between a ray and all primitives 
	IntersectData intersectAll(Ray r, double &t) {
		IntersectData data;
		Primitive* sphereHit = NULL;
		t = 999999;
		bool success = false;
		double t_temp = 0;


		for (int i = 0; i < primitives.size(); i++) {

			if (primitives[i]->intersect(r, t_temp)) {
				if (t_temp < t && t_temp > 0.001) {
					t = t_temp;
					sphereHit = primitives[i];
					success = true;
				}
			}
		}

		data.success = success;
		data.distance = t;
		data.hit = sphereHit;

		return data;

	}

	//get the intensity of a given light at a given position (pos) on primitive p
	//trials is the number of time the calculation will be performed with a random offset position (for smooth shadows)
	Color getIntensityFromLight(PointLight light, Vec3 pos, Primitive* p, int trials) {

		double radius = light.getRadius();
		Vec3 lightPosition = light.getPosition();
		Vec3 offset;
		double fallofPower = light.getFallof();
		double t, t_temp, r1, r2;
		Color intensity(0,0,0);
		Ray ray;
		bool result = false;
		int k = 0;

		do {

			result = true;

			r1 = rng.rand();
			r2 = rng.rand();
			//cout << r1 << endl;

			//random offset (TO DO : fix for better sampling)
			offset.x = 2 * radius*cos(2 * PI*r1)*sqrt(r2*(1 - r2));
			offset.y = 2 * radius*sin(2 * PI*r1)*sqrt(r2*(1 - r2));
			offset.z = radius*(1 - 2 * r2);

			//offset.set(0, 0, 0);
			if (trials == 0) { offset.set(0, 0, 0); }

			t = (pos - lightPosition - offset).norm();

			ray.set(pos, lightPosition + offset - pos, 1, false);

			//visibility test for each primitive
			for (int j = 0; j < primitives.size(); j++) {
				if (primitives[j]->intersect(ray, t_temp) && primitives[j]->getMat().transparency < 0.5) {
					if (t_temp < t) {
						result = false;
						break;
					}
				}
			}
			//if no primitive is blocking the light, we had the light's intensity
			if (result) {
				intensity = intensity + light.getIntensity() * max(0, p->getNormalAtPos(pos).dot(ray.getDirection())) / pow(t, fallofPower);
			}
			k++;

		} while (k < trials);

		return intensity;

	}

	Color getIntensityFromPrimitive(Primitive* l, Vec3 pos, Primitive* p, int trials) {


		Vec3 pointOnPrimitive;
		Vec3 offset;
		double fallofPower = 2;
		double t, t_temp, r1, r2;
		Ray ray;
		bool result = false;
		int k = 0;
		IntersectData data;
		Color intensity(0,0,0);

		do {

			result = true;

			pointOnPrimitive = l->getRandomPointOnSurface();


			t = (pos - pointOnPrimitive).norm();

			ray.set(pos, pointOnPrimitive - pos, 1, false);

			data = intersectBoundingBox(boundingBox, ray);

			/*
			//visibility test for each primitive
			for (int j = 0; j < primitives.size(); j++) {
				if (primitives[j]->intersect(ray, t_temp) && primitives[j]->getMat().transparency < 0.5) {
					if (t_temp < t) {
						result = false;
						break;
					}
				}
			}
			*/
			

			//if no primitive is blocking the light, we add the light's intensity
			if (data.distance>t) {
				intensity = intensity + l->getMat().emission * max(0, p->getNormalAtPos(pos).dot(ray.getDirection())) / pow(t, fallofPower);
			}
			k++;

		} while (k < trials);

		return intensity;

	}

	//get the total intensity (all lights) at a given position (pos) on primitive p
	//softShadowsTrials is the number of time the calculation will be performed with a random offset position (for smooth shadows)
	Color getLightingAtPoint(Vec3 pos, Primitive* hit, int softShadowsTrials) {

		Color intensity(0,0,0);

		// offset to make sure we stay out of the primitive
		//pos = pos + hit->getNormalAtPos(pos)*0.001;

		for (int i = 0; i < lights.size(); i++) {

			intensity = intensity + getIntensityFromLight(lights[i], pos, hit, softShadowsTrials);

		}

		for (int j = 0; j < primitives.size(); j++) {

			if (primitives[j]->getMat().emission.norm() > 0.01 && primitives[j]->getIndex() != hit->getIndex()) {


				intensity = intensity + getIntensityFromPrimitive(primitives[j], pos, hit, softShadowsTrials);
			}



		}
		return (softShadowsTrials == 0) ? intensity : intensity / softShadowsTrials;

	}

};
