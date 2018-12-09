#ifndef COLORRECT_H
#define COLORRECT_H

#include "EditPrim.h"
#include "AddData.h"

#include <string>
using namespace std;

class ColorRect
{
private:
	unsigned int _color1;
	unsigned int _color2;

	int _r1;
	int _g1;
	int _b1;

	int _r2;
	int _g2;
	int _b2;
	
	bool _transEnable;
	float _timeCount;
	
	float _x;
	float _y;
	float _w;
	float _h;
	
	unsigned int RandColor();

public:
	
	ColorRect(float x, float y, float w, float h);
	~ColorRect();
	
	void SetColorTrans(bool enable);
	void SetColor(unsigned int color);
	void AddTime(float dTime);
	void SetBounds(float x, float y, float w, float h);
	bool Contains(float x, float y);
	bool IsClicked();

	void SetX(float x);

	float GetX();
	float GetY();
	float GetW();
	float GetH();
	
	void Draw();
	void Draw(AddData* addData);
	void Draw(int borderW, unsigned int color);
	void Draw(int borderW, unsigned int color, string text, float fontH, unsigned int fontColor);
};

#endif

