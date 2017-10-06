#pragma once

#include "utils.h"
#include "scene.h"
#include <omp.h>
#include <cstdlib>

//===================================================================
// performs the render of a scene using raytracing
struct Renderer {

	int width;
	int height;
	double fov;
	int maxReflections = 7;
	int maxRefractions = 5;
	int softShadowsTrials;
	int samplesPerPixel;

	Renderer() {};
	Renderer(int w, int h) { width = w; height = h; };
		
	void setFOV(double angleDeg) { fov = angleDeg * PI / 180; };

	//=========================
	// performs a raytracing in scene sc with ray r, and updates the pixelcolor at each iteration
	// the function is recursive and will call itself when a reflexion and/or refraction occurs
	// the two integers 'reflexions' and 'refractions' are used to limit the recursion
	Color traceRay(Scene sc, Ray r, int reflexions, int refractions, Color pixelColor, int softshadowtrials) {

		// setup
		double x, y, z, opacity, depth, energy, reflectedEnergy, refractedEnergy, startEnergy, d, t;
		Color pixelColorTemp, pixelColorSum, intensity;
		Vec3 pos;
		Primitive* hit;
		int k;

		IntersectData intersectData = sc.intersectBoundingBox (sc.boundingBox, r);
		//IntersectData intersectData = sc.intersectAll(r,t);


		// if we hit a Scene object
		if (intersectData.success) {

			// holds the scene object
			hit = intersectData.hit;

			t = intersectData.distance;

			// impact position
			pos = r.getPosAtDistance(t);

			// distance between camera and impact
			depth = (sc.getCameraPosition() - pos).norm();

			// we use energy (pseudo realistic) to attenuate the ray after each reflection/refraction
			energy = r.getEnergy();
			startEnergy = energy;

			// direct lighting intensity at the impact point
			intensity = sc.getLightingAtPoint(pos, hit, softshadowtrials);

	
			// update pixel color (blended with previous values using energy value)
			//pixelColor = Color::lerp(pixelColor, rgbLight *hit->getAlbedo() * (intensity + sc.getAmbientIntensity()), energy);
			pixelColor = pixelColor + hit->getAlbedo() * (intensity) * hit->getMat().roughness * energy/PI;

		

			if (hit->getMat().emission.norm() > 0.1) {

				Color emi = hit->getMat().emission;
				emi.r = clamp(log10(1 + emi.r) * 255, 0, 5 * 255);
				emi.g = clamp(log10(1 + emi.g) * 255, 0, 5 * 255);
				emi.b = clamp(log10(1 + emi.b) * 255, 0, 5 * 255);

				Color circ = hit->getAlbedo()*255;
				//circ = circ.normalize() * 255;

				pixelColor = pixelColor + Color::lerp(circ, emi, pow(clamp(r.getDirection().dot(hit->getNormalAtPos(pos)*-1), 0, 1),3)) * energy;
			}

			//pixelColor = pixelColor + emi * energy;

			//pixelColor = pixelColor + rgbLight * clamp(log10(1 + hit->getMat().emission), 0, 5) * energy;

			// reflections
			energy = startEnergy * hit->getMat().reflectance;
			
			if (energy > 0.01 && hit->getMat().reflectance > 0.01 && reflexions < maxReflections) {

				r.setEnergy(energy);
				
				//pixelColorTemp = pixelColor;
				Ray reflected = hit->getReflectedRay(r, r.getPosAtDistance(t));

				//trace reflected ray with limited softshadows (50%)
				double PDF = reflected.getDirection().dot(hit->getNormalAtPos(reflected.getOrigin()))/PI;
				PDF = 1/PI;
				PDF = 1;
				pixelColor=pixelColor + hit->getAlbedo()*traceRay(sc, reflected, reflexions+1, refractions, pixelColor,softShadowsTrials)*1*energy/PDF;
			
				//pixelColor = Color::lerp(pixelColor,pixelColorSum/k,hit->getMat().reflectance);


			}
			
			// refractions
			energy = startEnergy * hit->getMat().transparency;
		
			if (energy > 0.01 && hit->getMat().transparency > 0.01 && refractions < maxRefractions) {

				r.setEnergy(energy);

				Ray refracted = hit->getRefractedRay(r, r.getPosAtDistance(t));

				//trace refracted ray with limited softshadows (25%)
				pixelColor = pixelColor + hit->getAlbedo()*traceRay(sc, refracted, reflexions, refractions + 1, pixelColor, softShadowsTrials)*energy;

			}

			// blend pixel color with Fog (power law of depth) : TO DO -> fix for refraction
			opacity = lerp(0, sc.getFog().maxOpacity, pow(clamp((depth - sc.getFog().startDistance) / sc.getFog().smoothDistance, 0, 1), sc.getFog().exponent));
			//opacity = opacity / log10((10+hit->getMat().emission.norm()));
			pixelColor = Color::lerp(pixelColor, Color::lerp(pixelColor, sc.getFog().color, opacity), startEnergy);

		}

		// if no object was hit, pixel is black
		else {
			pixelColor = sc.getEnvironmentLightingColor();
		}

		return pixelColor;

	};


	Color evaluatePixel(int i, int j, Scene sc) {

		int s;

		{

			Color pixelColorSum(0, 0, 0);
			//Color pixelColorSum(0, 0, 0);
			int k = 0;
			int k_max = samplesPerPixel;
			double r1, r2;



			for (int k = 0; k < k_max; k++) {

				//unsigned int myseed = omp_get_thread_num();
			


				//r1 = -0.5 + rng.rand();
				//r2 = -0.5 + rng.rand();
				r1 = -0.5 + rng.rand();
				r2 = -0.5 + rng.rand();

				// xyz for ray direction
				double x = r1 + j - width / 2;
				double y = r2 + i - height / 2;
				double z = -width / (2 * tan(fov / 2));

				Ray _ray;

				// initiate the ray
				_ray.set(sc.getCameraPosition(), Vec3(x, -y, z), 1, false);

				Color pixelColor(0, 0, 0);

				// performs a ray tracing (recursive)
				pixelColorSum = pixelColorSum + traceRay(sc, _ray, 0, 0, pixelColor, softShadowsTrials);
				//pixelColorSum = pixelColorSum + pixelColor;
			}

			return pixelColorSum / (k_max);

		}
	}


	//=========================
	Color* renderScene(Scene sc) {

		// array of all the pixels
		// each pixel value will be modified with traceRay
		Color* pixels = new Color[width * height];




		cout << endl << endl << "calculating pixel values..." << endl << endl;

		// calculating the rgb value for each pixel

#pragma omp parallel for schedule(dynamic)

		for (int i = 0; i < height; i++) {

			// display progress
			if (omp_get_thread_num()==0) {
			
				cout << "\r" << "  " << setprecision(3) << 100 * (double)i / height << " %      ";
			}
			


			for (int j = 0; j < width; j++) {

		

				pixels[i*width + j] = evaluatePixel(i,j ,sc);

				
			}
		}

		// pixel array is sent to renderSceneToFile() function in main.cpp
		return pixels;

	};







};