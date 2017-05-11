#ifndef Box_H
#define Box_H

#include "Shape.h"
#include "../Util/RandomColor.h"


class Box : public Shape
{
private:
	float _w;
	float _h;
	float _d;

	RandomColor _randomColor;

public:
	Box();
	Box(Box* box);
	Box(float* mat, CVector3 size);
	Box(float x, float y, float z, float w, float h, float d);
	~Box();

	void Set(Box* box);

	static Box CalcAABB(float* vertexBuf, int arrSize);
	static Box CalcBoundingBox(float* vertexBuf, int arrSize);
	static Box GetBoundingBoxAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);
	
	void SetPos(float x, float y, float z);
	void SetSize(float w, float h, float d);
	CVector3 GetPos();
	CVector3 GetSize();
	float Volume();
	vector<CVector3> GetAABBVertices();

	void Draw();
};

#endif