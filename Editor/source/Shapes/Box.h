#ifndef Box_H
#define Box_H

#include "Shape.h"

class Box : Shape
{
private:
	float _w;
	float _h;
	float _d;
	unsigned char _color[4];
	float _mat[16];

	unsigned char _randColor[6*4];

public:
	Box(float x, float y, float z, float w, float h, float d);
	~Box();

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetOpenGLMatrix(float* mat);
	void GetOpenGLMatrix(float* mat);
	void SetPos(float x, float y, float z);
	void SetSize(float w, float h, float d);

	void SetW(float w);
	void SetH(float h);
	void SetD(float d);

	float x();
	float y();
	float z();

	float w();
	float h();
	float d();

	void Draw(bool randomColors = true);
};

#endif