#pragma once
#include "../Libraries/lib_Point/Point.h"

class BallColor
{
public:
	enum class ballColors {
		RED = 0,
		GREEN,
		BLUE,
		YELLOW,
		ORANGE,
		PINK,
		AQUA
	};

	static const int NB_COLORS = 7;

	static Color getColor(ballColors color);
};

