#ifndef Triangle_H
#define Triangle_H

#include "Vector3.h"
#include "ShaderProgram.h"

class Triangle
{
	ShaderProgram* _shaderProgram;
	GLint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;

public:
	Triangle(CVector3 v1, CVector3 v2, CVector3 v3);
	~Triangle();

	void Draw();
};

#endif