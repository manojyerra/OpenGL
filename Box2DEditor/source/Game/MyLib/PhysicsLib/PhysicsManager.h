#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
#pragma once

#include "Box2D.h"
#include "PhyWorld.h"
#include "PhyRect.h"
#include "PhyLine.h"
#include "PhyCircle.h"
#include "PhyPoly.h"
#include "PhyShape.h"
#include "PhyCompoundShape.h"

#include <cstdio>
#include <vector>
using namespace std;

class PhysicsManager
{
private:
	static PhysicsManager* _ref;

	PhysicsManager();
	~PhysicsManager();

public:
	static PhysicsManager* GetInstance();

	vector<PhyShape*> _bodyVec;
	void Init();

	PhyLine* AddLine(float x1, float y1, float x2, float y2, int type);
	PhyRect* AddRect(float x, float y, float width, float height, int type);
	PhyCircle* AddCircle(float x, float y, float r, int type);
	PhyPoly* AddPolygon(vector<Point> vec, int type, float density=1.0f);
	PhyCompoundShape* AddCompoundShape(PhyCompoundShape* compoundShape);

	void Update(float deltaTime);
	void Draw();
	void RemoveBodies();
	void Terminate();
};

#endif

