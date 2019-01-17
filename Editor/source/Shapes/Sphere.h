#ifndef Sphere_H
#define Sphere_H

#include "Shape.h"
#include "Sui/SuiIncludes.h"

class Sphere : Shape
{
private:
	float _r;
	unsigned char _color[4];
	float _mat[16];

	GLUquadricObj* _quad;

public:
	Sphere(float x, float y, float z, float r);
	~Sphere();

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetOpenGLMatrix(float* mat);
	void GetOpenGLMatrix(float* mat);
	void Draw(bool randomColors = true);
};

#endif