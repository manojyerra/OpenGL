#ifndef POSF_H
#define POSF_H
#pragma once

#include "math.h"

class Posf
{
public:
	float x;
	float y;
	Posf()
	{
		x = 0.0f;
		y = 0.0f;
	}
	Posf(float xPos, float yPos)
	{
		x = xPos;
		y = yPos;
	}

	inline bool operator == (const Posf &p2)	
	{
		return (x == p2.x && y == p2.y);
	}

	static Posf RotatePoint(float a, float b, float centerX, float centerY, float angleInDegrees)
	{
		float r = angleInDegrees * 22.0f / (7.0f * 180.0f);
		float aa = ((a-centerX)*cosf(r) - (b-centerY)*sinf(r)) + centerX;
		float bb = ((a-centerX)*sinf(r) + (b-centerY)*cosf(r)) + centerY;
		return Posf(aa, bb);
	}

};

#endif