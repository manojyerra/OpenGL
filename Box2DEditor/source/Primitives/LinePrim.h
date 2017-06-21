#ifndef LINE_PRIM_H
#define LINE_PRIM_H
#pragma once

#include "../MyDefines.h"
#include "Prim.h"
#include "RectPrim.h"
#include "../MathLib/Point.h"

class LinePrim : public Prim
{
private:
	void AddRotate(float addAngle);
	int _compoundShapeID;

public:
	float _x[2];
	float _y[2];
	
	int _r;
	int _g;
	int _b;
	int _a;

	RectPrim* _rectPrims[3];
	float _rw;
	float _rh;
	bool _isPrimActive;
	int _activeRectIndex;
	bool _isSelected;
	bool _isDynamicBody;
	bool _actionPerformed;

	LinePrim(float x1, float y1, float x2, float y2);
	~LinePrim();

	bool Update();
	void Move(float x, float y);
	void SetColor(int r, int g, int b, int a);
	bool Contains(float x, float y);
	void SetSelect(bool select);
	void SetCompoundShapeID(int id);
	int GetCompoundShapeID();
	bool IsStaticBody();
	Point GetCenter();
	bool IsDynamicBody();
	void Draw();
	Prim* Clone();
	static void Draw(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color);
};

#endif
