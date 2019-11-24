#include <math.h>
#include <iostream>
#include "Vector3.h"
using namespace std;

// constructors
Vector3::Vector3() {};

Vector3::Vector3(double x1, double y1, double z1) {
	x = x1; y = y1; z = z1;
}

// vector ops
void Vector3::normalize() {
	double fTemp = 1 / Length();

	x *= fTemp;
	y *= fTemp;
	z *= fTemp;
}

Vector3 Vector3::cross(const Vector3& rhs) {
	return(Vector3(y * rhs.z - z * rhs.y,
		-x * rhs.z - z * rhs.x,
		x * rhs.y - y * rhs.x));
}

double Vector3::dot(const Vector3& rhs) {
	// dot product
	return((x * rhs.x) + (y * rhs.y) + (z * rhs.z));
}


double Vector3::Length() {
	return(sqrt((x * x) + (y * y) + (z * z)));
}

//  operators
Vector3 Vector3::operator + (const Vector3& rhs) const {
	return(Vector3(x + rhs.x, y + rhs.y, z + rhs.z));
}

Vector3 Vector3::operator - (const Vector3& rhs) const {
	return(Vector3(x - rhs.x, y - rhs.y, z - rhs.z));
}

Vector3 Vector3::operator / (double k) const {
	return(Vector3(x / k, y / k, z / k));
}

Vector3 Vector3::operator * (const double k) const {
	// scalar multiply
	return(Vector3(x * k, y * k, z * k));
}

Vector3 Vector3::operator *= (const double k) {
	x *= k;
	y *= k;
	z *= k;

	return(*this);
}

Vector3 Vector3::operator += (const Vector3& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return(*this);
}

double& Vector3::operator [] (int n) {

	// access like an array
	switch (n) {

	case 0: return(x);
	case 1: return(y);
	case 2: return(z);
	default: /* bug out */;
	}
}

ostream& operator << (ostream& o, Vector3 v) {
	o << v.x << " " << v.y << " " << v.z << endl;
	return(o);
}