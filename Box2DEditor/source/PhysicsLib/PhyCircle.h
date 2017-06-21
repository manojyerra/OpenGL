#ifndef PHY_CIR_H
#define PHY_CIR_H
#pragma once

#include "../Box2D/Box2D.h"
#include "PhyShape.h"

class PhyCircle : public PhyShape
{
private:
	float _r;

public:
	b2Body* _body;
	b2Fixture* _fixture;

	PhyCircle(float x, float y, float r, int bodyType);
	~PhyCircle();

	void Update();
	float GetX();
	float GetY();
	float GetR();
	float GetAngle();
	b2Body* GetBody();
	void Draw();
};

#endif

