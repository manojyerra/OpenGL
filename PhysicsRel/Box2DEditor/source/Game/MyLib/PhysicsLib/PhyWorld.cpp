#include "PhyWorld.h"

PhyWorld* PhyWorld::_ref = NULL;

PhyWorld::PhyWorld()
{
	world = NULL;
}
PhyWorld::~PhyWorld(){}

PhyWorld* PhyWorld::GetInstance()
{
	if(_ref == NULL)
		_ref = new PhyWorld();
	return _ref;
}

void PhyWorld::Init()
{
	world = new b2World(0.0f,10.0f);
}

void PhyWorld::Terminate()
{
	if(world)
		delete world;
	
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}
