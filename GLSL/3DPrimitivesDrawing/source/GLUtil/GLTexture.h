#ifndef GLTexture_H
#define GLTexture_H

#include "Macros.h"
#include <string>
#include "ShaderProgram.h"
#include "GLBuffer.h"

using namespace std;


class GLTexture
{
private:
	unsigned int _textureID;
	float _drawW;
	float _drawH;

	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _uvBufferID;
	GLuint _vertexCount;
	ShaderProgram* _shaderProgram;

	void GenerateBufferID();
	unsigned int GenerateGLTextureID(int width, int height, int bytesPP, void* buffer);

public:
	GLTexture(int drawW, int drawH);
	~GLTexture();

	void Draw();
};


#endif
