#ifndef UIFONT_H
#define UIFONT_H
#pragma once

#include "Sprite.h"
#include <vector>
#include <string>
using namespace std;


class UIFont
{
private:
	static UIFont* _ref;
	bool _isInit;
	UIFont();

	vector<Sprite*> _normalFontVec;
	vector<Sprite*>	_hudNumbersVec;
	int _gap;

	float _horGap;
	float _verGap;

	int _unitWidth;

	int _width;
	int _height;

	int _r,_g,_b,_a;
	int _borderR,_borderG,_borderB,_borderA;

	int _cx;
	int _cy;
	int _cw;
	int _ch;

	vector<Sprite*> _pspButtons;

public:
	static const int BLACK_BORDER = 1;
	static const int TRANS_BORDER = 2;
	static const int NO_BORDER = 3;
	static const int CUSTOM_BORDER = 4;
	static const int NONE = 5;
	
	~UIFont();
	static UIFont* GetInstance();
	
	void Init();
	void SetWidth(float width);
	void SetHeight(float height);

	void SetColor(int r, int g, int b, int a);
	void SetBorderColor(int r, int g, int b, int a);
	void SetColor(unsigned int color);
	void SetBorderColor(unsigned int borderColor);

	void SetHorGap(float horGap);
	void SetVerGap(float verGap);
	void SetClipArea(int cx, int cy, int cw, int ch);
	int GetLength(string text, int y, float width, float height, float horGap);

	//void DrawFromCenter(string text,int x, int y, float width, float height, int border = BLACK_BORDER);
	void DrawFromCenter(string text,int x, int y, float width, float height, float horGap, int border = BLACK_BORDER);

	void Draw(string text, int x, int y, float width, float height, float horGap=0, int border = BLACK_BORDER);//, bool playSinWave = true);
	void Draw(int number, int x, int y, float width, float height, float horGap=0, int border = BLACK_BORDER);
	void Draw(char ch, int x, int y, float w, float h, float horGap, int border = BLACK_BORDER);

	void RemoveClipArea();

	void Terminate();
};

#endif