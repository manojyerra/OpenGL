#ifndef EDIT_LINE_PRIM_H
#define EDIT_LINE_PRIM_H

#include "Includes.h"
#include "EditPrim.h"
#include "RectPrim.h"
#include "Point.h"
#include <vector>
using namespace std;

class EditLinePrim : public EditPrim
{
private:
	RectPrim* _rectPrims[3];
	float _rw;
	float _rh;

	bool _isPrimActive;
	int _activeRectIndex;
	bool _isSelected;
	unsigned int _color;
	float _width;

	void AddRotate(float addAngle);
	Point GetCenter();

public:
	vector<Point> vec;

	EditLinePrim(float x1, float y1, float x2, float y2);
	~EditLinePrim();

	void Update();
	void Draw();

	EditPrim* Clone();
	void Move(float x, float y);
	void AddRotation(float angleInDegrees, float pivotX, float pivotY);
	bool Contains(float x, float y);
	bool CollidesWithRect(float x, float y, float w, float h);
	void SetSelect(bool select);
	bool IsSelected();
	void SetLineWidth(float width);
	float GetLineWidth();
};

#endif
