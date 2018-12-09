#include "Line2D.h"
#include <math.h>


Line2D::Line2D()
{
}


Line2D::Line2D( CVector3 p1, CVector3 p2 )
{
	P1 = p1;
	P2 = p2;
}

Line2D::Line2D( float x1, float y1, float x2, float y2 )
{
	P1.x = x1;
	P1.y = y1;
	P2.x = x2;
	P2.y = y2;
}

Line2D::Line2D( CVector3* p1, CVector3* p2 )
{
	P1.x = p1->x;
	P1.y = p1->y;
	
	P2.x = p2->x;
	P2.y = p2->y;
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

bool Line2D::AtSameSide(CVector3* p1, CVector3* p2)
{
	float A = P2.y-P1.y;
	float B = P1.x-P2.x;
	float C = P2.x*P1.y - P1.x*P2.y;

	float p1Value = A*p1->x + B*p1->y + C;
	float p2Value = A*p2->x + B*p2->y + C;
	
	return ( (p1Value < 0 && p2Value < 0) ||  (p1Value > 0 && p2Value > 0) );
}


CVector3 Line2D::IntersectionPoint(Line2D* line, bool* intersectionPointExist)
{
	float A1 = P2.y- P1.y;
	float B1 = P1.x - P2.x;
	float C1 = P2.x * P1.y - P1.x * P2.y;

	float A2 = line->P2.y- line->P1.y;
	float B2 = line->P1.x - line->P2.x;
	float C2 = line->P2.x * line->P1.y - line->P1.x * line->P2.y;

	float denom = A1*B2 - A2*B1;

	if(denom == 0)
	{
		intersectionPointExist[0] = false;
		return CVector3(0,0,0);
	}

	float x = (B1*C2 - B2*C1)/denom;
	float y = (C1*A2 - C2*A1)/denom;

	return CVector3(x,y,0);
}

float Line2D::PerpendicularDist(CVector3* p)
{
	float A = P2.y - P1.y;
	float B = P1.x - P2.x;
	float C = P2.x * P1.y - P1.x * P2.y;

	float numerator = A*p->x + B*p->y + C;
	if(numerator < 0) numerator = -numerator;
	float denom = sqrt( A*A + B*B );

	return numerator / denom;
}

float Line2D::PerpendicularDist( CVector3* p1, CVector3* p2, CVector3* p )
{
	float A = p2->y - p1->y;
	float B = p1->x - p2->x;
	float C = p2->x * p1->y - p1->x * p2->y;

	float numerator = A*p->x + B*p->y + C;
	if(numerator < 0) numerator = -numerator;
	float denom = sqrt( A*A + B*B );

	return numerator / denom;
}

CVector3 Line2D::FootOfPerpendicular( CVector3* p1, CVector3* p2, CVector3* p )
{
	float A = p2->y - p1->y;
	float B = p1->x - p2->x;
	float C = p2->x * p1->y - p1->x * p2->y;

	float coff = - ( A*p->x + B*p->y + C ) / (A*A + B*B);

	float h = (coff * A) + p->x;
	float k = (coff * B) + p->y;

	return CVector3(h, k);
}

