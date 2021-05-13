#include "Cannon.h"
#include "SDL.h"
#include "Globals.h"
#include "BallColor.h"

#include <iostream>
#include <time.h>

using namespace std;

/// <summary>
/// Constructor
/// </summary>
/// <param name="gameDir"></param>
Cannon::Cannon(SDL_Renderer* rnd)
{
	m_rnd = rnd;
	//m_font = TTF_OpenFont((gameDir + "Chilanka-Regular.ttf").c_str(), 64);
	
	m_pBall = nullptr;
	m_pBallQueue[0] = nullptr;
	m_pBallQueue[1] = nullptr;
	
	m_texture = nullptr;
	m_cannonPos = { APP_WIDTH / 2, 660 - (32 * 1.5) };
	m_cannonRect = { (APP_WIDTH / 2) - (85 / 2), (660 - 112), 85, 119 }; // {x, y, w, h}
	m_ballRect = { (APP_WIDTH / 2) - 32 / 2 , 596, 32, 32 };
	m_ballQueueRect1 = { 420, 630, 32, 32 };
	m_ballQueueRect2 = { 380, 630, 32, 32 };
}

/// <summary>
/// Destructor
/// </summary>
Cannon::~Cannon() {
	delete m_pBallQueue[0];
	delete m_pBallQueue[1];
}

/// <summary>
/// initialize the main ball and queue
/// </summary>
void Cannon::initBalls()
{
	delete m_pBall;
	m_pBall = nullptr;

	for (int i = 0; i < QUEUE_SIZE; i++) {
		delete m_pBallQueue[i];
		m_pBallQueue[i] = nullptr;
		addRandomBall(i);
	}

	getNextBall();
}

/// <summary>
/// Initialize the cannon texture
/// </summary>
/// <param name="texture"></param>
void Cannon::setTexture(SDL_Texture* texture)
{
	m_texture = texture;
}

/// <summary>
/// Draws cannon and main ball textures with rotation and the balls in the queue
/// </summary>
/// <param name="rnd"></param>
void Cannon::draw(SDL_Renderer* rnd)
{
	setAngleDeg();

	SDL_RenderCopyEx(rnd, m_texture, NULL, &m_cannonRect, m_angle, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(rnd, m_pBall->getTexture(), NULL, &m_ballRect, m_angle, NULL, SDL_FLIP_NONE);

	SDL_RenderCopy(rnd, m_pBallQueue[0]->getTexture(), NULL, &m_ballQueueRect1);
	SDL_RenderCopy(rnd, m_pBallQueue[1]->getTexture(), NULL, &m_ballQueueRect2);

#ifdef _DEBUG
	/**********************************************************************************************
	* Debug Display
	*
	**********************************************************************************************/
	// Display rectangles, squares and lines
	//m_cannonPos.draw(rnd, Color(0, 0, 0, 0), 2);
	//SDL_RenderDrawRect(rnd, &m_CannonRect);
	//SDL_RenderDrawRect(rnd, &m_BallRect);
	//SDL_RenderDrawRect(rnd, &m_BallQueueRect1);
	//SDL_RenderDrawRect(rnd, &m_BallQueueRect2);
	//SDL_RenderDrawLine(rnd, (int)PLAY_AREA_MIN.x, (int)PLAY_AREA_MAX.y, (int)PLAY_AREA_MAX.x, (int)PLAY_AREA_MAX.y);
	//
	//// Display angle on screen
	//SDL_Surface* surfaceMessage = TTF_RenderText_Solid(m_font, to_string(m_angle).c_str(), { 255, 255, 255 });
	//SDL_Texture* Message = SDL_CreateTextureFromSurface(rnd, surfaceMessage);
	//SDL_Rect Message_rect = { APP_WIDTH /2 , APP_HEIGHT - 64, 64 * 1.5, 64 };
	//SDL_RenderCopy(rnd, Message, NULL, &Message_rect);
	//SDL_FreeSurface(surfaceMessage);
	//SDL_DestroyTexture(Message);
	/**********************************************************************************************/
#endif // DEBUG
}

/// <summary>
/// Generates a random ball and adds it to queue
/// </summary>
void Cannon::addRandomBall(int i)
{
	Ball* ball = new Ball();
	int r = rand() % BallColor::NB_COLORS;
	ball->setColor((BallColor::ballColors)r);
	ball->setPosX(m_cannonPos.x);
	ball->setPosY(m_cannonPos.y);
	ball->setTexture(BallController::getTexture(r));
	ball->setAngle(0);
	m_pBallQueue[i] = ball;
}

/// <summary>
/// Sets the front ball in the queue as main ball
/// </summary>
void Cannon::getNextBall()
{
	m_pBall = nullptr;
	m_pBall = m_pBallQueue[0];
	m_pBallQueue[0] = nullptr;
	m_pBallQueue[0] = m_pBallQueue[1];
	m_pBallQueue[1] = nullptr;
	
	addRandomBall(1);
}

/// <summary>
/// Sets the current mouse position to member variable
/// </summary>
/// <param name="mouse"></param>
void Cannon::setMousePos(Vector mouse)
{
	m_mousePos = mouse;
}

/// <summary>
/// Finds the current angle to mouse position in degrees
/// </summary>
/// <returns></returns>
void Cannon::setAngleDeg()
{
	double angle = -90 + (atan2(m_cannonPos.y - m_mousePos.y, m_cannonPos.x - m_mousePos.x) * 180.00) / M_PI;
	angle = angle >= 0 ? angle : 360 + angle; // turn degree to 360°
	
	// right
	if (angle >= 0 && angle <= 180) { // quadrant +, +
		if (angle > MIN_DEG) {
			angle = MIN_DEG;
		}
	}// left
	else if (angle > 180 && angle <= 360) { // quadrand -, +
		if (angle < MAX_DEG) {
			angle = MAX_DEG;
		}		
	}

	m_angle = angle;
}

/// <summary>
/// Sends the main ball to ball controller for shooting mechanism if mouse position in play area.
/// </summary>
void Cannon::shoot()
{
	if (m_mousePos.x > PLAY_AREA_MIN.x && m_mousePos.x < PLAY_AREA_MAX.x &&
		m_mousePos.y > PLAY_AREA_MIN.y && m_mousePos.y < PLAY_AREA_MAX.y) {

		if(!BallController::isBallInPlay()){
			m_pBall->setAngle(m_angle);
			BallController::setMovingBall(m_pBall);
			getNextBall();
		}
	}
}
