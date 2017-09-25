#pragma once

#include "utils.h"
#include "scene.h"

class Renderer {

	int width;
	int height;
	double fov;
	int maxReflections = 100;
	int maxRefractions = 30;
	bool useSoftShadows;

public:

	Renderer() {};
	Renderer(int w, int h, double _fov, bool softShadows) {
		width = w; height = h;
		fov = _fov* 3.1415 / 180;
		useSoftShadows = softShadows;
	};

	void traceRay(Scene sc, Ray r, int reflexions, int refractions, Vec3 &pixelColor, bool useSoftShadows) {

		// setup
		double x, y, z, opacity, depth, energy, reflectedEnergy, refractedEnergy, startEnergy, d, intensity;
		double t = 0;
		Vec3 rgbLight(255, 255, 255), pos;
		Sphere sphereHit;

		IntersectData intersectData = sc.intersectAll(r, t);


		// if we hit a Scene object (Sphere)
		if (intersectData.success) {

			sphereHit = intersectData.hit;

			pos = r.getPosAtDistance(t);

			depth = (sc.getCameraPosition() - pos).norm();

			energy = r.getEnergy();
			startEnergy = energy;

			intensity = sc.getLightingAtPoint(r, pos, sphereHit, rgbLight, useSoftShadows);

			if (sphereHit.getReflectance() > -0.01) {
				pixelColor = Vec3::lerp(pixelColor, rgbLight * sphereHit.getAlbedo() * (intensity + sc.getAmbientIntensity()), energy);
				

				energy = r.getEnergy() * sphereHit.getReflectance();

				// reflections
				if (energy > 0.01 && sphereHit.getReflectance()>0.01 && reflexions < maxReflections) {

					Ray reflected = sphereHit.getReflectedRay(r, r.getPosAtDistance(t));

					reflected.setEnergy(energy);

					traceRay(sc, reflected, reflexions + 1, refractions, pixelColor, false);
				
				}

				energy = r.getEnergy() * sphereHit.getTransparency();

				// refractions
				if (energy > 0.01 && sphereHit.getTransparency() > 0.01 && refractions < maxRefractions) {

					//cout << "transparency" << endl;

					Ray refracted = sphereHit.getRefractedRay(r, r.getPosAtDistance(t));

					refracted.setEnergy(energy);
					refracted.setAlpha(sphereHit.getTransparency());

					traceRay(sc, refracted, reflexions, refractions + 1, pixelColor, false);

				}

			}
			else { pixelColor = rgbLight*sphereHit.getAlbedo(); }

			

			// fog
			opacity = lerp(0, sc.getFog().maxOpacity, pow(clamp((depth - sc.getFog().startDistance) / sc.getFog().smoothDistance, 0, 1), sc.getFog().exponent));
			pixelColor = Vec3::lerp(pixelColor, Vec3::lerp(pixelColor, sc.getFog().color, opacity), (reflexions ==0)? startEnergy : r.getEnergy());
			
	
		}

		else {
			pixelColor.set(0, 0, 0);
		}
	

	};

	Vec3* renderScene(Scene sc) {

		Vec3* pixels = new Vec3[width * height];

		// setup
		double x, y, z;
		Ray _ray;
		Vec3 pixelColor(0,0,0);

		cout << "calculating pixel values..." << endl;

		// calculating the rgb value for each pixel
		for (int i = 0; i < height; i++) {

			// display progress
			cout << "\r" << "  " << setprecision(3) << 100 * (double)i / height << " %      ";

			for (int j = 0; j < width; j++) {

				// xyz for ray direction
				x = j - width / 2;
				y = -i + height / 2;
				z = -width / (2 * tan(fov / 2));

				_ray.set(sc.getCameraPosition(), Vec3(x, y, z), 1, false);

				traceRay(sc, _ray, 0, 0, pixelColor, useSoftShadows);

			// fill pixel array with final rgb values
				pixels[i*width + j] = pixelColor;
			}
		}

		return pixels;

	};







};