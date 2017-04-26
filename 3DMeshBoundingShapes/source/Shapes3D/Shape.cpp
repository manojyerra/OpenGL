#include "Shape.h"
#include "Box.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"

Shape::Shape()
{
	_id = NONE;
	memset(m, 0, 16*4);
	m[0] = m[5] = m[10] = m[15] = 1;
	_color[0] = _color[1] = _color[2] = _color[3] = 255;
}

Shape::Shape(int id)
{
	_id = id;
	memset(m, 0, 16*4);
	m[0] = m[5] = m[10] = m[15] = 1;
	_color[0] = _color[1] = _color[2] = _color[3] = 255;
}

int Shape::GetID()
{
	return _id;
}

void Shape::SetGLMatrix(float* mat)
{
	memcpy(m, mat, 16*4);
}

float* Shape::GetGLMatrix()
{
	return m;
}

void Shape::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
	_color[3] = a;
}

Shape* Shape::GetBestFitBoundingShape(float* vertexBuf, int arrSize)
{
	Box boxShape			= Box::CalcBoundingBox(vertexBuf, arrSize);
	Cone coneShape			= Cone::CalcBoundingCone(vertexBuf, arrSize);
	Cylinder cylinderShape	= Cylinder::CalcBoundingCylinder(vertexBuf, arrSize);
	Sphere sphereShape		= Sphere::CalcBoundingSphere(vertexBuf, arrSize);

	Shape* shape1 = new Box( &boxShape );
	Shape* shape2 = new Cone( &coneShape );
	Shape* shape3 = new Cylinder( &cylinderShape );
	Shape* shape4 = new Sphere( &sphereShape );

	float volume1 = shape1->Volume();
	float volume2 = shape2->Volume();
	float volume3 = shape3->Volume();
	float volume4 = shape4->Volume();

	Shape* returnShape = NULL;

	if(volume1 <= volume2 && volume1 <= volume3 && volume1 <= volume4)			returnShape = shape1;
	else if(volume2 <= volume1 && volume2 <= volume3 && volume2 <= volume4)		returnShape = shape2;
	else if(volume3 <= volume1 && volume3 <= volume2 && volume3 <= volume4)		returnShape = shape3;
	else																		returnShape = shape4;

	if(returnShape != shape1) delete shape1;
	if(returnShape != shape2) delete shape2;
	if(returnShape != shape3) delete shape3;
	if(returnShape != shape4) delete shape4;

	return returnShape;
}

Shape::~Shape()
{
}