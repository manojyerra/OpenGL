#ifndef BoundingCone_H
#define BoundingCone_H

#include "BoundingSphere.h"
#include "MathLib/MatrixLib/MatrixUtil.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

class BoundingCone
{
private:
	float radius;
	float height;
	float volume;
	float mat[16];

public:
	BoundingCone();
	~BoundingCone();
	void CalcBoundingCone(float* vertexBuf, int arrSize, float* cylinderMat, float cylinderH);
	Point WayOne(float* vertexBuf, int arrSize, float cylinderH);
	float GetRadius();
	float GetHeight();
	Point GetScale();
	float GetVolume();
	float* GetMat();
};

#endif