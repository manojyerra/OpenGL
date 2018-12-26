#include "Triangle3D.h"
#include "Triangle2D.h"
#include "Point.h"
#include "math.h"
#include "Plane.h"

Triangle3D::Triangle3D( Point p1, Point p2, Point p3 )
{
	P1 = p1;
	P2 = p2;
	P3 = p3;
}

Triangle3D::~Triangle3D(){}

bool Triangle3D::Contains( float x, float y, float z )
{
	return Contains(&Point(x, y, z));
}

bool Triangle3D::Contains(Point* p)
{
	float area = GetArea();

	float subTriangleArea1 = GetArea(&P1, &P2, p);
	float subTriangleArea2 = GetArea(&P2, &P3, p);
	float subTriangleArea3 = GetArea(&P3, &P1, p);

	return (subTriangleArea1 + subTriangleArea2 + subTriangleArea3 <= area+0.0001);
}


float Triangle3D::GetArea()
{
	float a = Point::Dist(P1.x, P1.y, P1.z,   P2.x, P2.y, P2.z);
	float b = Point::Dist(P2.x, P2.y, P2.z,   P3.x, P3.y, P3.z);
	float c = Point::Dist(P3.x, P3.y, P3.z,   P1.x, P1.y, P1.z);

	float s = (a+b+c)/2;
	return sqrt(s*(s-a)*(s-b)*(s-c));
}


float Triangle3D::GetArea(Point* p1, Point* p2, Point* p3)
{
	float a = Point::Dist(p1->x, p1->y, p1->z,   p2->x, p2->y, p2->z);
	float b = Point::Dist(p2->x, p2->y, p2->z,   p3->x, p3->y, p3->z);
	float c = Point::Dist(p3->x, p3->y, p3->z,   p1->x, p1->y, p1->z);

	float s = (a+b+c)/2;
	return sqrt(s*(s-a)*(s-b)*(s-c));
}

bool Triangle3D::IsColliding(Triangle3D* triangle)
{
	//int sameSide1 = Plane::AtSameSide(&P1, &P2, &P3, &triangle->P1, &triangle->P2, &triangle->P3);

	//if(sameSide1 == 1)		// if all the nodes of first triangle are at same side to the plane which is formed
	//	return false;	// second triangle then both triangles are not intersecting and vice versa.

	//int sameSide2 = Plane::AtSameSide(&triangle->P1, &triangle->P2, &triangle->P3, &P1, &P2, &P3);

	//if(sameSide2 == 1)
	//	return false;


	//if(sameSide1 == 2 && sameSide2 == 2)	//both the triangles are at same plane
	//{
	//	if(triangle->Contains(&P1) || triangle->Contains(&P2) || triangle->Contains(&P3))
	//		return true;
	//	if(Contains(&triangle->P1) || Contains(&triangle->P2) || Contains(&triangle->P3))
	//		return true;
	//}

	//Point p1 = Plane::IntersectionPoint_LineSegment3D(&P1, &P2, &P3, &triangle->P1, &triangle->P2);
	//if(p1.EXIST && Contains(&p1)) return true;

	//Point p2 = Plane::IntersectionPoint_LineSegment3D(&P1, &P2, &P3, &triangle->P2, &triangle->P3);
	//if(p2.EXIST && Contains(&p2)) return true;

	//Point p3 = Plane::IntersectionPoint_LineSegment3D(&P1, &P2, &P3, &triangle->P3, &triangle->P1);
	//if(p3.EXIST && Contains(&p3)) return true;


	//Point p4 = Plane::IntersectionPoint_LineSegment3D(&triangle->P1, &triangle->P2, &triangle->P3, &P1, &P2);
	//if(p4.EXIST && Contains(&p4)) return true;

	//Point p5 = Plane::IntersectionPoint_LineSegment3D(&triangle->P1, &triangle->P2, &triangle->P3, &P2, &P3);
	//if(p5.EXIST && Contains(&p5)) return true;

	//Point p6 = Plane::IntersectionPoint_LineSegment3D(&triangle->P1, &triangle->P2, &triangle->P3, &P3, &P1);
	//if(p6.EXIST && Contains(&p6)) return true;

	return false;
}
