#ifndef Cone_H
#define Cone_H

#include "Shape.h"
#include "../Util/RandomColor.h"

class Cone : public Shape
{
private:
	float _r;
	float _h;

	RandomColor _randomColor;

public:
	Cone();
	Cone(float* mat, float r, float h);
	Cone(float x, float y, float z, float r, float h);
	~Cone();

	CVector3 GetPos();

	float GetRadius();
	float GetHeight();

	void SetRadius(float r);
	void SetHeight(float h);

	float Volume();

	static float CalcRadius(float* vertexBuf, int arrSize, float height);
	static Cone CalcBoundingCone(float* vertexBuf, int arrSize);
	//static Cone GetBoundingConeAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);

	void Draw();
};

#endif