#ifndef EDIT_POLY_PRIM_H
#define EDIT_POLY_PRIM_H
#pragma once

#include "Includes.h"
#include "Poly2D.h"
#include "EditPrim.h"
#include "RectPrim.h"

class EditPolyPrim : public EditPrim
{
private:
	unsigned int _color;
	unsigned int _borderColor;

	Poly2D* _poly2D;

	vector<RectPrim*> _rectPrims;
	float _rw;
	float _rh;
	bool _isPrimActive;
	int _activeRectIndex;
	bool _isSelected;
	bool _isBgOpaque;

	void AddRotate(float angle);

public:
	vector<Point> vec;

	EditPolyPrim(vector<Point> vecPoints);
	~EditPolyPrim();

	EditPrim* Clone();
	void SetBgOpaque(bool isBgOpaque);
	void Update();
	void Move(float x, float y);
	void AddRotation(float angleInDegrees, float pivotX, float pivotY);
	bool Contains(float x, float y);
	bool CollidesWithRect(float x, float y, float w, float h);
	void SetSelect(bool select);
	bool IsSelected();
	Point GetCenter();
	void Draw();
};

#endif
