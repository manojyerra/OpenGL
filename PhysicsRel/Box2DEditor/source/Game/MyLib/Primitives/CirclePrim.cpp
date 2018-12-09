#include "CirclePrim.h"
#include "AddData.h"
#include "Collisions.h"
#include "Circle.h"
#include "Rect2D.h"
#include <math.h>

CirclePrim::CirclePrim(float cx, float cy, float r)
{
	_cx = cx;
	_cy = cy;
	_radius = r;
}

bool CirclePrim::Contains(float x, float y)
{
	return ((_cx - x)*(_cx - x) + (_cy - y)*(_cy - y)) < _radius*_radius;
}

bool CirclePrim::CollidesWithRect(float x, float y, float w, float h)
{
	Point cirCenter(_cx, _cy);
	Circle cir(&cirCenter, _radius);

	Rect2D rect2D(x, y, w, h);

	return Collisions::IsColliding(&cir, &rect2D);
}

void CirclePrim::SetCenter(float cx, float cy)
{
	_cx = cx;
	_cy = cy;
}

Point CirclePrim::GetCenter()
{
	Point p(_cx, _cy, 0);
	return p;
}

float CirclePrim::GetCX()
{
	return _cx;
}

float CirclePrim::GetCY()
{
	return _cy;
}

float CirclePrim::GetRadius()
{
	return _radius;
}

void CirclePrim::SetRadius(float r)
{
	_radius = r;
}

void CirclePrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	addData->glColor( _color );
	addData->glBegin(GL_TRIANGLE_FAN);
	addData->glVertex3f(_cx, _cy, 0);
	addData->glVertex3f(_cx+_radius, _cy, 0);

	for( int angle=10; angle<360; angle += 10 )
	{
		float radians = angle*(22.0f/7.0f)/180.0f;
		float x = _cx+_radius*cosf(radians);
		float y = _cy+_radius*sinf(radians);

		addData->glColor4ub(angle*2, angle+50, angle+100, 255);				//temp
		addData->glVertex3f(x,y,0);
	}

	addData->glVertex3f(_cx+_radius, _cy, 0);
	addData->glEnd();


	//BORDER
	addData->glColor(_borderColor);
	addData->glBegin(GL_LINE_STRIP);
	addData->glVertex3f(_cx+_radius, _cy, 0);

	for( int angle=10; angle<360; angle += 10 )
	{
		float radians = angle*(22.0f/7.0f)/180.0f;
		float x = _cx+_radius*cosf(radians);
		float y = _cy+_radius*sinf(radians);
		addData->glVertex3f(x,y,0);
	}

	addData->glVertex3f(_cx+_radius, _cy, 0);
	addData->glEnd();
}

CirclePrim::~CirclePrim()
{
}
