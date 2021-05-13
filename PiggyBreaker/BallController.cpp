#include "BallController.h"
#include "Globals.h"
#include <SDL_image.h>
#include "CollisionSystem.h"
#include "PointSystem.h"
#include <SDL_mixer.h>

vector<Ball*>* BallController::m_ballsVector = new vector<Ball*>();
vector<SDL_Texture*> BallController::m_vectorTextures;
string BallController::m_textureNames[7] = { "bubbles_red.png", "bubbles_green.png", "bubbles_blue.png", "bubbles_yellow.png", "bubbles_orange.png", "bubbles_purple.png", "bubbles_cyan.png" };
double BallController::m_minPosX;
double BallController::m_maxPosX;
double BallController::m_minPosY;
double BallController::m_maxPosY;
double BallController::m_currentPosX;
double BallController::m_currentPosY;
bool BallController::m_stop = false;
double BallController::checkLostPosition = 0;
Ball* BallController::m_movingBall = nullptr;

BallController::~BallController()
{
	delete m_ballsVector;
}

vector<SDL_Texture*> BallController::getVectorTextures()
{
	return m_vectorTextures;
}

void BallController::generateBalls(SDL_Renderer* renderer, int shape)
{
	int randomShape = shape;

	switch (randomShape) {
	case 1:
		//Screen fill
		m_minPosX = CANVAS_MIN.x;
		m_minPosY = CANVAS_MIN.y + 16;
		m_currentPosX = m_minPosX;
		m_currentPosY = m_minPosY;
		break;
	case 2:
		//Big Triangle
		m_minPosX = CANVAS_MIN.x;
		m_minPosY = CANVAS_MIN.y + 16;
		m_maxPosX = CANVAS_MAX.x;
		m_currentPosX = m_minPosX;
		m_currentPosY = m_minPosY;
		break;
	case 3:
		// Small Triangle
		m_minPosX = CANVAS_MIN.x * 2;
		m_minPosY = CANVAS_MIN.y + 16;
		m_maxPosX = CANVAS_MAX.x / 1.2;
		m_currentPosX = m_minPosX;
		m_currentPosY = m_minPosY;
		break;
	case 4:
		// Small Triangle Inverted
		m_minPosX = CANVAS_MIN.x * 2.8;
		m_minPosY = CANVAS_MIN.y + 16;
		m_maxPosX = CANVAS_MAX.x / 1.5;
		m_currentPosX = m_minPosX;
		m_currentPosY = m_minPosY;
		break;
	case 5:
		//Big Triangle Inverted
		m_minPosX = CANVAS_MIN.x * 2;
		m_minPosY = CANVAS_MIN.y + 16;
		m_maxPosX = CANVAS_MAX.x / 1.2;
		m_currentPosX = m_minPosX;
		m_currentPosY = m_minPosY;
		break;
	default:
		break;
	}

	while (!m_stop) {
		Ball* ball = new Ball();
		//Generate a number between 0 and 6 (This will decide of the ball color according to the enum
		int colorIndex = rand() % BallColor::NB_COLORS;

		ball->setPosX(m_currentPosX);
		ball->setPosY(m_currentPosY);
		//Sets the color by casting int to Color
		ball->setColor((BallColor::ballColors)colorIndex);
		//Sets the texture color
		ball->setTexture(m_vectorTextures[colorIndex]);
		m_ballsVector->push_back(ball);

		m_currentPosX += (ball->getRadius() * 2);

		switch (randomShape) {
		case 1:
			//This fills entire screen with ball lines
			if (m_currentPosX >= m_maxPosX) {
				if (m_minPosX == CANVAS_MIN.x) {
					m_minPosX = CANVAS_MIN.x + 16;
				}
				else {
					m_minPosX = CANVAS_MIN.x;
				}
				m_currentPosX = m_minPosX;
				m_currentPosY += (ball->getRadius() * 2);
			}
			break;
		case 2:
			//Big Triangle
			if (m_currentPosX >= m_maxPosX) {
				m_minPosX += ball->getRadius();
				m_maxPosX -= ball->getRadius();
				m_currentPosX = m_minPosX;
				m_currentPosY += (ball->getRadius() * 2);
			}
			break;
		case 3:
			//Small Triangle
			if (m_currentPosX >= m_maxPosX) {
				m_minPosX += ball->getRadius();
				m_maxPosX -= ball->getRadius();
				m_currentPosX = m_minPosX;
				m_currentPosY += (ball->getRadius() * 2);
			}			
			break;
		case 4:
			//Small Triangle
			if (m_currentPosX >= m_maxPosX) {
				m_minPosX -= ball->getRadius();
				m_maxPosX += ball->getRadius();
				m_currentPosX = m_minPosX;
				m_currentPosY += (ball->getRadius() * 2);
			}
			break;
		case 5:
			//Big Triangle Inverted
			if (m_currentPosX >= m_maxPosX) {
				m_minPosX -= ball->getRadius();
				m_maxPosX += ball->getRadius();
				m_currentPosX = m_minPosX;
				m_currentPosY += (ball->getRadius() * 2);
			}
			break;
		default:
			break;
		}

		if (m_currentPosY >= CANVAS_MAX.y) {
			m_stop = true;
		}
	}
}

vector<Ball*>* BallController::getVectorBalls()
{
	return m_ballsVector;
}

string BallController::getTextureName(int textureName)
{
	return m_textureNames[textureName];
}

void BallController::setVectorBalls(SDL_Texture* textureToAdd)
{
	m_vectorTextures.push_back(textureToAdd);
}

SDL_Texture* BallController::getTexture(int i)
{
	return m_vectorTextures[i];
}

void BallController::resetCanvas()
{
	m_ballsVector->clear();
	m_currentPosX = CANVAS_MIN.x;
	m_currentPosY = CANVAS_MIN.y;
	m_minPosX = 16;
	m_maxPosX = CANVAS_MAX.x;
	checkLostPosition = 0;
	m_stop = false;
}

void BallController::setMovingBall(Ball* pBall)
{
	m_movingBall = pBall;
}

double BallController::DegToRad(double angle) {
	return angle * (M_PI / 180);
}

bool BallController::update(SDL_Renderer* rnd) {
	bool playSound = false;
	bool snapToRoof = false;

	if (m_movingBall != NULL) {

		double rAngle = DegToRad(m_movingBall->getAngle() - 90);
		m_movingBall->setPosX(m_movingBall->getPosX() + m_speed * cos(rAngle));
		m_movingBall->setPosY(m_movingBall->getPosY() + m_speed * sin(rAngle));

		SDL_Rect rectBall = { m_movingBall->getPosX() - 16, m_movingBall->getPosY() - 16, 32, 32 };
		SDL_RenderCopy(rnd, m_movingBall->getTexture(), NULL, &rectBall);

		
		if (m_movingBall->getPosX() <= CANVAS_MIN.x) {
			m_movingBall->setAngle(-m_movingBall->getAngle() + 360);
			m_movingBall->setPosX(CANVAS_MIN.x);
			CollisionSystem::isRebound = true;
		}
		else if (m_movingBall->getPosX() >= CANVAS_MAX.x) {
			m_movingBall->setAngle(-m_movingBall->getAngle() + 360);
			m_movingBall->setPosX(CANVAS_MAX.x);
			CollisionSystem::isRebound = true;
		}

		snapToRoof = CollisionSystem::snapToRoof(m_movingBall);

		if (snapToRoof) {			
			m_movingBall->setPosX(m_movingBall->getPosX());
			m_movingBall->setPosY(CANVAS_MIN.y + m_movingBall->getRadius());
			BallController::getVectorBalls()->push_back(m_movingBall);
			m_movingBall = nullptr;
		}

		if (!snapToRoof) {
			for (size_t i = 0; i < m_ballsVector->size(); i++) {
				if (CollisionSystem::isTouching(m_movingBall, m_ballsVector->at(i))) {
					m_movingBall->setPosX(m_movingBall->getPosX());
					m_movingBall->setPosY(m_movingBall->getPosY());

					CollisionSystem::snapTo(m_movingBall, m_ballsVector->at(i));

					std::vector<Ball*>* clusterFound = new std::vector<Ball*>();
					clusterFound = CollisionSystem::findCluster(m_movingBall);

					if (clusterFound->size() < 3) {
						for (size_t i = 0; i < clusterFound->size(); i++) {
							m_ballsVector->push_back(clusterFound->at(i));
						}
						for (size_t j = 0; j < clusterFound->size(); j++) {
							clusterFound->erase(clusterFound->begin() + j);
						}
						clusterFound->clear();
						
					}
					else if (clusterFound->size() >= 3) {
						PointSystem::add(clusterFound->size());

						for (size_t j = 0; j < clusterFound->size(); j++) {
							clusterFound->erase(clusterFound->begin() + j);
						}
						clusterFound->clear();
						playSound = true;
					}

					std::vector<Ball*>* floatingClusterFound = new std::vector<Ball*>();
					floatingClusterFound = CollisionSystem::findFloatingCluster();
					
					if (floatingClusterFound->size() != NULL) {
						for (size_t i = 0; i < floatingClusterFound->size(); i++) {
							for (size_t m = 0; m < m_ballsVector->size(); m++) {
								if (m_ballsVector->at(m) == floatingClusterFound->at(i)) {
									m_ballsVector->erase(m_ballsVector->begin() + m);
								}
							}
						}
						playSound = true;
						floatingClusterFound->clear();
					}
					
					checkLostPosition = m_movingBall->getPosY();
					m_movingBall = nullptr;
					
					//std::cout << "floating cluster size : " << floatingClusterFound->size() << std::endl;
					
					break;
					delete clusterFound;
					delete floatingClusterFound;
					
				}				
			}			
		}		
	}
	return playSound;
}

bool BallController::isBallInPlay()
{
	if (m_movingBall == NULL) { return false; } return true;
}

bool BallController::hasLost()
{
	bool hasLost = false;

	if (checkLostPosition >= CANVAS_MAX.y + 116) {
		hasLost = true;
	}
	return hasLost;
}
