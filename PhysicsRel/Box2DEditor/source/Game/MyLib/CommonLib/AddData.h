#ifndef AddData_H
#define AddData_H
#pragma once

#include "Includes.h"

class AddData
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

	void ReCreateMem();

	static AddData* _ref;

public:

	AddData(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent);
	~AddData();

	static AddData* GetInstance();
	static void DeleteInstance();

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
