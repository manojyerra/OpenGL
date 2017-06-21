#ifndef PHY_COMPOUND_SHAPE_H
#define PHY_COMPOUND_SHAPE_H
#pragma once

#include "../Box2D/Box2D.h"
#include "PhyShape.h"
#include "../MathLib/Point.h"
#include "../Primitives/Prim.h"

#include <vector>
using namespace std;

class PhyCompoundShape : public PhyShape
{
private:
	b2BodyDef bodyDef;
	int _numShapes;
	Point _pos;
	vector<Prim*> _primVec;

public:
	b2Body* _body;
	PhyCompoundShape();
	~PhyCompoundShape();

	int GetNumParts();
	void SetBodyType(int bodyType);
	void AddCircle(float cx, float cy, float radius);
	void AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void AddPoly(vector<Point> vec);
	b2Body* GetBody();
	void Update();
	void Draw();
};

#endif

