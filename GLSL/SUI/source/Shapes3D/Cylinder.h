#ifndef Cylinder_H
#define Cylinder_H

#include "Macros.h"
#include "Shape.h"
#include "RandomColor.h"
#include "ShaderProgram.h"
#include "GLBuffer.h"

class Cylinder : public Shape
{
private:
	float _r;
	float _h;

	RandomColor _randomColor;

	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;
	ShaderProgram* _shaderProgram;

	void GenerateBufferID();
	void InitCommon();

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
