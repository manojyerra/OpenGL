#ifndef PHY_BOX_H
#define PHY_BOX_H
#pragma once

#include "Box2D.h"
#include "PhyShape.h"

class PhyRect : public PhyShape
{
private:
	b2Body* _body;
	float _halfW;
	float _halfH;
	
public:
	PhyRect(float centerX, float centerY, float halfW, float halfH, float angle, int bodyType);
	~PhyRect();

	void Update();
	float GetCX();
	float GetCY();
	float GetHalfW();
	float GetHalfH();
	float GetAngle();
	b2Body* GetBody();
	void Draw();
};

#endif

