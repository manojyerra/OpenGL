#pragma once
#include "Point.h"

class Plane
{
private:

public:
	//Point P1;
	//Point P2;
	//Point P3;

	float A;
	float B;
	float C;
	float D;

	Plane();
	Plane(Point p1, Point p2, Point p3);
	Plane(	float x1, float y1, float z1, 
			float x2, float y2, float z2, 
			float x3, float y3, float z3);

	Plane(Point* normal, Point* passingThrough);

	~Plane();

	bool Contains(Point* p);

	int AtSameSide(Point* p1, Point* p2);
	int AtSameSide(Point* p1, Point* p2, Point* p3);

	float PerpendicularDist(Point* p);
	Point FootOfPerpendicular(Point* p);

	Point IntersectionPoint_LineSegment3D(Point* lineSegPoint1, Point* lineSegPoint2);
	Point IntersectionPoint_Line3D(Point* linePoint1, Point* linePoint2);
};
