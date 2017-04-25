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

	static const int BOX = 1;
	static const int CONE = 2;
	static const int CYLINDER = 3;
	static const int SPHERE = 4;
	static const int MESH = 5;

	Shape(int id)
	{
		_id = id;
		memset(m, 0, 16*4);
		m[0] = m[5] = m[10] = m[15] = 1;
		_color[0] = _color[1] = _color[2] = _color[3] = 255;
	}

	int GetID()
	{
		return _id;
	}

	void SetGLMatrix(float* mat)
	{
		memcpy(m, mat, 16*4);
	}

	float* GetGLMatrix()
	{
		return m;
	}

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		_color[0] = r;
		_color[1] = g;
		_color[2] = b;
		_color[3] = a;
	}

	virtual void Draw() = 0;
};

#endif