#include "Sphere.h"
#include "../Util/GLUtil.h"
#include "../Math/TransformVertexBuf.h"

Sphere::Sphere(float x, float y, float z, float r) : Shape(Shape::SPHERE)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;

	_r = r;

	_quad = gluNewQuadric();
}


Sphere::Sphere(Sphere* sphere) : Shape(Shape::SPHERE)
{
	memcpy(m, sphere->m, 16*4);

	_r = sphere->GetRadius();

	_quad = gluNewQuadric();
}


Sphere::Sphere(float* mat, float r) : Shape(Shape::SPHERE)
{
	memcpy(m, mat, 16*4);
	
	_r = r;

	_quad = gluNewQuadric();
}


Sphere::Sphere(CVector3 pos, float r) : Shape(Shape::SPHERE)
{
	m[12] = pos.x;
	m[13] = pos.y;
	m[14] = pos.z;

	_r = r;

	_quad = gluNewQuadric();
}

Sphere Sphere::CalcBoundingSphere(float* vertexBuf, int arrSize)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize*sizeof(float));

	CVector3 center = TransformVertexBuf::CalcCenter(localVertexBuf, arrSize);
	TransformVertexBuf::Subtract(localVertexBuf, arrSize, center);
        
	float rPow2 = 0;

	for(int i=0; i<arrSize; i+=3)
	{
		float x = localVertexBuf[i+0];
		float y = localVertexBuf[i+1];
		float z = localVertexBuf[i+2];

		float lenPow2 = x*x + y*y + z*z;

		if(rPow2 < lenPow2)
			rPow2 = lenPow2;
	}

	delete[] localVertexBuf;

	return Sphere(center, sqrt(rPow2));
}

float Sphere::GetRadius()
{
	return _r;
}

void Sphere::SetRadius(float r)
{
	if(r > 0)
		_r = r;
}

float Sphere::Volume()
{
	return (4.0f/3.0f) * PI_VAL * _r * _r * _r;
}

void Sphere::Draw()
{
	unsigned int prevColor = GLUtil::GLColor(0xff0000ff);
	GLboolean colorMtlEnabled = GLUtil::GLEnable(GL_COLOR_MATERIAL, true);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, true);

	glPushMatrix();
	glMultMatrixf(m);
	gluSphere(_quad,_r,20,20);
	glPopMatrix();

	GLUtil::GLEnable(GL_COLOR_MATERIAL, colorMtlEnabled);
	GLUtil::GLEnable(GL_BLEND, blend);
	GLUtil::GLColor(0xff0000ff);
}

Sphere::~Sphere()
{
	//gluDeleteQuadric(_quad);
}


	//// Create light components
	//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	//GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
	//GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientLight);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseLight);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);


	//GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.0f};
	//GLfloat cyan[] = {1.0f, 1.0f, 1.0f, 1.f};
	//GLfloat shininess[] = {50};

	//glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	//glMaterialfv(GL_FRONT, GL_SHININESS, shininess);