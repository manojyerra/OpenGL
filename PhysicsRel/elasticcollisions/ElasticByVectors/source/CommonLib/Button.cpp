#include "Button.h"
#include "../CommonLib/UIFont.h"
#include "../CommonLib/Input.h"
//#include "../CommonLib/MyGL.h"
//#include "../GameInfo.h"
#include "../CommonLib/MemTrace.h"
//#include "../SoundUtil/SoundManager.h"

Button::Button(){}
Button::~Button(){}

void Button::Init(int color, float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	_fontColor = 0x000000ff;
	_horGap = -2;
	_labelFontWidth = 18;
	_labelFontHeight = 18;

	_cx = 0;
	_cy = 0;
	_cw = 1250;
	_ch = 950;

	_color = color;
	_type = PROPERTY;
	_isOn = false;
	_isJustNowOn = false;
}

bool Button::Update()
{
	return (Input::IsMouseReleased() && Contains(Input::MouseX, Input::MouseY));
}

void Button::SetType(int type)	
{
	_type = type;
}

int Button::GetType()			{ return _type; }
void Button::On()				{ _isOn = true; }
void Button::Off()				{ _isOn = false;}
bool Button::IsOn()				{ return _isOn; }
void Button::JustNowOn()		{ _isJustNowOn = true; }
bool Button::IsJustNowOn()		{ return _isJustNowOn; }

void Button::SetColor(int color)
{
	_color = color;

	//if(color == WHITE)		_rectPrim->SetColor(206,203,255,255);
	//else if(color == RED)	_rectPrim->SetColor(150,0,0,255);
	//else if(color == BLUE)	_rectPrim->SetColor(26,127,255,255);
	//else if(color == GREEN)	_rectPrim->SetColor(0,142,0,255);
	//else if(color == DGREEN)_rectPrim->SetColor(107,170,0,255);
	//else if(color == ORANGE)_rectPrim->SetColor(255,128,28,255);
	//else if(color == PINK)	_rectPrim->SetColor(255,3,132,255);
	//else if(color == CYAN)	_rectPrim->SetColor(20,186,169,255);
	//else if(color == YELLOW)_rectPrim->SetColor(255,199,0,255);
	//else if(color == PURPLE)_rectPrim->SetColor(165,56,255,255);
}

int Button::GetColor()
{
	return _color;
}

string Button::GetLabel() {return _label;}

void Button::SetLabel(string label, unsigned int fontColor, int horGap)
{
	_fontColor = fontColor;
	_label = label;
	_horGap = horGap;
}

void Button::SetLabelFontSize(float width, float height)
{
	_labelFontWidth = width;
	_labelFontHeight = height;
}

float Button::GetX(){ return _x; }
float Button::GetY(){ return _y; }
float Button::GetWidth(){ return _width; }
float Button::GetHeight(){ return _height; }

void Button::SetPos(float x, float y) { SetBounds(x, y, _width, _height); }
void Button::SetSize(float width, float height) { SetBounds(_x, _y, width, height); }
void Button::SetY(float y)	{ SetBounds(_x,y,_width,_height);}//;_y = y;}
void Button::SetX(float x) { SetBounds(x,_y,_width,_height);}//;_y = y;}
void Button::SetBounds(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}

bool Button::Contains(float xx, float yy)
{
	return (xx >= _x && xx <= _x+_width && yy >=_y && yy <= _y+_height);
}

//void Button::SetClipArea(int x, int y, int w, int h)
//{
//	if(_rectPrim)
//		_rectPrim->SetClipArea(x,y,w,h);
//
//	_cx = x;
//	_cy = y;
//	_cw = w;
//	_ch = h;
//}


void Button::Draw()
{
	if(_type != ON_OFF)
	{
		if(_isOn)
			RectPrim::Draw(_x, _y, _width, _height, 0, 255, 0, 255, false);
		else
			RectPrim::Draw(_x, _y, _width, _height, 255, 0, 128, 255, false);
	}
	else
	{
		RectPrim::Draw(_x, _y, _width, _height, 255, 0, 128, 255, false);
		if(_isOn)
			RectPrim::Draw(_x+1, _y+(_height/2.0f)-8.0f/2.0f, 8, 8, 0, 255, 0, 255, false);
	}

	if(_label.compare("") != 0)
	{
		UIFont::GetInstance()->SetColor(_fontColor);
		UIFont::GetInstance()->DrawFromCenter(_label, _x+_width/2, _y+_height/2+2, _labelFontWidth, _labelFontHeight, _horGap, UIFont::NO_BORDER);
	}
	_isJustNowOn = false;
}

void Button::Terminate()
{
}