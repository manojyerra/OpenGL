#ifndef Box_H
#define Box_H

#include "DefinesAndIncludes.h"
#include "Shape.h"
#include "RandomColor.h"
#include "ShaderProgram.h"
#include "GLBuffer.h"

class Box : public Shape
{
private:
	float _w;
	float _h;
	float _d;

	RandomColor _randomColor;

	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _uvBufferID;
	GLuint _vertexCount;
	ShaderProgram* _shaderProgram;

	void InitBox();

	void GenerateBufferID();


	unsigned int GenerateGLTextureID(int width, int height, int bytesPP, void* buffer);
	unsigned int _textureID;

public:
	Box();
	Box(Box* box);
	Box(float* mat, CVector3 size);
	Box(float x, float y, float z, float w, float h, float d);
	~Box();

	void Set(Box* box);

	static Box CalcAABB(float* vertexBuf, int arrSize);
	static Box CalcBoundingBox(float* vertexBuf, int arrSize);
	static Box GetBoundingBoxAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);

	void SetPos(float x, float y, float z);
	void SetSize(float w, float h, float d);
	CVector3 GetPos();
	CVector3 GetSize();
	float Volume();
	vector<CVector3> GetAABBVertices();

	void Draw();
};

#endif
