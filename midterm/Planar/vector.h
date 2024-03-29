#include <math.h>
#include <iostream>

class Vector {

public:

	double x, y, z;

	Vector() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	double dot(Vector v) {
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

	Vector cross(Vector v) {
		double x, y, z;
		x = this->y * v.z - this->z * v.y;
		y = -(this->x * v.z - this->z * v.x);
		z = this->x * v.y - this->y * v.x;
		return Vector(x, y, z);
	}

	double magnitude() {
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}
};

Vector operator+(const Vector& v1, const Vector& v2) {
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector operator-(const Vector& v1, const Vector& v2) {
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector operator*(const Vector& v, const double c) {
	return Vector(v.x * c, v.y * c, v.z * c);
}

Vector operator/(const Vector& v, const double c) {
	return Vector(v.x / c, v.y / c, v.z / c);
}

std::ostream& operator<< (std::ostream& out, Vector const& v) {
	out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return out;
}