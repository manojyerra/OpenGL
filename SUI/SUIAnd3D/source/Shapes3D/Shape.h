#ifndef Shape_H
#define Shape_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "../Math/Vector3.h"

class Shape
{
protected:
	int _id;
	unsigned char _color[4];

public:
	float m[16];

	static const int NONE = 0;
	static const int BOX = 1;
	static const int CONE = 2;
	static const int CYLINDER = 3;
	static const int SPHERE = 4;
	static const int MESH = 5;

	Shape();
	Shape(int id);
	int GetID();
	void SetGLMatrix(float* mat);
	float* GetGLMatrix();
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	static Shape* GetBestFitBoundingShape(float* vertexBuf, int arrSize);
	virtual float Volume() = 0;
	virtual void Draw() = 0;
	virtual ~Shape();
};

#endif