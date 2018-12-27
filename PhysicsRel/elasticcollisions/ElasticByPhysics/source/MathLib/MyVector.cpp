#include "MyVector.h"
#include "math.h"
#include "Line2D.h"

MyVector::MyVector(Point* head)
{
	p.Set(head);
	Length = sqrt( p.x*p.x + p.y*p.y + p.z*p.z );
}

MyVector::MyVector(Point* tail, Point* head)
{
	p = Point::Subtract(head, tail);
	Length = sqrt( p.x*p.x + p.y*p.y + p.z*p.z );

	//tailImg = new Sprite();
	//tailImg->Init("data/Tail.png", 0,0,0,0,32);

	//headImg = new Sprite();
	//headImg->Init("data/Head.png", 0,0,0,0,32);
}

void MyVector::Set(float x, float y, float z)
{
	p.Set(x, y, z);
	Length = sqrt( p.x*p.x + p.y*p.y + p.z*p.z );
}

MyVector::~MyVector()
{
}

void MyVector::Normalize()
{
	SetLength(1);
}

void MyVector::SetLength(float newLength)
{
	if(Length == 0)
		return;
	p.Multiply(newLength/Length);
	Length = newLength;
}

void MyVector::Reverse()
{
	p.Reverse();
}


float MyVector::GetProjValue(MyVector* myVec)
{
	if(myVec->Length == 1)
	{
		return(p.x * myVec->p.x + p.y * myVec->p.y);
	}
	else
	{
		float saveX = myVec->p.x;
		float saveY = myVec->p.y;

		myVec->Normalize();
		float returnValue = p.x * myVec->p.x + p.y * myVec->p.y;

		myVec->p.x = saveX;
		myVec->p.y = saveY;

		return returnValue;
	}
	return 0;
}


float MyVector::Dot(MyVector* myVec)
{
	return (p.x*myVec->p.x + p.y*myVec->p.y);
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
	vec.SetLength(2*vec.Length);

	p.x = vec.p.x + p.x;
	p.y = vec.p.y + p.y;
}


float MyVector::angleInX()
{
	return atan2(p.z,p.y) * 180.0f / (22.0f/7.0f);
}

float MyVector::angleInY()
{
	return atan2(p.x,p.z) * 180.0f / (22.0f/7.0f);
}

float MyVector::angleInZ()
{
	return atan2(p.y,p.x) * 180.0f / (22.0f/7.0f);
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
