#ifndef BASEPRIM_H
#define BASEPRIM_H
#pragma once

#include "Point.h"

class BasePrim
{
protected:
	unsigned int _color;
	unsigned int _borderColor;
	float _angle;

public:
	BasePrim();
	virtual ~BasePrim();

	void SetColor(unsigned int color);
	void SetColor(int r, int g, int b, int a);

	void SetBorderColor(unsigned int color);
	void SetBorderColor(int r, int g, int b, int a);

	unsigned int GetColor();
	unsigned int GetBorderColor();

	virtual bool Contains(float x, float y) = 0;
	virtual bool CollidesWithRect(float x, float y, float w, float h) = 0;
	virtual Point GetCenter() = 0;
	virtual float GetCX() = 0;
	virtual float GetCY() = 0;
};

#endif
