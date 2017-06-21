#include "LinePrim.h"
#include "AddData.h"
#include "Collisions.h"
#include "Rect2D.h"

LinePrim::LinePrim(float x1, float y1, float x2, float y2)
{
	x[0] = x1;
	y[0] = y1;
	x[1] = x2;
	y[1] = y2;

	_lineSeg2D = new LineSeg2D(x[0], y[0], x[1], y[1]);
}

bool LinePrim::Contains(float xPos, float yPos)
{
	_lineSeg2D->P1.Set(x[0], y[0], 0);
	_lineSeg2D->P2.Set(x[1], y[1], 0);

	return _lineSeg2D->Contains(xPos, yPos);
}

bool LinePrim::CollidesWithRect(float xPos, float yPos, float width, float height)
{
	Rect2D rect2D(xPos, yPos, width, height);

	_lineSeg2D->P1.Set(x[0], y[0], 0);
	_lineSeg2D->P2.Set(x[1], y[1], 0);

	return Collisions::IsColliding(&rect2D, _lineSeg2D);
}

Point LinePrim::GetCenter()
{
	float cx = (x[0] + x[1]) / 2.0f;
	float cy = (y[0] + y[1]) / 2.0f;;

	return Point(cx, cy, 0);
}

float LinePrim::GetCX()
{
	return (x[0] + x[1]) / 2.0f;
}

float LinePrim::GetCY()
{
	return (y[0] + y[1]) / 2.0f;;
}

void LinePrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	addData->glColor(_color);

	addData->glBegin(GL_LINES);
	addData->glVertex3f(x[0], y[0], 0);
	addData->glVertex3f(x[1], y[1], 0);
	addData->glEnd();
}

LinePrim::~LinePrim()
{
	if (_lineSeg2D)
	{
		delete _lineSeg2D;
		_lineSeg2D = NULL;
	}
}
