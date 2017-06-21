#ifndef CIRCLE_PRIM_H
#define CIRCLE_PRIM_H
#pragma once

#include "../MyDefines.h"
#include "Prim.h"
#include "RectPrim.h"

class CirclePrim : public Prim
{
	int _red;
	int _green;
	int _blue;
	int _alpha;
	int _compoundShapeID;

	RectPrim* _rectPrim;
	float _rw;
	float _rh;
	bool _isPrimActive;
	int _activeRectIndex;
	bool _isSelected;


public:
	float _cx;
	float _cy;
	float _radius;
	bool _isDynamicBody;

	CirclePrim(float cx, float cy, float r);
	~CirclePrim();

	Prim* Clone();
	bool Update();
	void Move(float x, float y);
	bool Contains(float x, float y);
	void SetCompoundShapeID(int id);
	int GetCompoundShapeID();
	void SetSelect(bool select);
	bool IsStaticBody();
	bool IsDynamicBody();
	void SetColor(int r, int g, int b, int a);
	void SetPos(float cx, float cy);
	void SetRadius(float r);
	void Draw();

};

#endif
