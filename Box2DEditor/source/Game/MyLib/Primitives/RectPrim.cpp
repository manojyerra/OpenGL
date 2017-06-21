#include "RectPrim.h"
#include "Includes.h"
#include "math.h"
#include "Utils.h"
#include "GameInfo.h"
#include "Rect2D.h"

RectPrim::RectPrim(float xPos, float yPos, float width, float height, float angle, unsigned int color)
{
	x = xPos;
	y = yPos;
	w = width;
	h = height;
}

RectPrim::RectPrim()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

bool RectPrim::Contains(float X, float Y)
{
	return ( X >= x && X <= x+w && Y >= y && Y <= y+h );
}

bool RectPrim::CollidesWithRect(float xPos, float yPos, float width, float height)
{
	Rect2D rect1(xPos, yPos, width, height);
	Rect2D rect2(x, y, w, h);

	return rect1.IsColliding(&rect2);
}

void RectPrim::SetSize(float width, float height)
{
	w = width;
	h = height;
}

void RectPrim::SetPos(float xPos, float yPos)
{
	x = xPos;
	y = yPos;
}

void RectPrim::SetBounds(float xPos, float yPos, float width, float height)
{
	x = xPos;
	y = yPos;
	w = width;
	h = height;
}

Point RectPrim::GetCenter()
{
	return Point(x + w / 2, y + h / 2);
}

float RectPrim::GetCX() { return x+w/2; }
float RectPrim::GetCY() { return y+h/2; }

void RectPrim::SetRotation(float angle)
{
	_angle = angle;
}

float RectPrim::GetRotation()
{
	return _angle;
}

void RectPrim::Draw()
{
	Draw(x, y, w, h, _angle, _color);
}

void RectPrim::Draw(float x, float y, float w, float h, float angle, unsigned int color, AddData* addData)
{
	GLfloat ver[8];

	ver[0] = x;			ver[1] = y;
	ver[2] = x+w;		ver[3] = y;
	ver[4] = x;			ver[5] = y+h;
	ver[6] = x+w;		ver[7] = y+h;

	float r	= (float)((color >> 24) & 255);
	float g	= (float)((color >> 16) & 255);
	float b	= (float)((color >> 8) & 255);
	float a	= (float)((color ) & 255);

	if(GameInfo::gray)
	{
		int val = (r + g + b) / 3;
		r = g = b = (GLubyte) val;
	}

	if(angle != 0)
	{
		float pi = 3.14159f;
		float cosV = cos(angle * pi / 180.0f);
		float sinV = sin(angle * pi / 180.0f);
		
		float tX = x+w/2;
		float tY = y+h/2;
		
		for(int i=0;i<8;i+=2)
		{
			float x = ver[i+0] - tX;
			float y = ver[i+1] - tY;
			
			ver[i+0] = (x*cosV - y*sinV) + tX;
			ver[i+1] = (x*sinV + y*cosV) + tY;
		}
	}
	
	if(addData)
	{
		addData->glColor4ub(r,g,b,a);
		
		addData->glVertex3f(ver[0], ver[1],	0);
		addData->glVertex3f(ver[2], ver[3],	0);
		addData->glVertex3f(ver[4], ver[5],	0);
		addData->glVertex3f(ver[2], ver[3],	0);
		addData->glVertex3f(ver[4], ver[5],	0);
		addData->glVertex3f(ver[6], ver[7],	0);
	}
	else
	{
		glColor4f(((float)r)/255.0f, ((float)g)/255.0f, ((float)b)/255.0f, ((float)a)/255.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, ver);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void RectPrim::DrawBorder(float x, float y, float w, float h, float lineWidth, unsigned int color)
{
	x = (int)x;
	y = (int)y;
	w = (int)w;
	h = (int)h;

	float half = lineWidth / 2;
	GLfloat ver[16];

	ver[0] = x - half;			ver[1] = y;
	ver[2] = x + w + half;		ver[3] = y;

	ver[4] = x + w;				ver[5] = y;
	ver[6] = x + w;				ver[7] = y + h;

	ver[8] = x + w + half;		ver[9] = y + h;
	ver[10] = x - half;			ver[11] = y + h;

	ver[12] = x;				ver[13] = y + h;
	ver[14] = x;				ver[15] = y;

	float red = (float)((color >> 24) & 255);
	float green = (float)((color >> 16) & 255);
	float blue = (float)((color >> 8) & 255);
	float alpha = (float)((color)& 255);

	float prevLW = 1.0f;
	glGetFloatv(GL_LINE_WIDTH, &prevLW);

	glLineWidth(lineWidth);
	glColor4f(((float)red) / 255.0f, ((float)green) / 255.0f, ((float)blue) / 255.0f, ((float)alpha) / 255.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, ver);
	glDrawArrays(GL_LINES, 0, 8);
	glDisableClientState(GL_VERTEX_ARRAY);

	glLineWidth(prevLW);
}

RectPrim::~RectPrim()
{
}


//void RectPrim::GetVertex(float x, float y, float w, float h, float* ver)
//{
//	x = (int)x;
//	y = (int)y;
//	w = (int)w;
//	h = (int)h;
//
//	ver[0] = x;			ver[1] = y;
//	ver[2] = x+w;		ver[3] = y;
//
//	ver[4] = x+w;		ver[5] = y;
//	ver[6] = x+w;		ver[7] = y+h;
//
//	ver[8] = x+w;		ver[9] = y+h;
//	ver[10] = x;		ver[11] = y+h;
//
//	ver[12] = x;		ver[13] = y+h;
//	ver[14] = x;		ver[15] = y;
//}


//void RectPrim::Draw(AddData* addData)
//{
//	addData->glColor(_color);
//	addData->glVertex3f(x, y, 0);
//	addData->glVertex3f(x + w, y, 0);
//	addData->glVertex3f(x, y + h, 0);
//	addData->glVertex3f(x + w, y, 0);
//	addData->glVertex3f(x, y + h, 0);
//	addData->glVertex3f(x + w, y + h, 0);
//}


//void RectPrim::Draw(int x, int y, int w, int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2)
//{
//	AddData addData(32, true, false, false);
//
//	addData.glBegin(GL_TRIANGLE_STRIP);
//
//	addData.glColor4ub(r1, g1, b1, a1);
//	addData.glVertex3f(x, y, 0);
//	addData.glVertex3f(x + w, y, 0);
//
//	addData.glColor4ub(r2, g2, b2, a2);
//	addData.glVertex3f(x, y + h, 0);
//	addData.glVertex3f(x + w, y + h, 0);
//
//	addData.glEnd();
//}
//
//void RectPrim::DrawBatch(float x, float y, float w, float h, AddData* addData)
//{
//	addData->glVertex3f(x, y, 0);
//	addData->glVertex3f(x + w, y, 0);
//	addData->glVertex3f(x, y + h, 0);
//	addData->glVertex3f(x + w, y, 0);
//	addData->glVertex3f(x, y + h, 0);
//	addData->glVertex3f(x + w, y + h, 0);
//}
