#include "Triangle2D.h"
#include "Line2D.h"

Triangle2D::Triangle2D( Point p1, Point p2, Point p3 )
{
	P1 = p1;
	P2 = p2;
	P3 = p3;
}

Triangle2D::Triangle2D( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	P1.x = x1;
	P1.y = y1;

	P2.x = x2;
	P2.y = y2;

	P3.x = x3;
	P3.y = y3;
}

Triangle2D::~Triangle2D()
{
}

bool Triangle2D::Contains( float x, float y )
{
	Point p = Point(x, y);
	return Contains( &p );
}

bool Triangle2D::Contains(Point* p)
{
	return (Line2D(P1, P2).AtSameSide(p, &P3) && Line2D(P2, P3).AtSameSide(p, &P1) && Line2D(P3, P1).AtSameSide(p, &P2));
}

bool Triangle2D::IsColliding(Triangle2D* triangle)
{
	return(Contains(&triangle->P1) || Contains(&triangle->P2) || Contains(&triangle->P3)
		|| triangle->Contains(&P1) || triangle->Contains(&P2) || triangle->Contains(&P3) );
}


bool Triangle2D::Contains(Point* p1, Point* p2, Point* p3, Point* p )
{
	return (Line2D(p1,p2).AtSameSide(p,p3) && Line2D(p2,p3).AtSameSide(p,p1) && Line2D(p3,p1).AtSameSide(p, p2));
}




