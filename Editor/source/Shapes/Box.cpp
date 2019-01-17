#include "Box.h"
#include "Sui/SuiIncludes.h"

Box::Box(float x, float y, float z, float w, float h, float d) : Shape(Shape::BOX)
{
	_w = w;
	_h = h;
	_d = d;

	_color[0] = _color[1] = _color[2] = _color[3] = 255;
	
	_randColor[0] = 64;
	_randColor[1] = 64;
	_randColor[2] = 255;
	_randColor[3] = 255;

	_randColor[4] = 64;
	_randColor[5] = 255;
	_randColor[6] = 64;
	_randColor[7] = 255;

	_randColor[8] = 255;
	_randColor[9] = 64;
	_randColor[10] = 64;
	_randColor[11] = 255;

	_randColor[12] = 255;
	_randColor[13] = 255;
	_randColor[14] = 64;
	_randColor[15] = 255;

	_randColor[16] = 64;
	_randColor[17] = 255;
	_randColor[18] = 255;
	_randColor[19] = 255;

	_randColor[20] = 255;
	_randColor[21] = 64;
	_randColor[22] = 255;
	_randColor[23] = 255;

	for(int i=0;i<16;i++)
		_mat[i] = 0;

	_mat[0] = 1;
	_mat[5] = 1;
	_mat[10] = 1;
	_mat[15] = 1;

	_mat[12] = x;
	_mat[13] = y;
	_mat[14] = z;
}

void Box::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
	_color[3] = a;
}

void Box::SetOpenGLMatrix(float* mat)
{
	for(int i=0;i<16;i++)
		_mat[i] = mat[i];
}

void Box::GetOpenGLMatrix(float* mat)
{
	for(int i=0;i<16;i++)
		mat[i] = _mat[i];
}

void Box::SetPos(float x, float y, float z)
{
	_mat[12] = x;
	_mat[13] = y;
	_mat[14] = z;
}

void Box::SetSize(float w, float h, float d)
{
	_w = w;
	_h = h;
	_d = d;
}

void Box::SetW(float w)	{ _w = w; }
void Box::SetH(float h)	{ _h = h; }
void Box::SetD(float d)	{ _d = d; }

float Box::x() { return _mat[12]; }
float Box::y() { return _mat[13]; }
float Box::z() { return _mat[14]; }

float Box::w() { return _w; }
float Box::h() { return _h; }
float Box::d() { return _d; }

void Box::Draw(bool randomColors)
{
	float w = _w/2.0f;
	float h = _h/2.0f;
	float d = _d/2.0f;

	glColor4ub(_color[0], _color[1], _color[2], _color[3]);

	glPushMatrix();
	glMultMatrixf(_mat);

	glBegin(GL_QUADS);

	int i=0;

	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	// Front Face
	glVertex3f(-w, -h,  d);  // Bottom Left Of The Texture and Quad
	glVertex3f( w, -h,  d);  // Bottom Right Of The Texture and Quad
	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	glVertex3f( w,  h,  d);  // Top Right Of The Texture and Quad
	glVertex3f(-w,  h,  d);  // Top Left Of The Texture and Quad

	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	// Back Face
	glVertex3f(-w, -h, -d);  // Bottom Right Of The Texture and Quad
	glVertex3f(-w,  h, -d);  // Top Right Of The Texture and Quad
	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	glVertex3f( w,  h, -d);  // Top Left Of The Texture and Quad
	glVertex3f( w, -h, -d);  // Bottom Left Of The Texture and Quad

	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	// Top Face
	glVertex3f(-w,  h, -d);  // Top Left Of The Texture and Quad
	glVertex3f(-w,  h,  d);  // Bottom Left Of The Texture and Quad
	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4; i =0;
	glVertex3f( w,  h,  d);  // Bottom Right Of The Texture and Quad
	glVertex3f( w,  h, -d);  // Top Right Of The Texture and Quad
	
	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	// Bottom Face
	glVertex3f(-w, -h, -d);  // Top Right Of The Texture and Quad
	glVertex3f( w, -h, -d);  // Top Left Of The Texture and Quad
	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	glVertex3f( w, -h,  d);  // Bottom Left Of The Texture and Quad
	glVertex3f(-w, -h,  d);  // Bottom Right Of The Texture and Quad

	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	// Right face
	glVertex3f( w, -h, -d);  // Bottom Right Of The Texture and Quad
	glVertex3f( w,  h, -d);  // Top Right Of The Texture and Quad
	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	glVertex3f( w,  h,  d);  // Top Left Of The Texture and Quad
	glVertex3f( w, -h,  d);  // Bottom Left Of The Texture and Quad

	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	// Left Face
	glVertex3f(-w, -h, -d);  // Bottom Left Of The Texture and Quad
	glVertex3f(-w, -h,  d);  // Bottom Right Of The Texture and Quad
	if(randomColors)	glColor4ub(_randColor[i+0], _randColor[i+1], _randColor[i+2], _randColor[i+3]);		i+=4;
	glVertex3f(-w,  h,  d);  // Top Right Of The Texture and Quad
	glVertex3f(-w,  h, -d);  // Top Left Of The Texture and Quad

	glEnd();

	glPopMatrix();
}

Box::~Box()
{
}
