#pragma once
#include "Point.h"

class Circle
{
private:
	Point _origin;
	float _radius;

public:
	Circle(Point* origin, float radius);
	bool Contains(Point* p);
	bool Contains( float x, float y );
	bool IsColliding(Circle* circle);
	float GetRadius();
	Point* GetOrigin();
	~Circle();
};
