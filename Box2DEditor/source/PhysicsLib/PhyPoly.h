#ifndef PHY_TRI_H
#define PHY_TRI_H
#pragma once

#include "../Box2D/Box2D.h"
#include "PhyShape.h"
#include "../MathLib/Point.h"

#include <vector>
using namespace std;

class PhyPoly : public PhyShape
{
private:

	int _vertexCount;

public:
	float _vertex[16];
	b2Body* _body;
	PhyPoly(float* vertex, int vertexCount, int bodyType);
	PhyPoly(vector<Point> vec, int bodyType);
	~PhyPoly();
	
	void ArrangeInCWDirection(vector<Point>* pointsVec);
	bool IsConvexPolygon(vector<Point>* pointsVec);
	void Update();
	float GetX();
	float GetY();
	float GetAngle();
	b2Body* GetBody();
	void Draw();
};

#endif

