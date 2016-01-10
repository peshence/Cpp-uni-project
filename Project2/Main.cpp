#include "Main.h"
#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "Ball.h"
#include <list>

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
int c1;
int c1Amp;
int c2;
int c2Amp;
int c3;
int c3Amp;
int alpha;
bool quit;
double start;


int main(int argc, char* args[])
{	
	Setup();
	Ball ball1 = Ball(0, 0, 10, windowWidth, windowHeight, 5, 1);
	Ball ball2 = Ball(640, 0, 20, windowWidth, windowHeight, -5, 1);
	balls.push_back(ball1);
	balls.push_back(ball2);

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
		}

		DrawBalls();

		//makes sure drawn screen is visible to human eye and slows down movements
		SDL_Delay(10);

		MoveBalls();

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
	}

	SDL_Quit();
	return 0;
}

void Setup()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_SHOWN, &window, &renderer);
	c1 = rand() % 255;
	c1Amp = rand() % 250;
	c2 = rand() % 255;
	c2Amp = rand() % 250;
	c3 = rand() % 255;
	c3Amp = rand() % 250;
	alpha = rand() % 255;
	quit = false;
	start = clock();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void DrawBalls()
{
	for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
	{
		c1 = 255 * (sin((clock() - start) / c1Amp) + 1) / 2;
		c2 = 255 * (sin((clock() - start) / c2Amp) + 1) / 2;
		c3 = 255 * (sin((clock() - start) / c3Amp) + 1) / 2;
		alpha += sin((clock() - start) / 150);
		SDL_SetRenderDrawColor(renderer, c1, c2, c3, alpha);
		(*ball).Render(renderer);
		SDL_RenderPresent(renderer);
	}
}

void MoveBalls()
{
	for (list<Ball>::iterator ball = balls.begin(); ball != balls.end(); ball++)
	{
		(*ball).Move(&balls);
		/*for (list<Ball>::iterator ball2 = ball; ball2 != balls.end(); ball2++)
		{
			if (ball != ball2)
				Ball::Collide(&(*ball), &(*ball2));
		}*/
	}
}

int move(int xi, int *vi, int rectSizei, int windowSizei)
{
	xi += *vi;
	int temp = xi + rectSizei*((*vi) + abs(*vi)) / (2 * abs(*vi));
	if (temp >= windowSizei || temp <= 0)
		*vi = -*vi;
	return xi;
}

