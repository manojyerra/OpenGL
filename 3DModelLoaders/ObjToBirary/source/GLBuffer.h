#ifndef GLBuffer_H
#define GLBuffer_H
#pragma once

#include "DefinesAndIncludes.h"
#include "Vector3.h"

class GLBuffer
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

	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _uvBufferID;
	GLuint _normalBufferID;

	void ReCreateMem();

public:
	GLBuffer(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent);
	~GLBuffer();

	void SetIncrement(unsigned int increment);

	void glBegin(GLenum mode);
	void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void glColor4f(float r, float g, float b, float a);
	void glColoruia(unsigned int color, unsigned char alpha);

    //void glColor(unsigned int color);
	void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
	void glTexCoord2f(GLfloat u, GLfloat v);
	void glNormal3f(GLfloat x, GLfloat y, GLfloat z);
	
	void glVertex3f(const CVector3& vec);
	void glNormal3f(const CVector3& vec);
	void glTexCoord2f(const CVector3& vec);

	//void glEnd();

	GLfloat* GetVertexArr();
	GLfloat* GetUVArr();
	GLubyte* GetColorArr();
	GLfloat* GetNormalArr();

	GLuint GetVertexBufferID();
	GLuint GetColorBufferID();
	GLuint GetUVBufferID();
	GLuint GetNormalBufferID();

	//void Draw(GLuint programID);

	void ResetCount();
	unsigned int GetVertexCount();
};

#endif
