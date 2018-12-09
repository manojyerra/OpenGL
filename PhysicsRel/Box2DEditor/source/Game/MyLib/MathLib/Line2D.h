#pragma once

#include "Includes.h"
#include "Point.h"

class Line2D
{
private:

public:
	Point P1;
	Point P2;

	float A;
	float B;
	float C;

	//Line2D(float xCoffient, float yCoffient, float constant);
	Line2D( Point p1, Point p2 );
	Line2D( Point* p1, Point* p2 );
	Line2D( float x1, float y1, float x2, float y2 );

	//float Slope();
	//float AngleInDegrees();
	bool AtSameSide(Point* p1, Point* p2);
	Point IntersectionPoint(Line2D* line);
	float PerpendicularDist(Point* p);
	float PerpendicularDist(float x, float y);

	Point FootOfPerpendicular(Point* p);
	//Line2D GetPerpendicularLine(Point* p);
	//Point GetAnyPoint();
	//float GetY(float x);
	//float GetX(float y);

	static float PerpendicularDist( Point* p1, Point* p2, Point* p );
	static Point FootOfPerpendicular( Point* p1, Point* p2, Point* p );

	void Draw();
	~Line2D();
};
