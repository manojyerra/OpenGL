#ifndef MY_VECTOR_H
#define MY_VECTOR_H
#pragma

#include "Point.h"
#include "../CommonLib/Sprite.h"

class MyVector
{
private:
	//Sprite* tailImg;
	//Sprite* headImg;

public:
	Point p;
	float Length;

	MyVector(float x, float y, float z);
	MyVector(Point* head);
	MyVector(Point* tail, Point* head);

	~MyVector();

	void Normalize();
	void SetLength(float length);
	void Set(float x, float y, float z);

	void Reverse();
	float Dot(MyVector* myVec);
	MyVector Cross(MyVector* myVec);

	MyVector GetReflectionVector(Point* p1, Point* p2);
	MyVector GetNormalVector(Point* p1, Point* p2);

	float GetProjValue(MyVector* myVec);
	void Reflect(Point* p1, Point* p2);

	float angleInX();
	float angleInY();
	float angleInZ();

	void Draw();
};

#endif