#ifndef BoundingSphere_H
#define BoundingSphere_H

#include "Def.h"
#include "Util/VertexBufUtils.h"

class BoundingSphere
{
private:
	float radius;
	float volume;
	float mat[16];

public:
	BoundingSphere();
	~BoundingSphere();

	void CalcBoundingSphere(float* vertexBuf, int arrSize);
	float GetRadius();
	Point GetScale();
	float* GetMat();
	float GetVolume();
};

#endif