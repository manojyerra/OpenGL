#ifndef CIRCLE_PRIM_H
#define CIRCLE_PRIM_H

#include "BasePrim.h"
#include "Point.h"

class CirclePrim : public BasePrim
{
	float _cx;
	float _cy;
	float _radius;

public:
	CirclePrim(float cx, float cy, float r);
	~CirclePrim();

	bool Contains(float x, float y);
	bool CollidesWithRect(float x, float y, float w, float h);
	Point GetCenter();
	float GetCX();
	float GetCY();
	void Draw();

	void SetCenter(float cx, float cy);
	void SetRadius(float r);
	float GetRadius();
};

#endif
