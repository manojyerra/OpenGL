#ifndef PHY_WORLD_H
#define PHY_WORLD_H
#pragma once

#include "../Box2D/Box2D.h"

class PhyWorld
{
private:
	static PhyWorld* _ref;
	PhyWorld();
	~PhyWorld();

public:
	b2World* world;
	static PhyWorld* GetInstance();

	void Init();
	void Terminate();
};

#endif
