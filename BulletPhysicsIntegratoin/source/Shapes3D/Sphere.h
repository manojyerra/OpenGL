#ifndef Sphere_H
#define Sphere_H

#include "Shape.h"

class Sphere : public Shape
{
private:
	float _r;
	unsigned char _color[4];

	GLUquadricObj* _quad;

public:
	Sphere(float x, float y, float z, float r);
	Sphere(Sphere* sphere);
	Sphere(CVector3 pos, float r);
	Sphere(float* mat, float r);
	~Sphere();

	float GetRadius();
	void SetRadius(float r);
	static Sphere CalcBoundingSphere(float* vertexBuf, int arrSize);
	float Volume();

	void Draw();
};

#endif
