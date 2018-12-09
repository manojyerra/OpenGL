#include "LineSeg2D.h"
#include <math.h>

LineSeg2D::LineSeg2D()
{
}

LineSeg2D::LineSeg2D( Point p1, Point p2 )
{
	P1 = p1;
	P2 = p2;
}

LineSeg2D::LineSeg2D( float x1, float y1, float x2, float y2 )
{
	P1.x = x1;
	P1.y = y1;
	P2.x = x2;
	P2.y = y2;
}

LineSeg2D::~LineSeg2D()
{
}

float LineSeg2D::Slope()
{
	float numerator = P2.y - P1.y;
	float denom = P2.x - P1.x;

	if( denom == 0 )
		denom = 0.0000001f;
	return numerator / denom;
}

float LineSeg2D::AngleInDegrees()
{
	return atan(Slope()) * 180.0f/(22.0f/7.0f);
}

bool LineSeg2D::AtSameSide(Point* p1, Point* p2)
{
	float A = P2.y-P1.y;
	float B = P1.x-P2.x;
	float C = P2.x*P1.y - P1.x*P2.y;

	float p1Value = A*p1->x + B*p1->y + C;
	float p2Value = A*p2->x + B*p2->y + C;
	
	return ( (p1Value <= 0 && p2Value <= 0) ||  (p1Value >= 0 && p2Value >= 0) );
}

Point LineSeg2D::IntersectionPoint(LineSeg2D* lineSeg)
{
	float A1 = lineSeg->P2.y - lineSeg->P1.y;
	float B1 = lineSeg->P1.x - lineSeg->P2.x;
	float C1 = lineSeg->P2.x * lineSeg->P1.y - lineSeg->P1.x * lineSeg->P2.y;

	float A2 = P2.y - P1.y;
	float B2 = P1.x - P2.x;
	float C2 = P2.x * P1.y - P1.x * P2.y;

	float denom = A1*B2 - A2*B1;		// if denom is zero then lines are parallel

	if(!denom == 0)
	{
		float x = (B1*C2 - B2*C1)/denom;
		float y = (C1*A2 - C2*A1)/denom;

		if(lineSeg->Contains(x, y) && Contains(x, y))
			return Point(x,y);
	}

	return Point(0,0,0,false);
}

bool LineSeg2D::IsColliding(LineSeg2D* lineSeg)
{
	float A1 = lineSeg->P2.y - lineSeg->P1.y;
	float B1 = lineSeg->P1.x - lineSeg->P2.x;
	float C1 = lineSeg->P2.x * lineSeg->P1.y - lineSeg->P1.x * lineSeg->P2.y;

	float A2 = P2.y - P1.y;
	float B2 = P1.x - P2.x;
	float C2 = P2.x * P1.y - P1.x * P2.y;

	float denom = A1*B2 - A2*B1;		// if denom is zero then lines are parallel

	if(!denom == 0)
	{
		float x = (B1*C2 - B2*C1)/denom;
		float y = (C1*A2 - C2*A1)/denom;

		bool temp1 = lineSeg->Contains(x, y);
		bool temp2 = Contains(x, y);

		if(temp1 && temp2)
			return true;
	}
	return false;
}

bool LineSeg2D::IsColliding(Line2D* line2D)
{
	float A1 = line2D->P2.y - line2D->P1.y;
	float B1 = line2D->P1.x - line2D->P2.x;
	float C1 = line2D->P2.x * line2D->P1.y - line2D->P1.x * line2D->P2.y;

	float A2 = P2.y - P1.y;
	float B2 = P1.x - P2.x;
	float C2 = P2.x * P1.y - P1.x * P2.y;

	float denom = A1*B2 - A2*B1;		// if denom is zero then lines are parallel

	if(!denom == 0)
	{
		float x = (B1*C2 - B2*C1)/denom;
		float y = (C1*A2 - C2*A1)/denom;

		if(Contains(x, y))
			return true;
	}
	return false;
}

bool LineSeg2D::IsColliding(Point* p1, Point* p2)
{
	LineSeg2D lineSeg( Point(p1->x, p1->y), Point(p2->x, p2->y) );

	return IsColliding( &lineSeg );
}

bool LineSeg2D::Contains(float x, float y)
{
	Point p = Point(x, y);

	return Contains(&p);
}

bool LineSeg2D::Contains(Point* p)
{
	float d = Point::Dist(&P1, &P2);
	float d1 = Point::Dist(&P1, p);
	float d2 = Point::Dist(&P2, p);

	return (d1+d2 >= d-0.0001 && d1+d2 <= d+0.0001);
}

bool LineSeg2D::ContainsWithErrLimit(float x, float y, float errLimit)
{
	Point p(x, y);

	float d = Point::Dist(&P1, &P2);
	float d1 = Point::Dist(&P1, &p);
	float d2 = Point::Dist(&P2, &p);

	return (d1+d2 >= d-errLimit && d1+d2 <= d+errLimit);
}