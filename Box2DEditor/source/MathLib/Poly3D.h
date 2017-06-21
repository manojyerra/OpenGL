#pragma once

#include "Point.h"
#include <vector>
using namespace std;

class Poly3D
{
private:

public:
	vector<Point> pointsVec;

	Poly3D(vector<Point> vec);
	~Poly3D();

	bool Contains(Point* p);
	bool Contains( float x, float y, float z );

	float GetMinX();
	float GetMinY();
	float GetMaxX();
	float GetMaxY();
};
