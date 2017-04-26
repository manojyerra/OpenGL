#include "Cone.h"
#include "math.h"
#include "../Util/GLUtil.h"
#include "../Math/TransformVertexBuf.h"
#include "Cylinder.h"

Cone::Cone() : Shape(Shape::CONE)
{
	_r = 0;
	_h = 0;
}


Cone::Cone(float* mat, float r, float h) : Shape(Shape::CONE)
{
	memcpy(m, mat, 16*4);

	_r = r;
	_h = h;
}


Cone::Cone(float x, float y, float z, float r, float h) : Shape(Shape::CONE)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;

	_r = r;
	_h = h;
}


CVector3 Cone::GetPos()
{
	return CVector3(m[12], m[13], m[14]);
}


float Cone::GetRadius()			{ return _r; }
float Cone::GetHeight()			{ return _h; }

void Cone::SetRadius(float r)	{ _r = r; }
void Cone::SetHeight(float h)	{ _h = h; }


float Cone::Volume()
{
	return (1.0f/3.0f) * PI_VAL * _r * _r * _h;
}


Cone Cone::CalcBoundingCone(float* vertexBuf, int arrSize)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize*sizeof(float));

	Cylinder cylinder = Cylinder::CalcBoundingCylinder(vertexBuf, arrSize);

	float cylMatInvert[16];
	GLMat::InvertMatrix(cylinder.GetGLMatrix(), cylMatInvert);

	cylMatInvert[13] += cylinder.GetHeight()/2.0f;

	TransformVertexBuf::MulBufWithMatrix(localVertexBuf, arrSize, cylMatInvert);

	float height = cylinder.GetHeight() * 1.01;
	float radius = CalcRadius(localVertexBuf, arrSize, height);
	float volume = (1.0f/3.0f) * PI_VAL * radius * radius * height;

	float finalH = height;

	for(float i=1.1; i<2; i+=0.1)
	{
		float currH = height * i;

		float r = CalcRadius(localVertexBuf, arrSize, currH);
		float v = (1.0f/3.0f) * PI_VAL * r * r * currH;

		if(v < volume)
		{
			volume = v;
			radius = r;
			finalH = currH;
		}
	}

	delete[] localVertexBuf;

	float matInvert[16];
	GLMat::InvertMatrix(cylMatInvert, matInvert);

	GLMat mat;
	mat.glMultMatrixf(matInvert);
	mat.glTranslatef(0,finalH/2,0);

	Cone cone(mat.m, radius, finalH);

	return cone;
}


float Cone::CalcRadius(float* vertexBuf, int arrSize, float height)
{
	float x = vertexBuf[0];
	float y = vertexBuf[1];
	float z = vertexBuf[2];

	float dy = abs(height - y);
	float xOnXZPlane = height * x / dy;
	float zOnXZPlane = height * z / dy;

	float rPow2 = xOnXZPlane*xOnXZPlane + zOnXZPlane*zOnXZPlane;

	for(int i=3; i<arrSize; i+=3)
	{
		x = vertexBuf[i+0];
		y = vertexBuf[i+1];
		z = vertexBuf[i+2];

		dy = height - y;
		xOnXZPlane = height * x / dy;
		zOnXZPlane = height * z / dy;

		float dist = xOnXZPlane*xOnXZPlane + zOnXZPlane*zOnXZPlane;

		if(rPow2 < dist)
			rPow2 = dist;
	}

	return sqrt(rPow2);
}


void Cone::Draw()
{
	bool isLightOn = glUtil::GLEnable( GL_LIGHTING, false );

	glPushMatrix();
	glMultMatrixf(m);

	float radius = _r;
	float halfLength = _h/2.0f;
	float piVal = 3.14159265f;

	_randomColor.Reset();

	glBegin(GL_TRIANGLES);

	for(int i=20; i<=360; i+=20)
	{
		float theta = i*piVal/180.0f;
		float nextTheta = (i+20)*piVal/180.0f;

		glColor(_randomColor.NextColor());
		glVertex3f(0, halfLength, 0);

		glColor4ub(80, 80, 80, 255);
		glVertex3f(radius*cos(theta),	-halfLength,	radius*sin(theta));
		glVertex3f(radius*cos(nextTheta),	-halfLength,	radius*sin(nextTheta));
		glVertex3f(0, -halfLength, 0);

		glColor(_randomColor.NextColor());
		glVertex3f(radius*cos(theta),	-halfLength,	radius*sin(theta));
		glVertex3f(radius*cos(nextTheta),	-halfLength,	radius*sin(nextTheta));
	}

	glEnd();

	glPopMatrix();

	glUtil::GLEnable( GL_LIGHTING, isLightOn );
}

Cone::~Cone()
{
}
