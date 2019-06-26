#include "GLTexture.h"
#include "GLUtil.h"
#include "TransformVertexBuf.h"
#include "ImageBuffer.h"


GLTexture::GLTexture(int drawW, int drawH)
{
	_vertexBufferID = 0;
	_colorBufferID = 0;
	_vertexCount = 0;

	_drawW = drawW;
	_drawH = drawH;

	_shaderProgram = new ShaderProgram("shaders/UVArray/UVArray.vs", "shaders/UVArray/UVArray.fs");
	GenerateBufferID();

	ImageBuffer* imgBuf = new ImageBuffer("data/Sample.png");
	_textureID = GenerateGLTextureID(imgBuf->GetWidth(), imgBuf->GetHeight(), 
		imgBuf->GetBytesPerPixel(), imgBuf->GetBuffer());
}

void GLTexture::Draw()
{
	//glBindTexture(GL_TEXTURE_2D, _textureID);

	_shaderProgram->Begin();

	GLuint uvLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
	glEnableVertexAttribArray(uvLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
	glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_TRUE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);

	_shaderProgram->End();

	//glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::GenerateBufferID()
{
	GLBuffer* buffer = new GLBuffer(100, true, true, false);

	buffer->glBegin(GL_TRIANGLE_STRIP);

	float w = _drawW;
	float h = _drawH;

	buffer->glTexCoord2f(0, 1);
	buffer->glVertex3f(0, 0, 0);

	buffer->glTexCoord2f(1, 1);
	buffer->glVertex3f(+w, 0, 0);

	buffer->glTexCoord2f(0, 0);
	buffer->glVertex3f(0, h, 0);

	buffer->glTexCoord2f(1, 0);
	buffer->glVertex3f(w, h, 0);

	buffer->glEnd();

	_vertexBufferID = buffer->GetVertexBufferID();
	_uvBufferID = buffer->GetUVBufferID();
	_vertexCount = buffer->GetVertexCount();

	delete buffer;
}

GLTexture::~GLTexture()
{
}

unsigned int GLTexture::GenerateGLTextureID(int width, int height, int bytesPP, void* buffer)
{
	unsigned int textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (bytesPP == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if (bytesPP == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	else if (bytesPP == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_RGB8, GL_UNSIGNED_BYTE, buffer);
	else
		return 0;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//glBindTexture(GL_TEXTURE_2D, 0);
	
	unsigned int errorID = glGetError();

	return textureID;
}
