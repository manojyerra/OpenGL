#include "Def.h"
#include "Util/VertexBufUtils.h"

class BoundingCylinder
{
private:
	float _radius;
	float _height;
	float _volume;
	float _mat[16];

	Point Way_One(float* localVertexBuf, int arrSize, Point boxWHD, float* boxMat, float* mat);
	Point Way_Two(float* localVertexBuf, int arrSize, float* boxMat, float* mat);

public:
	BoundingCylinder();
	~BoundingCylinder();

	void CalcBoundingCylinder(float* vertexBuf, int arrSize, float* boxMat, Point boxWHD);
	float GetRadius();
	float GetHeight();
	float* GetMat();
	float GetVolume();
	Point GetScale();
};