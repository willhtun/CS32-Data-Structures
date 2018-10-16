#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

double randNum(double min, double max);

class Actor : public GraphObject {
public:
	Actor(double x, double y, Direction dir, double size, unsigned int depth, int id);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual int getFirePower();
	virtual bool isAlienShip();
	virtual double getHealth();
	virtual void setHealth(double hp);
	virtual void setTorpedo(int t);
	virtual int getTorpedo();
	virtual int getPoints();
	StudentWorld* getWorld();
	void setCurrentWorld(StudentWorld* sw);
	bool isAlive();
	void setIsAlive(bool status);
private:
	StudentWorld * currentWorld;
	bool m_isAlive = true;
};

class Star : public Actor {
public:
	Star();
	virtual ~Star();
	Star(int x, int y);
	virtual void doSomething();
};

class Explosion : public Actor {
public:
	Explosion(double x, double y);
	virtual ~Explosion();
	virtual void doSomething();
private:
	int lifetime;
};

class NachenBlaster : public Actor {
public:
	NachenBlaster();
	virtual ~NachenBlaster();
	virtual void doSomething();
	virtual double getHealth();
	virtual int getFirePower();
	virtual void setHealth(double hp);
	virtual void setTorpedo(int t);
	virtual int getTorpedo();
private:
	double m_hp;
	int m_cabbagePoints;
	int m_torpedoes;
};

class Projectile : public Actor {
public:
	Projectile(int dmgPoint, int sp, double x, double y, bool isRotating, bool fromNachenblaster, int id);
	virtual ~Projectile();
	virtual void doSomething();
	int getSpeed();
	int getDmgPoint();
	virtual void collisionCheck() = 0;
private:
	int m_speed;
	int m_dmgPoint;
	bool m_isRotating;
	bool m_fromNachenblaster;
};

class Cabbage : public Projectile {
public:
	Cabbage(double x, double y);
	virtual ~Cabbage();
	virtual void collisionCheck();
};

class Turnip : public Projectile {
public:
	Turnip(double x, double y);
	virtual ~Turnip();
	virtual void collisionCheck();
};

class Torpedo : public Projectile {
public:
	Torpedo(double x, double y);
	virtual ~Torpedo();
	virtual void collisionCheck();
};

class FriendlyTorpedo : public Projectile {
public:
	FriendlyTorpedo(double x, double y);
	virtual ~FriendlyTorpedo();
	virtual void collisionCheck();
};

class Alien : public Actor {
public:
	Alien(double health, int collisiondmg, int fp, double ts, double x, double y, Direction dir, double size, unsigned int depth, int id, int points);
	virtual ~Alien();
	virtual void doSomething();
	virtual bool isAlienShip();
	virtual void changeFlightPlan();
	virtual void changeTravelDirection();
	virtual void fire() = 0;
	virtual double getHealth();
	virtual void setHealth(double hp);
	int getFlightPlan();
	double getTravelSpeed();
	double getTravelDirection();
	int getCollisionDmg();
	void setFlightPlan(int fp);
	void setTravelSpeed(double sp);
	void setTravelDirection(int td);
	void collisionCheck();
	virtual int getPoints();
	virtual void dropGoodie();
private:
	int m_collisionDmg;
	double m_ehp;
	int m_points;
	int m_flightPlan;
	double m_travelSpeed;
	int m_travelDirection;
};

class Smallgon : public Alien {
public:
	Smallgon(double x, double y, double hp);
	virtual ~Smallgon();
	virtual void fire();
};

class Smoregon : public Alien {
public:
	Smoregon(double x, double y, double hp);
	virtual ~Smoregon();
	virtual void changeFlightPlan();
	virtual void fire();
	virtual void dropGoodie();
};

class Snagglegon : public Alien {
public:
	Snagglegon(double x, double y, double hp);
	virtual ~Snagglegon();
	virtual void changeTravelDirection();
	virtual void fire();
	virtual void dropGoodie();
};

class Goodie : public Actor {
public:
	Goodie(double x, double y, int id);
	virtual ~Goodie();
	virtual void doSomething();
	void collisionCheck();
	virtual void powerUp();
};

class ExtraLifeGoodie : public Goodie {
public:
	ExtraLifeGoodie(double x, double y);
	virtual ~ExtraLifeGoodie();
	virtual void powerUp();
};

class RepairGoodie : public Goodie {
public:
	RepairGoodie(double x, double y);
	virtual ~RepairGoodie();
	virtual void powerUp();
};

class TorpedoGoodie : public Goodie {
public:
	TorpedoGoodie(double x, double y);
	virtual ~TorpedoGoodie();
	virtual void powerUp();
};

#endif // ACTOR_H_
