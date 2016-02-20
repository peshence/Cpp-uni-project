#include "Main.h"
#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "Ball.h"
#include <list>
#include <iostream>

using namespace std;

const int windowWidth = 640;
const int windowHeight = 480;
const int maxBallSize = 40;
const int maxNumberOfBalls = 20;


SDL_Window* window = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;

list<Ball> balls;
list<Line> walls;
bool quit;
double speed = 5;
bool pause = false;


int main(int argc, char* args[])
{
	Setup();
	Vector northWest = Vector(0, 0);
	Vector southEast = Vector(windowWidth, windowHeight);
	walls.push_back(Line(Vector(0, windowWidth), northWest));
	walls.push_back(Line(Vector(windowHeight, 0), southEast));
	walls.push_back(Line(Vector(windowHeight, 0), northWest));
	walls.push_back(Line(Vector(0, windowWidth), southEast));
	Ball ball1 = Ball(11, 21, 10, windowWidth, windowHeight, 5.0 / 100, 0.0 / 100, &speed);
	Ball ball2 = Ball(619, 21, 20, windowWidth, windowHeight, 5.0 / 100, 0.0 / 100, &speed);
	Ball ball3 = Ball(320, 29, 15, windowWidth, windowHeight, 0.0 / 100, 10.0 / 100, &speed);
	balls.push_back(ball1);
	balls.push_back(ball2);
	balls.push_back(ball3);
	double energy = 0;
	for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
	{
		energy += (*ball).Energy();
	}

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_MOUSEWHEEL:
				speed += e.wheel.y;
				if (speed < 1)
					speed = 1;
				if (speed > 20)
					speed = 20;
				break;
			case SDL_MOUSEBUTTONDOWN:
				pause = !pause;
			}
		}


		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		DrawBalls();

		if (!pause)
			MoveBalls();
		double energy2 = 0;
		for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
		{
			energy2 += (*ball).Energy();
		}
		if (energy2 > energy + 1)
			cout << "Energy accumulation";
	}

	SDL_Quit();
	return 0;
}

void Setup()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_SHOWN, &window, &renderer);
	quit = false;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void DrawBalls()
{
	for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
	{
		(*ball).Render(renderer);
	}
	SDL_RenderPresent(renderer);
}

void MoveBalls()
{
	for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
	{
		(*ball).Move(&balls, &walls);
		/*for (list<Ball>::iterator ball2 = ball; ball2 != balls.end(); ball2++)
		{
			if (ball != ball2)
				Ball::Collide(&(*ball), &(*ball2));
		}*/
	}
	//for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
	//{
	//	(*ball).FirstCollision(&balls);
	//	/*for (list<Ball>::iterator ball2 = ball; ball2 != balls.end(); ball2++)
	//	{
	//	if (ball != ball2)
	//	Ball::Collide(&(*ball), &(*ball2));
	//	}*/
	//}
}

int move(int xi, int *vi, int rectSizei, int windowSizei)
{
	xi += *vi;
	int temp = xi + rectSizei*((*vi) + abs(*vi)) / (2 * abs(*vi));
	if (temp >= windowSizei || temp <= 0)
		*vi = -*vi;
	return xi;
}

