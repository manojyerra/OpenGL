#ifndef GLMat_H
#define GLMat_H

#include "Macros.h"
#include "Vector3.h"

class GLMat
{
private:
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
	void glScalef(float x, float y, float z, bool reverseOrder = false);
	void glMultMatrixf(float* mat);

	float* Get();
	void Copy(float* mat);
	void CopyRotation(float* mat);

	CVector3 GetEulerXYZRot_In_Degrees();
	CVector3 GetEulerXYZRot_In_Radians();
	CVector3 GetScale();
	void RemoveScale();
	void SetScale(CVector3 scale);

	void SetPos(float x, float y, float z);
	void SetPos(CVector3 pos);
	CVector3 GetPos();
	CVector3 GetRotation();
	void SetRotation(CVector3 rot);

	void AddTransInWorld(float x, float y, float z);
	void AddTransInLocal(char axis, float move);
	void AddRotateInWorld(char axis, float angle);
	void AddRotateInLocal(char axis, float angle);

	static void MultMat(float* a, float* b, float* result);
	static int InvertMatrix(const float src[16], float inverse[16]);
	static void GetGluLookAtParameters(float* m, float* gluLookAtParams);
};

#endif
