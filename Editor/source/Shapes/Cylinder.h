#ifndef Cylinder_H
#define Cylinder_H

#include "Shape.h"

class Cylinder : Shape
{
private:
	float _r;
	float _h;
	unsigned char _color[4];
	float _mat[16];

public:
	Cylinder(float x, float y, float z, float r, float h);
	~Cylinder();

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetOpenGLMatrix(float* mat);
	void GetOpenGLMatrix(float* mat);
	float GetRadius();
	float GetHeight();
	void Draw(bool randomColors = true);
};

#endif