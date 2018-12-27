#pragma once
#include "Line2D.h"

class LineSeg3D
{
private:

public:
	Point P1;
	Point P2;

	LineSeg3D();
	LineSeg3D(Point p1, Point p2);
	LineSeg3D(float x1, float y1, float z1, float x2, float y2, float z2);
	~LineSeg3D();

	bool Contains(Point* p);
	Point IntersectionPoint(LineSeg3D* lineSeg3D);
};
