#include "Line3D.h"
#include <math.h>

Line3D::Line3D( Point p1, Point p2 )
{
	P1 = p1;
	P2 = p2;
}

Line3D::Line3D( float x1, float y1, float z1, float x2, float y2, float z2)
{
	P1.x = x1;
	P1.y = y1;
	P1.z = z1;

	P2.x = x2;
	P2.y = y2;
	P2.z = z2;
}

Line3D::~Line3D()
{
}

Point Line3D::IntersectionPoint(Line3D* line3D)
{
	Point* p1 = &P1;
	Point* p2 = &P2;

	Point* p3 = &line3D->P1;
	Point* p4 = &line3D->P2;

	float dx1 = p2->x-p1->x;
	float dy1 = p2->y-p1->y;
	float dz1 = p2->z-p1->z;

	float dx2 = p4->x-p3->x;
	float dy2 = p4->y-p3->y;
	float dz2 = p4->z-p3->z;

	float denom = 0.0f;
	float r1 =  0.0f;
	float r2 =  0.0f;

	if(dx1 == 0 && dx2 == 0)				//parllel to x-axis
	{
		denom = dy1*dz2 - dy2*dz1;
		
		if(denom == 0)
			return Point(0,0,0,false);
		
		r1 = ( (p3->y - p1->y)*dz2 - (p3->z - p1->z)*dy2 ) / denom;
		r2 = ( (p3->y - p1->y)*dz1 - (p3->z - p1->z)*dy1 ) / denom;

		if( (int)((r1*dx1)+p1->x) != (int)((r2*dx2)+p3->x))
			return Point(0,0,0,false);
	}
	else if(dy1 == 0 && dy2 == 0)			//parllel to y-axis
	{
		denom = dz1*dx2 - dz2*dx1;

		if(denom == 0)
			return Point(0,0,0,false);

		r1 = ( (p3->z - p1->z)*dx2 - (p3->x - p1->x)*dz2 ) / denom;
		r2 = ( (p3->z - p1->z)*dy1 - (p3->x - p1->x)*dz1 ) / denom;

		if( (int)((r1*dy1)+p1->y) != (int)((r2*dy2)+p3->y))
			return Point(0,0,0,false);
	}
	else									//may be parllel to z or not parllel to any axis.
	{
		denom = dx1*dy2 - dx2*dy1;

		if(denom == 0)
			return Point(0,0,0,false);

		r1 = ( dy2*(p3->x-p1->x) - dx2*(p3->y-p1->y) ) / denom;
		r2 = ( dy1*(p3->x-p1->x) - dx1*(p3->y-p1->y) ) / denom;

		if( (int)((r1*dz1)+p1->z) != (int)((r2*dz2)+p3->z))
			return Point(0,0,0,false);
	}

	return Point(r1*dx1+p1->x, r1*dy1+p1->y, r1*dz1+p1->z);
}




float Line3D::PerpendicularDist( Point* p1, Point* p2, Point* p )
{
	float dx = p->x - p1->x;
	float dy = p->y - p1->y;
	float dz = p->z - p1->z;

	float l = p2->x - p1->x;
	float m = p2->y - p1->y;
	float n = p2->z - p1->z;

	float denom = sqrt(l*l + m*m + n*n);

	float val = (l*dx + m*dy + n*dz)/denom;

	return sqrt( fabsf((dx*dx) + (dy*dy) + (dz*dz) -(val*val)) );
}
