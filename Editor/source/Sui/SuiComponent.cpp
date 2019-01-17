#include "SuiComponent.h"
#include "SuiIncludes.h"
#include "SuiInput.h"

SuiComponent::SuiComponent(int id)
{
	_id = id;

	_x = 0;
	_y = 0;
	_w = 0;
	_h = 0;

	_name = "";
	_nameAlignment = CENTER;

	_data = "";

	_visible = true;

	_isBgVisible = false;
	_isBorderVisible = false;
	_isBgGradient = false;

	_bgR = 170;
	_bgG = 170;
	_bgB = 170;
	_bgA = 255;

	_borderR = 0;
	_borderG = 0;
	_borderB = 0;
	_borderA = 255;

	_borderWidth = 1.0f;

	_leftMargin = 0;
	_rightMargin = 0;
	_topMargin = 0;
	_bottomMargin = 0;
}

SuiComponent::~SuiComponent()
{
}

void SuiComponent::SetName(string name, int nameAlignment)
{
	_name = name;
	_nameAlignment = nameAlignment;
}

string SuiComponent::GetName()
{
	return _name;
}

void SuiComponent::SetData(string data)
{
	_data = data;
}

string SuiComponent::GetData()
{
	return _data;
}

bool SuiComponent::Contains(float x, float y)
{
	return (x >= _x && x <= _x+_w && y >= _y && y <= _y+_h);
}

void SuiComponent::SetVisible(bool visible)
{
	_visible = visible;
}

bool SuiComponent::IsVisible()
{
	return _visible;
}

int SuiComponent::GetComponentID()
{
	return _id;
}

void SuiComponent::SetMargin(float left, float right, float top, float bottom)
{
	_leftMargin = left;
	_rightMargin = right;
	_topMargin = top;
	_bottomMargin = bottom;
}

void SuiComponent::SetHorMarign(float horMargin)
{
	_leftMargin = horMargin;
	_rightMargin = horMargin;
}

void SuiComponent::SetVerMarign(float verMargin)
{
	_topMargin = verMargin;
	_bottomMargin = verMargin;
}

float SuiComponent::LeftMargin()		{ return _leftMargin;	}
float SuiComponent::RightMargin()	{ return _rightMargin;	}
float SuiComponent::TopMargin()		{ return _topMargin;	}
float SuiComponent::BottomMargin()	{ return _bottomMargin;	}

void SuiComponent::SetBgColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_bgR = r;
	_bgG = g;
	_bgB = b;
	_bgA = a;
}

void SuiComponent::SetBgColor(unsigned char* color)
{
	_bgR = color[0];
	_bgG = color[1];
	_bgB = color[2];
	_bgA = color[3];
}

void SuiComponent::SetBgVisible(bool visible)
{
	_isBgVisible = visible;
}

void SuiComponent::SetBgGradient(bool isBgGradient)
{
	_isBgGradient = isBgGradient;
}

void SuiComponent::SetBorderColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_borderR = r;
	_borderG = g;
	_borderB = b;
	_borderA = a;
}

void SuiComponent::SetBorderVisible(bool visible)
{
	_isBorderVisible = visible;
}

void SuiComponent::SetBorderWidth(float borderWidth)
{
	_borderWidth = borderWidth;
}

// Position and Size related functions....

void SuiComponent::SetWidth(float w)
{
	_w = w;
}

void SuiComponent::SetPos(float x, float y)
{
	_x = x;
	_y = y;
}

float SuiComponent::GetX()	{ return _x; }
float SuiComponent::GetY()	{ return _y; }
float SuiComponent::GetW()	{ return _w; }
float SuiComponent::GetH()	{ return _h; }

// Drawing related....

void SuiComponent::DrawBackground(bool onPress)
{
	int x = _x;
	int y = _y;
	int w = _w;
	int h = _h;

	if(!_isBgGradient)
	{
		unsigned int baseColor = (unsigned int)( (_bgR<<24) + (_bgG<<16) + (_bgB<<8) + _bgA );

		if(onPress)
			baseColor = MixColor(baseColor, 0x000000ff, 20);

		unsigned char r = (baseColor >> 24) & 255;
		unsigned char g = (baseColor >> 16) & 255;
		unsigned char b = (baseColor >> 8) & 255;
		unsigned char a = (baseColor & 255);

		glColor4ub(r, g, b, a);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(x, y);
		glVertex2f(x+w, y);
		glVertex2f(x, y+h);
		glVertex2f(x+w, y+h);
		glEnd();
	}
	else
	{
		unsigned int baseColor = (unsigned int)( (_bgR<<24) + (_bgG<<16) + (_bgB<<8) + _bgA );

		if(onPress)
			baseColor = MixColor(baseColor, 0x000000ff, 20);

		unsigned int topBottomColor = MixColor(baseColor, 0x000000ff, 15);
		unsigned int middleColor = MixColor(baseColor, 0xffffffff, 15);

		unsigned char tbR = (topBottomColor >> 24) & 255;
		unsigned char tbG = (topBottomColor >> 16) & 255;
		unsigned char tbB = (topBottomColor >> 8) & 255;
		unsigned char tbA = (topBottomColor & 255);

		unsigned char mR = (middleColor >> 24) & 255;
		unsigned char mG = (middleColor >> 16) & 255;
		unsigned char mB = (middleColor >> 8) & 255;
		unsigned char mA = (middleColor & 255);

		glBegin(GL_TRIANGLE_STRIP);

		glColor4ub(tbR, tbG, tbB, tbA);
		glVertex2f(x, y);
		glVertex2f(x+w, y);
		
		glColor4ub(mR, mG, mB, mA);
		glVertex2f(x, y+h/2.0f);
		glVertex2f(x+w, y+h/2.0f);
		
		glColor4ub(tbR, tbG, tbB, tbA);
		glVertex2f(x, y+h);
		glVertex2f(x+w, y+h);

		glEnd();
	}
}

void SuiComponent::DrawBorder()
{
	int x = _x;
	int y = _y;
	int w = _w;
	int h = _h;

	glColor4ub(_borderR, _borderG, _borderB, _borderA);

	glLineWidth(_borderWidth);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y+h);
	glVertex2f(x, y+h);
	glEnd();
	glLineWidth(1.0f);
}

unsigned int SuiComponent::MixColor(unsigned int baseColor, unsigned int mixColor, float percent)
{
	unsigned char baseR = (baseColor >> 24) & 255;
	unsigned char baseG = (baseColor >> 16) & 255;
	unsigned char baseB = (baseColor >> 8) & 255;
	unsigned char baseA = (baseColor & 255);

	unsigned char mixR = (mixColor >> 24) & 255;
	unsigned char mixG = (mixColor >> 16) & 255;
	unsigned char mixB = (mixColor >> 8) & 255;
	unsigned char mixA = (mixColor & 255);

	float alpha = (float)mixA/255.0f;

	alpha *= percent/100.0f;

	unsigned char newR = (unsigned char)(mixR*alpha + baseR * (1.0f - alpha));
	unsigned char newG = (unsigned char)(mixG*alpha + baseG * (1.0f - alpha));
	unsigned char newB = (unsigned char)(mixB*alpha + baseB * (1.0f - alpha));
	unsigned char newA = (unsigned char)(baseA);

	if(baseA + mixA > 255)
		newA = 255;
	else
		newA += mixA;

	return (unsigned int)((newR<<24) + (newG<<16) + (newB<<8) + newA);
}
