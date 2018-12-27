#include "MyVector.h"
#include "math.h"
#include "Line2D.h"

MyVector::MyVector(Point* head)
{
	p.Set(head);
	Tail.Set(0,0,0);
	Head.Set(head);
	_length = sqrt( p.x*p.x + p.y*p.y + p.z*p.z );
}

MyVector::MyVector(Point* tail, Point* head)
{
	p = Point::Subtract(head, tail);
	Tail.Set(tail);
	Head.Set(head);
	_length = sqrt( p.x*p.x + p.y*p.y + p.z*p.z );

	//tailImg = new Sprite();
	//tailImg->Init("data/Tail.png", 0,0,0,0,32);

	//headImg = new Sprite();
	//headImg->Init("data/Head.png", 0,0,0,0,32);
}

void MyVector::SetPoint(Point* point)
{
	p.Set(point);
	Head.x = Tail.x + p.x;
	Head.y = Tail.y + p.y;
	Head.z = Tail.z + p.z;

	_length = sqrt( p.x*p.x + p.y*p.y + p.z*p.z );
}

MyVector::~MyVector()
{
}

float MyVector::Length()
{
	return _length;
}

void MyVector::Normalize()
{
	SetLength(1);
}

void MyVector::SetLength(float newLength)
{
	if(_length == 0)
		return;
	p.Multiply(newLength/_length);
	_length = newLength;
	Head = Point::Add(&Tail, &p);
}

void MyVector::Reverse()
{
	p.Reverse();
	Head = Point::Add(&Tail, &p);
}

void MyVector::Add(MyVector* myVec)
{
	p = Point::Add(&p, &myVec->p);
	Head = Point::Add(&Tail, &p);
}

MyVector MyVector::Add(MyVector* aVec, MyVector* bVec)
{
	Point posVec = Point::Add(&aVec->p, &bVec->p);
	return MyVector(&aVec->Tail, &Point(1,1,1));
}

float MyVector::Dot(MyVector* myVec)
{
	return (p.x*myVec->p.x + p.y*myVec->p.y);
}

MyVector MyVector::GetNormalVector(Point* p1, Point* p2)
{
	float A = p2->y - p1->y;
	float B = p1->x - p2->x;

	MyVector normalVec( &Head, &Point(Head.x+A, Head.y+B) );
	normalVec.Normalize();

	return normalVec;
}

MyVector MyVector::GetReflectionVector(Point* p1, Point* p2)
{
	float A = p2->y - p1->y;
	float B = p1->x - p2->x;

	MyVector normalVec( &Head, &Point(Head.x+A, Head.y+B) );
	normalVec.Normalize();

	MyVector temp(&Head, &Tail);

	float mag = temp.Dot(&normalVec);
	normalVec.SetLength(mag);

	MyVector parllelVec(&Tail, &normalVec.Head);
	parllelVec.SetLength( 2 * parllelVec.Length() );

	return MyVector(&Head, &parllelVec.Head);
}

void MyVector::Reflect(Point* p1, Point* p2)
{
	Reverse();

	float A = p2->y - p1->y;
	float B = p1->x - p2->x;
	
	MyVector normalVec(&Point(A, B));
	normalVec.Normalize();

	float aOnBLength = Dot( &normalVec );

	normalVec.SetLength(aOnBLength);

	float x = normalVec.p.x - p.x;
	float y = normalVec.p.y - p.y;

	MyVector vec(&Point(x, y));
	vec.SetLength(2*vec.Length());

	vec.p.x += p.x;
	vec.p.y += p.y;

	p.x = vec.p.x;
	p.y = vec.p.y;

	Tail.x = 0;
	Tail.y = 0;

	Head.x = p.x;
	Head.y = p.y;

	Normalize();
}


float MyVector::angleInX()
{
	return atan2(Head.z-Tail.z,Head.y-Tail.y) * 180.0f / (22.0f/7.0f);
}

float MyVector::angleInY()
{
	return atan2(Head.x-Tail.x,Head.z-Tail.z) * 180.0f / (22.0f/7.0f);
}

float MyVector::angleInZ()
{
	return atan2(Head.y-Tail.y,Head.x-Tail.x) * 180.0f / (22.0f/7.0f);
}


void MyVector::Draw()
{
	//tailImg->SetBounds( Tail.x, Tail.y-1, _length, 3);
	//tailImg->SetRotation(0);
	//tailImg->AddRotation(angleInZ(), Tail.x, Tail.y);
	//tailImg->Draw();

	//float w = 15;
	//float h = 20;
	//headImg->SetBounds( Tail.x-w+_length , Tail.y-(h/2) , w, h);
	//headImg->SetRotation(0);
	//headImg->AddRotation(angleInZ(), Tail.x, Tail.y);
	//headImg->Draw();
}

//float MyVector::GetAngleInZ(MyVector* myVec1, MyVector* myVec2)
//{
//	float angle1 = atan2(myVec1->Y(), myVec1->X()) * 180.0f / (22.0f/7.0f);
//	float angle2 = atan2(myVec2->Y(), myVec2->X()) * 180.0f / (22.0f/7.0f);
//
//	return (angle2-angle1);
//}


