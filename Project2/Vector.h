#pragma once
#include <math.h>
using namespace std;
class Vector
{
public:
	double x;
	double y;
	Vector();
	Vector(double r);
	Vector(double x, double y);
	Vector operator*(double x);
	double operator*(Vector v);
	Vector operator+(Vector v);
	Vector operator+= (Vector v);
	Vector operator-(Vector v);
	double Length();
	~Vector();
};

