#include "BoundingCylinder.h"
#include "MathLib/MatrixLib/MatrixUtil.h"
#include "Util/VertexBufUtils.h"
#include "math.h"
#include "Def.h"

BoundingCylinder::BoundingCylinder()
{
	_radius = 0;
	_height = 0;
	_volume = 0;

	for(int i=0;i<16;i++)
		_mat[i] = 0;

	_mat[0] = _mat[5] = _mat[10] = _mat[15] = 1.0f;
}

BoundingCylinder::~BoundingCylinder()
{
}

float BoundingCylinder::GetRadius() { return _radius; }
float BoundingCylinder::GetHeight() { return _height; }
float* BoundingCylinder::GetMat() { return _mat; }
float BoundingCylinder::GetVolume() { return _volume; }
Point BoundingCylinder::GetScale()
{
	return Point(_radius, _radius, _height);
}

void BoundingCylinder::CalcBoundingCylinder(float* vertexBuf, int arrSize, float* boxMat, Point boxWHD)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize*sizeof(float));
	float wayMat[16];
	Point info(0,0,0);
	
	float a[16];
	MatrixUtil::InvertMatrix(boxMat, a);
	VertexBufUtils::MulBufWithMatrix(localVertexBuf, arrSize, a);
	
	info = Way_One(localVertexBuf, arrSize, boxWHD, boxMat, wayMat);
	_radius = info.x;
	_height = info.y;
	_volume = info.z;
	for(int i=0;i<16;i++)
		_mat[i] = wayMat[i];

	info = Way_Two(localVertexBuf, arrSize, boxMat, wayMat);
	if(info.z < _volume)
	{
		_radius = info.x;
		_height = info.y;
		_volume = info.z;		
		for(int i=0; i<16; i++)
			_mat[i] = wayMat[i];
	}
	
	float* tempBuf = new float[arrSize];
	memcpy(tempBuf, vertexBuf, arrSize*sizeof(float));
	Point center = VertexBufUtils::CalcCenter(tempBuf, arrSize);
	VertexBufUtils::Subtract(tempBuf, arrSize, center);
	float tempBoxMat[16];
	MatrixUtil::GetTransArray(center.x, center.y, center.z, tempBoxMat);

	info = Way_Two(tempBuf, arrSize, tempBoxMat, wayMat);	
	if(info.z < _volume)
	{
		_radius = info.x;
		_height = info.y;
		_volume = info.z;

		for(int i=0;i<16;i++)
			_mat[i] = wayMat[i];
	}

	delete[] localVertexBuf;
	delete[] tempBuf;
}

Point BoundingCylinder::Way_One(float* localVertexBuf, int arrSize, Point boxWHD, float* boxMat, float* mat)
{
	float radius = 0;
	float height = 0;
	float volume = 0;
	
	float firRot[3];
	
	float w = boxWHD.x;
	float h = boxWHD.y;
	float d = boxWHD.z;
	

	//height as height.

	float rPow2 = 0;
	
	for(int i=0;i<arrSize; i+=3)
	{
		float x = localVertexBuf[i+0];
		float z = localVertexBuf[i+2];

		if(rPow2 < x*x + z*z)
			rPow2 = x*x + z*z;
	}

	radius = sqrt(rPow2);
	height = h;
	volume = PI_VAL * radius * radius * height;
	firRot[0] = 0;
	firRot[1] = 0;
	firRot[2] = 0;


	//width as height.

	rPow2 = 0;

	for(int i=0; i<arrSize; i+=3)
	{
		float y = localVertexBuf[i+1];
		float z = localVertexBuf[i+2];

		if(rPow2 < y*y + z*z)
			rPow2 = y*y + z*z;
	}

	if(PI_VAL * rPow2 * w < volume)
	{
		radius = sqrt(rPow2);
		height = w;
		volume = PI_VAL * radius * radius * height;
		firRot[0] = 0;
		firRot[1] = 0;
		firRot[2] = 90;
	}

	//depth as height.

	rPow2 = 0;

	for(int i=0; i<arrSize; i+=3)
	{
		float x = localVertexBuf[i+0];
		float y = localVertexBuf[i+1];

		if(rPow2 < x*x + y*y)
			rPow2 = x*x + y*y;
	}

	if(PI_VAL * rPow2 * d < volume)
	{
		radius = sqrt(rPow2);
		height = d;
		volume = PI_VAL * radius * radius * height;
		firRot[0] = 90;
		firRot[1] = 0;
		firRot[2] = 0;
	}

	float firstRotArr[16];
	MatrixUtil::GetXYZRotArray(-firRot[0], -firRot[1], -firRot[2], firstRotArr);
	MatrixUtil::MultMat(boxMat, firstRotArr, mat);

	return Point(radius, height, volume);
}

Point BoundingCylinder::Way_Two(float* localVertexBuf, int arrSize, float* boxMat, float* mat)
{	
	float radius = 0;
	float height = 0;
	float volume = -1;
	
	float cosArr[360];
	float sinArr[360];
	
	for(int i=0; i<360; i++)
	{
		cosArr[i] = cos(i * DEG_RAD);
		sinArr[i] = sin(i * DEG_RAD);
	}

	int numLoops = 6;
	int needSpeed = 0;
			
	if(needSpeed == 1)
		numLoops = 7;

	float delta = 0;
	Point start(0,0,0);
	Point end(360,360,360);
	
	float rot[3];
	float trans[3];

	for(int i=0;i<3;i++)
	{
		rot[i] = 0;
		trans[i] = 0;
	}
	
	for(int loop=0; loop<numLoops; loop++)
	{
		if(loop != 0)
		{
			start = Point(rot[0]-delta, rot[1]-delta, rot[2]-delta);
			end = Point(rot[0]+delta, rot[1]+delta, rot[2]+delta);
		}

		if(numLoops == 6)
		{
			if(loop == 0)	delta = 30;
			else if(loop == 1) delta = 20;
			else if(loop == 2) delta = 10;
			else if(loop == 3) delta = 5;
			else if(loop == 4) delta = 2;
			else if(loop == 5) delta = 1;
		}
		else if(numLoops == 7)
		{
			if(loop == 0) delta = 60;
			else if(loop == 1) delta = 30;
			else if(loop == 2) delta = 15;
			else if(loop == 3) delta = 7;
			else if(loop == 4) delta = 4;
			else if(loop == 5) delta = 2;
			else if(loop == 6) delta = 1;
		}

		for(float zAng=start.z; zAng<end.z; zAng+=delta)
		{
			for(float yAng=start.y; yAng<end.y; yAng+=delta)
			{
				for(float xAng=start.x; xAng<end.x; xAng+=delta)
				{
					float minY = 0;
					float maxY = 0;
					float rPow2 = 0;

					for(int i=0; i<arrSize; i+=3)
					{
						float x = localVertexBuf[i+0];
						float y = localVertexBuf[i+1];
						float z = localVertexBuf[i+2];

						float angleX = xAng;
						float angleY = yAng;
						float angleZ = zAng;

						if(xAng < 0) angleX = 360+xAng;
						if(yAng < 0) angleY = 360+yAng;
						if(zAng < 0) angleZ = 360+zAng;

						float xx = 0;
						float yy = 0;
						float zz = 0;

						yy = y;
						zz = z;
						y = yy*cosArr[(int)angleX] - zz*sinArr[(int)angleX];
						z = yy*sinArr[(int)angleX] + zz*cosArr[(int)angleX];

						zz = z;
						xx = x;
						z = zz*cosArr[(int)angleY] - xx*sinArr[(int)angleY];
						x = zz*sinArr[(int)angleY] + xx*cosArr[(int)angleY];

						xx = x;
						yy = y;
						x = xx*cosArr[(int)angleZ] - yy*sinArr[(int)angleZ];
						y = xx*sinArr[(int)angleZ] + yy*cosArr[(int)angleZ];

						if(i != 0)
						{
							if(y < minY) minY = y;
							if(y > maxY) maxY = y;
						}
						else
						{
							minY = maxY = y;
						}

						if(rPow2 < x*x + z*z)
							rPow2 = x*x + z*z;
					}

					float h = maxY - minY;
					if(h < 0)
						h = -h;
						
					float area = PI_VAL * rPow2 * h;
					if(area < 0)
						area = -area;

					if(area < volume || volume < 0)
					{
						volume = area;
						radius = sqrt(rPow2);
						height = h;

						rot[0] = xAng;
						rot[1] = yAng;
						rot[2] = zAng;

						trans[0] = 0;
						trans[1] = (minY + maxY) / 2.0f;
						trans[2] = 0;
					}
				}
			}
		}
	}
	
	float firstRotMat[16];
	float firstTransMat[16];
	
	MatrixUtil::GetXYZRotArray(-rot[0], -rot[1], -rot[2], firstRotMat);
	MatrixUtil::GetTransArray(trans[0], trans[1], trans[2], firstTransMat);
	
	float afterRotMat[16];
	MatrixUtil::MultMat(boxMat, firstRotMat, afterRotMat);
	MatrixUtil::MultMat(afterRotMat, firstTransMat, mat);

	return Point(radius, height, volume);
}