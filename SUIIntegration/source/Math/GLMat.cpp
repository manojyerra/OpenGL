#include "GLMat.h"
#include "math.h"
#include "string.h"

GLMat::GLMat()
{
	SetIdentityMatrix(m);
}

GLMat::~GLMat()
{
}

void GLMat::glLoadIdentity()
{
	memset(m, 0, 16*sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void GLMat::glTranslatef(float x, float y, float z, bool reverseOrder)
{
	float trans[16];
	SetIdentityMatrix(trans);
	trans[12] = x;
	trans[13] = y;
	trans[14] = z;

	float result[16];

	if(reverseOrder == false)
		MultMat(m, trans, result);
	else
		MultMat(trans, m, result);

	memcpy(m, result, 16*sizeof(float));
}

void GLMat::glRotatef(float angleInDegrees, int isX, int isY, int isZ, bool reverseOrder)
{
	float rot[16];

	if(isX == 1)
		GetXRotArray(angleInDegrees, rot);
	else if(isY == 1)
		GetYRotArray(angleInDegrees, rot);
	else if(isZ == 1)
		GetZRotArray(angleInDegrees, rot);

	float result[16];

	if(reverseOrder == false)
		MultMat(m, rot, result);
	else
		MultMat(rot, m, result);

	memcpy(m, result, 16*sizeof(float));
}

void GLMat::glMultMatrixf(float* mat)
{
	float result[16];
	MultMat(m, mat, result);
	memcpy(m, result, 16*sizeof(float));
}

float* GLMat::Get()
{
	return m;
}

void GLMat::Copy(float* mat)
{
	memcpy(m, mat, 16*sizeof(float));
}


void GLMat::SetIdentityMatrix(float* m)
{
	memset(m, 0, 16*sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void GLMat::GetXRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[5] = cos (angleInRad);
	result[6] = sin (angleInRad);
	result[9] = -sin(angleInRad);
	result[10] = cos(angleInRad);
}

void GLMat::GetYRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[0] = cos(angleInRad);
	result[2] = -sin(angleInRad);
	result[8] = sin(angleInRad);
	result[10] = cos(angleInRad);
}

void GLMat::GetZRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[0] = cos(angleInRad);
	result[1] = sin(angleInRad);
	result[4] = -sin(angleInRad);
	result[5] = cos(angleInRad);
}

void GLMat::MultMat(float* a, float* b, float* result)
{
	for(int i=0;i<4;i++)
	{
		int j = i*4;
		result[j+0] = a[0]*b[j] + a[4]*b[j+1] + a[8] *b[j+2] + a[12]*b[j+3];
		result[j+1] = a[1]*b[j] + a[5]*b[j+1] + a[9] *b[j+2] + a[13]*b[j+3];
		result[j+2] = a[2]*b[j] + a[6]*b[j+1] + a[10]*b[j+2] + a[14]*b[j+3];
		result[j+3] = a[3]*b[j] + a[7]*b[j+1] + a[11]*b[j+2] + a[15]*b[j+3];
	}
}

int GLMat::InvertMatrix(const float src[16], float inverse[16])
{
    int i, j, k, swap;
    float t;
    float temp[4][4];

    for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			temp[i][j] = src[i*4+j];

	for(int i=0;i<16;i++)
		inverse[i] = 0;
	inverse[0] = inverse[5] = inverse[10] = inverse[15] = 1.0f;

    for(i=0; i<4; i++)
	{
		swap = i;
		for (j = i + 1; j < 4; j++)
			if (fabs(temp[j][i]) > fabs(temp[i][i]))
				swap = j;

		if (swap != i) {
			//Swap rows.
			for (k = 0; k < 4; k++) {
				t = temp[i][k];
				temp[i][k] = temp[swap][k];
				temp[swap][k] = t;
				
				t = inverse[i*4+k];
				inverse[i*4+k] = inverse[swap*4+k];
				inverse[swap*4+k] = t;
			}
		}
		
		if (temp[i][i] == 0)
			return 0;

		t = temp[i][i];
		for (k = 0; k < 4; k++) {
			temp[i][k] /= t;
			inverse[i*4+k] /= t;
		}

		for (j = 0; j < 4; j++) {
			if (j != i) {
				t = temp[j][i];
				for (k = 0; k < 4; k++) {
					temp[j][k] -= temp[i][k]*t;
					inverse[j*4+k] -= inverse[i*4+k]*t;
				}
			}
		}
    }

    return 1;
}




	/*
	result[0] = b[0]*a[0] + b[1]*a[4] + b[2]*a[8] + b[3]*a[12];
	result[1] = b[0]*a[1] + b[1]*a[5] + b[2]*a[9] + b[3]*a[13];
	result[2] = b[0]*a[2] + b[1]*a[6] + b[2]*a[10] + b[3]*a[14];
	result[3] = b[0]*a[3] + b[1]*a[7] + b[2]*a[11] + b[3]*a[15];

	result[4] = b[4]*a[0] + b[5]*a[4] + b[6]*a[8] + b[7]*a[12];
	result[5] = b[4]*a[1] + b[5]*a[5] + b[6]*a[9] + b[7]*a[13];
	result[6] = b[4]*a[2] + b[5]*a[6] + b[6]*a[10] + b[7]*a[14];
	result[7] = b[4]*a[3] + b[5]*a[7] + b[6]*a[11] + b[7]*a[15];

	result[8] = b[8]*a[0] + b[9]*a[4] + b[10]*a[8] + b[11]*a[12];
	result[9] = b[8]*a[1] + b[9]*a[5] + b[10]*a[9] + b[11]*a[13];
	result[10] = b[8]*a[2] + b[9]*a[6] + b[10]*a[10] + b[11]*a[14];
	result[11] = b[8]*a[3] + b[9]*a[7] + b[10]*a[11] + b[11]*a[15];

	result[12] = b[12]*a[0] + b[13]*a[4] + b[14]*a[8] + b[15]*a[12];
	result[13] = b[12]*a[1] + b[13]*a[5] + b[14]*a[9] + b[15]*a[13];
	result[14] = b[12]*a[2] + b[13]*a[6] + b[14]*a[10] + b[15]*a[14];
	result[15] = b[12]*a[3] + b[13]*a[7] + b[14]*a[11] + b[15]*a[15];
	*/