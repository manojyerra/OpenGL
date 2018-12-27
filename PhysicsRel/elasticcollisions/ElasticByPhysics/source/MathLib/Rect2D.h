#pragma once
#include "Point.h"

class Rect2D
{
private:
	float _x;
	float _y;
	float _w;
	float _h;
	float _angle;
	float _centerX;
	float _centerY;

public:
	Rect2D( float x, float y, float w, float h, float angle=0);
	~Rect2D();

	bool Contains( Point* p );
	bool IsColliding(Rect2D* rect);
	void SetRotation( float angle );
	void SetRotation( float angle, float centerX, float centerY );

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	float GetAngle();
	float GetCenterX();
	float GetCenterY();

	Point GetVertex1();
	Point GetVertex2();
	Point GetVertex3();
	Point GetVertex4();
};
