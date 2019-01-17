#include "DXMat.h"
#include "math.h"
#include "string.h"

DXMat::DXMat()
{
	SetIdentityMatrix(m);
}

DXMat::~DXMat()
{
}

void DXMat::dxLoadIdentity()
{
	memset(m, 0, 16*sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void DXMat::dxTranslatef(float x, float y, float z)
{
	float trans[16];
	SetIdentityMatrix(trans);
	trans[3] = x;
	trans[7] = y;
	trans[11] = z;

	float result[16];
	MultMat(m, trans, result);
	memcpy(m, result, 16*sizeof(float));
}

void DXMat::dxRotatef(float angleInDegrees, int isX, int isY, int isZ)
{
	float rot[16];

	if(isX == 1)
		GetXRotArray(angleInDegrees, rot);
	else if(isY == 1)
		GetYRotArray(angleInDegrees, rot);
	else if(isZ == 1)
		GetZRotArray(angleInDegrees, rot);


	float result[16];
	MultMat(m, rot, result);
	memcpy(m, result, 16*sizeof(float));
}

float* DXMat::Get()
{
	return m;
}

void DXMat::Copy(float* mat)
{
	memcpy(mat, m, 16*sizeof(float));
}


void DXMat::SetIdentityMatrix(float* m)
{
	memset(m, 0, 16*sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void DXMat::GetXRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[5] = cos (angleInRad);
	result[6] = -sin (angleInRad);
	result[9] = sin(angleInRad);
	result[10] = cos(angleInRad);
}

void DXMat::GetYRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[0] = cos(angleInRad);
	result[2] = sin(angleInRad);
	result[8] = -sin(angleInRad);
	result[10] = cos(angleInRad);
}

void DXMat::GetZRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[0] = cos(angleInRad);
	result[1] = -sin(angleInRad);
	result[4] = sin(angleInRad);
	result[5] = cos(angleInRad);
}

void DXMat::MultMat(float* a, float* b, float* result)
{
	result[0] = a[0]*b[0] + a[1]*b[4] + a[2]*b[8] + a[3]*b[12];
	result[1] = a[0]*b[1] + a[1]*b[5] + a[2]*b[9] + a[3]*b[13];
	result[2] = a[0]*b[2] + a[1]*b[6] + a[2]*b[10] + a[3]*b[14];
	result[3] = a[0]*b[3] + a[1]*b[7] + a[2]*b[11] + a[3]*b[15];

	result[4] = a[4]*b[0] + a[5]*b[4] + a[6]*b[8] + a[7]*b[12];
	result[5] = a[4]*b[1] + a[5]*b[5] + a[6]*b[9] + a[7]*b[13];
	result[6] = a[4]*b[2] + a[5]*b[6] + a[6]*b[10] + a[7]*b[14];
	result[7] = a[4]*b[3] + a[5]*b[7] + a[6]*b[11] + a[7]*b[15];

	result[8] = a[8]*b[0] + a[9]*b[4] + a[10]*b[8] + a[11]*b[12];
	result[9] = a[8]*b[1] + a[9]*b[5] + a[10]*b[9] + a[11]*b[13];
	result[10] = a[8]*b[2] + a[9]*b[6] + a[10]*b[10] + a[11]*b[14];
	result[11] = a[8]*b[3] + a[9]*b[7] + a[10]*b[11] + a[11]*b[15];

	result[12] = a[12]*b[0] + a[13]*b[4] + a[14]*b[8] + a[15]*b[12];
	result[13] = a[12]*b[1] + a[13]*b[5] + a[14]*b[9] + a[15]*b[13];
	result[14] = a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
	result[15] = a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];
}
