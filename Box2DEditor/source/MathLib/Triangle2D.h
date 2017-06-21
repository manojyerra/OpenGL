#pragma once
#include "Point.h"

class Triangle2D
{
private:

public:
	Point P1;
	Point P2;
	Point P3;

	Triangle2D( Point p1, Point p2, Point p3 );
	Triangle2D( float x1, float y1, float x2, float y2, float x3, float y3 );
	~Triangle2D();

	bool Contains(Point* p);
	bool Contains( float x, float y );
	static bool Contains(Point* p1, Point* p2, Point* p3, Point* p );

	bool IsColliding(Triangle2D* triangle);
};
