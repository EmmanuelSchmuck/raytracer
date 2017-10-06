/*
* various test scenes
*/

#pragma once

#include "scene.h"
#include "utils.h"


// basic scene
Scene testScene1() {

	Vec3 cameraPosition(0, 5, -5);

	// camera position, max amount of spheres & lights
	Scene myScene(cameraPosition);

	// ambient light
	myScene.setAmbientIntensity(0);

	// point lights
	//myScene.addLight(Vec3(25, -5, -80), white, 1000, 10, 2);
	myScene.addSphere(Vec3(25, -5, -80), 1, blue, Material(0,0,0,1, Color(150, 150, 255)*1000));



	double reflectance = 1;
	double roughness = 1;


	myScene.addSphere(Vec3(-20, 10, -85), 10, darkGrey, Material(reflectance, 0, roughness));
	myScene.addSphere(Vec3(0, -10, -85), 10, darkGrey, Material(1, 0, 0));
	//myScene.addBox(Vec3(0, -10, -85), 20, lightGrey, Material(1, 0, 1));

	//myScene.addSphere(Vec3(-5, -0, -105), 7, lightGrey, Material(reflectance, 0, roughness));

	//myScene.addSphere(Vec3(25, 25, -95), 7, lightGrey, Material(reflectance, 0, roughness));
	//myScene.addDisk(Vec3(0, -19, -85), Vec3(0, 1, 0), 20, white, Material(1, 0, 0.00, 1));



	// planes :
	myScene.addPlane(Vec3(0, -20, 0), Vec3(0, 1, 0), Color(125,125,200), Material(reflectance, 0, roughness)); // bottom
	myScene.addPlane(Vec3(-30, 100, 0), Vec3(-1, -1, 0), darkGrey, Material(reflectance, 0, roughness)); // right roof
	myScene.addPlane(Vec3(30, 100, 0), Vec3(1, -1, 0), darkGrey, Material(reflectance, 0, roughness)); // left roof
	myScene.addPlane(Vec3(-50, 50, 0), Vec3(1, 0, 0), darkGrey, Material(reflectance, 0, roughness)); // left wall
	myScene.addPlane(Vec3(50, 50, 0), Vec3(-1, 0, 0), Color(200,125,125), Material(reflectance, 0, roughness)); // right wall
	myScene.addPlane(Vec3(0, 50, -150), Vec3(0, 0, 1), darkGrey, Material(reflectance, 0, roughness)); // front wall
	myScene.addPlane(Vec3(0, 50, 50), Vec3(0, 0, -1), darkGrey, Material(reflectance, 0, roughness)); // back wall

	myScene.boundingBox = new BoundingBox(cameraPosition - Vec3(1, 1, 1) * 500, cameraPosition + Vec3(1, 1, 1) * 500);
	myScene.maxObjectPerBox = 2;


	myScene.computeBoundingBox();


	return myScene;

	

};


//
Scene testScene1000Spheres() {

	Vec3 cameraPosition(0, 30, 0);

	// camera position, max amount of spheres & lights
	Scene myScene(cameraPosition);

	Color skyColor(150, 150, 225);

	// ambient light
	myScene.setAmbientIntensity(0);
	myScene.setEnvironmentLighting(skyColor, 1);

	//Color fogColor(100, 75, 30);
	const double fogMaxOpacity = 1;
	const double fogSmoothDist = 150;
	const double fogStartDist = 50;
	const double fogExponent = 1.35;
	myScene.setFog(skyColor, fogExponent, fogMaxOpacity, fogSmoothDist, fogStartDist);

	double reflectance = 1;
	double roughness = 1;

	double xmax = 30;
	double ymax = 30;
	double zmax = 40;
	Vec3 center(0, ymax, -100);

	myScene.addSphere(center + Vec3(600, 600, 600), 30, Color(255, 255, 255), Material(0, 0, 0, 1, Color(255,255,255)*5000000));

	myScene.addPlane(Vec3(0, 0, 0), Vec3(0, 1, 0), Color(125, 75, 50), Material(reflectance, 0, roughness)); // bottom

	for (int i = 0; i < 2000; i++) {
		double x = xmax*(-1 + 2 * rng.rand());
		double y = ymax*(-1 + 2 * rng.rand());
		double z = zmax*(-1 + 2 * rng.rand());
		Vec3 pos = center + Vec3(x, y, z);
		myScene.addSphere(pos, 0.75+0.75*rng.rand(), Color(rng.rand(), rng.rand(), rng.rand()) * 255, Material(1, 0, 1));

	}

	myScene.boundingBox = new BoundingBox(center - Vec3(1, 1, 1) * 1000, center + Vec3(1, 1, 1) * 1000);

	myScene.maxObjectPerBox = 2;

	myScene.computeBoundingBox();

	return myScene;



};

// basic scene
Scene testScene0() {

	Vec3 cameraPosition(0, 30, 0);

	// camera position, max amount of spheres & lights
	Scene myScene(cameraPosition);

	Color skyColor(150, 150, 225);

	// ambient light
	myScene.setAmbientIntensity(0);
	myScene.setEnvironmentLighting(skyColor, 1);

	//Color fogColor(100, 75, 30);
	const double fogMaxOpacity = 1;
	const double fogSmoothDist = 200;
	const double fogStartDist = 50;
	const double fogExponent = 1.35;
	myScene.setFog(skyColor, fogExponent, fogMaxOpacity, fogSmoothDist, fogStartDist);

	double reflectance = 1;
	double roughness = 1;

	double xmax = 40;
	double ymax = 30;
	double zmax = 50;
	Vec3 center(0, ymax, -120);

	myScene.addSphere(center + Vec3(600, 600, 600), 30, Color(255, 255, 255), Material(0, 0, 0, 1, Color(255, 255, 255)*5000000));

	myScene.addPlane(Vec3(0, 0, 0), Vec3(0, 1, 0), Color(125, 75, 50), Material(reflectance, 0, roughness)); // bottom

	for (int i = 0; i < 1000; i++) {
		double x = xmax*(-1 + 2 * rng.rand());
		double y = ymax*(-1 + 2 * rng.rand());
		double z = zmax*(-1 + 2 * rng.rand());
		Vec3 pos = center + Vec3(x, y, z);
		myScene.addBox(pos, 1.33 + 0.66*rng.rand(), Color(rng.rand(), rng.rand(), rng.rand()) * 255, Material(1, 0, 1));

	}

	myScene.boundingBox = new BoundingBox(center - Vec3(1, 1, 1) * 1000, center + Vec3(1, 1, 1) * 1000);

	myScene.maxObjectPerBox = 4;

	myScene.computeBoundingBox();

	return myScene;


	return myScene;



};

// basic scene
Scene furnaceTestScene() {

	Vec3 cameraPosition(0, 5, -5);

	// camera position, max amount of spheres & lights
	Scene myScene(cameraPosition);

	// ambient light
	myScene.setEnvironmentLighting(Color(255, 255, 255),1);

	double reflectance = 1;
	double roughness = 1;

	myScene.addSphere(Vec3(0, -0, -85), 20, Color(255,255,255)*0.33, Material(reflectance, 0, roughness));

	return myScene;



};

// scene with mirrors & refraction
Scene testScene2() {

	Vec3 cameraPosition(0, 0, 0);

	// camera position, max amount of spheres & lights
	Scene myScene(cameraPosition);

	// ambient light
	myScene.setAmbientIntensity(0);

	myScene.addSphere(Vec3(0, 0, 10), 75, white, Material(1, 0, 1,1), false);

	myScene.addSphere(Vec3(-5, -5, -30), 5, red, Material(1, 0, 1));

	myScene.addSphere(Vec3(0, 0, 10), 3, white, Material(1, 0, 1,1, Color(255, 255, 255)*1000000));


	return myScene;

};





