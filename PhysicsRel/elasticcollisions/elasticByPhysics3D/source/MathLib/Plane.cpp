#include "Plane.h"
#include "math.h"
#include "MyVector.h"

Plane::Plane(){}
Plane::~Plane(){}

Plane::Plane(Point p1, Point p2, Point p3)
{
	Point P1 = p1;
	Point P2 = p2;
	Point P3 = p3;

	A =	(P3.z-P1.z)*(P2.y-P1.y) - (P3.y-P1.y)*(P2.z-P1.z);
	B = -((P2.x-P1.x)*(P3.z-P1.z) - (P3.x-P1.x)*(P2.z-P1.z));
	C =	(P2.x-P1.x)*(P3.y-P1.y) - (P3.x-P1.x)*(P2.y-P1.y);
	D = -( A*P1.x + B*P1.y + C*P1.z);
}

Plane::Plane(float x1, float y1, float z1, 	float x2, float y2, float z2, 	float x3, float y3, float z3)
{
	Point P1(x1, y1, z1);
	Point P2(x2, y2, z2);
	Point P3(x3, y3, z3);
	
	A =	(P3.z-P1.z)*(P2.y-P1.y) - (P3.y-P1.y)*(P2.z-P1.z);
	B = -((P2.x-P1.x)*(P3.z-P1.z) - (P3.x-P1.x)*(P2.z-P1.z));
	C =	(P2.x-P1.x)*(P3.y-P1.y) - (P3.x-P1.x)*(P2.y-P1.y);
	D = -( A*P1.x + B*P1.y + C*P1.z);
}

Plane::Plane(Point* normal, Point* p)
{
	A = normal->x;
	B = normal->y;
	C = normal->z;
	D = -( A*p->x + B*p->y + C*p->z );
}

bool Plane::Contains(Point* p)
{
	return (A*p->x + B*p->y + C*p->z + D == 0);
}

int Plane::AtSameSide(Point* p1, Point* p2)
{
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
	float numerator = fabsf(A*p->x + B*p->y + C*p->z + D);
	float denominator = sqrt ( A*A + B*B + C*C );
	return numerator/denominator;
}


Point Plane::FootOfPerpendicular(Point* p)
{
	float perDist = PerpendicularDist(p);
	MyVector myVec(A,B,C);
	myVec.SetLength(perDist);

	Point retP(p);
	retP.x += myVec.p.x;
	retP.y += myVec.p.y;
	retP.z += myVec.p.z;

	float perDistFromNP = PerpendicularDist(&retP);
	
	if(perDistFromNP != 0)
	{
		retP.Set(p);
		retP.x -= myVec.p.x;
		retP.y -= myVec.p.y;
		retP.z -= myVec.p.z;
	}

	return retP;

	//float k = 2*D / (A*A + B*B + C*C);

	//float x = p->x - A * k;
	//float y = p->y - B * k;
	//float z = p->z - C * k;

	//return Point(x,y,z);
}

Point Plane::IntersectionPoint_LineSegment3D(Point* lineSegPoint1, Point* lineSegPoint2)
{
	Point* lp1 = lineSegPoint1;
	Point* lp2 = lineSegPoint2;

	if(AtSameSide(lp1, lp2) == false)
	{
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


Point Plane::IntersectionPoint_Line3D(Point* linePoint1, Point* linePoint2)
{
	Point* lp1 = linePoint1;
	Point* lp2 = linePoint2;

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
