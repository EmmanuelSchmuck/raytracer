#pragma once

#include "vec.h"

//==================================================
// Materical with reflectance, transparency, roughness and refractionIndex properties
// currently, this has no color information (albedo), the color info in on the primitives themselves
struct Material {

	double reflectance;
	double transparency;
	double roughness;
	double refractionIndex;
	Color emission;

	Material() {};
	Material(double refl, double transp) {
		reflectance = refl; transparency = transp; roughness = 0; refractionIndex = 1; emission = Color(0,0,0);
	};
	Material(double refl, double transp, double rough) {
		reflectance = refl; transparency = transp; roughness = rough; refractionIndex = 1; emission = Color(0, 0, 0);
	};
	Material(double refl, double transp, double rough, double refrac) {
		reflectance = refl; transparency = transp; roughness = rough; refractionIndex = refrac; emission = Color(0, 0, 0);
	};

	Material(double refl, double transp, double rough, double refrac, Color emi) {
		reflectance = refl; transparency = transp; roughness = rough; refractionIndex = refrac; emission = emi;
	};

};

const Material basic(0, 0);
const Material lowRefl(0.33, 0);
const Material highRefl(0.9, 0);
const Material mirrorRough(0.9, 0, 0.3);
const Material glass(0.9, 0.9, 0, 1.4);
