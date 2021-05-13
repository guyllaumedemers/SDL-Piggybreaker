#pragma once
#include <list>
#include "Game.h"
#include "Ball.h"
#include "BallController.h"
#include "SDL.h"

static const double MIN_DEG = 70; // 360°
static const double MAX_DEG = 290; // 360°

class Cannon
{
private:
	const static int BALL_HEIGHT = 32;
	const static int QUEUE_SIZE = 2;
	double m_angle = 0;

	Vector m_cannonPos;
	Vector m_mousePos;
	Ball* m_pBall;
	Ball* m_pBallQueue[2];

	SDL_Renderer* m_rnd;
	TTF_Font* m_font;
	SDL_Texture* m_texture;

	SDL_Rect m_cannonRect;
	SDL_Rect m_ballRect;
	SDL_Rect m_ballQueueRect1;
	SDL_Rect m_ballQueueRect2;
public:
	Cannon(SDL_Renderer* rnd);
	~Cannon();
	void initBalls();
	void setTexture(SDL_Texture* texture);
	void draw(SDL_Renderer* rnd);
	void addRandomBall(int i);
	void getNextBall();
	void setMousePos(Vector mouse);
	void setAngleDeg();
	void shoot();
};