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

Vector::Vector(const Vector & v)
{
	x = v.x;
	y = v.y;
}

Vector Vector::operator*(double x)
{
	double x1 = this->x * x;
	double y1 = this->y * x;
	return Vector(x1, y1);
}

Vector Vector::operator/(double x)
{
	return (*this)*(1.0 / x);
}

double Vector::operator*(Vector v)
{
	return (this->x)*v.x + (this->y)*v.y;
}

Vector Vector::operator+(Vector v)
{
	return Vector(this->x + v.x, this->y + v.y);
}

void Vector::operator+=(Vector v)
{
	*this = *this + v;
}

Vector Vector::operator-(Vector v)
{
	return Vector(this->x - v.x, this->y - v.y);
}

void Vector::operator-=(Vector v)
{
	*this = *this - v;
}

double Vector::Length()
{
	return sqrt(pow(x, 2) + pow(y, 2));
}

Vector Vector::Direction()
{
	double length = this->Length();
	return length != 0 ? *this / length : *this;
}


Vector::~Vector()
{
}