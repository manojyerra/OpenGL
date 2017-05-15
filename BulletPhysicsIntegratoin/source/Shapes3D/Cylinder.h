#ifndef Cylinder_H
#define Cylinder_H

#include "DefinesAndIncludes.h"
#include "Shape.h"
#include "Util/RandomColor.h"

class Cylinder : public Shape
{
private:
	float _r;
	float _h;
	unsigned char _color[4];
	RandomColor _randomColor;

public:
	Cylinder();
	Cylinder(Cylinder* cylinder);
	Cylinder(float* mat, float r, float h);
	Cylinder(float x, float y, float z, float r, float h);
	~Cylinder();

	CVector3 GetPos();

	float GetRadius();
	float GetHeight();

	void SetRadius(float r);
	void SetHeight(float h);

	float Volume();

	static Cylinder CalcBoundingCylinder(float* vertexBuf, int arrSize);
	static Cylinder GetBoundingCylinderAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);

	void Draw();
};

#endif