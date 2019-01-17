#include "Def.h"
#include "Util/VertexBufUtils.h"

class BoundingBox
{
private:
	float _mat[16];
	Point _scale;
	float _volume;

public:
	BoundingBox();
	~BoundingBox();

	void CalcBoundingBox(float* vertexBuf, int arrSize);
	float* GetMat();
	Point GetScale();
	float GetVolume();
};