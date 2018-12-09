#pragma once
#include "Point.h"

class Triangle3D
{
private:

public:
	Point P1;
	Point P2;
	Point P3;

	Triangle3D( Point p1, Point p2, Point p3 );
	~Triangle3D();

	bool Contains(Point* p);
	bool Contains( float x, float y, float z );
	float GetArea();
	static float GetArea(Point* p1, Point* p2, Point* p3);
	bool IsColliding(Triangle3D* triangle);
};
