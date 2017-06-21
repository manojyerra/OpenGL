#ifndef PHY_TRIANGLE_H
#define PHY_TRIANGLE_H
#pragma once

#include "../Box2D/Box2D.h"
#include "PhyShape.h"

class PhyTriangle  : public PhyShape
{
private:
	float triVertex[6];

public:
	b2Body* _body;

	PhyTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int bodyType);
	~PhyTriangle();

	void Update();
	float GetX();
	float GetY();
	float GetAngle();
	b2Body* GetBody();
	void Draw();
};

#endif

