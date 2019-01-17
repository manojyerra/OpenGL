#include "SuiRect.h"
#include "SuiIncludes.h"

SuiRect::SuiRect()
{
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;

	color[0] = color[1] = color[2] = color[3] = 255;

	borderColor[0] = borderColor[1] = borderColor[2] = 0;
	borderColor[3] = 255;
}

SuiRect::SuiRect(float X, float Y, float W, float H)
{
	this->x = X;
	this->y = Y;
	this->w = W;
	this->h = H;

	color[0] = color[1] = color[2] = color[3] = 255;
	borderColor[0] = borderColor[1] = borderColor[2] = 0;
	borderColor[3] = 255;
}

SuiRect::~SuiRect()
{
}

void SuiRect::Move(float dx, float dy)
{
	x += dx;
	y += dy;
}

void SuiRect::SetBounds(SuiRect* rect)
{
	x = rect->x;
	y = rect->y;
	w = rect->w;
	y = rect->h;
}

void SuiRect::SetBounds(float X, float Y, float W, float H)
{
	this->x = X;
	this->y = Y;
	this->w = W;
	this->h = H;
}

bool SuiRect::Contains(float X, float Y)
{
	return ( X >= x && X <= x+w && Y >= y && Y <= y+h );
}

void SuiRect::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}


void SuiRect::SetBorderColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	borderColor[0] = r;
	borderColor[1] = g;
	borderColor[2] = b;
	borderColor[3] = a;
}

bool SuiRect::Intersects(float rectX, float rectY, float rectWidth, float rectHeight)
{
	int tw = (int)w;
	int th = (int)h;
	int rw = (int)rectWidth;
	int rh = (int)rectHeight;

	if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) 
		return false; 

	int tx = (int)x;
	int ty = (int)y;
	int rx = (int)rectX;
	int ry = (int)rectY;

	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;

	return ((rw < rx || rw > tx) &&	(rh < ry || rh > ty) &&	(tw < tx || tw > rx) && (th < ty || th > ry));	
}

void SuiRect::DrawWithBorder()
{
	DrawWithoutBorder();
	DrawBorder();
}

void SuiRect::DrawWithoutBorder()
{
	glColor4ub(color[0], color[1], color[2], color[3]);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(x,	y);
	glVertex2f(x+w,	y);
	glVertex2f(x,	y+h);
	glVertex2f(x+w,	y+h);
	glEnd();
}

void SuiRect::DrawBorder()
{
	glColor4ub(borderColor[0], borderColor[1], borderColor[2], borderColor[3]);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
	glEnd();
}

void SuiRect::Draw(float x, float y, float w, float h, int red, int green, int blue, int alpha, bool wireFrame)
{
	if(!wireFrame)
	{
		glColor4ub(red, green, blue, alpha);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(x,	y);
		glVertex2f(x+w,	y);
		glVertex2f(x,	y+h);
		glVertex2f(x+w,	y+h);
		glEnd();
	}
	else
	{
		glColor4ub(red, green, blue, alpha);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x,y);
			glVertex2f(x+w,y);
			glVertex2f(x+w,y+h);
			glVertex2f(x,y+h);
		glEnd();
	}
}
