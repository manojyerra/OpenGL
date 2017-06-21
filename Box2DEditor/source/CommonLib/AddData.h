#ifndef ADD_DATA_H
#define ADD_DATA_H
#pragma once

#include "../MyDefines.h"

class AddData
{
private:
	static AddData* _ref;

	static const int CAPACITY = 50000;
	GLfloat* _vertexArr;
	GLfloat* _colorArr;
	GLfloat* _uvArr;

	int _count;
	GLenum _mode;

	float _r;
	float _g;
	float _b;
	float _a;

	AddData();
	~AddData();
	
public:
	static AddData* GetInstance();
	void Init();
	void glBegin(GLenum mode);
	void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void glColor4f(float r, float g, float b, float a);

	void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
	void glTexCoord2f(GLfloat u, GLfloat v);
	void glEnd();
	void Terminate();
};

#endif
