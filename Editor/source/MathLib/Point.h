#pragma once

#define PI_VALUE 3.141592653589793f

class Point
{
private:
	static const int X_AXIS = 1;
	static const int Y_AXIS = 2;
	static const int Z_AXIS = 3;

	void SetRotation(int axis, float angleInDegrees, float centerX, float centerY, float centerZ);

public:
	float x;
	float y;
	float z;
	bool EXIST;

	Point();
	Point(float X, float Y);
	Point(float X, float Y, float Z);
	Point(Point* point);
	Point(float X, float Y, float Z, bool exist);
	~Point();

	void Set(float x, float y, float z);
	void Set(Point* p);
	void Reverse();
	void MakeAbsolute();

	static Point Subtract(Point* p1, Point* p2);
	static Point Add(Point* p1, Point* p2);

	void Add(Point* p);
	void Add(float dx, float dy, float dz);
	void Subtract(Point* p);

	void Add(float val);
	void Subtract(float val);

	void Divide(float num);
	void Multiply(float num);

	float GetLength();
	void Normalize();

	void SetRotationX(float angleInDegrees);
	void SetRotationY(float angleInDegrees);
	void SetRotationZ(float angleInDegrees);

	void SetRotationX(float angleInDegrees, float centerX, float centerY, float centerZ);
	void SetRotationY(float angleInDegrees, float centerX, float centerY, float centerZ);
	void SetRotationZ(float angleInDegrees, float centerX, float centerY, float centerZ);

	void SetRotationX(float angleInDegrees, Point* center);
	void SetRotationY(float angleInDegrees, Point* center);
	void SetRotationZ(float angleInDegrees, Point* center);


	//static funtions...
	static float Dist(Point* p1, Point* p2);
	static float Dist(float x1, float y1, float x2, float y2);
	static float Dist(float x1, float y1, float z1, float x2, float y2, float z2);

	static Point RotatePointZ(float angle, Point* point, Point* center);
	static Point RotatePointY(float angle, Point* point, Point* center);
	static Point RotatePointX(float angle, Point* point, Point* center);
};
