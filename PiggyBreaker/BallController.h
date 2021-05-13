#pragma once
#include "Ball.h"
#include "Globals.h"
#include <SDL_mixer.h>
#include <vector>
#include <ctime>
#include <iostream>
class BallController
{
private:
	static vector<Ball*>* m_ballsVector;
	static vector<SDL_Texture*> m_vectorTextures;
	static Ball* m_movingBall;
	static double m_minPosX;
	static double m_maxPosX;
	static double m_minPosY;
	static double m_maxPosY;
	static double m_currentPosX;
	static double m_currentPosY;
	static bool m_stop;
	static string m_textureNames[7];	
	static const int m_speed = 20;
	static double checkLostPosition;
public:
	~BallController();
	static vector<SDL_Texture*> getVectorTextures();
	static void generateBalls(SDL_Renderer* renderer, int shape);
	static vector<Ball*>* getVectorBalls();
	static void setVectorBalls(SDL_Texture* textureToAdd);
	static SDL_Texture* getTexture(int texture);
	static string getTextureName(int textureName);	
	static void resetCanvas();
	static void setMovingBall(Ball* pBall);
	static double DegToRad(double angle);
	static bool update(SDL_Renderer* rnd);
	static bool isBallInPlay();
	static bool hasLost();
};