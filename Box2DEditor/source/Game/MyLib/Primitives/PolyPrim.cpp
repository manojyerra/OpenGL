#include "PolyPrim.h"
#include "AddData.h"

PolyPrim::PolyPrim(vector<Point> vecPoints)
{
	vec = vecPoints;
	_poly2D = new Poly2D(vec);
}

bool PolyPrim::Contains(float x, float y)
{
	return _poly2D->Contains(x, y);
}

bool PolyPrim::CollidesWithRect(float x, float y, float w, float h)
{
	if (_poly2D->Contains(x, y) || _poly2D->Contains(x+w, y) || _poly2D->Contains(x, y+h) || _poly2D->Contains(x+w, y+h))
		return true;

	for (int i = 0; i < vec.size(); i++)
	{
		float xx = vec[i].x;
		float yy = vec[i].y;

		if (xx >= x && xx < x + w && yy >= y && yy <= y + h)
			return true;
	}

	return false;
}

void PolyPrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	addData->glColor(_color);

	addData->glBegin(GL_TRIANGLE_FAN);
	for(int i=0;i<(int)vec.size();i++)
		addData->glVertex3f(vec[i].x, vec[i].y, 0);
	addData->glEnd();

	
	addData->glColor(_borderColor);

	addData->glBegin(GL_LINE_LOOP);
	for(int i=0;i<(int)vec.size();i++)
		addData->glVertex3f(vec[i].x, vec[i].y, 0);
	addData->glEnd();
}

Point PolyPrim::GetCenter()
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<(int)vec.size();i++)
	{
		cx += vec[i].x;
		cy += vec[i].y;
	}

	cx /= (float)vec.size();
	cy /= (float)vec.size();

	return Point(cx, cy, 0);
}

float PolyPrim::GetCX()
{
	float cx = 0;

	for (int i = 0; i<(int)vec.size(); i++)
		cx += vec[i].x;

	cx /= (float)vec.size();

	return cx;
}

float PolyPrim::GetCY()
{
	float cy = 0;

	for (int i = 0; i<(int)vec.size(); i++)
		cy += vec[i].y;

	cy /= (float)vec.size();

	return cy;
}

PolyPrim::~PolyPrim()
{
	if(_poly2D)
	{
		delete _poly2D;
		_poly2D = NULL;
	}
}
