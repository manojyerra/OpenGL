#ifndef POLY_PRIM_H
#define POLY_PRIM_H
#pragma once

#include "BasePrim.h"
#include "Poly2D.h"

class PolyPrim : public BasePrim
{
private:
	Poly2D* _poly2D;

public:
	vector<Point> vec;

	PolyPrim(vector<Point> vecPoints);
	~PolyPrim();

	bool Contains(float x, float y);
	bool CollidesWithRect(float xPos, float yPos, float width, float height);
	Point GetCenter();
	float GetCX();
	float GetCY();
	void Draw();
};

#endif
