#include "Shape.h"
#include "Box.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "../Math/GLMat.h"

Shape::Shape()
{
	_id = NONE;
	memset(m, 0, 16*4);
	m[0] = m[5] = m[10] = m[15] = 1;
	_color[0] = _color[1] = _color[2] = _color[3] = 255;
	_randomColorAlpha = 255;
}

Shape::Shape(int id)
{
	_id = id;
	memset(m, 0, 16*4);
	m[0] = m[5] = m[10] = m[15] = 1;
	_color[0] = _color[1] = _color[2] = _color[3] = 255;
	_randomColorAlpha = 255;
}

int Shape::GetID()
{
	return _id;
}

void Shape::SetRandomColorAlpha(unsigned char alpha)
{
	_randomColorAlpha = alpha;
}

void Shape::SetGLMatrix(float* mat)
{
	memcpy(m, mat, 16*4);
}

float* Shape::GetGLMatrix()
{
	return m;
}

void Shape::SetPos(float x, float y, float z)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

void Shape::SetPos(CVector3 pos)
{
	m[12] = pos.x;
	m[13] = pos.y;
	m[14] = pos.z;
}

CVector3 Shape::GetPos()
{
	return CVector3(m[12], m[13], m[14]);
}

void Shape::AddTransInWorld(float x, float y, float z)
{
	m[12] += x;
	m[13] += y;
	m[14] += z;
}

void Shape::AddRotateInWorld(char axis, float angle)
{
	GLMat newRot;
	
	if(axis == 'x' || axis == 'X')	newRot.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	newRot.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	newRot.glRotatef(angle, 0,0,1);

	newRot.glMultMatrixf( m );

	memcpy(m, newRot.m, 16*sizeof(float));
}

void Shape::AddTransInLocal(char axis, float move)
{
	CVector3 vec;

	if(axis == 'x')			vec = CVector3( m[0], m[1], m[2] );
	else if(axis == 'y')	vec = CVector3( m[4], m[5], m[6] );
	else if(axis == 'z')	vec = CVector3( m[8], m[9], m[10] );
	
	vec.Normalize();
	vec *= move;

	m[12] += vec.x;
	m[13] += vec.y;
	m[14] += vec.z;
}

void Shape::AddRotateInLocal(char axis, float angle)
{
	GLMat rotMat;
	
	if(axis == 'x' || axis == 'X')	rotMat.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	rotMat.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	rotMat.glRotatef(angle, 0,0,1);

	GLMat mat;
	mat.Copy(m);

	mat.glMultMatrixf(rotMat.Get());

	memcpy(m, mat.m, 16*sizeof(float));
}


void Shape::AddUniformScale(float scale)
{
	if(_id == CYLINDER)
	{
		Cylinder* cylinder = (Cylinder*)this;
		cylinder->SetRadius( cylinder->GetRadius() * scale );
		cylinder->SetHeight( cylinder->GetHeight() * scale );
	}
	else if(_id == SPHERE)
	{
		Sphere* sphere = (Sphere*)this;
		sphere->SetRadius( sphere->GetRadius() * scale );
	}
	else if(_id == CONE)
	{
		Cone* cone = (Cone*)this;
		cone->SetRadius( cone->GetRadius() * scale );
		cone->SetHeight( cone->GetHeight() * scale );
	}
	else if(_id == BOX)
	{
		Box* box = (Box*)this;
		
		CVector3 size = box->GetSize();

		box->SetSize( size.x * scale, size.y * scale, size.z * scale );
	}
}

void Shape::AddScale(CVector3 scale)
{
	if(_id == CYLINDER)
	{
		Cylinder* cylinder = (Cylinder*)this;
		cylinder->SetRadius( cylinder->GetRadius() + scale.x + scale.z );
		cylinder->SetHeight( cylinder->GetHeight() + scale.y*2.0f );
	}
	else if(_id == SPHERE)
	{
		Sphere* sphere = (Sphere*)this;
		sphere->SetRadius( sphere->GetRadius() + scale.x + scale.y + scale.z );
	}
	else if(_id == CONE)
	{
		Cone* cone = (Cone*)this;
		cone->SetRadius( cone->GetRadius() + scale.x + scale.z );
		cone->SetHeight( cone->GetHeight() + scale.y*2.0f );
	}
	else if(_id == BOX)
	{
		Box* box = (Box*)this;
		
		CVector3 size = box->GetSize();

		box->SetSize( size.x + scale.x*2.0f, size.y + scale.y*2.0f, size.z + scale.z*2.0f );
	}
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

	if(returnShape != shape1) { delete shape1; shape1 = NULL; }
	if(returnShape != shape2) { delete shape2; shape2 = NULL; }
	if(returnShape != shape3) { delete shape3; shape3 = NULL; }
	if(returnShape != shape4) { delete shape4; shape4 = NULL; }

	return returnShape;
}

Shape* Shape::GetBoundingShape(float* vertexBuf, int arrSize, int boundingShapeID)
{
	Shape* shape = NULL;

	if(boundingShapeID == BOX)		shape = new Box		( &(Box::CalcBoundingBox(vertexBuf, arrSize)) );
	if(boundingShapeID == CONE)		shape = new Cone	( &(Cone::CalcBoundingCone(vertexBuf, arrSize)) );
	if(boundingShapeID == CYLINDER)	shape = new Cylinder( &(Cylinder::CalcBoundingCylinder(vertexBuf, arrSize)) );
	if(boundingShapeID == SPHERE)	shape = new Sphere	( &(Sphere::CalcBoundingSphere(vertexBuf, arrSize)) );

	return shape;
}

Shape::~Shape()
{
}