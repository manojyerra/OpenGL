#include "MyButton.h"
#include "../CommonLib/UIFont.h"
#include "../CommonLib/Input.h"

MyButton::MyButton(int color, float x, float y, float width, float height)
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

bool MyButton::Update()
{
	return (Input::IsMouseReleased() && Contains((float)Input::MouseX, (float)Input::MouseY));
}

void MyButton::SetType(int type)	
{
	_type = type;
}

int MyButton::GetType()			{ return _type; }
void MyButton::On()				{ _isOn = true; }
void MyButton::Off()				{ _isOn = false;}
bool MyButton::IsOn()				{ return _isOn; }
void MyButton::JustNowOn()		{ _isJustNowOn = true; }
bool MyButton::IsJustNowOn()		{ return _isJustNowOn; }

void MyButton::SetColor(int color)
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

int MyButton::GetColor()
{
	return _color;
}

string MyButton::GetLabel() {return _label;}

void MyButton::SetLabel(string label, unsigned int fontColor, int horGap)
{
	_fontColor = fontColor;
	_label = label;
	_horGap = horGap;
}

void MyButton::SetLabelFontSize(float width, float height)
{
	_labelFontWidth = (int)width;
	_labelFontHeight = (int)height;
}

float MyButton::GetX(){ return _x; }
float MyButton::GetY(){ return _y; }
float MyButton::GetWidth(){ return _width; }
float MyButton::GetHeight(){ return _height; }

void MyButton::SetPos(float x, float y) { SetBounds(x, y, _width, _height); }
void MyButton::SetSize(float width, float height) { SetBounds(_x, _y, width, height); }
void MyButton::SetY(float y)	{ SetBounds(_x,y,_width,_height);}//;_y = y;}
void MyButton::SetX(float x) { SetBounds(x,_y,_width,_height);}//;_y = y;}
void MyButton::SetBounds(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}

bool MyButton::Contains(float xx, float yy)
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


void MyButton::Draw()
{
	if(_type != ON_OFF)
	{
		if(_isOn)
			RectPrim::Draw(_x, _y, _width, _height, 0, 0x00ff00ff);
		else
			RectPrim::Draw(_x, _y, _width, _height, 0, 0xFF0080FF);
	}
	else
	{
		RectPrim::Draw(_x, _y, _width, _height, 0, 0xFF0080FF);
		if(_isOn)
			RectPrim::Draw(_x+1, _y+(_height/2.0f)-8.0f/2.0f, 8, 8, 0, 0x00ff00ff);
	}

	if(_label.compare("") != 0)
	{
		UIFont::GetInstance()->Begin();
		UIFont::GetInstance()->SetColor(_fontColor);
		UIFont::GetInstance()->DrawFromCenter(_label, _x+_width/2, _y+_height/2+2, (float)_labelFontWidth, (float)_labelFontHeight, (float)_horGap);
		UIFont::GetInstance()->End();
	}
	_isJustNowOn = false;
}

MyButton::~MyButton()
{
}