#include "TransformVertexBuf.h"
#include "math.h"

CVector3 TransformVertexBuf::CalcCenter(float* vertexBuf, int arrSize)
{
	float minX = vertexBuf[0];
	float minY = vertexBuf[1];
	float minZ = vertexBuf[2];

	float maxX = vertexBuf[0];
	float maxY = vertexBuf[1];
	float maxZ = vertexBuf[2];

	for(int i=3; i<arrSize; i+=3)
	{
		if(vertexBuf[i+0] < minX)	minX = vertexBuf[i+0];
		if(vertexBuf[i+1] < minY)	minY = vertexBuf[i+1];
		if(vertexBuf[i+2] < minZ)	minZ = vertexBuf[i+2];

		if(vertexBuf[i+0] > maxX)	maxX = vertexBuf[i+0];
		if(vertexBuf[i+1] > maxY)	maxY = vertexBuf[i+1];
		if(vertexBuf[i+2] > maxZ)	maxZ = vertexBuf[i+2];
	}

	return CVector3((minX + maxX)/2.0, (minY + maxY)/2.0, (minZ + maxZ)/2.0);
}


void TransformVertexBuf::RotateBufXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng)
{
	float cosOfXAng = cos(xAng * DEG_RAD);
	float cosOfYAng = cos(yAng * DEG_RAD);
	float cosOfZAng = cos(zAng * DEG_RAD);

	float sinOfXAng = sin(xAng * DEG_RAD);
	float sinOfYAng = sin(yAng * DEG_RAD);
	float sinOfZAng = sin(zAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float x1 = x;
		float y1 = y*cosOfXAng - z*sinOfXAng;
		float z1 = y*sinOfXAng + z*cosOfXAng;

		float y2 = y1;
		float z2 = z1*cosOfYAng - x1*sinOfYAng;
		float x2 = z1*sinOfYAng + x1*cosOfYAng;

		vertexBuf[i+0] = x2*cosOfZAng - y2*sinOfZAng;
		vertexBuf[i+1] = x2*sinOfZAng + y2*cosOfZAng;
		vertexBuf[i+2] = z2;
	}
}


void TransformVertexBuf::RotateBufZYX(float* vertexBuf, int arrSize, float zAng, float yAng, float xAng)
{
	float cosOfXAng = cos(xAng * DEG_RAD);
	float cosOfYAng = cos(yAng * DEG_RAD);
	float cosOfZAng = cos(zAng * DEG_RAD);

	float sinOfXAng = sin(xAng * DEG_RAD);
	float sinOfYAng = sin(yAng * DEG_RAD);
	float sinOfZAng = sin(zAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float z1 = z;
		float x1 = x*cosOfZAng - y*sinOfZAng;
		float y1 = x*sinOfZAng + y*cosOfZAng;

		float y2 = y1;
		float z2 = z1*cosOfYAng - x1*sinOfYAng;
		float x2 = z1*sinOfYAng + x1*cosOfYAng;

		vertexBuf[i+0] = x2;
		vertexBuf[i+1] = y2*cosOfXAng - z2*sinOfXAng;
		vertexBuf[i+2] = y2*sinOfXAng + z2*cosOfXAng;
	}
}


void TransformVertexBuf::RotateBufX(float* vertexBuf, int arrSize, float xAng)
{
	float cosVal = cos(xAng * DEG_RAD);
	float sinVal = sin(xAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		vertexBuf[i+1] = y*cosVal - z*sinVal;
		vertexBuf[i+2] = y*sinVal + z*cosVal;
	}
}


void TransformVertexBuf::RotateBufY(float* vertexBuf, int arrSize, float yAng)
{
	float cosVal = cos(yAng * DEG_RAD);
	float sinVal = sin(yAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float z = vertexBuf[i+2];

		vertexBuf[i+0] = z*sinVal + x*cosVal;
		vertexBuf[i+2] = z*cosVal - x*sinVal;
	}
}


void TransformVertexBuf::RotateBufZ(float* vertexBuf, int arrSize, float zAng)
{
	float cosVal = cos(zAng * DEG_RAD);
	float sinVal = sin(zAng * DEG_RAD);

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];

		vertexBuf[i+0] = x*cosVal - y*sinVal;
		vertexBuf[i+1] = x*sinVal + y*cosVal;
	}
}


void TransformVertexBuf::Add(float* vertexBuf, int arrSize, CVector3 move)
{
	for(int i=0; i<arrSize; i+=3)
	{
		vertexBuf[i+0] += move.x;
		vertexBuf[i+1] += move.y;
		vertexBuf[i+2] += move.z;
	}
}


void TransformVertexBuf::Subtract(float* vertexBuf, int arrSize, CVector3 move)
{
	Add(vertexBuf, arrSize, -move);
}


void TransformVertexBuf::MulBufWithMatrix(float* vertexBuf, int bufSize, float* matrix)
{
	float* a = matrix;

	for(int i=0; i<bufSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];
		
		vertexBuf[i+0] = a[0]*x + a[4]*y + a[8]*z + a[12];
		vertexBuf[i+1] = a[1]*x + a[5]*y + a[9]*z + a[13];
		vertexBuf[i+2] = a[2]*x + a[6]*y + a[10]*z + a[14];
	}
}