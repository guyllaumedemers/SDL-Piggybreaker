#pragma once
#include "Cannon.h"
#include <vector>
class CollisionSystem
{
public:
	static bool isRebound;
	static Ball* m_ball;
	static std::vector<Ball*>* m_ballsVector;
	static bool isTouching(Ball* ball1, Ball* ball2);
	static void snapTo(Ball* m_ball, Ball* m_ball2);
	static bool isColor(Ball* ball1, Ball* ball2);
	static void rebound();
	static std::vector<Ball*>* findCluster(Ball* ball);
	static bool snapToRoof(Ball* ball);
	static std::vector<Ball*>* findFloatingCluster();
	static void remove(std::vector<Ball*> vectorRemove);
};

