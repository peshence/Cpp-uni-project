#include "Line.h"
#include "Ball.h"


Line::Line()
{
}

Line::Line(Vector length, Vector start)
{
	this->length = length;
	this->start = start;
}

Vector Line::operator-(Vector r)
{
	Vector d = r - start;
	Vector dn = length.Direction()*(d*(length.Direction()));
	return dn - d;
}

double Line::PathToCollision(Ball b)
{
	Vector d = *this - b.r;
	return b.movement.Direction()*(d.Direction()*(d.Length() - b.radius));
}

Line::~Line()
{
}
