#pragma once

#include "Point.h"
#include <vector>
using namespace std;

class Poly2D
{
private:

public:
    vector<Point> pointsVec;

    Poly2D(vector<Point> vec);
	Poly2D(float* vertex, int vertexCount);

    ~Poly2D();

    bool Contains(Point* p);
    bool Contains(float x, float y);
	bool IsConvexPolygon();

    float GetMinX();
    float GetMinY();
    float GetMaxX();
    float GetMaxY();
};