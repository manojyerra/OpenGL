#ifndef GLBatch_H
#define GLBatch_H
#pragma once

#include "DefinesAndIncludes.h"

class GLBatch
{
private:
	unsigned int _capacity;
	unsigned int _incrementInCapacity;	

	GLfloat* _vertexArr;
	GLfloat* _uvArr;
	GLubyte* _colorArr;
	GLfloat* _normalArr;

	int _count;
	GLenum _mode;
	
	GLubyte _r;
	GLubyte _g;
	GLubyte _b;
	GLubyte _a;

	GLfloat _u;
	GLfloat _v;

	GLfloat _nx;
	GLfloat _ny;
	GLfloat _nz;

	void ReCreateMem();

public:
	GLBatch(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent);
	~GLBatch();

	void SetIncrement(unsigned int increment);

	void glBegin(GLenum mode);
	void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void glColor4f(float r, float g, float b, float a);

    void glColor(unsigned int color);
	void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
	void glTexCoord2f(GLfloat u, GLfloat v);
	void glNormal3f(GLfloat x, GLfloat y, GLfloat z);
	void glEnd();

	GLfloat* GetVertexArr();
	GLfloat* GetUVArr();
	GLubyte* GetColorArr();
	GLfloat* GetNormalArr();

	void ResetCount();
	unsigned int GetVertexCount();

    void Draw();
};

#endif
