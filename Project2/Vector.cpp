#include "Vector.h"



Vector::Vector()
{
}

Vector::Vector(double r)
{
	this->x = r;
	this->y = r;
}

Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
}

Vector Vector::operator*(double x)
{
	this->x *= x;
	this->y *= x;
	return *this;
}

double Vector::operator*(Vector v)
{
	return this->x*v.x;
}

Vector Vector::operator+(Vector v)
{
	return Vector(this->x+v.x,this->y + v.y);
}

Vector Vector::operator+=(Vector v)
{
	return *this + v;
}

Vector Vector::operator-(Vector v)
{
	return Vector(this->x - v.x, this->y - v.y);
}

double Vector::Length()
{
	return pow(x,2)+pow(y,2);
}


Vector::~Vector()
{
}
