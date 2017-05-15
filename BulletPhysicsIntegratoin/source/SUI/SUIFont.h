#ifndef SUIFont_H
#define SUIFont_H
#pragma once

#include <vector>
#include <string>
#include "SUIBatch.h"
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

class SUIFont
{
private:
	static SUIFont* _ref;
	SUIFont();
	~SUIFont();

	vector<LetterInfo*> _normalFontVec;

	float _unitWidth;
	int _r,_g,_b,_a;

	int u[127];
	int v[127];
	int cw[127];
	int ch[127];

	float _borderX1;
	float _borderX2;

	float _horGap;
	
	int _imgW;
	int _imgH;
	unsigned int _textureID;


	SUIBatch* _data;
	bool _enableBatch;

public:
	static SUIFont* GetInstance();
	static void DeleteInstance();

	void SetRotation(float angleX, float angleY, float angleZ);

	void SetColor(int r, int g, int b, int a);
	void SetColor(unsigned int color);

	float GetFontSize();
	float GetLength(string text, float fontSize);

	void HorBorder(float x1, float x2);

	void EnableBatch();
	void DrawBatch();

	void Begin();
	//void Draw(int number,	float xPos, float yPos, float fontSize);
	//void Draw(char ch,		float xPos, float yPos, float fontSize);
	void Draw(string text,	float xPos, float yPos, float fontSize);

	void DrawFromLeft(string text, float xPos, float yPos, float fontSize);
	void DrawFromRight(string text, float xPos, float yPos, float fontSize);
	void DrawFromCenter(string text, float xPos, float yPos, float fontSize);
	void End();

	void FillUVInfo_21();
	void FillUVInfo_Baskerville_bold_16();
	void FillUVInfo_Baskerville_plain_16();
	void FillUVInfo_Baskerville_bold_14();
	void FillUVInfo_Baskerville_plain_14();
	void FillUVInfo_Helvetica_plain_12();
	void FillUVInfo_GillSans_plain_12();
	void FillUVInfo_GillSans_plain_14();
	void FillUVInfo_Verdana_plain_14();
	void FillUVInfo_Verdana_plain_12();
};

#endif
