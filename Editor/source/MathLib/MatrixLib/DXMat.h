#ifndef DXMat_H
#define DXMat_H

#include "Def.h"

class DXMat
{
private:
	float m[16];

	void MultMat(float* a, float* b, float* result);
	void SetIdentityMatrix(float* m);
	void GetXRotArray(float angle, float* result);
	void GetYRotArray(float angle, float* result);
	void GetZRotArray(float angle, float* result);

public:	
	DXMat();
	~DXMat();
	void dxLoadIdentity();
	void dxTranslatef(float x, float y, float z);
	void dxRotatef(float angleInDegrees, int isX, int isY, int isZ);
	float* Get();
	void Copy(float* mat);
};

#endif
