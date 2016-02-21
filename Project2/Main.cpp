#include "Main.h"
#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "Ball.h"
#include <list>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int windowWidth = 640;
int windowHeight = 480;
int maxNumberOfBalls = 20;
const int maxBallSize = 30;


SDL_Window* window = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;

list<Ball> balls;
list<Line> walls;
bool quit;
double speed = 5;
bool pause = false;
bool newballspeed = false;
Ball newball = Ball(windowWidth / 2, windowHeight / 2, 5, windowWidth, windowHeight, 0, 0, &speed);


int main(int argc, char* args[])
{
	Setup();
	Vector northWest = Vector(0, 0);
	Vector southEast = Vector(windowWidth, windowHeight);
	walls.push_back(Line(Vector(0, windowWidth), northWest));
	walls.push_back(Line(Vector(windowHeight, 0), southEast));
	walls.push_back(Line(Vector(windowHeight, 0), northWest));
	walls.push_back(Line(Vector(0, windowWidth), southEast));
	balls.push_back(Ball(11, 21, 10, windowWidth, windowHeight, 5.0 / 100, 0.0 / 100, &speed));
	balls.push_back(Ball(619, 21, 20, windowWidth, windowHeight, 5.0 / 100, 0.0 / 100, &speed));
	balls.push_back(Ball(320, 21, 15, windowWidth, windowHeight, 0.0 / 100, 0.0 / 100, &speed));
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
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (newballspeed)
						newballspeed = false;
					else
						pause = !pause;
				}
				else if (pause && e.button.button == SDL_BUTTON_RIGHT)
				{
					if (newballspeed)
					{
						Ball newballp = balls.back();
						newballp.v = (Vector(e.button.x, e.button.y) - newballp.r) / 2000;
						balls.pop_back();
						balls.push_back(newballp);
						energy += newballp.Energy();
						newballspeed = false;
						break;
					}
					if (balls.size() >= maxNumberOfBalls)
					{
						cout << clock() << endl << "Sorry, you've reached the maximum number of balls!" << endl;
						break;
					}
					double start = clock();
					newball = Ball(e.button.x, e.button.y, 5, windowWidth, windowHeight, 0, 0, &speed);
					if (DrawNewBall())
					{
						SDL_PollEvent(&e);
						while (e.type != SDL_MOUSEBUTTONUP)
						{
							if ((clock() - start) > 100)
							{
								start = clock();
								newball.radius = newball.radius > maxBallSize ? maxBallSize : newball.radius + 1;
								if (!DrawNewBall()) break;
							}
							SDL_PollEvent(&e);
						}
						if (e.type == SDL_MOUSEBUTTONUP)
						{
							balls.push_back(newball);
							newballspeed = true;
						}
					}
				}
				break;
			}
		}




		if (!pause)
			MoveBalls();
		double energy2 = 0;
		for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
		{
			energy2 += (*ball).Energy();
		}
		if (energy2 > energy*1.1)
			cout << endl << "Energy accumulation" << endl;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		DrawBalls();
	}

	SDL_Quit();
	return 0;
}

void Setup()
{
	ifstream settings;
	settings.open("settings.txt");
	string temp;
	getline(settings, temp);
	windowWidth = stoi(temp);
	getline(settings, temp);
	windowHeight = stoi(temp);
	getline(settings, temp);
	maxNumberOfBalls = stoi(temp);
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
	}
}

bool DrawNewBall()
{
	for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
	{
		double displacement = (ball->r - newball.r).Length();
		if ((displacement - ball->radius - newball.radius) < 0)
		{
			cout << endl << clock() << " - Too close to another ball!" << endl;
			return false;
		}
	}

	for (list<Line>::iterator wall = walls.begin(); wall != walls.end(); wall++)
	{
		double distance = (*wall - newball.r).Length();
		if (distance < newball.radius)
		{
			cout << endl << clock() << " - Too close to wall!" << endl;
			return false;
		}
	}

	newball.Render(renderer);
	SDL_RenderPresent(renderer);
	return true;
}

