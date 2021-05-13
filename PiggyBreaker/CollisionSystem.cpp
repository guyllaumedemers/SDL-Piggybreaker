#include "CollisionSystem.h"

Ball* CollisionSystem::m_ball;
bool CollisionSystem::isRebound = false;
std::vector<Ball*> temp;
std::vector<Ball*>* CollisionSystem::m_ballsVector = BallController::getVectorBalls();

bool CollisionSystem::isTouching(Ball* ball1, Ball* ball2)
{
	bool retour = false;
	double deltaX, deltaY;
	deltaX = ball1->getPosX() - ball2->getPosX();
	deltaY = ball1->getPosY() - ball2->getPosY();
	deltaX = pow(deltaX, 2);
	deltaY = pow(deltaY, 2);
	if ((deltaX + deltaY) <= pow(ball1->getRadius() * 2.4, 2)) {
		retour = true;
	}
	return retour;
}

void CollisionSystem::snapTo(Ball* m_ball, Ball* m_ball2)
{
	int overlap = 0;

	if (!isRebound) {
		if (m_ball->getAngle() <= 90 || m_ball->getAngle() >= 270) {
			m_ball->setPosX(m_ball2->getPosX() + m_ball2->getRadius());
			m_ball->setPosY(m_ball2->getPosY() + 2 * m_ball2->getRadius());
		}
	}
	else {
		if (m_ball->getAngle() >= 270) {
			m_ball->setPosX(m_ball2->getPosX() + 2 * m_ball2->getRadius());
			m_ball->setPosY(m_ball2->getPosY());
		}
		else {
			m_ball->setPosX(m_ball2->getPosX() - 2 * m_ball2->getRadius());
			m_ball->setPosY(m_ball2->getPosY());
		}
	}
	m_ball = nullptr;	
	CollisionSystem::isRebound = false;
}

bool CollisionSystem::isColor(Ball* ball1, Ball* ball2)
{
	bool retour = false;
	double deltaRed, deltaGreen, deltaBlue, distance;
	deltaRed = ball1->getColor().r - ball2->getColor().r;
	deltaGreen = ball1->getColor().g - ball2->getColor().g;
	deltaBlue = ball1->getColor().b - ball2->getColor().b;
	distance = pow(deltaRed, 2) + pow(deltaGreen, 2) + pow(deltaBlue, 2);
	if (distance == 0) {
		retour = true;
	}
	return retour;
}

std::vector<Ball*>* CollisionSystem::findCluster(Ball* ball)
{
	std::vector<Ball*>* ballsGridVector = m_ballsVector;
	std::vector<Ball*>* toProcess = new std::vector<Ball*>();
	std::vector<Ball*>* processed = new std::vector<Ball*>();
	// on commence par ajouter la balle au vector toProcess puisque c'est la premiere verification a faire
	toProcess->push_back(ball);
	while (toProcess->size() != 0) {
		// boucle sur l'ensemble des balles de la grid
		// lorsqu'une balle est ajouter au vecteur toProcess, il est necessaire de l'enlever du gridVector pour garder le meme nombre de balles
		// en jeu
		for (size_t i = 0; i < ballsGridVector->size(); i++) {
			// si la balle a l'index zero et la balle à l'index i du grid se touche && on la meme couleur
			// on ajoute la balle a l'index i du grid dans le vecteur toProcess puisqu'il faudra ensuite verifier ses voisins
			// ne pas oublier qu'une fois ajouter au vecteur toProcess, il est necessaire de supprimer l'index du gridVector pour
			// eviter de processer 2x la meme balle
			if (isTouching(toProcess->at(0), ballsGridVector->at(i)) && isColor(toProcess->at(0), ballsGridVector->at(i))) {
				toProcess->push_back(ballsGridVector->at(i));
				ballsGridVector->erase(ballsGridVector->begin() + i);
			}
		}
		processed->push_back(toProcess->at(0));
		toProcess->erase(toProcess->begin());
	}
	return processed;

	delete toProcess;
	delete processed;	
}

bool CollisionSystem::snapToRoof(Ball* ball)
{
	bool retour = false;
	if (ball->getPosY() <= CANVAS_MIN.y + ball->getRadius()) {
		retour = true;
	}
	return retour;
}

std::vector<Ball*>* CollisionSystem::findFloatingCluster()
{
	std::vector<Ball*>* ballsGridVector = m_ballsVector;

	for (size_t i = 0; i < ballsGridVector->size(); i++) {
		temp.push_back(ballsGridVector->at(i));
	}
	std::vector<Ball*>* stick = new std::vector<Ball*>();
	std::vector<Ball*>* processedStick = new std::vector<Ball*>();
	std::vector<Ball*>* fall = new std::vector<Ball*>();

	for (size_t i = 0; i < temp.size(); i++) {
		if (snapToRoof(temp[i])) {
			stick->push_back(temp[i]);
			temp.erase(temp.begin() + i);
		}
	}
	while (stick->size() != 0) {
		for (size_t i = 0; i < temp.size(); i++) {
			if (isTouching(stick->at(0), temp[i])) {
				stick->push_back(temp[i]);
				temp.erase(temp.begin() + i);
			}
		}
		processedStick->push_back(stick->at(0));
		stick->erase(stick->begin());
	}
	for (size_t k = 0; k < temp.size(); k++) {
		fall->push_back(temp[k]);
	}
	return fall;

	delete stick;
	delete processedStick;
	delete fall;
}

void CollisionSystem::remove(std::vector<Ball*> vectorRemove)
{
	for (size_t i = 0; i < vectorRemove.size(); i++) {
		delete vectorRemove[i];
	}
	vectorRemove.clear();
}
