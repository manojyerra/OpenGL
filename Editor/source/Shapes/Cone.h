#ifndef Cone_H
#define Cone_H

#include "Shape.h"

class Cone : Shape
{
private:
	float _r;
	float _h;
	unsigned char _color[4];
	float _mat[16];

public:
	Cone(float x, float y, float z, float r, float h);
	~Cone();

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetOpenGLMatrix(float* mat);
	void GetOpenGLMatrix(float* mat);
	void Draw(bool randomColors = true);
};

#endif