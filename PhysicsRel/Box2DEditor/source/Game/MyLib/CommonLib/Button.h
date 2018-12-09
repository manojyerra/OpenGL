#ifndef BUTTON_H
#define BUTTON_H

#include "Sprite.h"
#include "../Primitives/RectPrim.h"

#include <string>
using namespace std;

class Button
{
private:
	Sprite* _displayImg;

	RectPrim* _rectPrim;

	float _x;
	float _y;
	float _w;
	float _h;

	bool _isClicked;

	unsigned int _color;
	int _state;
	bool _validTouch;

public:
	static const int NORMAL = 1;
	static const int ON_TOUCH = 2;

	Sprite* _normalStateImg;
	Sprite* _onTouchImg;
	Sprite* _disableImg;

	Button(string normalImgPath,float x,float y, float w,float h);
	Button(string normalImgPath,string onTouchImagPath,float x,float y, float w,float h);
	Button(string normalImgPath,string onTouchImagPath, string disableImgPath, float x,float y, float w,float h);

	void Update();	
	void SetEnable(bool enable);
	bool IsEnabled();
	bool IsClicked();
	bool Contains(float x, float y);
	void SetBackgroundColor(unsigned int color);
	void Draw();

	void SetWaveOn(bool waveOn);

	Sprite* GetSprite();
	int GetState();

	void SetRotation(float angle);
	void SetRotation(float angle, int centerX, int centerY);

	void SetX(float x);
	void SetY(float y);
	void SetPos(float x, float y);
	void SetBounds(float x, float y, float w, float h);
	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	~Button();
};

#endif
