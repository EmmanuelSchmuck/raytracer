// TP Lancer de Rayon, Partie 3


// all other includes are in utils.h
#include "utils.h"
#include "Test.h";

// crayola color palette
const vec3 fuschia(181, 51, 137);
const vec3 blue(31, 117, 254);
const vec3 green(28, 172, 120);
const vec3 yellow(253, 255, 0);
const vec3 white(255, 255, 255);
const vec3 red(237, 10, 63);



int main() {

	cout << endl << "TP Synthese d'Image" << endl << endl;

	// uncomment this to test functionalities
	//makeTest();

	cout << endl << "===== spheres : image generation start ... ===== " << endl << endl;

	const int width = 2048;
	const int height = 2048;

	const int max_rebound = 50;

	// camera fov
	const double fov = 60 * 3.1415 / 180;

	// fog color
	vec3 fogColor(30,30,60);

	// fog max opacity
	const double fogMaxOpacity = 1.00;

	// fog smooth distance (fog will reach max opacity at this distance)
	const double fogSmoothDist = 120;

	// fog start distance
	const double fogStartDist = 60;

	// fog exponent (power law)
	const double fogExponent = 1.5;

	vec3 cameraPosition(0, 10, 20);

	// array of vec3 with all the pixels
	vec3* pixels = new vec3[width * height];

	// camera position and max amount of spheres
	scene myScene(cameraPosition, 50, 10);

	// 7 spheres
	
	myScene.addSphere(vec3(-15, 15, -50), 2, vec3(255, 255, 255) * 100, -1);
	myScene.addSphere(vec3(15, 15, -50), 2, vec3(255, 255, 255) * 100, -1);

	myScene.addSphere(vec3(0, -5, -50), 10, white, 1);
	
	myScene.addSphere(vec3(25, -15, -50), 10, white, 1);
	myScene.addSphere(vec3(-25, -15, -50), 10, white, 1);
	myScene.addSphere(vec3(25, -15, -110), 10, white, 1);
	myScene.addSphere(vec3(-25, -15, -110), 10, white,1);
	myScene.addSphere(vec3(25, 35, -50), 10, white, 1);
	myScene.addSphere(vec3(-25, 35, -50), 10, white, 1);
	myScene.addSphere(vec3(25, 35, -110), 10, white, 1);
	myScene.addSphere(vec3(-25, 35, -110), 10, white, 1);
	
	
	// "walls"
	myScene.addSphere(vec3(0, -2000-20, 0), 2000, yellow,0.15);
	myScene.addSphere(vec3(0, 2000 +50, 0), 2000, fuschia,0.15);
	myScene.addSphere(vec3(-2000-50, 0, 0), 2000, green,0);
	myScene.addSphere(vec3(2000+50, 0, 0), 2000, blue,0);
	myScene.addSphere(vec3(0, 0, -2000-150), 2000, white ,0);
	myScene.addSphere(vec3(0, 0, 2000+150), 2000, white, 0);

	//lights
	myScene.addLight(vec3(-15, 15, -50), white, 0.35, 1);
	myScene.addLight(vec3(15, 15, -50), white, 0.35, 1);

	const double ambientIntensity = 0.15;

	// setup
	double x, y, z, opacity, depth, reflectance, d;
	double t = 0;
	int rebound = 0;
	ray _ray, _rayLight;
	vec3 finalColor, rgbLight, pos;
	double intensity = 0;
	sphere sphereHit;
	pointLight lightHit;
	bool stop;

	cout << "calculating pixel values..." << endl;

	// calculating the rgb value for each pixel
	for (int i = 0; i < height; i++) {

		// display progress
		cout << "\r" <<"  "<< setprecision(3) << 100 * (double)i / height << " %      ";

		for (int j = 0; j < width; j++) {

			// xyz for ray direction
			x = j - width / 2;
			y = - i + height / 2;
			z = -width / (2 * tan(fov / 2));

			_ray.set(myScene.getCameraPosition(), vec3(x, y, z));

			// if we hit a scene object (sphere)
			if (myScene.intersectSpheres(_ray, t, sphereHit)) {

				pos = _ray.getPosAtDistance(t);

				depth = (cameraPosition - pos).norm();

				d = depth;

				intensity = myScene.getLightingAtPoint(pos+sphereHit.getNormalAtPos(pos)*0.001, rgbLight);

				if (sphereHit.getReflectance() > -0.01) {
					finalColor = sphereHit.getColor() * (intensity + ambientIntensity);
				}
				else { finalColor = sphereHit.getColor(); }

				// reflections =======================
				rebound = 0;
				stop = false;
				//dirty
				reflectance = 1;
				while (rebound < max_rebound && max_rebound >0 && !stop) {
					stop = true;
					reflectance = reflectance * sphereHit.getReflectance() ;

					if (reflectance > 0.0001){	

						_ray = sphereHit.getReflectedRay(_ray, _ray.getPosAtDistance(t));

						if (myScene.intersectSpheres(_ray, t, sphereHit)) {

							pos = _ray.getPosAtDistance(t);

							intensity = myScene.getLightingAtPoint(pos + sphereHit.getNormalAtPos(pos)*0.001, rgbLight);

							//cout << "intensity : " << intensity << " : reflectance : " << reflectance << endl;

							finalColor = vec3::lerp(finalColor, sphereHit.getColor()* (intensity + ambientIntensity), reflectance);

							depth = (cameraPosition - pos).norm();
							opacity = lerp(0, fogMaxOpacity, pow(clamp((depth-fogStartDist) / fogSmoothDist,0,1), fogExponent));
							finalColor = vec3::lerp(finalColor, vec3::lerp(finalColor, fogColor*(intensity + ambientIntensity), opacity), reflectance);

								

							stop = false;
						}
						
					}
					rebound++;
				} ;

				// blend objects with power-law fog

				opacity = lerp(0, fogMaxOpacity, pow(clamp((d - fogStartDist) / fogSmoothDist, 0, 1), fogExponent));
				finalColor = vec3::lerp(finalColor, fogColor* (intensity + ambientIntensity), opacity);

			
			}
			else {	
				finalColor.set(0, 0, 0);
			}
			
			// fill pixel array with final rgb values
			pixels[i*width + j] = finalColor;
		}
	}

	cout << endl << "saving image to ppm file..." << endl;

	save_img("spheres.ppm", pixels, width, height);

	cout << endl << endl << "===== end of image generation ===== " << endl << endl;

	system("pause");

	delete[] pixels;

	return 0;

}