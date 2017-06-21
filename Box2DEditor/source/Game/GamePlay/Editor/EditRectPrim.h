#ifndef RECT_POLY_PRIM_H
#define RECT_POLY_PRIM_H
#pragma once

#include "Includes.h"
#include "Poly2D.h"
#include "EditPrim.h"
#include "RectPrim.h"

class EditRectPrim : public EditPrim
{
private:
	Poly2D* _poly2D;
	vector<RectPrim*> _rectPrims;
	float _rw;
	float _rh;
	bool _isPrimActive;
	int _activeRectIndex;
	int _activeRectIndexToRotate;
	bool _isSelected;

	void AddRotate(float addAngle);
	void Init(vector<Point> vec);
	float Dist(float x1, float y1, float x2, float y2);

public:
	vector<Point> vec;

	EditRectPrim(float x, float y, float w, float h);
	EditRectPrim(vector<Point> vec);

	~EditRectPrim();

	EditPrim* Clone();
	void Update();
	Point GetCenter();
	void Move(float x, float y);
	void AddRotation(float angleInDegrees, float pivotX, float pivotY);
	void SetRectPos();
	bool Contains(float x, float y);
	bool CollidesWithRect(float x, float y, float w, float h);
	void SetSelect(bool select);
	bool IsSelected();
	void Draw();
};

#endif
