#pragma once

#include "sphere.h"
#include "light.h"
#include "fog.h"
#include "ray.h"
#include "vec.h"






class Scene {

	Vec3 cameraPosition;
	Sphere* spheres;
	PointLight* lights;
	int n_spheres;
	int n_spheres_max;
	int n_lights;
	int n_lights_max;
	double ambientIntensity;
	Fog fog;


public :

	//constructors
	Scene() { };
	Scene(Vec3 campos, int n_sph, int n_li) {
		n_spheres_max = n_sph; spheres = new Sphere[n_spheres_max]; n_spheres = 0;
		n_lights_max = n_li; lights = new PointLight[n_lights_max]; n_lights = 0;
		cameraPosition = campos; };

	//get, set
	Vec3 getCameraPosition() { return cameraPosition; };
	void setCameraPosition(Vec3 pos) { cameraPosition = pos; };
	double getAmbientIntensity() { return ambientIntensity; };
	void setAmbientIntensity(double i) { ambientIntensity = i; };
	Sphere getSphere(int i) { return spheres[i]; };
	PointLight getLight(int i) { return lights[i]; };
	Fog getFog() { return fog; }

	//add a Sphere to the Scene
	void addSphere(Vec3 pos, double radius, Vec3 color, double refl, double specp, double specc, double trans) {
		if (n_spheres == n_spheres_max) { return; }
		Sphere s(n_spheres,pos,radius,color, refl, specp, specc, trans);
		spheres[n_spheres] = s;
		n_spheres++;
	};

	void addLight(Vec3 pos, Vec3 color, double intensity, double radius, double fallof) {
		if (n_lights == n_lights_max) { return; }
		PointLight l(pos, color, intensity, radius, fallof);
		lights[n_lights] = l;
		n_lights++;
	};

	void setFog(Vec3 col, double exp, double maxopa, double smoothdist, double startdist) {
		fog = Fog(col, exp, maxopa, smoothdist, startdist);
	};

	//intersect between a ray and all Spheres
	IntersectData intersectAll(Ray r, double &t) {
		IntersectData data;
		Sphere sphereHit;
		t = 999999;
		bool success = false;
		double t_temp=0;

		for (int i = 0; i < n_spheres; i++) {

			if (spheres[i].intersect(r, t_temp)) {
				if (t_temp < t) {
					t = t_temp;
					sphereHit = spheres[i];
					success = true;
				}
			}
		}

		data.success = success;
		data.distance = t;
		data.hit = sphereHit;

		return data;

	}

	double getLightingAtPoint(Ray origin, Vec3 pos, Sphere sphereHit, Vec3 &rgb, bool softShadows) {

		bool useSoftShadows = softShadows;

		double fallofPower;
	
		int trials = 50;
		int trialsuccesses = 0;
		double t,f;
		double t_temp = 0;
		double intensity = 0;
		double secondary_intensity=0;
		double intensityPerTrial = 0;
		int n_visible_lights = 0;
		bool result;
		Vec3 offset;
		Ray r;
		double radius;
		pos = pos + sphereHit.getNormalAtPos(pos)*0.001;


			for (int i = 0; i < n_lights; i++) {

				fallofPower = lights[i].getFallof();

				result = true;

					t = (pos - lights[i].getPosition()).norm();

					r.set(pos, lights[i].getPosition() - pos, 1, false);

					for (int j = 0; j < n_spheres; j++) {
						if (spheres[j].intersect(r, t_temp) && spheres[j].getReflectance()>-0.01 && spheres[j].getTransparency()<0.75) {

							if (t_temp < t) {
								result = false;
								break;
							}
						}
					}
					if (result) {

						n_visible_lights++;

						intensityPerTrial = lights[i].getIntensity() * max(0, origin.getDirection().dot(sphereHit.getReflectedRay(r, pos).getDirection())) / pow(t, fallofPower);
							
						
						//intensityPerTrial += sphereHit.getSpecularConstant() *
							//pow(max(0, (origin.getDirection()).dot(sphereHit.getReflectedRay(r, pos).getDirection())), max(0, sphereHit.getSpecularPower()));
							
							
						intensity += intensityPerTrial;

						//rgb = rgb + lights[i].getColor();
					}

			}

			if (useSoftShadows) {
				
				for (int i = 0; i < n_lights; i++) {

					radius = lights[i].getRadius();
					fallofPower = lights[i].getFallof();

					for (int k = 0; k < trials; k++) {

						result = true;

						//fix 
						offset = Vec3(1 - 2*(double)rand()/RAND_MAX, 1-2*(double)rand()/RAND_MAX, 1-2*(double)rand()/RAND_MAX) * radius;

						t = (pos - lights[i].getPosition() - offset).norm();

						r.set(pos, lights[i].getPosition() - pos + offset, 1, false);

						for (int j = 0; j < n_spheres; j++) {
							if (spheres[j].intersect(r, t_temp) && spheres[j].getReflectance()>-0.01&& spheres[j].getTransparency()<0.75) {

								if (t_temp < t) {
									result = false;
									break;
								}
							}
						}
						if (result) {

							intensity += lights[i].getIntensity() * max(0, origin.getDirection().dot(sphereHit.getReflectedRay(r, pos+offset).getDirection())) / pow(t,fallofPower);

							//intensityPerTrial += sphereHit.getSpecularConstant() *
								//pow(max(0, (origin.getDirection()).dot(sphereHit.getReflectedRay(r, pos).getDirection())), max(0, sphereHit.getSpecularPower()));

								

							//secondary_intensity += intensityPerTrial;

							//cout << intensity << endl;;

							//rgb = rgb + lights[i].getColor();
						}

					}

				}

				return (intensity / trials);

				

			}
		
			return intensity ;
		
		

	}



};








