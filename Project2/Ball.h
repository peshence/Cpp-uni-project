#pragma once
#include <SDL.h>
#include <list>
using namespace std;
class Ball
{
public:
	Ball();
	Ball(double x, double y, double r, int wsx, int wsy, double vx, double vy);
	~Ball();
	void Move();
	void Move(list<Ball> *balls);
	void Render(SDL_Renderer* renderer)	{ Render(x, y, r, renderer); }
	void Collide(Ball *ball);
	void CollideWithWall();
	int Mass();
	static void Render(double x, double y, double r, SDL_Renderer* renderer);
	static void Collide(Ball* ball1, Ball* ball2);
	static bool CollideWithWall(double *x, double r, double *v_x, double wsx);
private:
	double x = 0;
	double y = 0;
	double v_x = 0;
	double v_y = 0;

	int windowSizex = 640;
	int windowSizey = 480;

	double r;
};

