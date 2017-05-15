#ifndef Shape_H
#define Shape_H

#include "DefinesAndIncludes.h"
#include "Math/Vector3.h"

class Shape
{
protected:
	int _id;
	unsigned char _color[4];
	unsigned char _randomColorAlpha;

public:
	float m[16];

	static const int NONE = 0;
	static const int BOX = 1;
	static const int CONE = 2;
	static const int CYLINDER = 3;
	static const int SPHERE = 4;
	static const int MESH = 5;

	Shape();
	Shape(int id);
	int GetID();
	void SetRandomColorAlpha(unsigned char alpha);

	void SetGLMatrix(float* mat);
	float* GetGLMatrix();
	
	CVector3 GetPos();
	void SetPos(CVector3 pos);
	void SetPos(float x, float y, float z);

	void AddTransInWorld(float x, float y, float z);
	void AddRotateInWorld(char axis, float angle);

	void AddTransInLocal(char axis, float move);
	void AddRotateInLocal(char axis, float angle);

	void AddUniformScale(float scale);
	void AddScale(CVector3 scale);

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	
	static Shape* GetBestFitBoundingShape(float* vertexBuf, int arrSize);
	static Shape* GetBoundingShape(float* vertexBuf, int arrSize, int boundingShapeID);

	virtual float Volume() = 0;
	virtual void Draw() = 0;
	virtual ~Shape();
};

#endif