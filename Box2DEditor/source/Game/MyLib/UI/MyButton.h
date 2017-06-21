#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include "../CommonLib/Sprite.h"
#include "../Primitives/RectPrim.h"
#include "../CommonLib/UIFont.h"

#include <string>
using namespace std;

class MyButton
{
private:
	string _label;

	//RectPrim* _rectPrim;
	unsigned int _color;

	unsigned int _fontColor;
	int _horGap;
	int _labelFontWidth;
	int _labelFontHeight;

	float _x;
	float _y;
	float _width;
	float _height;

	int _cx;
	int _cy;
	int _cw;
	int _ch;

	int _type;
	bool _isOn;
	bool _isJustNowOn;

public:
	static const int PROPERTY = 1;
	static const int DIALOG = 2;
	static const int MODE = 3;
	static const int ON_OFF = 4;

	static const int WHITE = 1;
	static const int RED = 2;
	static const int BLUE = 3;
	static const int GREEN = 4;
	static const int DGREEN = 5;
	static const int ORANGE = 6;
	static const int PINK = 7;
	static const int CYAN = 8;
	static const int YELLOW = 9;
	static const int PURPLE = 10;

	MyButton(int color, float x, float y, float width, float height);
    ~MyButton();
	bool Update();

	void On();
	void Off();
	bool IsOn();

	void JustNowOn();
	bool IsJustNowOn();

	void SetType(int type);
	int GetType();
	
	bool Contains(float x,float y);

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	void SetY(float y);
	void SetX(float x);
	void SetPos(float x, float y);
	void SetSize(float w, float h);
	void SetBounds(float x, float y, float width, float height);

	void SetColor(int color);
	int GetColor();

	string GetLabel();
	void SetLabel(string label, unsigned int fontColor=0xffffffff, int horGap = -2);
	void SetLabelFontSize(float width, float height);

	void Draw();
};

#endif