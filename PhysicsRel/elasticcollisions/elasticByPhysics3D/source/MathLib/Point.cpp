#include "Point.h"
#include <math.h>

Point::Point()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

	EXIST = true;
}


Point::Point(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;

	EXIST = true;
}


Point::Point(float X, float Y)
{
	x = X;
	y = Y;
	z = 0;
	EXIST = true;
}

Point::Point(float X, float Y, float Z, bool exist)
{
	x = X;
	y = Y;
	z = Z;
	EXIST = exist;
}

Point::Point(Point* point)
{
	x = point->x;
	y = point->y;
	z = point->z;
	EXIST = point->EXIST;
}

Point::~Point(){}

void Point::Set(float xPos, float yPos, float zPos)
{
	x = xPos;
	y = yPos;
	z = zPos;
}

void Point::Set(Point* p)
{
	x = p->x;
	y = p->y;
	z = p->z;
}

void Point::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

void Point::Add(Point* p)
{
	x += p->x;
	y += p->y;
	z += p->z;
}

void Point::Subtract(Point* p)
{
	x -= p->x;
	y -= p->y;
	z -= p->z;
}


Point Point::Subtract(Point* p1, Point* p2)
{
	return Point(p1->x-p2->x, p1->y-p2->y, p1->z-p2->z);
}

Point Point::Add(Point* p1, Point* p2)
{
	return Point(p1->x+p2->x, p1->y+p2->y, p1->z+p2->z);
}

//scalar operations

void Point::Divide(float num)
{
	x /= num;
	y /= num;
	z /= num;
}

void Point::Multiply(float num)
{
	x *= num;
	y *= num;
	z *= num;
}

float Point::Length()
{
	return sqrt( x*x + y*y + z*z );
}

float Point::Dist(Point* p1, Point* p2)											{	return Point::Dist(p1->x, p1->y, p1->z, p2->x, p2->y, p2->z);				}
float Point::Dist(float x1, float y1, float x2, float y2)						{	return Point::Dist(x1, y1, 0, x2, y2, 0);									}
float Point::Dist(float x1, float y1, float z1, float x2, float y2, float z2)	{	return (float)sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );	}

void Point::SetRotationX(float angleInDegrees)	{	SetRotation(X_AXIS, angleInDegrees, 0,0,0);	}
void Point::SetRotationY(float angleInDegrees)	{	SetRotation(Y_AXIS, angleInDegrees, 0,0,0);	}
void Point::SetRotationZ(float angleInDegrees)	{	SetRotation(Z_AXIS, angleInDegrees, 0,0,0);	}

void Point::SetRotationX(float angleInDegrees, float centerX, float centerY, float centerZ)	{	SetRotation(X_AXIS, angleInDegrees, centerX,centerY,centerZ);	}
void Point::SetRotationY(float angleInDegrees, float centerX, float centerY, float centerZ)	{	SetRotation(Y_AXIS, angleInDegrees, centerX,centerY,centerZ);	}
void Point::SetRotationZ(float angleInDegrees, float centerX, float centerY, float centerZ)	{	SetRotation(Z_AXIS, angleInDegrees, centerX,centerY,centerZ);	}

void Point::SetRotationX(float angleInDegrees, Point* center)	{	SetRotation(X_AXIS, angleInDegrees, center->x,center->y,center->z);	}
void Point::SetRotationY(float angleInDegrees, Point* center)	{	SetRotation(Y_AXIS, angleInDegrees, center->x,center->y,center->z);	}
void Point::SetRotationZ(float angleInDegrees, Point* center)	{	SetRotation(Z_AXIS, angleInDegrees, center->x,center->y,center->z);	}

void Point::SetRotation(int axis, float angleInDegrees, float centerX, float centerY, float centerZ)
{
	x -= centerX;
	y -= centerY;
	z -= centerZ;
	
	float xx = x;
	float yy = y;
	float zz = z;

	angleInDegrees = angleInDegrees*(22.0f/7.0f)/180.0f;		//converting to radiuns

	if(axis == Z_AXIS)
	{
		x = xx*cos(angleInDegrees) - yy*sin(angleInDegrees);
		y = xx*sin(angleInDegrees) + yy*cos(angleInDegrees);
	}
	else if(axis == X_AXIS)
	{
		y = yy*cos(angleInDegrees) - zz*sin(angleInDegrees);
		z = yy*sin(angleInDegrees) + zz*cos(angleInDegrees);
	}
	else if(axis == Y_AXIS)
	{
		z = zz*cos(angleInDegrees) - xx*sin(angleInDegrees);
		x = zz*sin(angleInDegrees) + xx*cos(angleInDegrees);
	}

	x += centerX;
	y += centerY;
	z += centerZ;
}



Point Point::RotatePointZ(float angle, Point* point, Point* center)
{	
	Point newPoint;

	float x = point->x - center->x;
	float y = point->y - center->y;

	angle = angle*(22.0f/7.0f)/180.0f;		//converting to radiuns

	newPoint.x	= x*cos(angle) - y*sin(angle);
	newPoint.y	= x*sin(angle) + y*cos(angle);
	newPoint.z	= point->z;

	newPoint.x += center->x;
	newPoint.y += center->y;

	return newPoint;
}


Point Point::RotatePointY(float angle, Point* point, Point* center)
{	
	Point newPoint;

	float x = point->x - center->x;
	float z = point->z - center->z;

	angle = angle*(22.0f/7.0f)/180.0f;		//converting to radiuns

	newPoint.z	= z*cos(angle) - x*sin(angle);
	newPoint.x	= z*sin(angle) + x*cos(angle);
	newPoint.y	= point->y;

	newPoint.x += center->x;
	newPoint.z += center->z;

	return newPoint;
}


Point Point::RotatePointX(float angle, Point* point, Point* center)
{
	Point newPoint;

	float y = point->y - center->y;
	float z = point->z - center->z;

	angle = angle*(22.0f/7.0f)/180.0f;		//converting to radiuns

	newPoint.y	= y*cos(angle) - z*sin(angle);
	newPoint.z	= y*sin(angle) + z*cos(angle);
	newPoint.x	= point->x;

	newPoint.y += center->y;
	newPoint.z += center->z;

	return newPoint;
}
