#ifndef SuiBatch_H
#define SuiBatch_H
#pragma once

#include "SuiIncludes.h"

class SuiBatch
{
private:
	unsigned int _capacity;
	unsigned int _incrementInCapacity;	

	GLfloat* _uvArr;
	GLubyte* _colorArr;
	GLfloat* _normalArr;

	int _count;
	GLenum _mode;
	
	GLubyte _r;
	GLubyte _g;
	GLubyte _b;
	GLubyte _a;

	void ReCreateMem();

public:
	GLfloat* _vertexArr;

	SuiBatch(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent);
	~SuiBatch();

	void SetIncrement(unsigned int increment);

	void glBegin(GLenum mode);
	void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void glColor4f(float r, float g, float b, float a);

    void glColor(unsigned int color);
	void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
	void glTexCoord2f(GLfloat u, GLfloat v);
	void glNormal3f(GLfloat x, GLfloat y, GLfloat z);
	void glEnd();
    
    void Draw();

	void ResetCount();
	unsigned int GetCount();
};

#endif
