#include "RectPrim.h"
#include "../MathLib/Posf.h"

RectPrim::RectPrim(float x, float y, float width, float height, float angle, unsigned int color)
{
	_x = x;
	_y = y;
	_w = width;
	_h = height;
	_angle = angle;
	_color = color;
}

bool RectPrim::Contains(float X, float Y)
{
	return ( X >= _x && X <= _x+_w && Y >= _y && Y <= _y+_h );
}

void RectPrim::SetX(float x) { _x = x; }
void RectPrim::SetY(float y) { _y = y; }
void RectPrim::SetWidth(float w) { _w = w; }
void RectPrim::SetHeight(float h) { _h = h; }

void RectPrim::SetSize(float w, float h)
{
	_w = w;
	_h = h;
}

void RectPrim::SetPos(float X, float Y)
{
	_x = X;
	_y = Y;
}

void RectPrim::SetBounds(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_w = width;
	_h = height;
}

float RectPrim::GetX(){ return _x; }
float RectPrim::GetY(){ return _y; }
float RectPrim::GetWidth(){ return _w; }
float RectPrim::GetHeight(){ return _h; }

float RectPrim::GetMidX() { return _x+_w/2; }
float RectPrim::GetMidY() { return _y+_h/2; }

void RectPrim::SetRotation(float angle)
{
	_angle = angle;
}

void RectPrim::SetColor(int red, int green, int blue, int alpha)
{
	if(red<0)	red		= 0; else if(red	> 255)	red		= 255;
	if(green<0) green	= 0; else if(green	> 255)	green	= 255;
	if(blue<0)	blue	= 0; else if(blue	> 255)	blue	= 255;
	if(alpha<0) alpha	= 0; else if(alpha	> 255)	alpha	= 255;

	_color = (unsigned int)( (red<<24) + (green<<16) + (blue<<8) + alpha );
}

void RectPrim::SetColor(unsigned int color)
{
	_color	= color;
}

void RectPrim::Draw()
{
	Draw(_x, _y, _w, _h, _angle, _color);
}

void RectPrim::Draw(float x, float y, float w, float h, float angle, unsigned int color)
{
	GLfloat ver[12];

	ver[0] = x;			ver[1] = y;
	ver[2] = x+w;		ver[3] = y;
	ver[4] = x;			ver[5] = y+h;
	ver[6] = x+w;		ver[7] = y;
	ver[8] = x;			ver[9] = y+h;
	ver[10] = x+w;		ver[11] = y+h;

	float centerX = x+w/2;
	float centerY = y+h/2;

	if(angle != 0.0f)
	{
		for(int i=0;i<12;i+=2)
		{
			Posf newpos = Posf::RotatePoint(ver[i], ver[i+1], centerX, centerY, angle);
			ver[i] = newpos.x;
			ver[i+1] = newpos.y;
		}
	}

	float red	= (float)((color >> 24) & 255);
	float green	= (float)((color >> 16) & 255);
	float blue	= (float)((color >> 8) & 255);
	float alpha	= (float)((color ) & 255);

	glColor4f( red/255.0f, green/255.0f, blue/255.0f, alpha/(1.5f*255.0f));
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, ver);
	glDrawArrays(GL_TRIANGLES,0,6);
	glDisableClientState(GL_VERTEX_ARRAY);

	GLfloat lineVer[8];

	lineVer[0] = ver[0];	lineVer[1] = ver[1];
	lineVer[2] = ver[2];	lineVer[3] = ver[3];
	lineVer[4] = ver[10];	lineVer[5] = ver[11];
	lineVer[6] = ver[4];	lineVer[7] = ver[5];

	glColor4f( red/255.0f, green/255.0f, blue/255.0f, alpha/(255.0f));
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, lineVer);
	glDrawArrays(GL_LINE_LOOP,0,4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

RectPrim::~RectPrim()
{
}
