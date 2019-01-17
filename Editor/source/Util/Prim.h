#ifndef Prim_H
#define Prim_H

class Vertex
{
public:
	float x;
	float y;
	float z;

	bool valid;
	
	Vertex();
	Vertex(float x, float y, float z);
	void Set(Vertex* vertex);
	void Set(float x, float y, float z);
	void Move(float x, float y, float z);
	void RotateInXYZ(float angleX, float angleY, float angleZ);
	void RotateInX(float angleInDegrees);
	void RotateInY(float angleInDegrees);
	void RotateInZ(float angleInDegrees);
	Vertex CloneObject();
};

class UV
{
public:
	float u;
	float v;
	bool valid;

	UV();
	UV(float u, float v);
};

class Normal
{
public:
	float nx;
	float ny;
	float nz;
	bool valid;
	
	Normal();
	Normal(float nx, float ny, float nz);
	void Normalize();
	void RotateInXYZ(float angleX, float angleY, float angleZ);
	void RotateInX(float angleInDegrees);
	void RotateInY(float angleInDegrees);
	void RotateInZ(float angleInDegrees);
	Normal CloneObject();
};

class TriFace
{
public:
	int v1;
	int v2;
	int v3;
	
	int t1;
	int t2;
	int t3;
	
	int n1;
	int n2;
	int n3;
	
	TriFace(int v1, int t1, int n1, 	int v2, int t2, int n2, 	int v3, int t3, int n3					);
	TriFace(int v1, int t1,				int v2, int t2, 			int v3, int t3							);
	TriFace(int v1, int n1,				int v2, int n2, 			int v3, int n3,				bool temp	);
	TriFace(int v1,						int v2,						int v3									);
};

#endif