#include <iostream>
#include <math.h>

class Vector {

public:

	double x, y, z;

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

int main(int argc, char** argv) {
	Vector v1(1, 2, 3);
	Vector v2(2, 3, 4);

	std::cout << "Starting Vectors:\n";
	std::cout << "v1 = " << v1 << "\n";
	std::cout << "v2 = " << v2 << "\n";

	std::cout << "\nAddition:\n";
	std::cout << "v1 + v2 = " << v1 + v2 << "\n";
	std::cout << "v2 + v1 = " << v2 + v1 << "\n";

	std::cout << "\nSubtraction:\n";
	std::cout << "v1 - v2 = " << v1 - v2 << "\n";
	std::cout << "v2 - v1 = " << v2 - v1 << "\n";

	std::cout << "\nMultiplication by scalar:\n";
	std::cout << "v1 * 2 = " << v1 * 2<< "\n";
	std::cout << "v2 * 3 = " << v2 * 3 << "\n";

	std::cout << "\nDivision by scalar:\n";
	std::cout << "v1 / 2 = " << v1 / 2 << "\n";
	std::cout << "v2 / 3 = " << v2 / 3 << "\n";

	std::cout << "\nDot Product:\n";
	std::cout << "v1 . v2 = " << v1.dot(v2) << "\n";
	std::cout << "v2 . v1 = " << v2.dot(v1) << "\n";

	std::cout << "\nCross Product:\n";
	std::cout << "v1 x v2 = " << v1.cross(v2) << "\n";
	std::cout << "v2 x v1 = " << v2.cross(v1) << "\n";
	
	std::cout << "\nMagnitude:\n";
	std::cout << "|v1| = " << v1.magnitude() << "\n";
	std::cout << "|v2| = " << v2.magnitude() << "\n";
}