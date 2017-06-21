#ifndef UIFONT_H
#define UIFONT_H
#pragma once

#include "TextureManager.h"
#include "AddData.h"

#include <vector>
#include <string>
using namespace std;

class StringInfo
{
public:
	string text;
	float xPos;
	float yPos;
	float zPos;
	float width;
	float height;
	float horGap;
};


class LetterInfo
{
public:
	float x,y,w,h,u,v,cw,ch;

	LetterInfo(){}
	~LetterInfo(){}

	void SetBounds(float xx, float yy, float ww, float hh)
	{
		x = xx;
		y = yy;
		w = ww;
		h = hh;
	}

	void SetUVBounds(float U, float V, float CW, float CH)
	{
		u = U;
		v = V;
		cw = CW;
		ch = CH;
	}
};

class UIFont
{
private:
	static UIFont* _ref;
	UIFont();
	~UIFont();

	vector<LetterInfo*> _normalFontVec;
	vector<LetterInfo*> _newFontVec;

	AddData* _addData;

	TextureInfo* _textureInfo;
	TextureInfo* _textureInfoNew;

	int _r,_g,_b,_a;

	int u[127];
	int v[127];
	int cw[127];
	int ch[127];

	int u1[127];
	int v1[127];
	int cw1[127];
	int ch1[127];

	int _fontType;

public:

	static const int FONT_NEW = 1;

	static UIFont* GetInstance();
	static void DeleteInstance();
	
	void SetColor(int r, int g, int b, int a);
	void SetBorderColor(int r, int g, int b, int a);
	void SetColor(unsigned int color);
	void SetBorderColor(unsigned int borderColor);

	float GetLength(string text, float fontSize, float horGap);

	void Begin();
	void Begin(int fontType);
	void Draw(int number,	float xPos, float yPos, float fontSize, float horGap);
	void Draw(char ch,		float xPos, float yPos, float fontSize, float horGap);
	void Draw(string text,	float xPos, float yPos, float fontSize, float horGap, float verGap);
	void DrawFromCenter(string text, float xPos, float yPos, float fontSize, float horGap);
	void End();

	void FillUVInfo();
};

#endif
