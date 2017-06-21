#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::_ref = NULL;

PhysicsManager::PhysicsManager(){}
PhysicsManager::~PhysicsManager(){}

PhysicsManager* PhysicsManager::GetInstance()
{
	if(_ref == NULL)
		_ref = new PhysicsManager();
	return _ref;
}

void PhysicsManager::Init()
{
	PhyWorld::GetInstance()->Init();
}

PhyLine* PhysicsManager::AddLine(float x1, float y1, float x2, float y2, int type)
{
	PhyLine* line = new PhyLine(x1, y1, x2, y2, type);
	_bodyVec.push_back(line);
	return line;
}

PhyRect* PhysicsManager::AddRect(float x, float y, float width, float height, int type)
{
	PhyRect* phyRect = new PhyRect(x, y, width, height, 0, type);
	_bodyVec.push_back(phyRect);
	return phyRect;
}

PhyCircle* PhysicsManager::AddCircle(float x, float y, float r, int type)
{
	PhyCircle* circle = new PhyCircle(x, y, r, type);
	_bodyVec.push_back(circle);
	return circle;
}

PhyPoly* PhysicsManager::AddPolygon(vector<Point> vec, int type, float density)
{
	PhyPoly* poly = new PhyPoly(vec, type, density);
	_bodyVec.push_back(poly);
	return poly;
}

PhyCompoundShape* PhysicsManager::AddCompoundShape(PhyCompoundShape* compoundShape)
{
	_bodyVec.push_back(compoundShape);
	return compoundShape;
}

void PhysicsManager::Update(float deltaTime)
{
	float32 timeStep = 1.0f / 20.0f;
	int32 velocityIterations = 8*2;
	int32 positionIterations = 4*2;

	b2World* world = PhyWorld::GetInstance()->world;
	world->Step(deltaTime, velocityIterations, positionIterations);
}

void PhysicsManager::Draw()
{
	for(int i=0;i<(int)_bodyVec.size();i++)
		_bodyVec[i]->Draw();
}

void PhysicsManager::RemoveBodies()
{
	for(int i=0;i<(int)_bodyVec.size();i++)
	{
		if(_bodyVec[i])
		{
			delete _bodyVec[i];
			_bodyVec[i] = NULL;
		}
	}
	_bodyVec.clear();
}

void PhysicsManager::Terminate()
{
	RemoveBodies();
	PhyWorld::GetInstance()->Terminate();

	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}
