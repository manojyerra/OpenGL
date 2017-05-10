#include "Rect.h"
#include <windows.h>
#include <gl/gl.h>
#include "Util/GLUtil.h"

Rect::Rect()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;

	color = 0xffffffff;
}

Rect::Rect(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	color = 0xffffffff;
}

void Rect::SetBounds(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void Rect::SetBounds(Rect* rect)
{
	x = rect->x;
	y = rect->y;
	w = rect->w;
	h = rect->h;
}

void Rect::SetBoundsByPoints(float x1, float y1, float x2, float y2)
{
	if(x1 > x2)
	{
		float temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if(y1 > y2)
	{
		float temp = y1;
		y1 = y2;
		y2 = temp;
	}

	x = x1;
	y = y1;
	w = x2-x1;
	h = y2-y1;		
}

unsigned int Rect::GetColor()
{
	return color;
}

void Rect::SetColor(unsigned int color)
{
	this->color = color;
}

bool Rect::Contains(float xx, float yy)
{
	return (xx >= x && xx <= x+w && yy >= y && yy <= y+h);
}

void Rect::Draw()
{
	glColor(color);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(x+0, y+0);
	glVertex2f(x+w, y+0);
	glVertex2f(x+0,	y+h);
	glVertex2f(x+w, y+h);
	glEnd();
}