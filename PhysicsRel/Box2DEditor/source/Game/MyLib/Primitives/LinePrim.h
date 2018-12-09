#ifndef LINE_PRIM_H
#define LINE_PRIM_H
#pragma once

#include "BasePrim.h"
#include "LineSeg2D.h"

class LinePrim : public BasePrim
{
private:
	LineSeg2D* _lineSeg2D;

public:
	float x[2];
	float y[2];

	LinePrim(float x1, float y1, float x2, float y2);
	~LinePrim();

	bool Contains(float x, float y);
	bool CollidesWithRect(float xPos, float yPos, float width, float height);

	Point GetCenter();
	float GetCX();
	float GetCY();
	void Draw();
};

#endif
