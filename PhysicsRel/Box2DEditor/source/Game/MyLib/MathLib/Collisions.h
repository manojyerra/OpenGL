#pragma once
#include "LineSeg2D.h"
#include "Triangle2D.h"
#include "Triangle3D.h"
#include "Rect2D.h"
#include "Circle.h"
#include "LineSeg3D.h"
#include "Plane.h"

class Collisions
{
private:
	static bool IsColliding(Circle* circle, Point* p1, Point* p2);

public:
	static bool IsColliding(Triangle2D* triangle, LineSeg2D* lineSeg);

	static bool IsColliding(Rect2D* rect, LineSeg2D* lineSeg);
	static bool IsColliding(Rect2D* rect, Triangle2D* triangle);

	static bool IsColliding(Circle* circle, LineSeg2D* lineSeg);
	static bool IsColliding(Circle* circle, Triangle2D* triangle);
	static bool IsColliding(Circle* circle, Rect2D* rect);

	static Point IntersectionPoint(Triangle3D* triangle3D, LineSeg3D* line);

};
