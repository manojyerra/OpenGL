#include "VertexBufUtils.h"
#include "math.h"

Point VertexBufUtils::CalcCenter(float* vertexBuf, int arrSize)
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

	return Point((minX + maxX)/2.0, (minY + maxY)/2.0, (minZ + maxZ)/2.0);
}


void VertexBufUtils::RotateBufXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng)
{
	xAng = xAng * DEG_RAD;
	yAng = yAng * DEG_RAD;
	zAng = zAng * DEG_RAD;

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float xx = 0;
		float yy = 0;
		float zz = 0;
		
		yy = y;
		zz = z;
		y = yy*cos(xAng) - zz*sin(xAng);
		z = yy*sin(xAng) + zz*cos(xAng);

		zz = z;
		xx = x;
		z = zz*cos(yAng) - xx*sin(yAng);
		x = zz*sin(yAng) + xx*cos(yAng);

		xx = x;
		yy = y;
		x = xx*cos(zAng) - yy*sin(zAng);
		y = xx*sin(zAng) + yy*cos(zAng);

		vertexBuf[i+0] = x;
		vertexBuf[i+1] = y;
		vertexBuf[i+2] = z;
	}
}

void VertexBufUtils::RotateBufZYX(float* vertexBuf, int arrSize, float zAng, float yAng, float xAng)
{
	xAng = xAng * DEG_RAD;
	yAng = yAng * DEG_RAD;
	zAng = zAng * DEG_RAD;

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float xx = 0;
		float yy = 0;
		float zz = 0;

		xx = x;
		yy = y;
		x = xx*cos(zAng) - yy*sin(zAng);
		y = xx*sin(zAng) + yy*cos(zAng);

		zz = z;
		xx = x;
		z = zz*cos(yAng) - xx*sin(yAng);
		x = zz*sin(yAng) + xx*cos(yAng);

		yy = y;
		zz = z;
		y = yy*cos(xAng) - zz*sin(xAng);
		z = yy*sin(xAng) + zz*cos(xAng);

		vertexBuf[i+0] = x;
		vertexBuf[i+1] = y;
		vertexBuf[i+2] = z;
	}
}

void VertexBufUtils::Subtract(float* vertexBuf, int arrSize, Point move)
{
	for(int i=0; i<arrSize; i+=3)
	{
		vertexBuf[i+0] -= move.x;
		vertexBuf[i+1] -= move.y;
		vertexBuf[i+2] -= move.z;
	}
}

void VertexBufUtils::Add(float* vertexBuf, int arrSize, Point move)
{
	for(int i=0; i<arrSize; i+=3)
	{
		vertexBuf[i+0] += move.x;
		vertexBuf[i+1] += move.y;
		vertexBuf[i+2] += move.z;
	}
}

void VertexBufUtils::RotateBufX(float* vertexBuf, int arrSize, float xAng)
{
	xAng = xAng * DEG_RAD;

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float yy = y;
		float zz = z;
		y = yy*cos(xAng) - zz*sin(xAng);
		z = yy*sin(xAng) + zz*cos(xAng);

		vertexBuf[i+0] = x;
		vertexBuf[i+1] = y;
		vertexBuf[i+2] = z;
	}
}

void VertexBufUtils::RotateBufY(float* vertexBuf, int arrSize, float yAng)
{
	yAng = yAng * DEG_RAD;

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float zz = z;
		float xx = x;
		z = zz*cos(yAng) - xx*sin(yAng);
		x = zz*sin(yAng) + xx*cos(yAng);

		vertexBuf[i+0] = x;
		vertexBuf[i+1] = y;
		vertexBuf[i+2] = z;
	}
}

void VertexBufUtils::RotateBufZ(float* vertexBuf, int arrSize, float zAng)
{
	zAng = zAng * DEG_RAD;

	for(int i=0; i<arrSize; i+=3)
	{
		float x = vertexBuf[i+0];
		float y = vertexBuf[i+1];
		float z = vertexBuf[i+2];

		float xx = x;
		float yy = y;
		x = xx*cos(zAng) - yy*sin(zAng);
		y = xx*sin(zAng) + yy*cos(zAng);

		vertexBuf[i+0] = x;
		vertexBuf[i+1] = y;
		vertexBuf[i+2] = z;
	}
}

void VertexBufUtils::MulBufWithMatrix(float* vertexBuf, int bufSize, float* matrix)
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