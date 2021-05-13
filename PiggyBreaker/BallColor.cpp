#include "BallColor.h"

Color BallColor::getColor(ballColors color)
{
	Color ballColor{ 0,0,0,0 };
	switch (color) {
	case BallColor::ballColors::RED:
		ballColor = Color(255, 0, 0, SDL_ALPHA_OPAQUE);
		break;
	case BallColor::ballColors::GREEN:
		ballColor = Color(0, 255, 0, SDL_ALPHA_OPAQUE);
		break;
	case BallColor::ballColors::BLUE:
		ballColor = Color(0, 0, 255, SDL_ALPHA_OPAQUE);
		break;
	case BallColor::ballColors::YELLOW:
		ballColor = Color(255, 255, 0, SDL_ALPHA_OPAQUE);
		break;
	case BallColor::ballColors::ORANGE:
		ballColor = Color(255, 150, 0, SDL_ALPHA_OPAQUE);
		break;
	case BallColor::ballColors::PINK:
		ballColor = Color(255, 0, 255, SDL_ALPHA_OPAQUE);
		break;
	case BallColor::ballColors::AQUA:
		ballColor = Color(0, 255, 255, SDL_ALPHA_OPAQUE);
		break;
	}
	return ballColor;
}
