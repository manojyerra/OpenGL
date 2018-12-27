#ifndef RECT_PRIM_H
#define RECT_PRIM_H
#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif

#include "gl/gl.h"
//#include "Component.h"

class RectPrim //: public Component
{
private:
	float _x;
	float _y;
	float _w;
	float _h;

	float _borderWidth;

	float _cx;
	float _cy;
	float _cw;
	float _ch;
	bool _clipEnable;

	int _color;
	int _red;
	int _green;
	int _blue;
	int _alpha;

	int _color2;
	int _red2;
	int _green2;
	int _blue2;
	int _alpha2;

	bool _fill;
	bool _flip;

	void DrawAndFill();

public:
	RectPrim();
	~RectPrim();

	void Init(float x, float y, float width, float height, int borderWidth=1);
	void Init(float x, float y, float width, float height, bool fill);

	void Update();

	void SetFlip(bool flip);
	void SetClipArea(int x, int y, int w, int h);

	void SetBorderWidth(int borderWidth);

	void SetColor(int red, int green, int blue, int alpha);
	void SetColor(int color);
	void SetBottomColor(int red, int green, int blue, int alpha);

	void SetAlpha(int alpha);
	int GetAlpha();

	void SetX(float x);
	void SetY(float y);
	void SetWidth(float w);
	void SetHeight(float h);
	void SetPos(float X, float Y);
	void SetSize(float w, float h);
	void SetBounds(float x, float y, float width, float height);

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	void Draw();
	static void Draw(float x, float y, float w, float h, int red=255, int green=255, int blue=255, int alpha=255, bool wireFrame = false);

	void Terminate();

	bool Contains(float X, float Y);
};
#endif
