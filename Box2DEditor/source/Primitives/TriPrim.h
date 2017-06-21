#ifndef TRI_PRIM_H
#define TRI_PRIM_H
#pragma once

#include "../MyDefines.h"
#include "Prim.h"
#include "../MathLib/Triangle2D.h"
#include "RectPrim.h"

class TriPrim : public Prim
{
private:
	void AddRotate(float addAngle);
	int _compoundShapeID;

public:
	float _x[3];
	float _y[3];
	
	int _r;
	int _g;
	int _b;
	int _a;

	Triangle2D* _triangle2D;

	RectPrim* _rectPrims[3];
	float _rw;
	float _rh;
	bool _isTriActive;
	int _activeRectIndex;
	bool _isSelected;
	bool _isDynamicBody;
	bool _actionPerformed;

	TriPrim(float x1, float y1, float x2, float y2, float x3, float y3);
	~TriPrim();

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
