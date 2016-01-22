#pragma once
#include <SDL.h>
#include <list>
using namespace std;
class Ball
{
public:
	Ball();
	Ball(double x, double y, double r, int wsx, int wsy, double vx, double vy, double* speed);
	~Ball();
	void Move(list<Ball> *balls);
	void Render(SDL_Renderer* renderer);// { Render(x, y, r, renderer); }
	void Collide(Ball *ball);
	void CollideWithWall();
	int Mass();
	double Energy();
	//static void Render(double x, double y, double r, SDL_Renderer* renderer);
	static void Collide(Ball* movingBall, Ball* stationaryBall);
	static bool CollideWithWall(double *x_i, double r, double *v_i, double windowSizei);
	double* speed;
private:
	double x = 0;
	double y = 0;
	double v_x = 0;
	double v_y = 0;

	int windowSizex = 640;
	int windowSizey = 480;

	double r;


	int c1;
	int c1Amp;
	int c2;
	int c2Amp;
	int c3;
	int c3Amp;
	int alpha;
	double start;
};

