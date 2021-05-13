#include "..\Libraries\lib_Point\Point.h"
#include "Ball.h"
#include <iostream>
#include "BallColor.h"
using namespace std;

Ball::Ball()
{
	this->m_positionX = 0;
	this->m_positionY = 0;
	this->m_radius = 16;
	this->m_angle = 0;
	this->m_ballTexture = NULL;
}

Ball::Ball(double m_positionX, double m_positionY)
{
	this->m_positionX = m_positionX;
	this->m_positionY = m_positionY;
	this->m_radius = 16;
	this->m_angle = 0;
	this->m_ballTexture = NULL;
}

void Ball::setPosX(double posX)
{
	this->m_positionX = posX;
}

void Ball::setPosY(double posY)
{
	this->m_positionY = posY;
}

double Ball::getPosX()
{
	return this->m_positionX;
}

double Ball::getPosY()
{
	return this->m_positionY;
}

double Ball::getRadius()
{
	return this->m_radius;
}

void Ball::setColor(BallColor::ballColors ballColor)
{
	this->m_ballColor = BallColor::getColor(ballColor);
}

Color Ball::getColor()
{
	return this->m_ballColor;
}

void Ball::setTexture(SDL_Texture* ballTexture)
{
	this->m_ballTexture = ballTexture;
}

SDL_Texture* Ball::getTexture()
{
	return this->m_ballTexture;
}

void Ball::setReboundAngle(double m_angle)
{
	this->m_angle = m_angle;
}

double Ball::getAngle()
{
	return this->m_angle;
}

void Ball::setAngle(double angle)
{
	this->m_angle = angle;
}
