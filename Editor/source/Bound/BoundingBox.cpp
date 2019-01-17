#include "BoundingBox.h"
#include "MathLib/MatrixLib/MatrixUtil.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

BoundingBox::BoundingBox()
{
	memset(_mat, 0, 16*sizeof(float));
	_mat[0] = _mat[5] = _mat[10] = _mat[15] = 1.0f;
	_volume = 0;
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::CalcBoundingBox(float* vertexBuf, int arrSize)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize*sizeof(float));

	Point initialTrans = VertexBufUtils::CalcCenter(localVertexBuf, arrSize);
	VertexBufUtils::Subtract(localVertexBuf, arrSize, initialTrans);

	float cosArr[360];
	float sinArr[360];

	for(int i=0; i<360; i++)
	{
		float angleInRadians = i * DEG_RAD;

		cosArr[i] = cos(angleInRadians);
		sinArr[i] = sin(angleInRadians);
	}

	_volume = -1.0f;
	int numLoops = 6;
	int needSpeed = 0;
			
	if(needSpeed == 1)
		numLoops = 7;

	float delta = 0.0f;
	Point start(0,0,0);
	Point end(360,360,360);

	float rot[3];
	float trans[3];
		
	for(int loop = 0; loop < numLoops; loop++)
	{
		if(loop != 0)
		{
			start = Point(rot[0]-delta, rot[1]-delta, rot[2]-delta);
			end = Point(rot[0]+delta, rot[1]+delta, rot[2]+delta);
		}

        if(numLoops == 6)
		{
            if(loop == 0)		delta = 30;
            else if(loop == 1)	delta = 20;
            else if(loop == 2)	delta = 10;
            else if(loop == 3)	delta = 5;
            else if(loop == 4)	delta = 2;
            else if(loop == 5)	delta = 1;
		}
        else if(numLoops == 7)
		{
            if(loop == 0)		delta = 60;
            else if(loop == 1)	delta = 30;
            else if(loop == 2)	delta = 15;
            else if(loop == 3)	delta = 7;
            else if(loop == 4)	delta = 4;
            else if(loop == 5)	delta = 2;
            else if(loop == 6)	delta = 1;
		}

		for(float zAng=start.z; zAng<end.z; zAng+=delta)
		{
			for(float yAng=start.y; yAng<end.y; yAng+=delta)
			{
                for(float xAng=start.x; xAng<end.x; xAng+=delta)
				{
                    float minX = 0;
                    float maxX = 0;
                    float minY = 0;
                    float maxY = 0;
                    float minZ = 0;
                    float maxZ = 0;

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
                            if(x < minX) minX = x;
                            if(y < minY) minY = y;
                            if(z < minZ) minZ = z;

                            if(x > maxX) maxX = x;
                            if(y > maxY) maxY = y;
                            if(z > maxZ) maxZ = z;
						}
                        else
						{
                            minX = maxX = x;
                            minY = maxY = y;
                            minZ = maxZ = z;
						}
					}

                    float w = maxX - minX;
                    float h = maxY - minY;
                    float d = maxZ - minZ;

                    float area = w*h*d;

                    if(area < 0)
                        area = -area;

                    if(area < _volume || _volume < 0)
					{
                        _volume = area;

                        _scale.x = abs(w);
						_scale.y = abs(h);
                        _scale.z = abs(d);

                        rot[0] = xAng;
                        rot[1] = yAng;
                        rot[2] = zAng;

                        trans[0] = (minX+maxX)/2.0;
                        trans[1] = (minY+maxY)/2.0;
                        trans[2] = (minZ+maxZ)/2.0;
					}
				}
			}
		}
	}

	delete[] localVertexBuf;

	float trans1[16];
	float xyzRot[16];
	float trans2[16];

	MatrixUtil::GetTransArray(initialTrans.x, initialTrans.y, initialTrans.z, trans1);
	MatrixUtil::GetXYZRotArray(-rot[0], -rot[1], -rot[2], xyzRot);
	MatrixUtil::GetTransArray(trans[0], trans[1], trans[2], trans2);

	float result1[16];

	MatrixUtil::MultMat(trans1, xyzRot, result1);
	MatrixUtil::MultMat(result1, trans2, _mat);
}

float BoundingBox::GetVolume()
{
	return _volume;
}

float* BoundingBox::GetMat()
{
	return _mat;
}

Point BoundingBox::GetScale()
{
	return _scale;
}
