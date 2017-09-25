#pragma once

#include "utils.h";

void makeTest() {
	//testing vectors

	Vec3 v1(2, 2, 1);
	Vec3 v2(-3, 1, 3);

	cout << endl << "===== vector test =====" << endl << endl;

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl << "operation : v1 = v1 * 100" << endl << endl;

	v1 = v1 * 100;

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl << "operation : v1 = v1 / 100" << endl << endl;

	v1 = v1 / 100;

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;


	cout << endl << "operation : v1 = v1 + v2" << endl << endl;

	v1 = v1 + v2;

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl << "operation : v1 = v1 - v2" << endl << endl;

	v1 = v1 - v2;

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl << "operation : v1 = v1 * v2" << endl << endl;

	v1 = v1 * v2;

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl << "operation : v1 = v1 / v2" << endl << endl;

	v1 = v1 / v2;

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl << "operation : v1 = v1.normalize()" << endl << endl;

	v1 = v1.normalize();

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl;
	cout << "v1.norm() : " << v1.norm() << endl;
	cout << "v1.normSquared() : " << v1.normSquared() << endl;
	cout << "v2.norm() : " << v2.norm() << endl;
	cout << "v2.normSquared() : " << v2.normSquared() << endl;

	cout << endl;
	cout << "scalar (dot) product : v1.dot(v2) : " << v1.dot(v2) << endl;

	cout << endl << "operation : v1 = v1.cross(v2) (vector product)" << endl << endl;

	v1 = v1.cross(v2);

	cout << "v1 : " << v1.getX() << " " << v1.getY() << " " << v1.getZ() << endl;
	cout << "v2 : " << v2.getX() << " " << v2.getY() << " " << v2.getZ() << endl;

	cout << endl << "===== end of vector test ===== " << endl << endl;

	system("pause");

	cout << endl << "===== image generation test ===== " << endl << endl;

	//testing image generation

	const int width = 800;
	const int height = 500;

	// array of Vec3 with all the pixels
	Vec3* pixels = new Vec3[width * height];

	float r, g, b;

	// setting the rgb value for each pixel : top left is black, bottom left is red, top right is blue, bottom right is purple (red + blue)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			r = 255 * (float)i / height;
			g = 000;
			b = 255 * (float)j / width;
			pixels[i*width + j] = Vec3(r, g, b);
		}
	}

	cout << "saving image to ppm file..." << endl;

	save_img("test.ppm", pixels, width, height);

	cout << endl << "===== end of image generation ===== " << endl << endl;

	cout << endl << "===== end of tests ===== " << endl << endl;

	system("pause");

	delete[] pixels;

}