#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>
#include <vector>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

double randNum(double min, double max) {
	return ((double)rand() * (max - min)) / (double)RAND_MAX + min;
}

//ACTOR=====================================================================================
Actor::Actor(double x, double y, Direction dir, double size, unsigned int depth, int id) :
	GraphObject(id, x, y, dir, size, depth) {
}
Actor::~Actor() {}
bool Actor::isAlienShip() {
	return false;
}
void Actor::setCurrentWorld(StudentWorld* sw) {
	currentWorld = sw;
}
StudentWorld* Actor::getWorld() {
	return currentWorld;
}
bool Actor::isAlive() {
	return m_isAlive;
}
void Actor::setIsAlive(bool status) {
	m_isAlive = status;
}
double Actor::getHealth() {
	return -999;
}
int Actor::getPoints() {
 	return 0;
}
int Actor::getFirePower() {
	return 0;
}
int Actor::getTorpedo() {
	return 0;
}
void Actor::setHealth(double hp) {
	(void)hp;
}
void Actor::setTorpedo(int t) {
	(void)t;
}

//STAR======================================================================================
Star::Star() :
	Actor((int)randNum(0, VIEW_WIDTH), (int)randNum(0, VIEW_HEIGHT), 0, randNum(0.05, 0.5), 3, IID_STAR) {}
Star::Star(int x, int y) :
	Actor(x, y, 0, randNum(0.05, 0.5), 3, IID_STAR) {}
Star::~Star() {}
void Star::doSomething() {
	if (getX() > -1)
		moveTo(getX() - 1, getY());
	else
		setIsAlive(false);
}

//EXPLOSION======================================================================================
Explosion::Explosion(double x, double y) :
	Actor(x, y, 0, 1, 0, IID_EXPLOSION) {
	lifetime = 4;
}
Explosion::~Explosion() {}
void Explosion::doSomething() {
	setSize(getSize()*1.5);
	lifetime--;
	if (lifetime == 0)
		setIsAlive(false);
}

//NACHENBLASTER=============================================================================
NachenBlaster::NachenBlaster() :
	Actor(0, 128, 0, 1.0, 0, IID_NACHENBLASTER) {
	m_hp = 50;
	m_cabbagePoints = 30;
	m_torpedoes = 0;
}
NachenBlaster::~NachenBlaster() {}
void NachenBlaster::doSomething() {
	if (!isAlive())
		return;
	if (getHealth() <= 0) {
		setIsAlive(false);
		return;
	}
	int ch;
	if (getWorld()->getKey(ch)) {
		// user hit a key during this tick
		switch (ch) {
		case KEY_PRESS_UP:
			if (getY() <= VIEW_HEIGHT)
				moveTo(getX(), getY() + 6);
			break;
		case KEY_PRESS_DOWN:
			if (getY() >= 0)
				moveTo(getX(), getY() - 6);
			break;
		case KEY_PRESS_LEFT:
			if (getX() >= 0)
				moveTo(getX() - 6, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getX() <= VIEW_WIDTH)
				moveTo(getX() + 6, getY());
			break;
		case KEY_PRESS_SPACE:
			if (m_cabbagePoints >= 5) {
				m_cabbagePoints = m_cabbagePoints - 5;
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
				getWorld()->shootCabbage();
			}
			break;
		case KEY_PRESS_TAB:
			if (m_torpedoes > 0) {
				m_torpedoes--;
				getWorld()->playSound(SOUND_TORPEDO);
				getWorld()->shootTorpedo(getX(), getY(), true);
			}
			break;
		case KEY_PRESS_ESCAPE:
			break;
		default: break;
		}
	}
	if (m_cabbagePoints < 30)
		m_cabbagePoints++;
}
double NachenBlaster::getHealth() {
	return m_hp;
}
void NachenBlaster::setHealth(double hp) {
	m_hp = hp;
}
int NachenBlaster::getFirePower() {
	return (int)((m_cabbagePoints / 30.0) * 100);
}
void NachenBlaster::setTorpedo(int t) {
	m_torpedoes = t;
}
int NachenBlaster::getTorpedo() {
	return m_torpedoes;
}

//PROJECTILE===================================================================================
Projectile::Projectile(int dp, int sp, double x, double y, bool isRotating, bool fromNachenblaster, int id):
	Actor(x, y, 0, 0.5, 1, id) {
	m_speed = sp;
	m_dmgPoint = dp;
	m_isRotating = isRotating;
	m_fromNachenblaster = fromNachenblaster;
}
Projectile::~Projectile() {}
void Projectile::doSomething() {
	if (!isAlive())
		return;
	if ((getX() >= VIEW_WIDTH) || (getX() <= 0)) {
		setIsAlive(false);
		return;
	}
	// CHECK COLLISION
	collisionCheck();
	if (m_fromNachenblaster)
		moveTo(getX() + getSpeed(), getY());
	else
		moveTo(getX() - getSpeed(), getY());
	if (m_isRotating)
		setDirection(getDirection() + 20);
	// CHECK COLLISION AGAIN
	collisionCheck();
}
int Projectile::getSpeed() {
	return m_speed;
}
int Projectile::getDmgPoint() {
	return m_dmgPoint;
}

//PROJ_CABBAGE=====================================================================================
Cabbage::Cabbage(double x, double y) :
	Projectile (2, 8, x, y, true, true, IID_CABBAGE) {
}
Cabbage::~Cabbage() {}
void Cabbage::collisionCheck() {
	std::vector<Actor*> temp = getWorld()->getActors();
	for (int i = 0; i < (int)(temp.size()); i++) {
		if (temp[i]->isAlienShip()) {
			if (sqrt(((getX() - (temp[i]->getX()))*(getX() - (temp[i]->getX()))) +
				((getY() - (temp[i]->getY()))*(getY() - (temp[i]->getY())))) < 0.75 * (getRadius() + (temp[i]->getRadius()))) {
				// COLLIDED with alien ship
				temp[i]->setHealth(temp[i]->getHealth() - getDmgPoint());
				getWorld()->playSound(SOUND_BLAST);
				setIsAlive(false);	
			}
		}
	}
}

//PROJ_TURNIP======================================================================================
Turnip::Turnip(double x, double y) :
	Projectile(2, 6, x, y, true, false, IID_TURNIP) {
}
Turnip::~Turnip() {}
void Turnip::collisionCheck() {
	if (sqrt(((getX() - (getWorld()->getNachenBlaster()->getX()))*(getX() - (getWorld()->getNachenBlaster()->getX()))) +
		((getY() - (getWorld()->getNachenBlaster()->getY()))*(getY() - (getWorld()->getNachenBlaster()->getY())))) < 0.75 * (getRadius() + (getWorld()->getNachenBlaster()->getRadius()))) {
		// COLLIDED with nachenblaster
		getWorld()->playSound(SOUND_BLAST);
		getWorld()->getNachenBlaster()->setHealth(getWorld()->getNachenBlaster()->getHealth() - getDmgPoint());
		setIsAlive(false);
	}
}

//PROJ_TORPEDO======================================================================================
Torpedo::Torpedo(double x, double y) :
	Projectile(8, 8, x, y, false, false, IID_TORPEDO) {
}
Torpedo::~Torpedo() {}
void Torpedo::collisionCheck() {
	if (sqrt(((getX() - (getWorld()->getNachenBlaster()->getX()))*(getX() - (getWorld()->getNachenBlaster()->getX()))) +
		((getY() - (getWorld()->getNachenBlaster()->getY()))*(getY() - (getWorld()->getNachenBlaster()->getY())))) < 0.75 * (getRadius() + (getWorld()->getNachenBlaster()->getRadius()))) {
		// COLLIDED with nachenblaster
		getWorld()->playSound(SOUND_BLAST);
		getWorld()->getNachenBlaster()->setHealth(getWorld()->getNachenBlaster()->getHealth() - getDmgPoint());
		setIsAlive(false);
	}
}

//PROJ_FRIENDLYTORPEDO==============================================================================
FriendlyTorpedo::FriendlyTorpedo(double x, double y) :
	Projectile(8, 8, x, y, false, true, IID_TORPEDO) {
}
FriendlyTorpedo::~FriendlyTorpedo() {}
void FriendlyTorpedo::collisionCheck() {
	std::vector<Actor*> temp = getWorld()->getActors();
	for (size_t i = 0; i < temp.size(); i++) {
		if (temp[i]->isAlienShip()) {
			if (sqrt(((getX() - (temp[i]->getX()))*(getX() - (temp[i]->getX()))) +
				((getY() - (temp[i]->getY()))*(getY() - (temp[i]->getY())))) < 0.75 * (getRadius() + (temp[i]->getRadius()))) {
				// COLLIDED with alien ship
				getWorld()->playSound(SOUND_BLAST);
				temp[i]->setHealth(temp[i]->getHealth() - getDmgPoint());
				setIsAlive(false);
			}
		}
	}
}

//ALIEN======================================================================================
Alien::Alien(double health, int collisiondmg, int fp, double ts, double x, double y, Direction dir, double size, unsigned int depth, int id, int points) :
	Actor(x, y, dir, size, depth, id) {
	m_collisionDmg = collisiondmg;
	m_ehp = health;
	m_flightPlan = fp;
	m_travelSpeed = ts;
	m_points = points;
}
Alien::~Alien() {}
void Alien::doSomething() {
	if (!isAlive())
		return;
	if (getX() < 0) {
		setIsAlive(false);
		return;
	}
	collisionCheck();
	changeTravelDirection();
	setFlightPlan(rand() % 32 + 1);
	if ((getWorld()->getNachenBlaster()->getX() < getX()) &&
		(getWorld()->getNachenBlaster()->getY() < getY() + 4) &&
		(getWorld()->getNachenBlaster()->getY() > getY() - 4)) {
		if ((rand() % ((20 / (getWorld()->getLevel())) + 5)) == 1) {
			fire();
			return;
		}
		if ((rand() % ((20 / (getWorld()->getLevel())) + 5)) == 1)
			changeFlightPlan();
	}
	if (getTravelDirection() == 0)
		moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());
	if (getTravelDirection() == 1)
		moveTo(getX() - getTravelSpeed(), getY());
	if (getTravelDirection() == 2)
		moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());
	setFlightPlan(getFlightPlan() - 1);
	collisionCheck();
	return;
}
bool Alien::isAlienShip() {
	return true;
}
int Alien::getFlightPlan() {
	return m_flightPlan;
}
double Alien::getTravelSpeed() {
	return m_travelSpeed;
}
double Alien::getTravelDirection() {
	return m_travelDirection;
}
int Alien::getCollisionDmg() {
	return m_collisionDmg;
}
void Alien::setTravelDirection(int td) {
	m_travelDirection = td;
}
double Alien::getHealth() {
	return m_ehp;
}
void Alien::setHealth(double hp) {
	m_ehp = hp;
}
void Alien::setFlightPlan(int fp) {
	m_flightPlan = fp;
}
void Alien::setTravelSpeed(double sp) {
	m_travelSpeed = sp;
}
void Alien::changeFlightPlan() {
	return;
}
void Alien::changeTravelDirection() {
	if (getY() >= VIEW_HEIGHT - 1)
		setTravelDirection(2); //down and left
	else if (getY() <= 0)
		setTravelDirection(0); //up and left
	else if (getFlightPlan() == 0)
		setTravelDirection(rand() % 3); // random from one
}
void Alien::collisionCheck() {
	if (sqrt(((getX() - (getWorld()->getNachenBlaster()->getX()))*(getX() - (getWorld()->getNachenBlaster()->getX()))) +
		((getY() - (getWorld()->getNachenBlaster()->getY()))*(getY() - (getWorld()->getNachenBlaster()->getY())))) < 0.75 * (getRadius() + (getWorld()->getNachenBlaster()->getRadius()))) {
		// COLLIDED with nachenblaster
		getWorld()->getNachenBlaster()->setHealth(getWorld()->getNachenBlaster()->getHealth() - getCollisionDmg());
		dropGoodie();
		getWorld()->increaseShipsDestoryed();
		setIsAlive(false);
		getWorld()->increaseScore(getPoints());
		getWorld()->playSound(SOUND_DEATH);
		getWorld()->explode(getX(), getY());
	}
	if ((getHealth() <= 0) && isAlive()) {
		dropGoodie();
		getWorld()->increaseShipsDestoryed();
		setIsAlive(false);
		getWorld()->increaseScore(getPoints());
		getWorld()->playSound(SOUND_DEATH);
		getWorld()->explode(getX(), getY());
		return;
	}
}
int Alien::getPoints() {
	return m_points;
}
void Alien::dropGoodie() {
	return;
}

//SMALLGON===================================================================================
Smallgon::Smallgon(double x, double y, double hp) :
	Alien(hp, 5, 0, 2.0, x, y, 0, 1.5, 1, IID_SMALLGON, 250) { // ------------------------------------------CHANGE HP
}
Smallgon::~Smallgon() {}
void Smallgon::fire() {
	getWorld()->playSound(SOUND_ALIEN_SHOOT);
	getWorld()->shootTurnip(getX(), getY());
}


//SMOREGON===================================================================================
Smoregon::Smoregon(double x, double y, double hp) :
	Alien(hp, 5, 0, 2.0, x, y, 0, 1.5, 1, IID_SMOREGON, 250) {// ------------------------------------------CHANGE HP
}
Smoregon::~Smoregon() {}
void Smoregon::changeFlightPlan() {
	setTravelDirection(1);
	setFlightPlan(VIEW_WIDTH);
	setTravelSpeed(5);
	return;
}
void Smoregon::fire() {
	getWorld()->playSound(SOUND_ALIEN_SHOOT);
	getWorld()->shootTurnip(getX(), getY());
}
void Smoregon::dropGoodie() {
	if (rand() % 3 == 1) { // 1 in 3 chances
		if (rand() % 2 == 1)
			getWorld()->dropRepairGoodie(getX(), getY());
		else
			getWorld()->dropTorpedoGoodie(getX(), getY());
	}
}

//SNAGGLEGON===================================================================================
Snagglegon::Snagglegon(double x, double y, double hp) :
	Alien(hp, 15, 0, 1.75, x, y, 0, 1.5, 1, IID_SNAGGLEGON, 1000) {// ------------------------------------------CHANGE HP
	setTravelDirection(2);
}
Snagglegon::~Snagglegon() {}
void Snagglegon::changeTravelDirection() {
	if (getY() >= VIEW_HEIGHT - 1)
		setTravelDirection(2); //down and left
	else if (getY() <= 0)
		setTravelDirection(0); //up and left
}
void Snagglegon::fire() {
	getWorld()->playSound(SOUND_TORPEDO);
	getWorld()->shootTorpedo(getX(), getY(), false);
}
void Snagglegon::dropGoodie() {
	if (rand() % 6 == 1) { // 1 in 6 chances
		getWorld()->dropExtraLifeGoodie(getX(), getY());
	}
}

//GOODIE===================================================================================
Goodie::Goodie(double x, double y, int id) :
	Actor(x, y, 0, 0.5, 1, id) {
}
Goodie::~Goodie() {}
void Goodie::doSomething() {
	if (!isAlive())
		return;
	if ((getX() < 0) || (getX() > VIEW_WIDTH) || (getY() < 0) || (getY() > VIEW_HEIGHT)) {
		setIsAlive(false);
		return;
	}
	collisionCheck();
	moveTo(getX() - 0.75, getY() - 0.75);
	collisionCheck();
}
void Goodie::collisionCheck() {
	if (sqrt(((getX() - (getWorld()->getNachenBlaster()->getX()))*(getX() - (getWorld()->getNachenBlaster()->getX()))) +
		((getY() - (getWorld()->getNachenBlaster()->getY()))*(getY() - (getWorld()->getNachenBlaster()->getY())))) < 0.75 * (getRadius() + (getWorld()->getNachenBlaster()->getRadius()))) {
		// COLLIDED with nachenblaster
		getWorld()->playSound(SOUND_GOODIE);
		powerUp();
		setIsAlive(false);
	}
}
void Goodie::powerUp() {
	return;
}

//EXTRALIFE===================================================================================
ExtraLifeGoodie::ExtraLifeGoodie(double x, double y) :
	Goodie(x, y, IID_LIFE_GOODIE) {
}
ExtraLifeGoodie::~ExtraLifeGoodie() {}
void ExtraLifeGoodie::powerUp() {
	getWorld()->increaseScore(100);
	getWorld()->incLives();
}

//REPAIRGOODIE===================================================================================
RepairGoodie::RepairGoodie(double x, double y) :
	Goodie(x, y, IID_REPAIR_GOODIE) {
}
RepairGoodie::~RepairGoodie() {}
void RepairGoodie::powerUp() {
	getWorld()->increaseScore(100);
	getWorld()->getNachenBlaster()->setHealth(getWorld()->getNachenBlaster()->getHealth() + 10);
	if ((getWorld()->getNachenBlaster()->getHealth()) > 50)
		getWorld()->getNachenBlaster()->setHealth(50);
}

//TORPEDOGOODIE===================================================================================
TorpedoGoodie::TorpedoGoodie(double x, double y) :
	Goodie(x, y, IID_TORPEDO_GOODIE) {
}
TorpedoGoodie::~TorpedoGoodie() {}
void TorpedoGoodie::powerUp() {
	getWorld()->increaseScore(100);
	getWorld()->getNachenBlaster()->setTorpedo(getWorld()->getNachenBlaster()->getTorpedo() + 5);
}