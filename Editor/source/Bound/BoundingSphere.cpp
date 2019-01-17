#include "BoundingSphere.h"
#include "MathLib/MatrixLib/MatrixUtil.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

BoundingSphere::BoundingSphere()
{
	radius = 0;
	volume = 0;
	memset(mat, 0, 16*sizeof(float));
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::CalcBoundingSphere(float* vertexBuf, int arrSize)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize*sizeof(float));

	Point initialTrans = VertexBufUtils::CalcCenter(localVertexBuf, arrSize);
	VertexBufUtils::Subtract(localVertexBuf, arrSize, initialTrans);
        
	float rpow2 = 0;
	for(int i=0; i<arrSize; i+=3)
	{
		float x = localVertexBuf[i+0];
		float y = localVertexBuf[i+1];
		float z = localVertexBuf[i+2];

		float temp = x*x + y*y + z*z;
		if(rpow2 < temp)
			rpow2 = temp;
	}

	radius = sqrt(rpow2);
	volume = (4.0f/3.0f) * PI_VAL * radius * radius * radius;

	delete[] localVertexBuf;

	MatrixUtil::GetTransArray(initialTrans.x, initialTrans.y, initialTrans.z, mat);
}

float BoundingSphere::GetRadius()
{
    return radius;
}

Point BoundingSphere::GetScale()
{
	return Point(radius, radius, radius);
}

float BoundingSphere::GetVolume()
{
	return volume;
}

float* BoundingSphere::GetMat()
{
	return mat;
}
