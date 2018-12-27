#ifndef RECT_SHAPE_H
#define RECT_SHAPE_H
#pragma once

#include "CommonLib/Sprite.h"
#include "Ball.h"

class RectShape
{
private:
	Sprite* _sprite;
	float _initDist;

public:
	Ball* ball1;
	Ball* ball2;

	RectShape(float x, float y);
	~RectShape();

	void ResetDist();
	void Update();
	void Draw();
};

#endif