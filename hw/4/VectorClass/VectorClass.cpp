#include <iostream>
#include "vector.h"

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