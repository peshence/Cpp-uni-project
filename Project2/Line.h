#pragma once
#include "Vector.h"
class Ball;
//for now it's wall, really
class Line
{
public:
	Vector length;
	Vector start;
	Line();
	Line(Vector length, Vector start);
	Vector operator-(Vector r);
	double PathToCollision(Ball b);
	~Line();
};

