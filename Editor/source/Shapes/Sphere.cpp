#include "Sphere.h"

Sphere::Sphere(float x, float y, float z, float r) : Shape(Shape::SPHERE)
{
	_r = r;

	_color[0] = _color[1] = _color[2] = _color[3] = 255;

	for(int i=0;i<16;i++)
		_mat[i] = 0;

	_mat[0] = 1;
	_mat[5] = 1;
	_mat[10] = 1;
	_mat[15] = 1;

	_mat[12] = x;
	_mat[13] = y;
	_mat[14] = z;

	_quad = gluNewQuadric();
}

void Sphere::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
	_color[3] = a;
}

void Sphere::SetOpenGLMatrix(float* mat)
{
	for(int i=0;i<16;i++)
		_mat[i] = mat[i];
}

void Sphere::GetOpenGLMatrix(float* mat)
{
	for(int i=0;i<16;i++)
		mat[i] = _mat[i];
}

void Sphere::Draw(bool randomColors)
{
	glColor4ub(_color[0], _color[1], _color[2], _color[3]);

	bool lightOn = glIsEnabled(GL_LIGHTING);

	if(randomColors)
		glEnable(GL_LIGHTING);

	glPushMatrix();
	glMultMatrixf(_mat);
	gluSphere(_quad,_r,20,20);
	glPopMatrix();

	if(lightOn)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

Sphere::~Sphere()
{
	gluDeleteQuadric(_quad);
}
