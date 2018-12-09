#include "Triangle2D.h"
#include "Line2D.h"


Triangle2D::Triangle2D()
{
}

Triangle2D::Triangle2D( CVector3 p1, CVector3 p2, CVector3 p3 )
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
	return Contains( &CVector3(x,y) );
}

bool Triangle2D::Contains(CVector3* p)
{
	return (Line2D(P1, P2).AtSameSide(p, &P3) && Line2D(P2, P3).AtSameSide(p, &P1) && Line2D(P3, P1).AtSameSide(p, &P2));
}

bool Triangle2D::IsColliding(Triangle2D* triangle)
{
	return(Contains(&triangle->P1) || Contains(&triangle->P2) || Contains(&triangle->P3)
		|| triangle->Contains(&P1) || triangle->Contains(&P2) || triangle->Contains(&P3) );
}

bool Triangle2D::Contains(CVector3* p1, CVector3* p2, CVector3* p3, CVector3* p )
{
	return (Line2D(p1,p2).AtSameSide(p,p3) && Line2D(p2,p3).AtSameSide(p,p1) && Line2D(p3,p1).AtSameSide(p, p2));
}

//TwoPos Triangle2D::GetP3(Pos p1, float distanceFromP1, Pos p2, float distanceFromP2)
//{
//	double d = sqrt( (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) );
//
//	//if(d > (distanceFromP1 + distanceFromP2) || p1.equals(p2) || d < Math.abs(distanceFromP1 - distanceFromP2)) {
//	//    // there does not exist a 3rd point, or there are an infinite amount of them
//	//    return new Point[]{};
//	//}
//
//	double a = (distanceFromP1*distanceFromP1 - distanceFromP2*distanceFromP2 + d*d) / (2*d);
//	double h = sqrt(distanceFromP1*distanceFromP1 - a*a);
//
//	Pos temp( p1.x + a*(p2.x - p1.x)/d, p1.y + a*(p2.y - p1.y)/d);
//
//	Pos one(temp.x + h * (p2.y - p1.y) / d, temp.y - h * (p2.x - p1.x) / d);
//	Pos two(temp.x - h * (p2.y - p1.y) / d, temp.y + h * (p2.x - p1.x) / d);
//
//	return TwoPos(one, two);
//}


CVector3 Triangle2D::FindIntersectionByPercentages(CVector3 t1, CVector3 t2, CVector3 t3, float percent13, float percent23)
{
	CVector3 vec13( t3.x-t1.x, t3.y-t1.y );
	CVector3 vec23( t3.x-t2.x, t3.y-t2.y );

	float vec13Len = sqrtf(vec13.x*vec13.x + vec13.y*vec13.y);
	float vec23Len = sqrtf(vec23.x*vec23.x + vec23.y*vec23.y);

	float percent13Len = percent13 * vec13Len / 100.0f;
	float percent23Len = percent23 * vec23Len / 100.0f;

	float dx = vec13.x * percent13Len / vec13Len;
	float dy = vec13.y * percent13Len / vec13Len;

	CVector3 pointOnP1P3( t1.x + dx, t1.y + dy );

	dx = vec23.x * percent23Len / vec23Len;
	dy = vec23.y * percent23Len / vec23Len;

	CVector3 pointOnP2P3( t2.x + dx, t2.y + dy );

	return IntersectionPoint(t2, pointOnP1P3, t1, pointOnP2P3);
}


CVector3 Triangle2D::FindPercentages(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p)
{
	CVector3 pointOnP1P3 = IntersectionPoint(p2,p, p1,p3);
	CVector3 pointOnP2P3 = IntersectionPoint(p1,p, p2,p3);

	float p1p3Length = Dist(p1,p3);
	float p2p3Length = Dist(p2,p3);

	float length13 = Dist(p1,pointOnP1P3);		// length13 is From P1 to pointOnP1P3
	float length23 = Dist(p2,pointOnP2P3);		// length23 is From P2 to pointOnP2P3
	
	float percent13 = length13 * 100.0f / p1p3Length;
	float percent23 = length23 * 100.0f / p2p3Length;

	return CVector3(percent13, percent23);
}



/////////////////////////////////////////////////////////////////////////////////


float Triangle2D::Dist(CVector3 p1, CVector3 p2)
{
	return sqrtf ( (p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y) );
}

CVector3 Triangle2D::IntersectionPoint(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4)
{
	float a1 = p2.y-p1.y;
	float b1 = p1.x-p2.x;
	float c1 = p2.x*p1.y - p1.x*p2.y;

	float a2 = p4.y-p3.y;
	float b2 = p3.x-p4.x;
	float c2 = p4.x*p3.y - p3.x*p4.y;

	float denom = a1*b2 - a2*b1;

	float x = (b1*c2 - b2*c1)/denom;
	float y = (c1*a2 - c2*a1)/denom;

	return CVector3(x,y);
}
