#ifndef _Vector3_H
#define _Vector3_H

#include <math.h>
#include <iostream>
using namespace std;

class Vector3 {

public:

	// vars
	double x, y, z;

	// constructors
	Vector3();
	Vector3(double x1, double y1, double z1);

	// vector ops
	void normalize();
	Vector3 cross(const Vector3& rhs);
	double dot(const Vector3& rhs);
	inline double Length();

	//  operators
	Vector3 operator + (const Vector3& rhs) const;
	Vector3 operator - (const Vector3& rhs) const;
	Vector3 operator / (double k) const;
	Vector3 operator * (const double k) const;
	Vector3 operator *= (const double k);
	Vector3 operator += (const Vector3& rhs);
	double& operator [] (int n);
	//friend ostream& operator << (ostream& o, Vector3 v);

}; // end class


ostream& operator << (ostream& o, Vector3 v);

#endif