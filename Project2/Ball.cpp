#include "Ball.h"
#include <SDL.h>
#include <iostream>
#include <list>
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
}


Ball::~Ball()
{
}

void Ball::Move(list<Ball>* balls)
{
	x += v_x;
	y += v_y;
	int tempx = x + r*v_x / abs(v_x);
	int tempy = y + r*v_y / abs(v_y);
	if (tempx >= windowSizex || tempx <= 0)
	{
		x = x > r ? windowSizex - r : r;
		v_x = -v_x;
	}
	if (tempy >= windowSizey || tempy <= 0)
	{
		y = y > r ? windowSizey - r : r;
		v_y = -v_y;
	}
}


void Ball::Move()
{
	x += v_x;
	y += v_y;
	int tempx = x + r*v_x / abs(v_x);
	int tempy = y + r*v_y / abs(v_y);
	if (tempx >= windowSizex || tempx <= 0)
	{
		x = x > r ? windowSizex - r : r;
		v_x = -v_x;
	}
	if (tempy >= windowSizey || tempy <= 0)
	{
		y = y > r ? windowSizey - r : r;
		v_y = -v_y;
	}
}


void Ball::Collide(Ball * ball)
{
	Collide(this, ball);

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
void Ball::Render(double x, double y, double r, SDL_Renderer* renderer)
{
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

	if (overlap>=0)
	{
		double phi = atan((double)((*ball1).y - (*ball2).y) / (double)((*ball1).x - (*ball2).x));

		// convert to tangent and normal components of velocity
		double vt1 = ((*ball1).v_x*sin(phi) + (*ball1).v_y*cos(phi));
		double vn1 = ((*ball1).v_x*cos(phi) + (*ball1).v_y*sin(phi));
		
		double vt2 = ((*ball2).v_x*sin(phi) + (*ball2).v_y*cos(phi));
		double vn2 = ((*ball2).v_x*cos(phi) + (*ball2).v_y*sin(phi));

		//calculate new normal velocities
		double _vn2 = ((*ball1).Mass() * (2 * vn1 - vn2) + (*ball2).Mass()*vn2) / ((*ball1).Mass() + (*ball2).Mass());
		double _vn1 = vn2 + _vn2 - vn1;


		//balls shouldn't overlap
		ball1->x -= overlap*cos(phi)*ball1->v_x / abs(ball1->v_x);
		ball1->y -= overlap*sin(phi)*ball1->v_y / abs(ball1->v_y);


		//convert back to (x,y) components of velocity
		ball1->v_x = (_vn1*cos(phi) + vt1*sin(phi));
		ball1->v_y = (_vn1*sin(phi) + vt1*cos(phi));
					 
		ball2->v_x = (_vn2*cos(phi) + vt2*sin(phi));
		ball2->v_y = (_vn2*sin(phi) + vt2*cos(phi));
	}
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
