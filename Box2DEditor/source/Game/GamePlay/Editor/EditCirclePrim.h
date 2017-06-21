#ifndef EDIT_CIRCLE_PRIM_H
#define EDIT_CIRCLE_PRIM_H
#pragma once

#include "Includes.h"
#include "EditPrim.h"
#include "CirclePrim.h"
#include "RectPrim.h"

class EditCirclePrim : public CirclePrim, public EditPrim
{
private:
	bool _isPrimActive;
	int _activeRectIndex;
	bool _isSelected;

	float _rw;
	float _rh;
	RectPrim* _rectPrim;

public:
	EditCirclePrim(float cx, float cy, float r);
	~EditCirclePrim();

	void Update();
	void Draw();

	EditPrim* Clone();
	void Move(float x, float y);
	void AddRotation(float angleInDegrees, float pivotX, float pivotY);
	bool Contains(float x, float y);
	bool CollidesWithRect(float x, float y, float w, float h);
	void SetSelect(bool select);
	bool IsSelected();
};

#endif
