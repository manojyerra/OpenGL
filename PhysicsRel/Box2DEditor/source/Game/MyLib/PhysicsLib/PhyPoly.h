#ifndef PHY_TRI_H
#define PHY_TRI_H
#pragma once

#include "Box2D.h"
#include "PhyShape.h"
#include "Point.h"

#include <vector>
using namespace std;

class PhyPoly : public PhyShape
{
private:
	b2Body* _body;
	float _vertex[16];
	int _vertexCount;

	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	void Init(float* vertex, int vertexCount, int bodyType, float density);
	
	bool IsConvexPolygon(vector<Point>* pointsVec);
	
public:
	PhyPoly(float* vertex, int vertexCount, int bodyType, float density=1.0f);
	PhyPoly(vector<Point> vec, int bodyType, float density=1.0f);
	~PhyPoly();
	
	void SetColor(int r, int g, int b, int a);
	void Update();
	float GetX();
	float GetY();
	float GetAngle();
	b2Body* GetBody();
	void Draw();

	static void ArrangeInCWDirection(vector<Point>* pointsVec);
};

#endif
