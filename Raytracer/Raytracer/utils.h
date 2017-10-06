/*
* Various utilities : sign, clamp, max, lerp...
* save_img function (generate ppm file)
*/


#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <ctime>
#include <random>
#include <vector>
#include "vec.h"
#include "Color.h"

const double PI = 3.14159265359;

using namespace std;

int sign(double val) {

	return (val < 0) ? -1 : +1;

}

struct RandomNumberGenerator {

	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;

	RandomNumberGenerator() {};
	RandomNumberGenerator(int seed) {
		generator = std::default_random_engine(seed);
		distribution = std::uniform_real_distribution<double>(0,1);
	};

	double rand() {

		return distribution(generator);
	
	}

};

// global number generator : seed is initialised in main.cpp
extern RandomNumberGenerator rng(0);


// clamp a double value between min and max
double clamp(double value, double min, double max) {

	if (value < min) { return min; }
	else if (value > max) { return max; }
	else { return value; }
}

// clamp a double value between min and max
double max(double a, double b) {
	return (a < b) ? b : a;
}


// lerp (linear interpolation) between two doubles
double lerp(double a, double b, double alpha) {
	alpha = clamp(alpha, 0, 1);
	return (1 - alpha) * a + alpha * b;
}

double randomGaussNumber(double variance) {

	double x = rng.rand();
	x = exp(-(x*x) / (2 * variance)) / sqrt(2 * PI*variance);

	return x;

}

Vec3 randomGaussVector(double variance) {

	if (variance < 0.001) {
		return Vec3(1, 1, 1).normalize();
	}

	variance = 10 * variance;

	Vec3 v(randomGaussNumber(variance), randomGaussNumber(variance), randomGaussNumber(variance));

	//cout << v.normalize().x<<" " << v.normalize().y << " " << v.normalize().z << endl;

	return v.normalize();

}



Vec3 randomDirectionInsideUnitHemisphere_CosineLobeAroundNormal(int n, Vec3 normal) {

	Vec3 dir;

	double r1 = rng.rand();
	double r2 = rng.rand();

	double thetaMax = PI / 2;

	double A = sqrt(1-pow((1-r2*(1-pow(cos(thetaMax),n+1))),2/(n+1)));
	dir.x = cos(2 * PI*r1)*A;
	dir.y = sin(2 * PI*r1)*A;
	dir.z = pow(1-r2*(1-pow(cos(thetaMax),n+1)), 1 / (n + 1));

	return dir.rotateToward(normal);

}

int pixelValueToFile(double v) {

	return clamp(pow(v/255, 1.0 / 1.8)*255,0,255);


}



// save a ppm image file from the rgb value in a pixel array
// filename : name of the file, ending with .ppm (string)
// pixels : a Vec3 array of pixels (r,g,b)
// width : image width (int)
// height : image height (int)
void save_img(string filename, Color* pixels, int width, int height) {

	int IMG_WIDTH = width;
	int IMG_HEIGHT = height;
	ofstream myfile;

	myfile.open(filename);
	myfile << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n" << "256 \n";

	double r, g, b;

	cout << "writing in file : " << filename << endl;

	for (int i = 0; i < IMG_HEIGHT; i++) {

		cout << "\r" << "  " << setprecision(3) << 100 * (double)i / IMG_HEIGHT << " %      ";
		for (int j = 0; j < IMG_WIDTH; j++) {

			r = pixelValueToFile(pixels[i*IMG_WIDTH + j].r);
			g = pixelValueToFile(pixels[i*IMG_WIDTH + j].g);
			b = pixelValueToFile(pixels[i*IMG_WIDTH + j].b);
			myfile << r << " " << g << " " << b << "\n";
		}
	}
	cout << endl;
	myfile.close();
}