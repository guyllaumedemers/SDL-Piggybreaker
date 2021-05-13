#include "PointSystem.h"
#include "Game.h"

void PointSystem::add(int size)
{
	if (size <= 4) {
		Game::setHighScore(Game::getHighScore() + 15);
	}
	if (size == 5) {
		Game::setHighScore(Game::getHighScore() + 40);
	}
	else if (size > 5) {
		Game::setHighScore(Game::getHighScore() + 50);
	}
}
