#ifndef VertexBufUtils_H
#define VertexBufUtils_H

#include "Def.h"
#include "MathLib/Point.h"

class VertexBufUtils
{
public:
	static Point CalcCenter(float* vertexBuf, int arrSize);
	static void RotateBufXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);
	static void RotateBufZYX(float* vertexBuf, int arrSize, float zAng, float yAng, float xAng);
	static void Subtract(float* vertexBuf, int arrSize, Point move);
	static void Add(float* vertexBuf, int arrSize, Point move);
	static void RotateBufX(float* vertexBuf, int arrSize, float xAng);
	static void RotateBufY(float* vertexBuf, int arrSize, float yAng);
	static void RotateBufZ(float* vertexBuf, int arrSize, float zAng);
	static void MulBufWithMatrix(float* vertexBuf, int bufSize, float* matrix);
};
#endif	
