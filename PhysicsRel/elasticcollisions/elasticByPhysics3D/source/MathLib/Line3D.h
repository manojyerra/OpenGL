#pragma once
#include "Point.h"

class Line3D
{
private:

public:
	Point P1;
	Point P2;

	Line3D();
	Line3D(Point p1, Point p2);
	Line3D(float x1, float y1, float z1, float x2, float y2, float z3);
	~Line3D();

	Point IntersectionPoint(Line3D* line3D);
	static float PerpendicularDist( Point* p1, Point* p2, Point* p );
};
