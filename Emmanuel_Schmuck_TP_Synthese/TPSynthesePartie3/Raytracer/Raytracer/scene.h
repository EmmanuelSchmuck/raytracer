#pragma once

#include "sphere.h"
#include "light.h"
#include <iostream>

class scene {

	vec3 cameraPosition;
	sphere* spheres;
	pointLight* lights;
	int n_spheres;
	int n_spheres_max;
	int n_lights;
	int n_lights_max;


public :

	//constructors
	scene() { };
	scene(vec3 campos, int n_sph, int n_li) {
		n_spheres_max = n_sph; spheres = new sphere[n_spheres_max]; n_spheres = 0;
		n_lights_max = n_li; lights = new pointLight[n_lights_max]; n_lights = 0;
		cameraPosition = campos; };

	//get, set
	vec3 getCameraPosition() { return cameraPosition; };
	void setCameraPosition(vec3 pos) { cameraPosition = pos; };
	sphere getSphere(int i) { return spheres[i]; };
	pointLight getLight(int i) { return lights[i]; };

	//add a sphere to the scene
	void addSphere(vec3 pos, double radius, vec3 color, double refl) {
		if (n_spheres == n_spheres_max) { return; }
		sphere s(pos,radius,color, refl);
		spheres[n_spheres] = s;
		n_spheres++;
	};

	void addLight(vec3 pos, vec3 color, double intensity, double radius) {
		if (n_lights == n_lights_max) { return; }
		pointLight l(pos, color, intensity, radius);
		lights[n_lights] = l;
		n_lights++;
	};

	//intersect between a ray and all spheres
	bool intersectSpheres(ray r, double &t, sphere &sphereHit) {
		t = 999999;
		bool result = false;
		double t_temp=0;

		for (int i = 0; i < n_spheres; i++) {

			if (spheres[i].intersect(r, t_temp)) {
				if (t_temp < t) {
					t = t_temp;
					sphereHit = spheres[i];
					result = true;
				}
			}
		}

		return result;

	}

	double getLightingAtPoint(vec3 pos, vec3 &rgb) {
	
		double t;
		double t_temp = 0;
		double intensity = 0;
		bool result;

		for (int i = 0; i < n_lights; i++) {

			result = true;

			t = (pos - lights[i].getPosition()).norm();

			ray r(pos, lights[i].getPosition() - pos);

			for (int j = 0; j < n_spheres; j++) {
				if (spheres[j].intersect(r, t_temp) && spheres[j].getReflectance()>-0.01) {

					if (t_temp < t) {
						result = false;
					}
				}
			}
			if (result) {
				intensity += lights[i].getIntensity();
				rgb = rgb + lights[i].getColor();
			}
			
		}

		return intensity;

	}
	
















};
