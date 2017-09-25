// TP Lancer de Rayon, Partie 3


// all other includes are in utils.h
#include "utils.h"
#include "test.h"
#include "renderer.h"

// crayola color palette
Vec3 fuschia(181, 51, 137);
Vec3 blue(31, 117, 254);
Vec3 green(28, 172, 120);
Vec3 yellow(253, 255, 0);
Vec3 orange(255,136,100);
Vec3 white(255, 255, 255);
Vec3 black(0, 0, 0);
Vec3 red(237, 10, 63);



int main() {

	srand(time(NULL));

	cout << endl << "TP Synthese d'Image" << endl << endl;

	// uncomment this to test functionalities
	//makeTest();

	cout << endl << "===== spheres : image generation start ... ===== " << endl << endl;

	system("pause");

	const int width = 2048;
	const int height = 2048;
	const double fov = 60;
	const bool useSoftShadows = false;

	Renderer renderer(width, height, fov, useSoftShadows);

	Vec3 cameraPosition(0, 10, 20);

	// camera position, max amount of spheres & lights
	Scene myScene(cameraPosition, 50, 10);

	// ambient light
	myScene.setAmbientIntensity(0.1);

	Vec3 fogColor(50, 50, 75);
	const double fogMaxOpacity = 1;
	const double fogSmoothDist = 100;
	const double fogStartDist = 60;
	const double fogExponent = 1.35;

	myScene.setFog(fogColor,fogExponent,fogMaxOpacity,fogSmoothDist,fogStartDist);

	// mirror spheres
	myScene.addSphere(Vec3(0, -10, -50), 15, blue/255, 0.25, 10, 1, 0.6);
	
	myScene.addSphere(Vec3(0, 5, -75), 10, red/255, 0.4, 10, 1, 0);
	myScene.addSphere(Vec3(0, 40, -90), 10, yellow / 255, 0.4, 10, 1, 0);
	myScene.addSphere(Vec3(25, -15, -60), 12, yellow /255, 0.5, 10, 1, 0.5);
	myScene.addSphere(Vec3(-25, -15, -60), 12, yellow /255, 0.5, 10, 1, 0.5);
	myScene.addSphere(Vec3(25, 35, -60), 12, yellow /255, 0.5, 10, 1, 0.5);
	myScene.addSphere(Vec3(-25, 35, -60), 12, yellow /255, 0.5, 10, 1, 0.5);
	myScene.addSphere(Vec3(25, -15, -110), 12, white /255, 0.9, 10, 1, 0);
	myScene.addSphere(Vec3(-25, -15, -110), 12, white /255,0.9, 10, 1, 0);
	myScene.addSphere(Vec3(25, 35, -110), 12, white /255, 0.9, 10, 1, 0);
	myScene.addSphere(Vec3(-25, 35, -110), 12, white / 255, 0.9, 10, 1, 0);
	
	// "walls"
	myScene.addSphere(Vec3(0, -2000-20, 0), 2000, white / 255, 0.25, 1, 0.5, 0);
	myScene.addSphere(Vec3(0, 2000 +50, 0), 2000, fuschia / 255, 0.05, 1, 0.5, 0);
	myScene.addSphere(Vec3(-2000-40, 0, 0), 2000, green / 255, 0.05, 1, 0.5, 0);
	myScene.addSphere(Vec3(2000+40, 0, 0), 2000, blue / 255, 0.05, 1, 0.5, 0);
	myScene.addSphere(Vec3(0, 0, -2000-150), 2000, white / 255, 0, 1, 0.5, 0);
	myScene.addSphere(Vec3(0, 0, 2000+150), 2000, yellow / 255, 0, 1, 0.5, 0);

	//lights + unlit spheres (to symbolize light sources)
	myScene.addLight(Vec3(20, 10, -70), white, 800, 3, 2);
	myScene.addSphere(Vec3(20, 10, -70), 2, Vec3(1, 1, 1) * 100, -1, 0, 0, 0);
	myScene.addLight(Vec3(-20, 10, -70), white, 800, 3, 2);
	myScene.addSphere(Vec3(-20, 10, -70), 2, Vec3(1, 1, 1) * 100, -1, 0, 0, 0);
	myScene.addLight(Vec3(-0, 15, 20), white, 3, 3, 0.2);

	// array of Vec3 with all the pixels
	Vec3* pixels;
	pixels = renderer.renderScene(myScene);

	cout << endl << "saving image to ppm file..." << endl;

	save_img("spheres.ppm", pixels, width, height);

	cout << endl << endl << "===== end of image generation ===== " << endl << endl;

	system("pause");

	delete[] pixels;

	return 0;

}