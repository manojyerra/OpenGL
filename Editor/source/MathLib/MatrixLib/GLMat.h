#ifndef GLMat_H
#define GLMat_H

#include "Def.h"

class GLMat
{
private:
	float m[16];

	void MultMat(float* a, float* b, float* result);
	void SetIdentityMatrix(float* m);
	void GetXRotArray(float angle, float* result);
	void GetYRotArray(float angle, float* result);
	void GetZRotArray(float angle, float* result);

public:	
	GLMat();
	~GLMat();
	void glLoadIdentity();
	void glTranslatef(float x, float y, float z, bool reverseOrder = false);
	void glRotatef(float angleInDegrees, int isX, int isY, int isZ, bool reverseOrder = false);
	void glMultMatrixf(float* mat);

	float* Get();
	void Copy(float* mat);
	int __gluInvertMatrixd(const float src[16], float inverse[16]);
};

#endif
