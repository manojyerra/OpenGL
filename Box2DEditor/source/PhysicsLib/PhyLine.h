#ifndef PHY_LINE_H
#define PHY_LINE_H
#pragma once

#include "../Box2D/Box2D.h"
#include "PhyShape.h"

class PhyLine : public PhyShape
{
private:

public:
	float _x1;
	float _y1;
	float _x2;
	float _y2;
	b2Body* _body;

	PhyLine(float x1, float y1, float x2, float y2, int bodyType);
	~PhyLine();

	void Update();
	float GetAngle();
	b2Body* GetBody();
	void Draw();
};

#endif

