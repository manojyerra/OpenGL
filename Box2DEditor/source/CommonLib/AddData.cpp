#include "AddData.h"

AddData* AddData::_ref = NULL;

AddData::AddData()
{
	_count = 0;
}

AddData::~AddData()
{
}
	
AddData* AddData::GetInstance()
{
	if(_ref == NULL)		
		_ref = new AddData();
	return _ref;
}

void AddData::Init()
{
	_vertexArr = new GLfloat[CAPACITY*3];
	_colorArr = new GLfloat[CAPACITY*4];
	_uvArr = new  GLfloat[CAPACITY*2];

	_r = 255;
	_g = 255;
	_b = 255;
	_a = 255;

	//_vertexArr = (GLfloat*)GLMalloc(CAPACITY*3*sizeof(GLfloat));
	//_colorArr = (GLfloat*)GLMalloc(CAPACITY*4*sizeof(GLfloat));
	//_uvArr = (GLfloat*)GLMalloc(CAPACITY*2*sizeof(GLfloat));
}

void AddData::glBegin(GLenum mode)
{
	_mode = mode;
	_count = 0;
}

void AddData::glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r*1.0f/255.0f;
	_g = g*1.0f/255.0f;
	_b = b*1.0f/255.0f;
	_a = a*1.0f/255.0f;
}

void AddData::glColor4f(float r, float g, float b, float a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void AddData::glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	_colorArr[_count*4 + 0] = _r;
	_colorArr[_count*4 + 1] = _g;
	_colorArr[_count*4 + 2] = _b;
	_colorArr[_count*4 + 3] = _a;

	_vertexArr[_count*3 + 0] = x;
	_vertexArr[_count*3 + 1] = y;
	_vertexArr[_count*3 + 2] = z;

	_count++;
}

void AddData::glTexCoord2f(GLfloat u, GLfloat v)
{
	_uvArr[_count*2 + 0] = u;
	_uvArr[_count*2 + 1] = v;
}


void AddData::glEnd()
{
	GLboolean enableTexture =  glIsEnabled(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	if(enableTexture)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, _colorArr);
	glVertexPointer(3, GL_FLOAT, 0, _vertexArr);
	if(enableTexture)
		glTexCoordPointer (2, GL_FLOAT, 0, _uvArr);

	glDrawArrays(_mode,0,_count);
	
	if(enableTexture)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	_count = 0;
}

void AddData::Terminate()
{
	if(_vertexArr)
		delete[] _vertexArr;

	if(_colorArr)
		delete[] _colorArr;

	if(_uvArr)
		delete[] _uvArr;

	if(_ref)
	{
		_ref->~AddData();
		delete _ref;
		_ref = NULL;
	}
}