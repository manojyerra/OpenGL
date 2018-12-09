#include "LineSeg3D.h"
#include <math.h>

LineSeg3D::LineSeg3D( Point p1, Point p2 )
{
	P1 = p1;
	P2 = p2;
}

LineSeg3D::LineSeg3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	P1.x = x1;
	P1.y = y1;
	P1.z = z1;

	P2.x = x2;
	P2.y = y2;
	P2.z = z2;
}

LineSeg3D::~LineSeg3D()
{
}

bool LineSeg3D::Contains(Point* p)
{
	float d = Point::Dist(&P1, &P2);
	float d1 = Point::Dist(&P1, p);
	float d2 = Point::Dist(&P2, p);

	return (d+0.00001 <= d1+d2);
}

Point LineSeg3D::IntersectionPoint(LineSeg3D* lineSeg3D)
{
	Point* p1 = &P1;
	Point* p2 = &P2;

	Point* p3 = &lineSeg3D->P1;
	Point* p4 = &lineSeg3D->P2;

	float dx1 = p2->x-p1->x;
	float dy1 = p2->y-p1->y;
	float dz1 = p2->z-p1->z;

	float dx2 = p4->x-p3->x;
	float dy2 = p4->y-p3->y;
	float dz2 = p4->z-p3->z;

	float denom = 0.0f;
	float r1 =  0.0f;
	float r2 =  0.0f;

	if(dx1 == 0 && dx2 == 0)				//prarllel to x
	{
		denom = dy1*dz2 - dy2*dz1;

		if(denom == 0)
			return Point(0,0,0,false);

		r1 = ( (p3->y - p1->y)*dz2 - (p3->z - p1->z)*dy2 ) / denom;
		r2 = ( (p3->y - p1->y)*dz1 - (p3->z - p1->z)*dy1 ) / denom;

		if( (int)((r1*dx1)+p1->x) != (int)((r2*dx2)+p3->x))
			return Point(0,0,0,false);
	}
	else if(dy1 == 0 && dy2 == 0)				//prarllel to y
	{
		denom = dz1*dx2 - dz2*dx1;

		if(denom == 0)
			return Point(0,0,0,false);

		r1 = ( (p3->z - p1->z)*dx2 - (p3->x - p1->x)*dz2 ) / denom;
		r2 = ( (p3->z - p1->z)*dy1 - (p3->x - p1->x)*dz1 ) / denom;

		if( (int)((r1*dy1)+p1->y) != (int)((r2*dy2)+p3->y))
			return Point(0,0,0,false);
	}
	else
	{
		denom = dx1*dy2 - dx2*dy1;

		if(denom == 0)
			return Point(0,0,0,false);

		r1 = ( dy2*(p3->x-p1->x) - dx2*(p3->y-p1->y) ) / denom;
		r2 = ( dy1*(p3->x-p1->x) - dx1*(p3->y-p1->y) ) / denom;

		if( (int)((r1*dz1)+p1->z) != (int)((r2*dz2)+p3->z))
			return Point(0,0,0,false);
	}

	Point p(r1*dx1+p1->x, r1*dy1+p1->y, r1*dz1+p1->z);

	if(Contains(&p) && lineSeg3D->Contains(&p))
		return p;

	return Point(0,0,0,false);
}

