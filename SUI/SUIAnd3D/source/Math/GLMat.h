#ifndef GLMat_H
#define GLMat_H

#include "Vector3.h"

class GLMat
{
private:
	void MultMat(float* a, float* b, float* result);
	void SetIdentityMatrix(float* m);
	void GetXRotArray(float angle, float* result);
	void GetYRotArray(float angle, float* result);
	void GetZRotArray(float angle, float* result);

public:
	float m[16];

	GLMat();
	~GLMat();
	void glLoadIdentity();
	void glTranslatef(float x, float y, float z, bool reverseOrder = false);
	void glRotatef(float angleInDegrees, int isX, int isY, int isZ, bool reverseOrder = false);
	void glMultMatrixf(float* mat);

	float* Get();
	void Copy(float* mat);
	static int InvertMatrix(const float src[16], float inverse[16]);
};

#endif
