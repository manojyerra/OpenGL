#include "ColorRect.h"
#include "Utils.h"
#include "GameInfo.h"
#include "RectPrim.h"
#include "UIFont.h"
#include "Input.h"

ColorRect::ColorRect(float x, float y, float w, float h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	
	_color1 = RandColor();
	_color2 = RandColor();
	
    _r1	= (_color1 >> 24) & 255;
	_g1	= (_color1 >> 16) & 255;
	_b1	= (_color1 >> 8) & 255;

    _r2	= (_color2 >> 24) & 255;
	_g2	= (_color2 >> 16) & 255;
	_b2	= (_color2 >> 8) & 255;
	
	_timeCount = 0.0f;
	_transEnable = true;
}

ColorRect::~ColorRect()
{
}

void ColorRect::SetColorTrans(bool enable)
{
	_transEnable = enable;
}

void ColorRect::SetColor(unsigned int color)
{
	_color1 = color;
	
    _r1	= (_color1 >> 24) & 255;
	_g1	= (_color1 >> 16) & 255;
	_b1	= (_color1 >> 8) & 255;
}

void ColorRect::SetBounds(float x, float y, float w, float h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

void ColorRect::SetX(float x)
{
	_x = x;
}

float ColorRect::GetX()	{ return _x; }
float ColorRect::GetY()	{ return _y; }
float ColorRect::GetW()	{ return _w; }
float ColorRect::GetH()	{ return _h; }

void ColorRect::AddTime(float dTime)
{
	_timeCount += dTime;
}

bool ColorRect::Contains(float x, float y)
{
	return (x >= _x && x <= _x+_w && y >= _y && y <= _y+_h );
}

bool ColorRect::IsClicked()
{
	if(Input::IsMouseReleased())
		return Contains(Input::MouseX, Input::MouseY);
	
	return false;
}

void ColorRect::Draw(AddData* addData)
{
	if(_transEnable)
		_timeCount += GameInfo::deltaTime;
	
	if(_timeCount > 1.0f)
	{
		_timeCount = _timeCount - 1.0f;
		
		_color1 = _color2;
		_r1 = _r2;
		_g1 = _g2;
		_b1 = _b2;
		
		_color2 = RandColor();
	    _r2	= (_color2 >> 24) & 255;
		_g2	= (_color2 >> 16) & 255;
		_b2	= (_color2 >> 8) & 255;
	}
	
	addData->glColor4ub( _r1+ (_r2-_r1)*_timeCount, _g1+ (_g2-_g1)*_timeCount, _b1+ (_b2-_b1)*_timeCount, 255 );
	
	addData->glVertex3f(_x,_y,0);
	addData->glVertex3f(_x+_w,_y,0);
	addData->glVertex3f(_x,_y+_h,0);
	addData->glVertex3f(_x+_w,_y,0);
	addData->glVertex3f(_x,_y+_h,0);
	addData->glVertex3f(_x+_w,_y+_h,0);
}

void ColorRect::Draw()
{
	if(_transEnable)
		_timeCount += GameInfo::deltaTime;
	
	if(_timeCount > 1.0f)
	{
		_timeCount = _timeCount - 1.0f;
		
		_color1 = _color2;
		_r1 = _r2;
		_g1 = _g2;
		_b1 = _b2;
		
		_color2 = RandColor();
	    _r2	= (_color2 >> 24) & 255;
		_g2	= (_color2 >> 16) & 255;
		_b2	= (_color2 >> 8) & 255;
	}
	
	int r = _r1+ (_r2-_r1)*_timeCount;
	int g = _g1+ (_g2-_g1)*_timeCount;
	int b = _b1+ (_b2-_b1)*_timeCount;
	
	if(GameInfo::gray)
	{
		int val = (r+g+b) / 3;
		r = g = b = (GLubyte) val;
	}	

	
	GLfloat ver[8];

	ver[0] = _x;			ver[1] = _y;
	ver[2] = _x+_w;			ver[3] = _y;
	ver[4] = _x;			ver[5] = _y+_h;
	ver[6] = _x+_w;			ver[7] = _y+_h;
	
	glColor4f(((float)r)/255.0f, ((float)g)/255.0f, ((float)b)/255.0f, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, ver);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glDisableClientState(GL_VERTEX_ARRAY);	
}

void ColorRect::Draw(int borderW, unsigned int color)
{
	Draw();
	RectPrim::DrawBorder(_x, _y, _w, _h, borderW, color);
}

void ColorRect::Draw(int borderW, unsigned int color, string text, float fontH, unsigned int fontColor)
{
	Draw(borderW, color);
	
	UIFont::GetInstance()->SetColor(fontColor);
	UIFont::GetInstance()->Begin(1);
	UIFont::GetInstance()->DrawFromCenter(text, _x+_w/2, _y+_h/2, fontH, 0);
	UIFont::GetInstance()->End();
}

unsigned int ColorRect::RandColor()
{
	int r = 25 + absInt(randInt()%200);
	int g = 25 + absInt(randInt()%200);
	int b = 25 + absInt(randInt()%200);
	
	int val = absInt(randInt()%10);
	
	if(val == 0)
	{
		r = 0xAA;
		g = 0x34;
		b = 0xF2;		
	}
	else if(val == 1)
	{
		r = 0xFD;
		g = 0xD5;
		b = 0x1C;
	}	
	else if(val == 2)
	{
		r = 0xFE;
		g = 0xD9;
		b = 0x3C;
	}
	else if(val == 3)
	{
		r = 0xEF;
		g = 0x65;
		b = 0x52;
	}
	else if(val == 4)
	{
		r = 0x75;
		g = 0xF8;
		b = 0x79;		
	}
	else if(val == 5)
	{
		r = 0x71;
		g = 0x9C;
		b = 0xFF;
	}
	else if(val == 6)
	{
		r = 0xFB;
		g = 0x71;
		b = 0xE7;
	}
	else if(val == 7)
	{
		r = 255;
		g = 165;
		b = 0;
	}
	else if(val == 8)
	{
		r = 0x74;
		g = 0xDE;
		b = 0xE9;
	}
	else if(val == 9)
	{
		r = 0xF1;
		g = 0x43;
		b = 0x27;
	}
		
	return (unsigned int)( (r<<24) + (g<<16) + (b<<8) + 255 );
}
