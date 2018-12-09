#pragma once
#include "Line2D.h"

class LineSeg2D
{
private:

public:
	Point P1;
	Point P2;

	LineSeg2D();
	LineSeg2D( Point p1, Point p2 );
	LineSeg2D( float x1, float y1, float x2, float y2 );
	~LineSeg2D();

	float Slope();
	float AngleInDegrees();
	bool AtSameSide(Point* p1, Point* p2);
	bool Contains(float x, float y);
	bool Contains(Point* p);
	bool ContainsWithErrLimit(float x, float y, float errLimit);
	Point IntersectionPoint(LineSeg2D* lineSeg);
	bool IsColliding(LineSeg2D* lineSeg);
	bool IsColliding(Line2D* lineSeg);
	bool IsColliding(Point* p1, Point* p2);
};
