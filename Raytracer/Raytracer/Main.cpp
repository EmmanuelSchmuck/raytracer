/*
* TP Lancer de Rayon
* Emmanuel Schmuck (M2 Gamagora)
*
* Current features :
*	- primitives : Sphere, Plane, Disk
*   - lights (lambert) + hard shadows & smooth shadows (very slow render)
*   - materials : transparency, reflectance, roughness, refraction index
*   - depth fog (blend objects with fog according to a power law of the scene depth)
*
*/

//READ : SAH, QBVH boxes, SQBVH boxes

#include "utils.h"
#include "test.h"
#include "testscenes.h"
#include "renderer.h"




//===================================================================
void renderSceneToFile(Scene sc, Renderer rend, string fileName) {

	clock_t timer;
	timer = clock();

	Color* pixels;
	pixels = rend.renderScene(sc);
	cout << endl << "image generated in : " <<(clock()-timer)/CLOCKS_PER_SEC <<" seconds" << endl;
	cout << endl << "saving image to ppm file..." << endl;
	save_img(fileName, pixels, rend.width, rend.height);

	delete[] pixels;
}

//===================================================================
int main() {

	cout << endl << "TP Raytracer (Emmanuel Schmuck, Gamagora 2017)" << endl << endl;

	// uncomment this to test basic functionalities (vectors, image generation)
	//makeTest();


	cout << endl << "===== image generation start ... ===== " << endl;

	const int width = 1024;
	const int height = 1024;

	//global RandomNumberGenerator(seed)
	rng = RandomNumberGenerator(555);

	Renderer renderer(width, height);

	//omp_set_dynamic(8);     
	omp_set_num_threads(6);
	renderer.samplesPerPixel = 100;
	renderer.softShadowsTrials = 1; //use 0 for hard shadows , 1 for soft
	renderer.maxReflections = 5;
	renderer.maxRefractions = 2;
	renderer.setFOV(60);


	// render test scenes and save the image to a ppm file
	// comment or uncomment any line if needed

	//renderSceneToFile(furnaceTestScene(), renderer, "furnaceTest.ppm");
	renderSceneToFile(testScene1000Spheres(), renderer, "testScene1000Spheres.ppm");
	renderSceneToFile(testScene1(), renderer, "scene1.ppm");
	//renderSceneToFile(testScene0(), renderer, "scene0.ppm");
	//renderSceneToFile(testScene2(), renderer, "scene2.ppm");

	cout << endl << "===== all done ! ===== " << endl << endl;

	system("pause");


	

	return 0;

}

