#ifndef PHY_COMPOUND_SHAPE_H
#define PHY_COMPOUND_SHAPE_H
#pragma once

#include "Box2D.h"
#include "PhyShape.h"
#include "Point.h"
#include "EditPrim.h"
#include "EditPolyPrim.h"
#include <vector>
using namespace std;

class PhyCompoundShape : public PhyShape
{
private:
	b2Body* _body;
	b2BodyDef bodyDef;
	int _numShapes;
	Point _pos;
	vector<EditPrim*> _primVec;

public:
	PhyCompoundShape();
	~PhyCompoundShape();

	void AddLine(float x1, float y1, float x2, float y2);
	void AddCircle(float cx, float cy, float radius);
	EditPolyPrim* AddPoly(vector<Point> vec, float density=1.0f);

	b2Body* GetBody();
	void SetBodyType(int bodyType);
	int GetNumParts();
	void Update();
	void Draw();
};

#endif

