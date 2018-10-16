#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir) {
}

StudentWorld::~StudentWorld() {
}

int StudentWorld::init()
{
	p_nachenblaster = new NachenBlaster;
	p_nachenblaster->Actor::setCurrentWorld(this);
	p_actors.clear();
	for (int i = 0; i < 30; i++) {
		p_actors.push_back(new Star);
	}
	targetShips = 6 + (4 * getLevel());
	shipsDestroyed = 0;
	shipsOnScreen = 0;
	maxShips = (int)(4 + (.5 * getLevel()));
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	p_nachenblaster->doSomething();
	if (!p_nachenblaster->isAlive()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (shipsDestroyed >= targetShips) {
		return GWSTATUS_FINISHED_LEVEL;
	}
	vector<Actor*>::iterator it;
	it = p_actors.begin();
	while (it != p_actors.end()) {
		if ((*it)->isAlive())
			(*it)->doSomething();
		it++;
	}
	if (shipsOnScreen < min(maxShips, (targetShips - shipsDestroyed))) {
		// Deciding chance
		int S1 = 60, S2 = 20 + (getLevel() + 5), S3 = 5 + (getLevel() * 10);
		int S = S1 + S2 + S3;
		int randSeed = rand() % S;
		if (randSeed < S1) {
			// add smallgon
			p_actors.push_back(new Smallgon(VIEW_WIDTH - 1, randNum(0, VIEW_HEIGHT - 1), 5 * (1 + (getLevel() - 1)*.1)));
			p_actors.back()->setCurrentWorld(this);
			shipsOnScreen++;
		}
		if (randSeed >= S1 && randSeed < (S1 + S2)) {
			// add smoregon
			p_actors.push_back(new Smoregon(VIEW_WIDTH - 1, randNum(0, VIEW_HEIGHT - 1), 5 * (1 + (getLevel() - 1)*.1)));
			p_actors.back()->setCurrentWorld(this);
			shipsOnScreen++;
		}
		if (randSeed >= (S1 + S2) && randSeed < (S1 + S2 + S3)) {
			// add snagglegon
			p_actors.push_back(new Snagglegon(VIEW_WIDTH - 1, randNum(0, VIEW_HEIGHT - 1), 10 * (1 + (getLevel() - 1)*.1)));
			p_actors.back()->setCurrentWorld(this);
			shipsOnScreen++;
		}
	}
	if (rand() % 16 == 1) // 0 to 15
		p_actors.push_back(new Star(VIEW_WIDTH - 1, (int)randNum(0, VIEW_HEIGHT)));
	removeDeadGameObjects();
	updateDisplayText();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete p_nachenblaster;
	vector<Actor*>::iterator it;
	it = p_actors.begin();
	while (it != p_actors.end()) {
		delete *it;
		it++;
	}
	p_actors.clear();
}

void StudentWorld::removeDeadGameObjects() {
	vector<Actor*>::iterator it;
	it = p_actors.begin();
	while (it != p_actors.end()) {
		if (!(*it)->isAlive()) {
			if ((*it)->isAlienShip())
				shipsOnScreen--;
			delete *it;
			it = p_actors.erase(it);
		}
		else
			it++;
	}
	return;
}

void StudentWorld::updateDisplayText() {
	ostringstream display;
	display << "Lives: " << getLives() << " Health: " << ((p_nachenblaster->getHealth()) / 50) * 100 << "% Score: " << getScore() <<
		" Level: " << getLevel() << " Cabbages: " << p_nachenblaster->getFirePower() << "% Torpedoes: " << p_nachenblaster->getTorpedo();
	setGameStatText(display.str());
}

void StudentWorld::shootCabbage() {
	p_actors.push_back(new Cabbage(p_nachenblaster->getX() + 12, p_nachenblaster->getY()));
	p_actors.back()->setCurrentWorld(this);
	return;
}

void StudentWorld::shootTurnip(double x, double y) {
	p_actors.push_back(new Turnip(x - 14, y));
	p_actors.back()->setCurrentWorld(this);
	return;
}

void StudentWorld::shootTorpedo(double x, double y, bool fromNB) {
	if (fromNB) {
		p_actors.push_back(new FriendlyTorpedo(p_nachenblaster->getX() + 12, p_nachenblaster->getY()));
		p_actors.back()->setCurrentWorld(this);
	}
	else {
		p_actors.push_back(new Torpedo(x - 14, y));
		p_actors.back()->setCurrentWorld(this);
	}
	return;
}

void StudentWorld::dropExtraLifeGoodie(double x, double y) {
	p_actors.push_back(new ExtraLifeGoodie(x, y));
	p_actors.back()->setCurrentWorld(this);
}

void StudentWorld::dropRepairGoodie(double x, double y) {
	p_actors.push_back(new RepairGoodie(x, y));
	p_actors.back()->setCurrentWorld(this);
}

void StudentWorld::dropTorpedoGoodie(double x, double y) {
	p_actors.push_back(new TorpedoGoodie(x, y));
	p_actors.back()->setCurrentWorld(this);
}

void StudentWorld::explode(double x, double y) {
	p_actors.push_back(new Explosion(x, y));
	p_actors.back()->setCurrentWorld(this);
}

Actor* StudentWorld::getNachenBlaster() {
	return p_nachenblaster;
}

std::vector<Actor*> StudentWorld::getActors() {
	return p_actors;
}

void StudentWorld::increaseShipsDestoryed() {
	shipsDestroyed++;
}