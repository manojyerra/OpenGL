#ifndef BALL_H
#define BALL_H
#pragma once

#include "MathLib/Point.h"
#include "MathLib/MyVector.h"
#include "CommonLib/Sprite.h"

class Ball
{
private:
	Point _cp;
	float _r;
	
	MyVector* _velocityVec;
	float _speed;
	float _mass;
	
	float _r1, _g1, _b1;
	float _r2, _g2, _b2;

public:
	Ball(Point* center, float radius);

	void SetCenter(Point* center);
	Point GetCenter();

	void SetMass(float mass);
	float GetMass();

	float GetR();
	void SetR(float r);

	void Move();
	void UndoMove();

	MyVector* GetVelocity();
	void SetVelocity(float xVelocity, float yVelocity, float zVelocity);

	void Update();
	void Draw();
};

#endif