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
///<param name="speed">controls the magnitude of the ball's velocity</param>
Ball::Ball(double x, double y, double r, int wsx, int wsy, double vx, double vy, double* speed)
{
	this->r.x = x;
	this->r.y = y;
	this->radius = r;
	this->windowSizex = wsx;
	this->windowSizey = wsy;
	this->v.x = vx;
	this->v.y = vy;
	c1 = rand() % 255;
	c1Amp = rand() % 250 + 200;
	c2 = rand() % 255;
	c2Amp = rand() % 250 + 200;
	c3 = rand() % 255;
	c3Amp = rand() % 250 + 200;
	alpha = 255;
	start = clock();
	this->speed = speed;
}


Ball::~Ball()
{
}

void Ball::Move(list<Ball>* balls, list<Line> *walls)
{
	movement = v**speed;
	FirstCollision(balls, walls);

}


void Ball::FirstCollision(list<Ball>* balls, list<Line>* walls)
{
	Ball* firstBall = NULL;
	double displacementAlongMovement = 0;
	for (list<Ball>::iterator ball = balls->begin(); ball != balls->end(); ball++)
	{
		if (&*ball == this)
			continue;

		Vector Displacement = (ball->r - this->r);
		if ((Displacement.Length() - ball->radius - this->radius) > movement.Length())
			continue;

		double displacementAlongMovementCur = Displacement*(movement.Direction());

		if (displacementAlongMovementCur < 0) continue;
		double ffstemp = (Displacement - movement.Direction()*displacementAlongMovementCur).Length();
		if (ffstemp > radius + ball->radius)
			continue;
		double tempdisplacementAlongMovementCur = displacementAlongMovementCur - sqrt(pow((this->radius + ball->radius), 2) - pow(ffstemp, 2));

		if (isnan(tempdisplacementAlongMovementCur))
		{
			cout << "stahp";
		}

		displacementAlongMovementCur -= sqrt(pow((this->radius + ball->radius), 2) - pow((Displacement - movement.Direction()*displacementAlongMovementCur).Length(), 2));

		if (displacementAlongMovementCur < 0) continue;

		if (firstBall == NULL)
		{
			firstBall = &(*ball);
			displacementAlongMovement = displacementAlongMovementCur;
			continue;
		}

		if (displacementAlongMovementCur < displacementAlongMovement)
		{
			firstBall = &*ball;
			displacementAlongMovement = displacementAlongMovementCur;
		}
	}

	Line* firstWall = NULL;
	for (list<Line>::iterator wall = walls->begin(); wall != walls->end(); wall++)
	{
		double pathToCollision = wall->PathToCollision(*this);
		if (pathToCollision > 0 && pathToCollision <= movement.Length() && (firstBall == NULL || pathToCollision < displacementAlongMovement))
		{
			firstWall = &*wall;
			displacementAlongMovement = pathToCollision;
			firstBall = NULL;
		}
	}

	if (firstBall == NULL && firstWall == NULL)
	{
		r += movement;
		return;
	}
	if (firstWall != NULL)
	{
		CollideWithWall(*firstWall, displacementAlongMovement);
	}
	else
	{
		Vector temp = (movement.Direction() * displacementAlongMovement);
		r += (movement.Direction() * displacementAlongMovement);
		movement -= (movement.Direction() * displacementAlongMovement);
		Collide(firstBall);
	}
	//if (movement.Length() / (v**speed).Length() > 0.1)
	//{
	//	if (firstBall != NULL)
	//	{
	//		//if ((movement * (firstBall->r - r).Direction()) > 0)
	//		if (firstBall->Mass() < Mass())
	//		{
	//			firstBall->FirstCollision(balls, walls);
	//			FirstCollision(balls, walls);
	//		}
	//		else
	//		{
	//			FirstCollision(balls, walls);
	//			firstBall->FirstCollision(balls, walls);
	//		}
	//	}
	//	else
	//		FirstCollision(balls, walls);
	//}
	//if (firstBall == NULL)
	//	CollideWithWall();
	//else
	//{
	//	Vector changeInMovement = movement.Direction() * displacementAlongMovement;
	//	Vector afterMove = r + changeInMovement;

	//	//NOTE: ineffective, should work on this in the future
	//	//this is done to check if there is wall collision before the first ball collision
	//	if (afterMove.x<0 || afterMove.x>windowSizex || afterMove.y<0 || afterMove.y>windowSizey)
	//	{
	//		CollideWithWall();
	//		FirstCollision(balls);
	//		return;
	//	}
	//	Vector temp = (movement.Direction() * displacementAlongMovement);
	//	r += (movement.Direction() * displacementAlongMovement);
	//	if (isnan(r.x))
	//	{
	//		cout << "stahp";
	//	}
	//	movement -= (movement.Direction() * displacementAlongMovement);
	//	Collide(firstBall);
	//}


	//if (movement.Length()/(v**speed).Length() > 0.1)
	//{
	//	if (firstBall != NULL)
	//	{
	//		//if ((movement * (firstBall->r - r).Direction()) > 0)
	//		if (firstBall->Mass() < Mass())
	//		{
	//			firstBall->FirstCollision(balls);
	//			FirstCollision(balls);
	//		}
	//		else
	//		{
	//			FirstCollision(balls);
	//			firstBall->FirstCollision(balls);
	//		}
	//	}
	//	else
	//		FirstCollision(balls);
	//}
}

void Ball::Collide(Ball * ball)
{
	double initenergy = Energy() + ball->Energy();
	double vn1 = (ball->r - r).Direction() * v;
	double vn2 = (ball->r - r).Direction() * ball->v;
	if (vn2 > vn1) return;

	double _vn2 = 2 * (vn1 - vn2) / (Mass() + ball->Mass());
	double movementRatio = movement.Length() / (v.Length()**speed);
	if (isnan(movementRatio))
		movementRatio = 0;
	Vector n = ball->r - r;
	Vector nn = n.Direction();
	v = v - nn * _vn2*ball->Mass();
	movement = v*movementRatio**speed;

	ball->v = ball->v + nn *_vn2*Mass();
	double newEnergy = Energy() + ball->Energy();
	if (newEnergy > initenergy)
		cout << "stahp";
	else if (newEnergy < initenergy)
		cout << "stahp";
	ball->movement = ball->v*movementRatio**speed;

}

void Ball::CollideWithWall(Line wall, double movementToCollision)
{
	Vector curMovement = movement.Direction() * movementToCollision;
	r += curMovement;
	double movementRatio = movementToCollision / movement.Length();

	Vector vn = v - wall.length.Direction()*(wall.length.Direction() * v);
	v = v - vn * 2;

	movement = v**speed*(1 - movementRatio);
}

//void Ball::CollideWithWall()
//{
//	/*CollideWithWall(&(r.x), radius, &(v.x), windowSizex);
//	CollideWithWall(&(r.y), radius, &(v.y), windowSizey);*/
//	Vector tempr = r + movement;
//	double overlap = 0;
//
//	if ((tempr.x + radius) > windowSizex)
//	{
//		overlap = tempr.x - (windowSizex - radius);
//		v.x = -v.x;
//	}
//	else if ((tempr.x - radius) < 0)
//	{
//		overlap = radius - tempr.x;
//		v.x = -v.x;
//	}
//	r += (movement - (movement).Direction() * overlap / movement.Direction().x);
//	movement = v * (movement.Direction()*overlap**speed / movement.Direction().x).Length() / movement.Length();
//
//	overlap = 0;
//	if ((tempr.y + radius) > windowSizey)
//	{
//		overlap = tempr.y - (windowSizey - radius);
//		v.y = -v.y;
//	}
//	else if ((tempr.y - radius) < 0)
//	{
//		overlap = radius - tempr.y;
//		v.y = -v.y;
//	}
//	r -= movement.Direction()*overlap / movement.Direction().x;
//
//	movement = v * (movement.Direction()*overlap**speed / movement.Direction().x).Length() / movement.Length();
//}

int Ball::Mass()
{
	return pow(radius, 2);
}

double Ball::Energy()
{
	return Mass()*(pow(v.x, 2) + pow(v.y, 2)) / 2;
}


///<summary>Draws a circle (lengths are in pixels)</summary>
///<param name="x">x coordinate of center</param>
///<param name="y">y coordinate of center</param>
///<param name="r">radius</param>
///<param name="renderer">SDL renderer</param>
//void Ball::Render(double x, double y, double r, SDL_Renderer* renderer)
void Ball::Render(SDL_Renderer* renderer)
{
	c1 = 200 * (sin((clock() - start) / c1Amp) + 1) / 2;
	c2 = 200 * (sin((clock() - start) / c2Amp) + 1) / 2;
	c3 = 200 * (sin((clock() - start) / c3Amp) + 1) / 2;
	//alpha = sin((clock() - start));
	SDL_SetRenderDrawColor(renderer, c1, c2, c3, alpha);

	double tx = radius;
	double ty = 0;
	double tr = pow(radius, 2);

	while (ty < tx)
	{
		SDL_RenderDrawLine(renderer, r.x - tx, r.y + ty, r.x + tx, r.y + ty);
		SDL_RenderDrawLine(renderer, r.x - tx, r.y - ty, r.x + tx, r.y - ty);
		std::cout << ty << ' ' << tx << ' ' << tr << '\n';

		ty++;
		tr = pow(tx, 2) + pow(ty, 2);
		if ((sqrt(tr)) >= radius)
			tx--;
	}

	tx = 0;
	ty = radius;
	while (ty >= tx)
	{
		SDL_RenderDrawLine(renderer, r.x - tx, r.y + ty, r.x + tx, r.y + ty);
		SDL_RenderDrawLine(renderer, r.x - tx, r.y - ty, r.x + tx, r.y - ty);
		std::cout << ty << ' ' << tx << ' ' << tr << '\n';

		tx++;
		tr = pow(tx, 2) + pow(ty, 2);
		if ((sqrt(tr)) >= radius)
			ty--;
	}
}

void Ball::Collide(Ball* ball1, Ball* ball2)
{

}

bool Ball::CollideWithWall(double *x_i, double r, double *v_i, double windowSizei)
{
	double tempx = *v_i != 0 ? *x_i + r*(*v_i) / abs(*v_i) : *x_i;


	if (tempx >= windowSizei)
		*x_i -= 2 * (tempx - windowSizei);

	else if (tempx <= 0)
		*x_i -= 2 * tempx;

	else
		return false;


	*v_i = -*v_i;
	return true;
}
//void Ball::Collide(Ball* ball1, Ball* ball2)
//{
//	double distance = sqrt(abs(pow((ball1->x - ball2->x), 2) + pow((ball1->y - ball2->y), 2)));
//	double overlap = (ball1->radius + ball2->radius) - distance;
//	int t = 0;
//	double energy = ball1->Energy() + ball2->Energy();
//
//	if (overlap >= 0)
//	{
//		//balls shouldn't overlap
//		while (overlap > 0)
//		{
//			ball1->x -= ball1->v_x != 0 ? ball1->v_x / abs(ball1->v_x) : 0;
//			ball1->y -= ball1->v_y != 0 ? ball1->v_y / abs(ball1->v_y) : 0;
//			t++;
//			distance = sqrt(abs(pow(((ball1->x) - ball2->x), 2) + pow(((ball1->y) - ball2->y), 2)));
//			overlap = (ball1->radius + ball2->radius) - distance;
//		}
//
//		double phi = (ball1->x - ball2->x) != 0 ? atan((ball1->y - ball2->y) / (ball1->x - ball2->x)) : 1;
//
//		// convert to tangent and normal components of velocity
//		double vt1 = (ball1->v_x*sin(phi) + ball1->v_y*cos(phi));
//		double vn1 = (-ball1->v_x*cos(phi) + ball1->v_y*sin(phi));
//
//		double vt2 = (ball2->v_x*sin(phi) + ball2->v_y*cos(phi));
//		double vn2 = (-ball2->v_x*cos(phi) + ball2->v_y*sin(phi));
//
//		//calculate new normal velocities
//		double _vn2 = (ball1->Mass() * (2 * vn1 - vn2) + ball2->Mass()*vn2) / (ball1->Mass() + ball2->Mass());
//		double _vn1 = vn2 + _vn2 - vn1;
//
//		////balls collide, first ball flies off
//		//ball1->x -= ball1->v_x != 0 ? 2 * overlap*cos(phi)*(ball1->v_x) / abs(ball1->v_x) : 0;
//		//ball1->y -= ball1->v_y != 0 ? 2 * overlap*sin(phi)*(ball1->v_y) / abs(ball1->v_y) : 0;
//
//
//		//convert back to (x,y) components of velocity
//		ball1->v_x = (-_vn1*cos(phi) + vt1*sin(phi));
//		ball1->v_y = (_vn1*sin(phi) + vt1*cos(phi));
//
//		ball2->v_x = (-_vn2*cos(phi) + vt2*sin(phi));
//		ball2->v_y = (_vn2*sin(phi) + vt2*cos(phi));
//	}
//}
//bool Ball::CollideWithWall(double *x_i, double r, double *v_i, double windowSizei)
//{
//	double tempx = *v_i != 0 ? *x_i + r*(*v_i) / abs(*v_i) : *x_i;
//
//
//	if (tempx >= windowSizei)
//		*x_i -= 2 * (tempx - windowSizei);
//
//	else if (tempx <= 0)
//		*x_i -= 2 * tempx;
//
//	else
//		return false;
//
//
//	*v_i = -*v_i;
//	return true;
//}
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
