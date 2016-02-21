#pragma once
#include <SDL.h>
#include <list>
#include "Vector.h"
#include "Line.h"
using namespace std;
class Ball
{
public:
	Vector r = Vector(0);
	double radius;
	Vector v = Vector(0);
	Ball();
	Ball(double x, double y, double r, int wsx, int wsy, double vx, double vy, double* speed);
	~Ball();
	void Move(list<Ball> *balls, list<Line> *walls);
	void Render(SDL_Renderer* renderer);
	void FirstCollision(list<Ball> *balls, list<Line> *walls);
	void Collide(Ball *ball);
	void CollideWithWall(Line wall, double movementToCollision);
	int Mass();
	double Energy();
	double* speed;
	Vector movement = Vector(0);

private:
	int c1;
	int c1Amp;
	int c2;
	int c2Amp;
	int c3;
	int c3Amp;
	int alpha;
	double start;
};

