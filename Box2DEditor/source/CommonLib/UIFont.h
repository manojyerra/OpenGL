#ifndef UIFONT_H
#define UIFONT_H
#pragma once

#include "Ref.h"
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
	ImageInfo* _imageInfo;

	int _gap;
	float _unitWidth;
	int _r,_g,_b,_a;
	int _borderR,_borderG,_borderB,_borderA;

	//int _cx;
	//int _cy;
	//int _cw;
	//int _ch;

	float _rotX;
	float _rotY;
	float _rotZ;

	int u[127];
	int v[127];
	int cw[127];
	int ch[127];


public:
	static UIFont* GetInstance();
	
	void Init();
	void SetWidth(float width);
	void SetHeight(float height);
	void SetRotation(float angleX, float angleY, float angleZ);

	void SetColor(int r, int g, int b, int a);
	void SetBorderColor(int r, int g, int b, int a);
	void SetColor(unsigned int color);
	void SetBorderColor(unsigned int borderColor);

	void SetHorGap(float horGap);
	void SetVerGap(float verGap);
	//void SetClipArea(int cx, int cy, int cw, int ch);

	void AddString(string text, float xPos, float yPos, float zPos, float width, float height, float horGap);
	float GetLength(string text, float width, float height, float horGap);

	void Begin();
	void Draw(int number,	float xPos, float yPos, float width, float height, float horGap);
	void Draw(char ch,		float xPos, float yPos, float width, float height, float horGap);
	void Draw(string text,	float xPos, float yPos, float width ,float height ,float horGap);
	void DrawAndFit(string text,	float xPos, float yPos, float width ,float height ,float horGap);
	void DrawFromCenter(string text, float xPos, float yPos, float width, float height, float horGap);
	void End();

	void FillUVInfo();

	//void RemoveClipArea();
	void Terminate();
};

#endif