#include "Prim.h"
#include <math.h>

Vertex::Vertex(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	valid = true;
}

Vertex::Vertex()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;

	valid = false;
}

void Vertex::Set(Vertex* vertex)
{
	this->x = vertex->x;
	this->y = vertex->y;
	this->z = vertex->z;
}

void Vertex::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vertex::Move(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void Vertex::RotateInXYZ(float angleX, float angleY, float angleZ)
{
	if(angleX != 0)
		RotateInX(angleX);
		
	if(angleY != 0)
		RotateInY(angleY);
	
	if(angleZ != 0)
		RotateInZ(angleZ);
}

void Vertex::RotateInX(float angleInDegrees)
{
	float yy = y;
	float zz = z;

	float angleInRad = angleInDegrees * 0.01745329252f;

	y = yy*(float)cos(angleInRad) - zz*(float)sin(angleInRad);
	z = yy*(float)sin(angleInRad) + zz*(float)cos(angleInRad);
}

void Vertex::RotateInY(float angleInDegrees)
{
	float zz = z;
	float xx = x;

	float angleInRad = angleInDegrees * 0.01745329252f;

	z = zz*(float)cos(angleInRad) - xx*(float)sin(angleInRad);
	x = zz*(float)sin(angleInRad) + xx*(float)cos(angleInRad);
}

void Vertex::RotateInZ(float angleInDegrees)
{
	float xx = x;
	float yy = y;

	float angleInRad = angleInDegrees * 0.01745329252f;

	x = xx*(float)cos(angleInRad) - yy*(float)sin(angleInRad);
	y = xx*(float)sin(angleInRad) + yy*(float)cos(angleInRad);
}

Vertex Vertex::CloneObject()
{
	return Vertex(x,y,z);
}


//**************************** UV Class functions........................//

UV::UV()
{
	u = 0;
	v = 0;
	valid = false;
}

UV::UV(float u, float v)
{
	this->u = u;
	this->v = v;
	valid = true;
}


//**************************** Normal Class functions........................//

Normal::Normal()
{
	nx = 0;
	ny = 0;
	nz = 0;

	valid = false;
}

Normal::Normal(float nx, float ny, float nz)
{
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;

	valid = true;
}

void Normal::Normalize()
{
	float mag = (float)sqrt((double)(nx*nx + ny*ny + nz*nz));

	nx = nx / mag;
	ny = ny / mag;
	nz = nz / mag;						
}

void Normal::RotateInXYZ(float angleX, float angleY, float angleZ)
{
	if(angleX != 0)
		RotateInX(angleX);
		
	if(angleY != 0)
		RotateInY(angleY);
	
	if(angleZ != 0)
		RotateInZ(angleZ);
}

void Normal::RotateInX(float angleInDegrees)
{
	float yy = ny;
	float zz = nz;

	float angleInRad = angleInDegrees * 0.01745329252f;

	ny = yy*(float)cos(angleInRad) - zz*(float)sin(angleInRad);
	nz = yy*(float)sin(angleInRad) + zz*(float)cos(angleInRad);
}

void Normal::RotateInY(float angleInDegrees)
{
	float zz = nz;
	float xx = nx;

	float angleInRad = angleInDegrees * 0.01745329252f;

	nz = zz*(float)cos(angleInRad) - xx*(float)sin(angleInRad);
	nx = zz*(float)sin(angleInRad) + xx*(float)cos(angleInRad);
}

void Normal::RotateInZ(float angleInDegrees)
{
	float xx = nx;
	float yy = ny;

	float angleInRad = angleInDegrees * 0.01745329252f;

	nx = xx*(float)cos(angleInRad) - yy*(float)sin(angleInRad);
	ny = xx*(float)sin(angleInRad) + yy*(float)cos(angleInRad);
}	

Normal Normal::CloneObject()
{
	return Normal(nx, ny, nz);
}

//************************* TriFace class functions *********************//

TriFace::TriFace(int v1, int t1, int n1, 	int v2, int t2, int n2, 	int v3, int t3, int n3)
{
	this->v1 = v1;
	this->t1 = t1;
	this->n1 = n1;
	
	this->v2 = v2;
	this->t2 = t2;
	this->n2 = n2;

	this->v3 = v3;
	this->t3 = t3;
	this->n3 = n3;
}

TriFace::TriFace(int v1, int t1,	int v2, int t2, 	int v3, int t3)
{
	this->v1 = v1;
	this->t1 = t1;
	this->n1 = 0;
	
	this->v2 = v2;
	this->t2 = t2;
	this->n2 = 0;

	this->v3 = v3;
	this->t3 = t3;
	this->n3 = 0;
}

TriFace::TriFace(int v1, int n1,	int v2, int n2, 	int v3, int n3,		bool temp)
{
	this->v1 = v1;
	this->t1 = 0;
	this->n1 = n1;
	
	this->v2 = v2;
	this->t2 = 0;
	this->n2 = n2;

	this->v3 = v3;
	this->t3 = 0;
	this->n3 = n3;
}

TriFace::TriFace(int v1, int v2, int v3)
{
	this->v1 = v1;
	this->t1 = 0;
	this->n1 = 0;
	
	this->v2 = v2;
	this->t2 = 0;
	this->n2 = 0;

	this->v3 = v3;
	this->t3 = 0;
	this->n3 = 0;
}	
