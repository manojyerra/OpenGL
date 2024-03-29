#include "Cylinder.h"
#include "Sui/SuiIncludes.h"
#include "math.h"

Cylinder::Cylinder(float x, float y, float z, float r, float h) : Shape(Shape::CYLINDER)
{
	_r = r;
	_h = h;

	_color[0] = _color[1] = _color[2] = _color[3] = 255;

	for(int i=0;i<16;i++)
		_mat[i] = 0;

	_mat[0] = _mat[5] = _mat[10] = _mat[15] = 1;

	_mat[12] = x;
	_mat[13] = y;
	_mat[14] = z;
}

void Cylinder::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
	_color[3] = a;
}

void Cylinder::SetOpenGLMatrix(float* mat)
{
	for(int i=0;i<16;i++)
		_mat[i] = mat[i];
}

void Cylinder::GetOpenGLMatrix(float* mat)
{
	for(int i=0;i<16;i++)
		mat[i] = _mat[i];
}

float Cylinder::GetRadius()
{
	return _r;
}

float Cylinder::GetHeight()
{
	return _h;
}

void Cylinder::Draw(bool randomColors)
{
	glColor4ub(_color[0], _color[1], _color[2], _color[3]);

	glPushMatrix();
	glMultMatrixf(_mat);

	float radius = _r;
	float halfLength = _h/2.0f;
	float piVal = 3.14159265f;

	int r = 0;
	int g = 0;
	int b = 0;

	int inc = 100;

	glBegin(GL_TRIANGLES);
	for(int i=5; i<=360; i+=5)
	{
		float theta = (i-5)*piVal/180.0f;
		float nextTheta = (i)*piVal/180.0f;

		if(randomColors)
			glColor4ub(r, g, b, 255);
		
		glVertex3f(radius*cos(theta),	halfLength,		radius*sin(theta));
		glVertex3f(radius*cos(theta),	-halfLength,	radius*sin(theta));
		glVertex3f(radius*cos(nextTheta),	halfLength,		radius*sin(nextTheta));

		glVertex3f(radius*cos(theta),	-halfLength,	radius*sin(theta));
		glVertex3f(radius*cos(nextTheta),	halfLength,		radius*sin(nextTheta));
		glVertex3f(radius*cos(nextTheta),	-halfLength,	radius*sin(nextTheta));

		glVertex3f(0, halfLength, 0);
		if(randomColors)
			glColor4ub(100, 100, 100, 255);
		glVertex3f(radius*cos(theta),	halfLength,		radius*sin(theta));
		glVertex3f(radius*cos(nextTheta),	halfLength,		radius*sin(nextTheta));

		if(randomColors)
			glColor4ub(r, g, b, 255);
		glVertex3f(0, -halfLength, 0);
		if(randomColors)
			glColor4ub(100, 100, 100, 255);
		glVertex3f(radius*cos(theta),	-halfLength,		radius*sin(theta));
		glVertex3f(radius*cos(nextTheta),	-halfLength,		radius*sin(nextTheta));

		if(randomColors)
		{
			r += inc;
			if(r > 255)
			{
				r = 0;
				g += inc;
				if(g > 255)
				{
					g = 0;
					b += inc;
					if(b > 255)
						b = 0;
				}
			}
		}
	}
	glEnd();

	glPopMatrix();
}

Cylinder::~Cylinder()
{
}
