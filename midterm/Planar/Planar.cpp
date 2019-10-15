#include <math.h>
#include <iostream>
#include "vector.h"
using namespace std;


int main(int argc, char** argv) {
	int numPoints;
	bool planar = true;
	Vector A, B, C, a, b, n, r;

	// POINTS INPUT
	cout << "How many points?" << endl;
	cin >> numPoints;

	Vector* points = new Vector[numPoints];

	for (int i = 0; i < numPoints; i++) {
		cout << "Point " << i << endl;
		cout << "  x: ";
		cin >> points[i].x;
		cout << "  y: ";
		cin >> points[i].y;
		cout << "  z: ";
		cin >> points[i].z;
		cout << points[i] << endl << endl;
	}

	// CHECK IF COPLANAR
	if (numPoints <= 3)
		planar = true;
	else {
		// first 3 points make a plane
		// find normal vector to the plane
		// page 152 4.4.2
		A = points[0];
		B = points[1];
		C = points[2];
		a = A - C;
		b = B - C;
		n = a.cross(b);

		// coplanar if n dot (point - A) == 0
		// check the 4th point and on
		for (int i = 3; i < numPoints; i++) {
			r = points[i] - A;
			if (n.dot(r) != 0) {
				planar = false;
				break;
			}
		}
	}

	if (planar)
		cout << "Planar" << endl;
	else
		cout << "Non-Planar" << endl;
}
