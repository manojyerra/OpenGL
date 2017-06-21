#ifndef RECT_POLY_PRIM_H
#define RECT_POLY_PRIM_H
#pragma once

#include "../MyDefines.h"
#include "../MathLib/Poly2D.h"
#include "Prim.h"
#include "RectPrim.h"

class RectPolyPrim : public Prim
{
private:
	void AddRotate(float addAngle);

public:
	int _r;
	int _g;
	int _b;
	int _a;
	int _compoundShapeID;

	Poly2D* _poly2D;

	vector<RectPrim*> _rectPrims;
	float _rw;
	float _rh;
	bool _isPrimActive;
	int _activeRectIndex;
	int _activeRectIndexToRotate;
	bool _isSelected;
	bool _isDynamicBody;
	bool _actionPerformed;
	vector<Point> _vec;

	RectPolyPrim(float x, float y, float w, float h);
	RectPolyPrim(vector<Point> vec);

	~RectPolyPrim();

	Prim* Clone();
	bool Update();
	Point GetCenter();
	void Move(float x, float y);
	void SetCompoundShapeID(int id);
	int GetCompoundShapeID();
	void SetRectPos();
	void SetColor(int r, int g, int b, int a);
	bool Contains(float x, float y);
	void SetSelect(bool select);
	bool IsStaticBody();
	bool IsDynamicBody();
	void Draw();
	void Terminate();
};

#endif
