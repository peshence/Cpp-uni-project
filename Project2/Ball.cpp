#include "Ball.h"
#include <SDL.h>
#include <iostream>
#include <list>
#include <ctime>

using namespace std;

Ball::Ball()
{
}

///<summary>Creates a 2 dimensional ball</summary>
///<param name="x">x coordinate of center</param>
///<param name="y">y coordinate of center</param>
///<param name="r">radius</param>
///<param name="wsx">Window width</param>
///<param name="wsy">window height</param>
///<param name="vx">horizontal speed</param>
///<param name="vy">vertical speed</param>
Ball::Ball(double x, double y, double r, int wsx, int wsy, double vx, double vy)
{
	this->x = x;
	this->y = y;
	this->r = r;
	this->windowSizex = wsx;
	this->windowSizey = wsy;
	this->v_x = vx;
	this->v_y = vy;
	c1 = rand() % 255;
	c1Amp = rand() % 250;
	c2 = rand() % 255;
	c2Amp = rand() % 250;
	c3 = rand() % 255;
	c3Amp = rand() % 250;
	alpha = rand() % 255;
	start = clock();
}


Ball::~Ball()
{
}

void Ball::Move(list<Ball>* balls)
{
	x += v_x;
	y += v_y;
	CollideWithWall();

	for (list<Ball>::iterator i = balls->begin(); i != balls->end(); i++)
	{
		if (&*i != this) Collide(&*i);
	}

	int i;
	if (x < 0 || y < 0 || x>640 || y>480)
	{
		cout << "error";
		cin >> i;
	}
}


void Ball::Move()
{
	x += v_x;
	y += v_y;
	CollideWithWall();
}


void Ball::Collide(Ball * ball)
{
	Collide(this, ball);
}

void Ball::CollideWithWall()
{
	CollideWithWall(&x, r, &v_x, windowSizex);
	CollideWithWall(&y, r, &v_y, windowSizey);
}

int Ball::Mass()
{
	return M_PI*pow(r, 2);
}


///<summary>Draws a circle (lengths are in pixels)</summary>
///<param name="x">x coordinate of center</param>
///<param name="y">y coordinate of center</param>
///<param name="r">radius</param>
///<param name="renderer">SDL renderer</param>
//void Ball::Render(double x, double y, double r, SDL_Renderer* renderer)
void Ball::Render(SDL_Renderer* renderer)
{
	c1 = 255 * (sin((clock() - start) / c1Amp) + 1) / 2;
	c2 = 255 * (sin((clock() - start) / c2Amp) + 1) / 2;
	c3 = 255 * (sin((clock() - start) / c3Amp) + 1) / 2;
	alpha += sin((clock() - start) / 150);
	SDL_SetRenderDrawColor(renderer, c1, c2, c3, alpha);

	double tx = r;
	double ty = 0;
	double tr = pow(r, 2);

	while (ty < tx)
	{
		SDL_RenderDrawLine(renderer, x - tx, y + ty, x + tx, y + ty);
		SDL_RenderDrawLine(renderer, x - tx, y - ty, x + tx, y - ty);
		std::cout << ty << ' ' << tx << ' ' << tr << '\n';

		ty++;
		tr = pow(tx, 2) + pow(ty, 2);
		if ((sqrt(tr)) >= r)
			tx--;
	}

	tx = 0;
	ty = r;
	while (ty >= tx)
	{
		SDL_RenderDrawLine(renderer, x - tx, y + ty, x + tx, y + ty);
		SDL_RenderDrawLine(renderer, x - tx, y - ty, x + tx, y - ty);
		std::cout << ty << ' ' << tx << ' ' << tr << '\n';

		tx++;
		tr = pow(tx, 2) + pow(ty, 2);
		if ((sqrt(tr)) >= r)
			ty--;
	}
}

void Ball::Collide(Ball* ball1, Ball* ball2)
{
	double distance = sqrt(abs(pow((((*ball1).x) - (*ball2).x), 2) + pow((((*ball1).y) - (*ball2).y), 2)));
	double overlap = ((*ball1).r + (*ball2).r) - distance;

	if (overlap >= 0)
	{
		double phi = ((*ball1).x - (*ball2).x)!=0 ? atan(((*ball1).y - (*ball2).y) / ((*ball1).x - (*ball2).x)) : 1;

		// convert to tangent and normal components of velocity
		double vt1 = ((*ball1).v_x*sin(phi) + (*ball1).v_y*cos(phi));
		double vn1 = ((*ball1).v_x*cos(phi) + (*ball1).v_y*sin(phi));

		double vt2 = ((*ball2).v_x*sin(phi) + (*ball2).v_y*cos(phi));
		double vn2 = ((*ball2).v_x*cos(phi) + (*ball2).v_y*sin(phi));

		//calculate new normal velocities
		double _vn2 = ((*ball1).Mass() * (2 * vn1 - vn2) + (*ball2).Mass()*vn2) / ((*ball1).Mass() + (*ball2).Mass());
		double _vn1 = vn2 + _vn2 - vn1;


		//balls shouldn't overlap
		ball1->x -= ball1->v_x != 0 ? 2 * overlap*cos(phi)*(ball1->v_x) / abs(ball1->v_x) : 0;
		ball1->y -= ball1->v_y != 0 ? 2 * overlap*sin(phi)*(ball1->v_y) / abs(ball1->v_y) : 0;


		//convert back to (x,y) components of velocity
		ball1->v_x = (_vn1*cos(phi) + vt1*sin(phi));
		ball1->v_y = (_vn1*sin(phi) + vt1*cos(phi));

		ball2->v_x = (_vn2*cos(phi) + vt2*sin(phi));
		ball2->v_y = (_vn2*sin(phi) + vt2*cos(phi));
	}
}
bool Ball::CollideWithWall(double *x, double r, double *v_x, double windowSizex)
{
	double tempx = *v_x != 0 ? *x + r*(*v_x) / abs(*v_x) : *x;


	if (tempx >= windowSizex)
		*x -= 2 * (tempx - windowSizex);

	else if (tempx <= 0)
		*x -= 2 * tempx;

	else
		return false;


	*v_x = -*v_x;
	return true;
}
//
//void Ball::Render(int x, int y, double r, SDL_Renderer* renderer)
//{
//	int tx = r;
//	int ty = 0;
//
//	while (ty != tx)
//	{
//		SDL_RenderDrawLine(renderer, x + tx, y + ty, x, y);
//		SDL_RenderDrawLine(renderer, x - tx, y - ty, x, y);
//		SDL_RenderDrawLine(renderer, y + ty, x + tx, y, x);
//		SDL_RenderDrawLine(renderer, y - ty, x - tx, y, x);
//		SDL_RenderDrawLine(renderer, x - tx, y + ty, x, y);
//		SDL_RenderDrawLine(renderer, x + tx, y - ty, x, y);
//		SDL_RenderDrawLine(renderer, y - ty, x + tx, x, y);
//		SDL_RenderDrawLine(renderer, y + ty, x - tx, x, y);
//		
//		if (ty ^ 2 + tx ^ 2 > r ^ 2) tx--;
//		else ty++;
//	}
//}
