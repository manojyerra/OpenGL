#include "TriPrim.h"
#include "AddData.h"
#include "Collisions.h"

TriPrim::TriPrim(float x1, float y1, float x2, float y2, float x3, float y3)
{
	x[0] = x1;
	y[0] = y1;
	x[1] = x2;
	y[1] = y2;
	x[2] = x3;
	y[2] = y3;

	_triangle2D = new Triangle2D(x[0], y[0], x[1], y[1], x[2], y[2]);
}

bool TriPrim::Contains(float xPos, float yPos)
{
	_triangle2D->P1.x = x[0];
	_triangle2D->P1.y = y[0];
	_triangle2D->P2.x = x[1];
	_triangle2D->P2.y = y[1];
	_triangle2D->P3.x = x[2];
	_triangle2D->P3.y = y[2];

	return _triangle2D->Contains(xPos, yPos);
}

bool TriPrim::CollidesWithRect(float xPos, float yPos, float width, float height)
{
	Rect2D rect2D(xPos, yPos, width, height);

	_triangle2D->P1.x = x[0];
	_triangle2D->P1.y = y[0];
	_triangle2D->P2.x = x[1];
	_triangle2D->P2.y = y[1];
	_triangle2D->P3.x = x[2];
	_triangle2D->P3.y = y[2];

	return Collisions::IsColliding(&rect2D, _triangle2D);
}

Point TriPrim::GetCenter()
{
	float cx = (x[0] + x[1] + x[2]) / 3.0f;
	float cy = (y[0] + y[1] + y[2]) / 3.0f;

	return Point(cx, cy, 0);
}

float TriPrim::GetCX()
{
	return (x[0] + x[1] + x[2]) / 3.0f;
}

float TriPrim::GetCY()
{
	return (y[0] + y[1] + y[2]) / 3.0f;
}

void TriPrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	addData->glColor(_color);

	addData->glBegin(GL_TRIANGLES);
	addData->glVertex3f(x[0], y[0], 0);
	addData->glVertex3f(x[1], y[1], 0);
	addData->glVertex3f(x[2], y[2], 0);
	addData->glEnd();

	addData->glColor( _borderColor );

	addData->glBegin(GL_LINE_LOOP);
	addData->glVertex3f(x[0], y[0], 0);
	addData->glVertex3f(x[1], y[1], 0);
	addData->glVertex3f(x[2], y[2], 0);
	addData->glEnd();
}

TriPrim::~TriPrim()
{
	if(_triangle2D)
	{
		delete _triangle2D;
		_triangle2D = NULL;
	}
}
