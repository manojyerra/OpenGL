#ifndef MY_VECTOR_H
#define MY_VECTOR_H
#pragma

#include "Point.h"
#include "../CommonLib/Sprite.h"

class MyVector
{
private:
	float Length(Point* tail, Point* head);
	float _length;

	Sprite*	tailImg;
	Sprite* headImg;

public:
	Point Tail;
	Point Head;

	Point p;

	MyVector(Point* head);
	MyVector(Point* tail, Point* head);

	~MyVector();

	float Length();
	void Normalize();
	void SetLength(float length);
	void SetPoint(Point* point);

	void Reverse();
	void Add(MyVector* myVec);
	static MyVector Add(MyVector* aVec, MyVector* bVec);
	float Dot(MyVector* myVec);

	MyVector GetReflectionVector(Point* p1, Point* p2);
	MyVector GetNormalVector(Point* p1, Point* p2);

	void Reflect(Point* p1, Point* p2);

	float angleInX();
	float angleInY();
	float angleInZ();

	void Draw();
};

#endif