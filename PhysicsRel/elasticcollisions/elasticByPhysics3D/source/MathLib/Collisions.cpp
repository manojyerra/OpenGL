#include "Collisions.h"

bool Collisions::IsColliding(Triangle2D* triangle, LineSeg2D* lineSeg)
{
	if(triangle->Contains(&lineSeg->P1) || triangle->Contains(&lineSeg->P2))
		return true;

	Point* p1 = &triangle->P1;
	Point* p2 = &triangle->P2;
	Point* p3 = &triangle->P3;

	return ( lineSeg->IsColliding( p1, p2) || lineSeg->IsColliding( p2, p3) || lineSeg->IsColliding( p3, p1) );
}

bool Collisions::IsColliding(Rect2D* rect, LineSeg2D* lineSeg)
{
	if(rect->Contains(&lineSeg->P1) || rect->Contains(&lineSeg->P2))
		return true;

	Point p1 = rect->GetVertex1();
	Point p2 = rect->GetVertex2();
	Point p3 = rect->GetVertex3();
	Point p4 = rect->GetVertex4();

	return(	lineSeg->IsColliding( &p1, &p2) || lineSeg->IsColliding( &p2, &p3) || 
			lineSeg->IsColliding( &p3, &p4) || lineSeg->IsColliding( &p4, &p1));
}

bool Collisions::IsColliding(Rect2D* rect, Triangle2D* triangle)
{
	if(rect->Contains(&triangle->P1) || rect->Contains(&triangle->P2) || rect->Contains(&triangle->P3) )
		return true;

	Point p1 = rect->GetVertex1();
	Point p2 = rect->GetVertex2();
	Point p3 = rect->GetVertex3();
	Point p4 = rect->GetVertex4();

	return (triangle->Contains(&p1) || triangle->Contains(&p2) || 
			triangle->Contains(&p3) || triangle->Contains(&p4));
}


bool Collisions::IsColliding(Circle* circle, LineSeg2D* lineSeg)
{
	return IsColliding(circle, &lineSeg->P1, &lineSeg->P2);
}


bool Collisions::IsColliding(Circle* circle, Point* p1, Point* p2)
{
	if(circle->Contains(p1) || circle->Contains(p2))
		return true;

	if( Line2D::PerpendicularDist(p1, p2, circle->GetOrigin()) > circle->GetRadius())
		return false;
	else
	{
		LineSeg2D lineSeg(p1->x, p1->y, p2->x, p2->y);
		return (lineSeg.Contains(&Line2D::FootOfPerpendicular(p1, p2, circle->GetOrigin())));
	}

	return false;
}


bool Collisions::IsColliding(Circle* circle, Triangle2D* triangle)
{
	if(triangle->Contains(circle->GetOrigin())) return true;

	if(circle->Contains(&triangle->P1) || circle->Contains(&triangle->P2) || circle->Contains(&triangle->P3))
		return true;

	Point* p1 = &triangle->P1;
	Point* p2 = &triangle->P2;
	Point* p3 = &triangle->P3;

	return ( IsColliding(circle, p1, p2) || IsColliding(circle, p2, p3) || IsColliding(circle, p3, p1) );
}

bool Collisions::IsColliding(Circle* circle, Rect2D* rect)
{
	if(rect->Contains(circle->GetOrigin())) return true;

	Point* p1 = &rect->GetVertex1();
	Point* p2 = &rect->GetVertex2();
	Point* p3 = &rect->GetVertex3();
	Point* p4 = &rect->GetVertex4();

	if(circle->Contains(p1) || circle->Contains(p2) || circle->Contains(p3) || circle->Contains(p4))
		return true;

	return (	IsColliding(circle, p1, p2) || IsColliding(circle, p2, p3) || 
				IsColliding(circle, p3, p4) || IsColliding(circle, p4, p1));
}

Point Collisions::IntersectionPoint(Triangle3D* triangle3D, LineSeg3D* line)
{
	Plane plane(triangle3D->P1, triangle3D->P2, triangle3D->P3);
	Point p = plane.IntersectionPoint_LineSegment3D(&line->P1, &line->P2);

	if(p.EXIST)
	{
		if(triangle3D->Contains(&p))
			return p;
	}
	return Point(0,0,0,false);
}

//Point Collisions::IntersectionPoint(Plane* plane, LineSeg3D* line)
//{
//	Point* p1 = &plane->P1;
//	Point* p2 = &plane->P2;
//	Point* p3 = &plane->P3;
//
//	Point* lp1 = &line->P1;
//	Point* lp2 = &line->P2;
//
//	if(plane->AtSameSide(lp1, lp2) == false)
//	{
//		float A =	(p3->z-p1->z)*(p2->y-p1->y) - (p3->y-p1->y)*(p2->z-p1->z);
//		float B = -((p2->x-p1->x)*(p3->z-p1->z) - (p3->x-p1->x)*(p2->z-p1->z));
//		float C =	(p2->x-p1->x)*(p3->y-p1->y) - (p3->x-p1->x)*(p2->y-p1->y);
//		float D = -(A*p1->x + B*p1->y + C*p1->z);
//
//		float dx = lp2->x-lp1->x;
//		float dy = lp2->y-lp1->y;
//		float dz = lp2->z-lp1->z;
//
//		float numerator = A*lp1->x+B*lp1->y+C*lp1->z+D;
//		float denom = A*dx+B*dy+C*dz;
//		float r = -numerator/denom;
//		
//		float x = r*dx+lp1->x;
//		float y = r*dy+lp1->y;
//		float z = r*dz+lp1->z;
//		
//		return Point(x,y,z);
//	}
//
//	return Point(0,0,0,false);
//}
