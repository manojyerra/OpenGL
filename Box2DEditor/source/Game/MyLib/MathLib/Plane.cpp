#include "Plane.h"
#include "math.h"

Plane::Plane(Point p1, Point p2, Point p3)
{
	P1 = p1;
	P2 = p2;
	P3 = p3;
}

Plane::Plane(	float x1, float y1, float z1, 	float x2, float y2, float z2, 	float x3, float y3, float z3)
{
	P1.x = x1;
	P1.y = y1;
	P1.z = z1;

	P2.x = x2;
	P2.y = y2;
	P2.z = z2;

	P3.x = x3;
	P3.y = y3;
	P3.z = z3;
}

Plane::~Plane(){}


bool Plane::Contains(Point* p)
{
	float A =	(P3.z-P1.z)*(P2.y-P1.y) - (P3.y-P1.y)*(P2.z-P1.z);
	float B = -((P2.x-P1.x)*(P3.z-P1.z) - (P3.x-P1.x)*(P2.z-P1.z));
	float C =	(P2.x-P1.x)*(P3.y-P1.y) - (P3.x-P1.x)*(P2.y-P1.y);
	
	float D = -( A*P1.x + B*P1.y + C*P1.z);

	return (A*p->x + B*p->y + C*p->z + D == 0);
}

int Plane::AtSameSide(Point* p1, Point* p2)
{
	return AtSameSide(&P1, &P2, &P3, p1, p2);
}


int Plane::AtSameSide( Point* planePoint1, Point* planePoint2, Point* planePoint3, Point* p1,	Point* p2)
{
	Point* pp1 = planePoint1;
	Point* pp2 = planePoint2;
	Point* pp3 = planePoint3;

	float A =	(pp3->z-pp1->z)*(pp2->y-pp1->y) - (pp3->y-pp1->y)*(pp2->z-pp1->z);
	float B = -((pp2->x-pp1->x)*(pp3->z-pp1->z) - (pp3->x-pp1->x)*(pp2->z-pp1->z));
	float C =	(pp2->x-pp1->x)*(pp3->y-pp1->y) - (pp3->x-pp1->x)*(pp2->y-pp1->y);
	float D = -( A*pp1->x + B*pp1->y + C*pp1->z);

	float val1 = A*p1->x + B*p1->y + C*p1->z + D;
	float val2 = A*p2->x + B*p2->y + C*p2->z + D;

	if(val1==0 && val2==0)	// if it is on the plane then return 2
		return 2;

	if( (val1 > 0 && val2 > 0) || (val1 < 0 && val2 < 0))	// if it is same side, return 1
		return 1;
	else
		return 0;	// if it is diff sides then return 0
}


int Plane::AtSameSide(Point* p1, Point* p2, Point* p3)
{
	return AtSameSide(&P1, &P2, &P3, p1, p2, p3);
}

int Plane::AtSameSide(Point* planePoint1, Point* planePoint2, Point* planePoint3, Point* p1, Point* p2, Point* p3)
{
	Point* pp1 = planePoint1;
	Point* pp2 = planePoint2;
	Point* pp3 = planePoint3;

	float A =	(pp3->z-pp1->z)*(pp2->y-pp1->y) - (pp3->y-pp1->y)*(pp2->z-pp1->z);
	float B = -((pp2->x-pp1->x)*(pp3->z-pp1->z) - (pp3->x-pp1->x)*(pp2->z-pp1->z));
	float C =	(pp2->x-pp1->x)*(pp3->y-pp1->y) - (pp3->x-pp1->x)*(pp2->y-pp1->y);
	float D = -( A*pp1->x + B*pp1->y + C*pp1->z);

	float val1 = A*p1->x + B*p1->y + C*p1->z + D;
	float val2 = A*p2->x + B*p2->y + C*p2->z + D;
	float val3 = A*p3->x + B*p3->y + C*p3->z + D;

	if(val1==0 && val2==0 && val3==0)	// if it is on the plane then return 2
		return 2;

	if( (val1 > 0 && val2 > 0 && val3 > 0) || (val1 < 0 && val2 < 0 && val3 < 0))	// if it is same side, return 1
		return 1;
	else
		return 0;	// if it is diff sides then return 0
}


float Plane::PerpendicularDist(Point* p)
{
	float A =	(P3.z-P1.z)*(P2.y-P1.y) - (P3.y-P1.y)*(P2.z-P1.z);
	float B = -((P2.x-P1.x)*(P3.z-P1.z) - (P3.x-P1.x)*(P2.z-P1.z));
	float C =	(P2.x-P1.x)*(P3.y-P1.y) - (P3.x-P1.x)*(P2.y-P1.y);
	
	float D = -( A*P1.x + B*P1.y + C*P1.z);

	float numerator = fabsf(A*p->x + B*p->y + C*p->z + D);
	float denominator = sqrt ( A*A + B*B + C*C );

	return numerator/denominator;
}


Point Plane::IntersectionPoint_LineSegment3D(Point* planePoint1, Point* planePoint2, Point* planePoint3, 
							   Point* lineSegPoint1, Point* lineSegPoint2)
{
	Point* p1 = planePoint1;
	Point* p2 = planePoint2;
	Point* p3 = planePoint3;

	Point* lp1 = lineSegPoint1;
	Point* lp2 = lineSegPoint2;

	if(AtSameSide(p1, p2, p3, lp1, lp2) == false)
	{
		float A =	(p3->z-p1->z)*(p2->y-p1->y) - (p3->y-p1->y)*(p2->z-p1->z);
		float B = -((p2->x-p1->x)*(p3->z-p1->z) - (p3->x-p1->x)*(p2->z-p1->z));
		float C =	(p2->x-p1->x)*(p3->y-p1->y) - (p3->x-p1->x)*(p2->y-p1->y);
		float D = -(A*p1->x + B*p1->y + C*p1->z);

		float dx = lp2->x-lp1->x;
		float dy = lp2->y-lp1->y;
		float dz = lp2->z-lp1->z;

		float numerator = A*lp1->x+B*lp1->y+C*lp1->z+D;
		float denom = A*dx+B*dy+C*dz;
		float r = -numerator/denom;
		
		float x = r*dx+lp1->x;
		float y = r*dy+lp1->y;
		float z = r*dz+lp1->z;
		
		return Point(x,y,z);
	}

	return Point(0,0,0,false);
}

Point Plane::IntersectionPoint_Line3D(Point* planePoint1, Point* planePoint2, Point* planePoint3, 
							   Point* linePoint1, Point* linePoint2)
{
	Point* p1 = planePoint1;
	Point* p2 = planePoint2;
	Point* p3 = planePoint3;

	Point* lp1 = linePoint1;
	Point* lp2 = linePoint2;

	float A =	(p3->z-p1->z)*(p2->y-p1->y) - (p3->y-p1->y)*(p2->z-p1->z);
	float B = -((p2->x-p1->x)*(p3->z-p1->z) - (p3->x-p1->x)*(p2->z-p1->z));
	float C =	(p2->x-p1->x)*(p3->y-p1->y) - (p3->x-p1->x)*(p2->y-p1->y);
	float D = -(A*p1->x + B*p1->y + C*p1->z);

	float dx = lp2->x-lp1->x;
	float dy = lp2->y-lp1->y;
	float dz = lp2->z-lp1->z;

	float numerator = A*lp1->x+B*lp1->y+C*lp1->z+D;
	float denom = A*dx+B*dy+C*dz;

	if(denom == 0)
		return Point(0,0,0, false);

	float r = -numerator/denom;
	
	float x = r*dx+lp1->x;
	float y = r*dy+lp1->y;
	float z = r*dz+lp1->z;
	
	return Point(x,y,z);
}
