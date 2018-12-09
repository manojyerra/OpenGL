#ifndef SUIBatch_H
#define SUIBatch_H
#pragma once

#include "SUIIncludes.h"

class SUIBatch
{
private:
	unsigned int _capacity;
	unsigned int _incrementInCapacity;	

	GLfloat* _uvArr;
	GLubyte* _colorArr;
	GLfloat* _normalArr;

	unsigned int _count;
	GLenum _mode;
	
	GLubyte _r;
	GLubyte _g;
	GLubyte _b;
	GLubyte _a;

	void ReCreateMem();

public:
	GLfloat* _vertexArr;

	SUIBatch(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent);
	~SUIBatch();

	void SetIncrement(unsigned int increment);

	void glBegin(GLenum mode);
	void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void glColor4f(float r, float g, float b, float a);

    void glColorui(unsigned int color);
	void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
	void glTexCoord2f(GLfloat u, GLfloat v);
	void glNormal3f(GLfloat x, GLfloat y, GLfloat z);
	void glEnd();
    
    void Draw();

	void ResetCount();
	unsigned int GetCount();
};

#endif
