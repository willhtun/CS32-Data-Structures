#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void removeDeadGameObjects();
	void updateDisplayText();
	void shootCabbage();
	void shootTurnip(double x, double y);
	void shootTorpedo(double x, double y, bool fromNB);
	void dropExtraLifeGoodie(double x, double y);
	void dropRepairGoodie(double x, double y);
	void dropTorpedoGoodie(double x, double y);
	void explode(double x, double y);
	void increaseShipsDestoryed();
	Actor* getNachenBlaster();
	std::vector<Actor*> getActors();
private:
	std::vector<Actor*> p_actors;
	Actor* p_nachenblaster;
	int shipsDestroyed;
	int shipsOnScreen;
	int maxShips;
	int targetShips;
};

#endif // STUDENTWORLD_H_
