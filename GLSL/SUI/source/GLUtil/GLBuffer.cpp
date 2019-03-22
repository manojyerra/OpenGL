#include "GLBuffer.h"
//#include "stdio.h"
//#include "string.h"

GLBuffer::GLBuffer(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent)
{
	_capacity = capacity;
	_incrementInCapacity = 512;

	_vertexArr = NULL;
	_uvArr = NULL;
	_colorArr = NULL;
	_normalArr = NULL;

	_vertexArr = new GLfloat[_capacity*3];
	if(colorDataPresent)	_colorArr = new GLubyte[_capacity*4];
	if(uvDataPresent)		_uvArr = new GLfloat[_capacity*2];
	if(normalsDataPresent)	_normalArr = new GLfloat[_capacity*3];

	_r = (GLubyte)255;
	_g = (GLubyte)255;
	_b = (GLubyte)255;
	_a = (GLubyte)255;

	_u = 0;
	_v = 0;

	_nx = 0;
	_ny = 0;
	_nz = 0;

	_count = 0;
	_mode = 0;

	_vertexBufferID = 0;
	_colorBufferID = 0;
	_uvBufferID = 0;
	_normalBufferID = 0;
}

void GLBuffer::SetIncrement(unsigned int increment)
{
	_incrementInCapacity = increment;
}

void GLBuffer::ReCreateMem()
{
	int newCapacity = _capacity + _incrementInCapacity;

	//prinf("\n\tNew Capacity : %d\n", newCapacity);

	GLfloat* newVertexArr = NULL;
	GLubyte* newColorArr = NULL;
	GLfloat* newUVArr = NULL;
	GLfloat* newNormalArr = NULL;

	if(_vertexArr)	newVertexArr = new GLfloat[newCapacity*3];
	if(_colorArr)	newColorArr = new GLubyte[newCapacity*4];
	if(_uvArr)		newUVArr = new GLfloat[newCapacity*2];
	if(_normalArr)	newNormalArr = new GLfloat[newCapacity*3];

	if(_vertexArr)	memcpy(newVertexArr,	_vertexArr,		_capacity*3*4);
	if(_colorArr)	memcpy(newColorArr,		_colorArr,		_capacity*4*1);
	if(_uvArr)		memcpy(newUVArr,		_uvArr,			_capacity*2*4);
	if(_normalArr)	memcpy(newNormalArr,	_normalArr,		_capacity*3*4);

	if(_vertexArr)	delete[] _vertexArr;
	if(_colorArr)	delete[] _colorArr;
	if(_uvArr)		delete[] _uvArr;
	if(_normalArr)	delete[] _normalArr;

	_vertexArr = newVertexArr;
	_colorArr = newColorArr;
	_uvArr = newUVArr;
	_normalArr = newNormalArr;

	_capacity = newCapacity;
}

void GLBuffer::glBegin(GLenum mode)
{
	_mode = mode;
	_count = 0;
}

void GLBuffer::glColor(unsigned int color)
{
    _r	= (color >> 24) & 255;
	_g	= (color >> 16) & 255;
	_b	= (color >> 8) & 255;
	_a	= (color ) & 255;
}

void GLBuffer::glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = (GLubyte)r;
	_g = (GLubyte)g;
	_b = (GLubyte)b;
	_a = (GLubyte)a;
}

void GLBuffer::glColoruia(unsigned int color, unsigned char alpha)
{
    _r	= (color >> 24) & 255;
	_g	= (color >> 16) & 255;
	_b	= (color >> 8) & 255;
	_a = alpha;		
}

void GLBuffer::glColor4f(float r, float g, float b, float a)
{
	_r = (GLubyte)(r*255.0f);
	_g = (GLubyte)(g*255.0f);
	_b = (GLubyte)(b*255.0f);
	_a = (GLubyte)(a*255.0f);
}

void GLBuffer::glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	if(_colorArr)
	{
		_colorArr[_count*4 + 0] = _r;
		_colorArr[_count*4 + 1] = _g;
		_colorArr[_count*4 + 2] = _b;
		_colorArr[_count*4 + 3] = _a;
	}

	if(_uvArr)
	{
		_uvArr[_count*2 + 0] = _u;
		_uvArr[_count*2 + 1] = _v;
	}

	if(_normalArr)
	{
		_normalArr[_count*3 + 0] = _nx;
		_normalArr[_count*3 + 1] = _ny;
		_normalArr[_count*3 + 2] = _nz;
	}

	_vertexArr[_count*3 + 0] = x;
	_vertexArr[_count*3 + 1] = y;
	_vertexArr[_count*3 + 2] = z;

	_count++;

	if(_count >= _capacity)
		ReCreateMem();
}

void GLBuffer::glTexCoord2f(GLfloat u, GLfloat v)
{
	_u = u;
	_v = v;
}

void GLBuffer::glNormal3f(GLfloat x, GLfloat y, GLfloat z)
{
	_nx = x;
	_ny = y;
	_nz = z;
}

GLfloat* GLBuffer::GetVertexArr()		{	return _vertexArr;	}
GLfloat* GLBuffer::GetUVArr()			{	return _uvArr;		}
GLubyte* GLBuffer::GetColorArr()		{	return _colorArr;	}
GLfloat* GLBuffer::GetNormalArr()		{	return _normalArr;	}

GLuint GLBuffer::GetVertexBufferID()	{	return _vertexBufferID;		}
GLuint GLBuffer::GetColorBufferID()		{	return _colorBufferID;		}
GLuint GLBuffer::GetUVBufferID()		{	return _uvBufferID;			}
GLuint GLBuffer::GetNormalBufferID()	{	return _normalBufferID;		}

void GLBuffer::glEnd()
{
	if(_vertexBufferID == 0)
	{
		glGenBuffers(1, &_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, _count * 3 * 4, _vertexArr, GL_STATIC_DRAW);

		if(_colorArr)
		{
			glGenBuffers(1, &_colorBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
			glBufferData(GL_ARRAY_BUFFER, _count * 4, _colorArr, GL_STATIC_DRAW);
		}

		if(_uvArr)
		{
			glGenBuffers(1, &_uvBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
			glBufferData(GL_ARRAY_BUFFER, _count * 2 * 4, _uvArr, GL_STATIC_DRAW);
		}

		if(_normalArr)
		{
			glGenBuffers(1, &_normalBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
			glBufferData(GL_ARRAY_BUFFER, _count * 3 * 4, _normalArr, GL_STATIC_DRAW);
		}

		if(_vertexArr)	delete[] _vertexArr;
		if(_colorArr)	delete[] _colorArr;
		if(_uvArr)		delete[] _uvArr;
		if(_normalArr)	delete[] _normalArr;
	}
}

//void GLBuffer::Draw(GLuint programID)
//{
//	glEnable(GL_BLEND);
//
//	if(_normalBufferID)
//	{
//		GLuint vertexNormalID = glGetAttribLocation(programID, "vertexNormal");
//		glEnableVertexAttribArray(vertexNormalID);
//		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
//		glVertexAttribPointer(
//			vertexNormalID,				// The attribute we want to configure
//			3,							// size
//			GL_FLOAT,					// type
//			GL_FALSE,					// normalized?
//			0,							// stride
//			(void*)0					// array buffer offset
//		);
//	}
//
//	if(_colorBufferID)
//	{
//		GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");
//		glEnableVertexAttribArray(vertexColorID);
//		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
//		glVertexAttribPointer(
//			vertexColorID,              // The attribute we want to configure
//			4,                          // size
//			GL_UNSIGNED_BYTE,           // type
//			GL_TRUE,                    // normalized?
//			0,                          // stride
//			(void*)0                    // array buffer offset
//		);
//	}
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer( GL_ARRAY_BUFFER, _vertexBufferID );
//	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
//
//	glDrawArrays(_mode, 0, _count);
//
//	glBindBuffer( GL_ARRAY_BUFFER, 0);
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
//}

void GLBuffer::ResetCount()
{
	_count = 0;
}

unsigned int GLBuffer::GetVertexCount()
{
	return _count;	
}

GLBuffer::~GLBuffer()
{
}
