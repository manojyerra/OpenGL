#ifndef PHY_BOX_H
#define PHY_BOX_H
#pragma once

#include "../Box2D/Box2D.h"
#include "PhyShape.h"

class PhyBox : public PhyShape
{
private:
	float _width;
	float _height;

public:
	b2Body* _body;
	PhyBox(float x, float y, float width, float height, float angle, int bodyType);
	~PhyBox();

	void Update();
	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();
	float GetAngle();
	b2Body* GetBody();
	void Draw();
};

#endif

