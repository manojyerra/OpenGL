#include "Line2D.h"
#include <math.h>

//Line2D::Line2D(float xCoffient, float yCoffient, float constant)
//{
//	A = xCoffient;
//	B = yCoffient;
//	C = constant;
//
//	if(B <= 1 && B >= -1)
//	{
//		P1.x = GetX(1);
//		P1.y = 1;
//
//		P2.x = GetX(2);
//		P2.y = 2;
//	}
//	else
//	{
//		P1.x = 1;
//		P1.y = GetY(1);
//
//		P1.x = 2;
//		P1.y = GetY(2);
//	}
//}

Line2D::Line2D( Point p1, Point p2 )
{
	P1 = p1;
	P2 = p2;

	A = P2.y-P1.y;
	B = P1.x-P2.x;
	C = P2.x*P1.y - P1.x*P2.y;
}

Line2D::Line2D( float x1, float y1, float x2, float y2 )
{
	P1.x = x1;
	P1.y = y1;
	P2.x = x2;
	P2.y = y2;

	A = P2.y-P1.y;
	B = P1.x-P2.x;
	C = P2.x*P1.y - P1.x*P2.y;
}

Line2D::Line2D( Point* p1, Point* p2 )
{
	P1.x = p1->x;
	P1.y = p1->y;
	P2.x = p2->x;
	P2.y = p2->y;

	A = P2.y-P1.y;
	B = P1.x-P2.x;
	C = P2.x*P1.y - P1.x*P2.y;
}

Line2D::~Line2D()
{
}

//float Line2D::Slope()
//{
//	float numerator = P2.y - P1.y;
//	float denom = P2.x - P1.x;
//
//	if( denom == 0 )
//		denom = 0.0000001f;
//	return numerator / denom;
//}
//
//float Line2D::AngleInDegrees()
//{
//	return atan(Slope()) * 180.0f/(22.0f/7.0f);
//}

bool Line2D::AtSameSide(Point* p1, Point* p2)
{
	float p1Value = A*p1->x + B*p1->y + C;
	float p2Value = A*p2->x + B*p2->y + C;
	return ( (p1Value < 0 && p2Value < 0) ||  (p1Value > 0 && p2Value > 0) );
}


Point Line2D::IntersectionPoint(Line2D* line)
{
	float denom = A*line->B - line->A*B;

	if(denom == 0)
		return Point(0,0,0,false);

	float x = (B*line->C - line->B*C)/denom;
	float y = (C*line->A - line->C*A)/denom;

	return Point(x,y,0);
}

float Line2D::PerpendicularDist(Point* p)
{
	float numerator = A*p->x + B*p->y + C;
	if(numerator < 0) numerator = -numerator;
	float denom = sqrt( A*A + B*B );
	return numerator / denom;
}

float Line2D::PerpendicularDist(float x, float y)
{
	float numerator = A*x + B*y + C;
	if(numerator < 0) numerator = -numerator;
	float denom = sqrt( A*A + B*B );
	return numerator / denom;
}

float Line2D::PerpendicularDist( Point* p1, Point* p2, Point* p )
{
	float a = p2->y - p1->y;
	float b = p1->x - p2->x;
	float c = p2->x * p1->y - p1->x * p2->y;

	float numerator = a*p->x + b*p->y + c;
	if(numerator < 0) numerator = -numerator;
	float denom = sqrt( a*a + b*b );

	return numerator / denom;
}

Point Line2D::FootOfPerpendicular(Point* p)
{
	float coff = - ( A*p->x + B*p->y + C ) / (A*A + B*B);

	float h = (coff * A) + p->x;
	float k = (coff * B) + p->y;

	return Point(h, k);
}

//Line2D Line2D::GetPerpendicularLine(Point* p)
//{
//	float dx = P2.x - P1.x;
//	float dy = P2.y - P1.y;
//
//	return Line2D(dx, dy, -(dx*p->x + dy*p->y));
//}
//
//float Line2D::GetY(float x)
//{
//	return -(A*x + C)/B;
//}
//
//float Line2D::GetX(float y)
//{
//	return -(B*y + C)/A;
//}


//Point Line2D::GetAnyPoint()
//{
//	if(B <= 1 && B >= -1)
//	{
//		float x = GetX(1);
//		return Point(x, 1);
//	}
//	else if(A <= 1 && A >= -1)
//	{
//		float y = GetY(1);
//		return Point(1, y);
//	}
//	
//	float x = GetX(1);
//	return Point(x, 1);
//}


Point Line2D::FootOfPerpendicular( Point* p1, Point* p2, Point* p )
{
	float a = p2->y - p1->y;
	float b = p1->x - p2->x;
	float c = p2->x * p1->y - p1->x * p2->y;

	float coff = - ( a*p->x + b*p->y + c ) / (a*a + b*b);

	float h = (coff * a) + p->x;
	float k = (coff * b) + p->y;

	return Point(h, k);
}


void Line2D::Draw()
{
	//float ver[4];
	//ver[0] = P1.x;
	//ver[1] = P1.y;
	//ver[2] = P2.x;
	//ver[3] = P2.y;

	//glColor4f(1,1,1,1);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(2, GL_FLOAT, 0, ver);
	//glDrawArrays(GL_LINES,0,2);
	//glDisableClientState(GL_VERTEX_ARRAY);
}