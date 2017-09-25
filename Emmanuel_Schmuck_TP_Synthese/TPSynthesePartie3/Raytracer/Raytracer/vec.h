#pragma once

#include <cmath>
using namespace std;

class Vec3 {

	double x;
	double y;
	double z;

public:

	// constructors
	Vec3() { x = 0; y = 0; z = 0; };
	Vec3(double _x, double _y, double _z) { x = _x; y = _y; z = _z; };

	// get, set
	double getX() { return x; };
	double getY() { return y; };
	double getZ() { return z; };
	void setX(double f) { x = f; };
	void setY(double f) { y = f; };
	void setZ(double f) { z = f; };
	void set(double _x, double _y, double _z) { x = _x; y = _y; z = _z; };

	// vector assign, add, sub, multiply, divide (member to member)
	void operator = (Vec3 v) { x = v.getX(); y = v.getY(); z = v.getZ(); };
	Vec3 operator + (Vec3 v) { Vec3 u(x + v.getX(), y + v.getY(), z + v.getZ()); return u; };
	Vec3 operator - (Vec3 v) { Vec3 u(x - v.getX(), y - v.getY(), z - v.getZ()); return u; };
	Vec3 operator * (Vec3 v) { Vec3 u(x * v.getX(), y * v.getY(), z * v.getZ()); return u; };
	Vec3 operator / (Vec3 v) { Vec3 u(x / v.getX(), y / v.getY(), z / v.getZ()); return u; };

	// scalar multiply and divide
	Vec3 operator * (double f) { Vec3 u(x*f, y*f, z*f); return u; };
	Vec3 operator / (double f) { Vec3 u(x / f, y / f, z / f); return u; };

	// norm and norm squared
	double norm() { return sqrt(x*x + y*y + z*z); };
	double normSquared() { return x*x + y*y + z*z; };

	// normalize the vector and returns a clone
	Vec3 normalize() { double n = norm(); Vec3 u(x / n, y / n, z / n); return u; };

	// scalar product (dot product)
	double dot(Vec3 v) { return (x*v.getX() + y*v.getY() + z*v.getZ()); };

	// vector product (cross product)
	Vec3 cross(Vec3 v) { Vec3 c(y*v.getZ() - z*v.getY(), z*v.getX() - x*v.getZ(), x*v.getY() - y*v.getX()); return c; };

	// vector lerp
	static Vec3 lerp(Vec3 v1, Vec3 v2, double alpha) {
		Vec3 v;
		v.setX((1 - alpha)*v1.getX() + alpha*v2.getX());
		v.setY((1 - alpha)*v1.getY() + alpha*v2.getY());
		v.setZ((1 - alpha)*v1.getZ() + alpha*v2.getZ());
		return v; };
};