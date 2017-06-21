#ifndef POLY_PRIM_H
#define POLY_PRIM_H
#pragma once

#include "../MyDefines.h"
#include "../MathLib/Poly2D.h"
#include "Prim.h"
#include "RectPrim.h"

class PolyPrim : public Prim
{
private:

public:
	vector<Point> _vec;
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
	bool _isSelected;
	bool _isDynamicBody;

	PolyPrim(vector<Point> vec);
	~PolyPrim();

	Prim* Clone();
	bool Update();
	void Move(float x, float y);
	void SetCompoundShapeID(int id);
	int GetCompoundShapeID();
	void SetColor(int r, int g, int b, int a);
	bool Contains(float x, float y);
	void SetSelect(bool select);
	Point GetCenter();
	bool IsStaticBody();
	bool IsDynamicBody();
	void Draw();
};

#endif
