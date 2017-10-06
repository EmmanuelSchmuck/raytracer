#pragma once

#include <cmath>
using namespace std;

//===================================================================
struct Vec3 {

	double x;
	double y;
	double z;

	// constructors
	Vec3() { x = 0; y = 0; z = 0; };
	Vec3(double _x, double _y, double _z) { x = _x; y = _y; z = _z; };

	void set(double _x, double _y, double _z) { x = _x; y = _y; z = _z; };

	// vector assign, add, sub, multiply, divide (member to member)
	void operator = (const Vec3 v) { x = v.x; y = v.y; z = v.z; };
	const Vec3 operator + (const Vec3 v) const { Vec3 u(x + v.x, y + v.y, z + v.z); return u; };
	const Vec3 operator - (const Vec3 v) const { Vec3 u(x - v.x, y - v.y, z - v.z); return u; };
	const Vec3 operator * (const Vec3 v) const { Vec3 u(x * v.x, y * v.y, z * v.z); return u; };
	const Vec3 operator / (const Vec3 v) const { Vec3 u(x / v.x, y / v.y, z / v.z); return u; };

	// scalar multiply and divide
	const Vec3 operator *  (const double f) const { Vec3 u(x*f, y*f, z*f); return u; };
	
	const Vec3 operator / (const double f) const { Vec3 u(x / f, y / f, z / f); return u; };

	// norm and norm squared
	double norm() const { return sqrt(x*x + y*y + z*z); };
	double normSquared() const { return x*x + y*y + z*z; };

	// normalize the vector and returns a clone
	Vec3 normalize() const { double n = norm(); Vec3 u(x / n, y / n, z / n); return u; };

	// scalar product (dot product)
	double dot(const Vec3 &v) { return (x*v.x + y*v.y + z*v.z); };

	// vector product (cross product)
	Vec3 cross(Vec3 v) { Vec3 c(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); return c; };

	// vector lerp
	static Vec3 lerp(Vec3 v1, Vec3 v2, double alpha) {
		Vec3 v;
		v.x=((1 - alpha)*v1.x + alpha*v2.x);
		v.y=((1 - alpha)*v1.y + alpha*v2.y);
		v.z=((1 - alpha)*v1.z + alpha*v2.z);
		return v;
	};

	Vec3 rotateToward(const Vec3 N) {

		Vec3 v;
		Vec3 Z = N;
		Vec3 X = Vec3(0.3, -0.2, 0.7).cross(Z);
		Vec3 Y = Z.cross(X);
		v = X*x + Y*y + Z*z;
		return v;

	}

	
};

std::ostream& operator<<(std::ostream &stream, Vec3 v) {

	stream << "{" << v.x << ", " << v.y << ", " << v.z << "}";
	return stream;

}

//Vec3 operator*  (const Vec3& v, double f) { Vec3 u(v.x*f, v.y*f, v.z*f); return u; };