#include "BoundingCone.h"
#include "MathLib/MatrixLib/MatrixUtil.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

BoundingCone::BoundingCone()
{
	radius = 0.0f;
	height = 0.0f;
	volume = 0.0f;
	memset(mat, 0, 16*sizeof(float));
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
}

BoundingCone::~BoundingCone()
{
}

void BoundingCone::CalcBoundingCone(float* vertexBuf, int arrSize, float* cylinderMat, float cylinderH)
{
	float cylMatInvert[16];
	MatrixUtil::InvertMatrix(cylinderMat, cylMatInvert);
	
	for(int loop=0; loop<2; loop++)
	{	
		float* localVertexBuf = new float[arrSize];
		memcpy(localVertexBuf, vertexBuf, arrSize*sizeof(float));
		
		float matLocal[16];
		for(int i=0; i<16; i++)
			matLocal[i] = cylMatInvert[i];

		if(loop == 1)
		{
			float xRotMat[16];
			MatrixUtil::GetXRotArray(180, xRotMat);
			MatrixUtil::MultMat(xRotMat, matLocal,  matLocal);
		}

		matLocal[13] += cylinderH/2.0f;
		VertexBufUtils::MulBufWithMatrix(localVertexBuf, arrSize, matLocal);

		Point info = WayOne(localVertexBuf, arrSize, cylinderH);
		
		if(loop == 0 || info.z < volume)
		{
			radius = info.x;
			height = info.y;
			volume = info.z;
	
			float transMat[16];
			float matLocalInvert[16];
			MatrixUtil::GetTransArray(0,height/2.0,0, transMat);
			MatrixUtil::InvertMatrix(matLocal, matLocalInvert);
			MatrixUtil::MultMat(matLocalInvert, transMat, mat);
		}

		delete[] localVertexBuf;
	}
}

Point BoundingCone::WayOne(float* vertexBuf, int arrSize, float cylinderH)
{
	float radius = 0;
	float height = 0;
	float volume = -1;

	float tempH = cylinderH*9;
	
	for(int i=0; i<1001; i+=10)
	{	
		tempH -= cylinderH/10.0f;
		if(tempH <= cylinderH+0.01)
			break;

		float rPow2 = 0;

		for(int i=0; i<arrSize; i+=3)
		{
			float x = vertexBuf[i+0];
			float y = vertexBuf[i+1];
			float z = vertexBuf[i+2];
			
			float t = -tempH / (y-tempH);

			//collision point on xz plane...
			float cx = x*t;
			float cy = tempH + (y-tempH)*t;
			float cz = z*t;
			
			float temp = cx*cx + cy*cy  + cz*cz;
			
			if(temp > rPow2)
				rPow2 = temp;
		}

		float tempVolume = (1.0/3.0)* tempH * PI_VAL * rPow2;

		if(volume == -1 || tempVolume < volume)
		{
			volume = tempVolume;
			height = tempH;
			radius = sqrt(rPow2);
		}
	}

	return Point(radius, height, volume);
}

float BoundingCone::GetRadius()
{
	return radius;
}

float BoundingCone::GetHeight()
{
	return height;
}

Point BoundingCone::GetScale()
{
	return Point(radius, radius, height);
}

float BoundingCone::GetVolume()
{
	return volume;
}

float* BoundingCone::GetMat()
{
	return mat;
}
