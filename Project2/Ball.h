#pragma once
#include <SDL.h>
#include <list>
#include "Vector.h"
#include "Line.h"
using namespace std;
class Ball
{
	//friend class Line;

public:
	Vector r = Vector(0);
	double radius;
	Vector v = Vector(0);
	Ball();
	Ball(double x, double y, double r, int wsx, int wsy, double vx, double vy, double* speed);
	~Ball();
	void Move(list<Ball> *balls, list<Line> *walls);
	void Render(SDL_Renderer* renderer);// { Render(x, y, r, renderer); }
	//void FirstCollision(list<Ball> *balls);
	void FirstCollision(list<Ball> *balls, list<Line> *walls);
	void Collide(Ball *ball);
	void CollideWithWall(Line wall, double movementToCollision);
	/*void CollideWithWall();*/
	int Mass();
	double Energy();
	//static void Render(double x, double y, double r, SDL_Renderer* renderer);
	static void Collide(Ball* movingBall, Ball* stationaryBall);
	static bool CollideWithWall(double *x_i, double r, double *v_i, double windowSizei);
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

