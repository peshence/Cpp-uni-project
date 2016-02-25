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
		/*if ((*wall - r - movement).Length() < radius)
			cout << "break";*/
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
}

void Ball::Collide(Ball * ball)
{
	/*double initenergy = Energy() + ball->Energy();*/
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
	/*double newEnergy = Energy() + ball->Energy();
	if (newEnergy > initenergy)
		cout << "stahp";
	else if (newEnergy < initenergy)
		cout << "stahp";*/
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
		//std::cout << ty << ' ' << tx << ' ' << tr << '\n';

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
		//std::cout << ty << ' ' << tx << ' ' << tr << '\n';

		tx++;
		tr = pow(tx, 2) + pow(ty, 2);
		if ((sqrt(tr)) >= radius)
			ty--;
	}
}
