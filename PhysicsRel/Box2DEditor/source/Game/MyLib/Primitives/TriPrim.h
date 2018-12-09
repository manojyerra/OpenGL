#ifndef TRI_PRIM_H
#define TRI_PRIM_H
#pragma once

#include "BasePrim.h"
#include "Triangle2D.h"

class TriPrim : public BasePrim
{
private:
	Triangle2D* _triangle2D;

public:
	float x[3];
	float y[3];

	TriPrim(float x1, float y1, float x2, float y2, float x3, float y3);
	~TriPrim();

	bool Contains(float x, float y);
	bool CollidesWithRect(float xPos, float yPos, float width, float height);
	Point GetCenter();
	float GetCX();
	float GetCY();
	void Draw();
};

#endif
