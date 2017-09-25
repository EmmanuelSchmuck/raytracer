#pragma once



#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "vec.h"
#include "Sphere.h"

using namespace std;

class IntersectData {

public:

	Vec3 position;
	Vec3 normal;
	Sphere hit;
	double distance;
	bool success;

	IntersectData() {};
	IntersectData(Vec3 p, Vec3 n, double d, bool s, Sphere _hit) { position = p; normal = n; distance = d, success = s; hit = _hit; };

};

// clamp a double value between min and max
double clamp(double value, double min, double max) {

	if (value < min) { return min; }
	else if (value > max) { return max; }
	else { return value; }
}

// clamp a double value between min and max
double max(double a, double b) {
	return (a<b) ? b : a;
}


// lerp (linear interpolation) between two doubles
double lerp(double a, double b, double alpha) {
	alpha = clamp(alpha, 0, 1);
	return (1 - alpha) * a + alpha * b;
}

Vec3 randomGaussVector(double variance) {

	if (variance < 0.001) {
		return Vec3(1, 1, 1).normalize();
	}

	variance = 10 * variance;

	double x = (double)rand() / RAND_MAX;
	x = exp(-(x*x) / (2 * variance)) / sqrt(2 * 3.1415*variance);
	double y = (double)rand() / RAND_MAX;
	y = exp(-(y*y) / (2 * variance)) / sqrt(2 * 3.1415*variance);
	double z = (double)rand() / RAND_MAX;
	z = exp(-(z*z) / (2 * variance)) / sqrt(2 * 3.1415*variance);

	Vec3 v(x, y, z);

	//cout << v.normalize().getX()<<" " << v.normalize().getY() << " " << v.normalize().getZ() << endl;

	return v.normalize();

}



// save a ppm image file from the rgb value in a pixel array
// filename : name of the file, ending with .ppm (string)
// pixels : a Vec3 array of pixels (r,g,b)
// width : image width (int)
// height : image height (int)
void save_img(string filename, Vec3* pixels, int width, int height) {

	int IMG_WIDTH = width;
	int IMG_HEIGHT = height;
	ofstream myfile;
	
	myfile.open(filename);
	myfile << "P3\n" << IMG_WIDTH <<" "<<IMG_HEIGHT<<"\n" << "255 \n";

	double r, g, b;

	cout << "writing in file : " <<filename<< endl;
	
	for (int i = 0; i < IMG_HEIGHT; i++) {
		
		cout << "\r" << setprecision(3) << 100 * (double)i / IMG_HEIGHT << " %      " ;
		for (int j = 0; j < IMG_WIDTH; j++) {

			r = clamp(pixels[i*IMG_WIDTH + j].getX(), 0, 255);
			g = clamp(pixels[i*IMG_WIDTH + j].getY(), 0, 255);
			b = clamp(pixels[i*IMG_WIDTH + j].getZ(), 0, 255);
			myfile <<r<<" "<<g<<" "<<b<<"\n";		
		}
	}
	myfile.close();
}