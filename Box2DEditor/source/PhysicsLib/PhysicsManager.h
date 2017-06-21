#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
#pragma once

#include "../Box2D/Box2D.h"
#include "PhyWorld.h"
#include "PhyBox.h"
#include "PhyLine.h"
#include "PhyCircle.h"
#include "PhyTriangle.h"
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
	PhyBox* AddBox(float x, float y, float width, float height, int type);
	PhyCircle* AddCircle(float x, float y, float r, int type);
	PhyTriangle* AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int type);
	PhyPoly* AddPolygon(vector<Point> vec, int type);
	PhyCompoundShape* AddCompoundShape(PhyCompoundShape* compoundShape);

	void Update(float deltaTime);
	void Draw();
	void RemoveBodies();
	void Terminate();
};

#endif

