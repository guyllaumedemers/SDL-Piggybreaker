#pragma once
#include "../Libraries/lib_Point/Point.h"
#include "Globals.h"
#include "BallColor.h"
#include <string>
using namespace std;

class Ball
{
private:
	double m_positionX;
	double m_positionY;
	double m_radius;
	double m_angle;	
	Color m_ballColor = { 0,0,0,0 };
	SDL_Texture* m_ballTexture;
public:
	Ball();
	Ball(double m_positionX, double m_positionY);
	double getPosX();
	void setPosX(double posX);
	double getPosY();
	void setPosY(double posY);
	double getAngle();
	void setAngle(double angle);
	double getRadius();
	Color getColor();
	void setColor(BallColor::ballColors ballColor);
	SDL_Texture* getTexture();
	void setTexture(SDL_Texture* ballTexture);
	void setReboundAngle(double angle);
};