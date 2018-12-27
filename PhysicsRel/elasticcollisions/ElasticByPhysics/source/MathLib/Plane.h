#pragma once
#include "Point.h"

class Plane
{
private:

public:
	Point P1;
	Point P2;
	Point P3;

	Plane();
	Plane(Point p1, Point p2, Point p3);
	Plane(	float x1, float y1, float z1, 
			float x2, float y2, float z2, 
			float x3, float y3, float z3);
	~Plane();

	bool Contains(Point* p);
	bool AtSameSide(Point* p1, Point* p2);
	bool AtSameSide(Point* p1, Point* p2, Point* p3);

	static int AtSameSide(	Point* planePoint1, Point* planePoint2, Point* planePoint3, Point* p1, Point* p2);
	static int AtSameSide(	Point* planePoint1, Point* planePoint2, Point* planePoint3, Point* p1, Point* p2, Point* p3);

	float PerpendicularDist(Point* p);

	static Point IntersectionPoint_LineSegment3D(Point* planePoint1, Point* planePoint2, Point* planePoint3, 
							   Point* lineSegPoint1, Point* lineSegPoint2);

	static Point IntersectionPoint_Line3D(Point* planePoint1, Point* planePoint2, Point* planePoint3, 
							   Point* linePoint1, Point* linePoint2);
};
