#include "SUIBatch.h"
#include "stdio.h"
#include "string.h"

namespace SUI
{
	SUIBatch::SUIBatch(unsigned int capacity, bool colorDataPresent, bool uvDataPresent, bool normalsDataPresent)
	{
		_capacity = capacity;
		_incrementInCapacity = 512;

		_vertexArr = NULL;
		_uvArr = NULL;
		_colorArr = NULL;
		_normalArr = NULL;

		_vertexArr = new GLfloat[_capacity * 3];
		if (colorDataPresent)	_colorArr = new GLubyte[_capacity * 4];
		if (uvDataPresent)		_uvArr = new GLfloat[_capacity * 2];
		if (normalsDataPresent)	_normalArr = new GLfloat[_capacity * 3];

		_r = (GLubyte)255;
		_g = (GLubyte)255;
		_b = (GLubyte)255;
		_a = (GLubyte)255;

		_count = 0;
		_mode = 0;
	}

	void SUIBatch::SetIncrement(unsigned int increment)
	{
		_incrementInCapacity = increment;
	}

	void SUIBatch::ReCreateMem()
	{
		int newCapacity = _capacity + _incrementInCapacity;

		GLfloat* newVertexArr = NULL;
		GLubyte* newColorArr = NULL;
		GLfloat* newUVArr = NULL;
		GLfloat* newNormalArr = NULL;

		if (_vertexArr)	newVertexArr = new GLfloat[newCapacity * 3];
		if (_colorArr)	newColorArr = new GLubyte[newCapacity * 4];
		if (_uvArr)		newUVArr = new GLfloat[newCapacity * 2];
		if (_normalArr)	newNormalArr = new GLfloat[newCapacity * 3];

		if (_vertexArr)	memcpy(newVertexArr, _vertexArr, _capacity * 3 * 4);
		if (_colorArr)	memcpy(newColorArr, _colorArr, _capacity * 4 * 1);
		if (_uvArr)		memcpy(newUVArr, _uvArr, _capacity * 2 * 4);
		if (_normalArr)	memcpy(newNormalArr, _normalArr, _capacity * 3 * 4);

		if (_vertexArr)	delete[] _vertexArr;
		if (_colorArr)	delete[] _colorArr;
		if (_uvArr)		delete[] _uvArr;
		if (_normalArr)	delete[] _normalArr;

		_vertexArr = newVertexArr;
		_colorArr = newColorArr;
		_uvArr = newUVArr;
		_normalArr = newNormalArr;

		_capacity = newCapacity;
	}

	void SUIBatch::glBegin(GLenum mode)
	{
		_mode = mode;
		_count = 0;
	}

	void SUIBatch::glColor(unsigned int color)
	{
		_r = (color >> 24) & 255;
		_g = (color >> 16) & 255;
		_b = (color >> 8) & 255;
		_a = (color) & 255;
	}

	void SUIBatch::glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		_r = (GLubyte)r;
		_g = (GLubyte)g;
		_b = (GLubyte)b;
		_a = (GLubyte)a;
	}

	void SUIBatch::glColor4f(float r, float g, float b, float a)
	{
		_r = (GLubyte)(r*255.0f);
		_g = (GLubyte)(g*255.0f);
		_b = (GLubyte)(b*255.0f);
		_a = (GLubyte)(a*255.0f);
	}

	void SUIBatch::glVertex3f(GLfloat x, GLfloat y, GLfloat z)
	{
		if (_colorArr)
		{
			_colorArr[_count * 4 + 0] = _r;
			_colorArr[_count * 4 + 1] = _g;
			_colorArr[_count * 4 + 2] = _b;
			_colorArr[_count * 4 + 3] = _a;
		}

		_vertexArr[_count * 3 + 0] = x;
		_vertexArr[_count * 3 + 1] = y;
		_vertexArr[_count * 3 + 2] = z;

		_count++;

		if (_count >= _capacity)
			ReCreateMem();
	}

	void SUIBatch::glTexCoord2f(GLfloat u, GLfloat v)
	{
		_uvArr[_count * 2 + 0] = u;
		_uvArr[_count * 2 + 1] = v;
	}

	void SUIBatch::glNormal3f(GLfloat x, GLfloat y, GLfloat z)
	{
		_normalArr[_count * 3 + 0] = x;
		_normalArr[_count * 3 + 1] = y;
		_normalArr[_count * 3 + 2] = z;
	}


	void SUIBatch::Draw()
	{
		if (_count == 0)
			return;

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, _vertexArr);

		if (_colorArr)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, _colorArr);
		}
		if (_uvArr)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, _uvArr);
		}
		if (_normalArr)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, _normalArr);
		}

		glDrawArrays(_mode, 0, _count);

		if (_uvArr)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		if (_colorArr)
			glDisableClientState(GL_COLOR_ARRAY);
		if (_normalArr)
			glDisableClientState(GL_NORMAL_ARRAY);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void SUIBatch::glEnd()
	{
		Draw();
		_count = 0;
	}

	void SUIBatch::ResetCount()
	{
		_count = 0;
	}

	unsigned int SUIBatch::GetCount()
	{
		return _count;
	}

	SUIBatch::~SUIBatch()
	{
		if (_vertexArr)	delete[] _vertexArr;
		if (_colorArr)	delete[] _colorArr;
		if (_uvArr)		delete[] _uvArr;
		if (_normalArr)	delete[] _normalArr;
	}
}
