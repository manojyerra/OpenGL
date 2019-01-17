#include "MatrixUtil.h"

void MatrixUtil::SetIdentityMatrix(float* m)
{
	memset(m, 0, 16*sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void MatrixUtil::GetTransArray(float xPos, float yPos, float zPos, float* result)
{
	SetIdentityMatrix(result);
	result[12] = xPos;
	result[13] = yPos;
	result[14] = zPos;
}

void MatrixUtil::GetXRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[5] = cos (angleInRad);
	result[6] = sin (angleInRad);
	result[9] = -sin(angleInRad);
	result[10] = cos(angleInRad);
}

void MatrixUtil::GetYRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[0] = cos(angleInRad);
	result[2] = -sin(angleInRad);
	result[8] = sin(angleInRad);
	result[10] = cos(angleInRad);
}

void MatrixUtil::GetZRotArray(float angleInDegrees, float* result)
{
	SetIdentityMatrix(result);
	float angleInRad = angleInDegrees * DEG_RAD;
	result[0] = cos(angleInRad);
	result[1] = sin(angleInRad);
	result[4] = -sin(angleInRad);
	result[5] = cos(angleInRad);
}

void MatrixUtil::MultMat(float* a, float* b, float* result)
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

void MatrixUtil::GetXYZRotArray(float xAng, float yAng, float zAng, float* result)
{
	float xRot[16];
	float yRot[16];
	float zRot[16];

	GetXRotArray(xAng, xRot);
	GetYRotArray(yAng, yRot);
	GetZRotArray(zAng, zRot);

	float temp[16];   
	MultMat(xRot, yRot, temp);
	MultMat(temp, zRot, result);
}

// Blender and GL matrix related functions...


GLMat MatrixUtil::GetBlenderToGLMat(float* pos, float* rot)
{
	return GetBlenderToGLMat(pos[0],pos[1],pos[2], rot[0],rot[1],rot[2]);
}

GLMat MatrixUtil::GetBlenderToGLMat(float x, float y, float z, float xRot, float yRot, float zRot)
{
	GLMat glMat;
	glMat.glRotatef(-90, 1,0,0);
	glMat.glRotatef(-90, 0,0,1);
	glMat.glTranslatef(x,y,z);
	glMat.glRotatef(zRot, 0,0,1);
	glMat.glRotatef(yRot, 0,1,0);
	glMat.glRotatef(xRot, 1,0,0);
	return glMat;
}

GLMat MatrixUtil::GetGapMatrixFromBlenderToGL()
{
	GLMat glMat;
	glMat.glRotatef(-90, 1,0,0);
	glMat.glRotatef(-90, 0,0,1);
	return glMat;
}

GLMat MatrixUtil::GetGLMat(float* pos, float* rot)
{
	return GetGLMat(pos[0],pos[1],pos[2], rot[0],rot[1],rot[2]);
}

GLMat MatrixUtil::GetGLMat(float x, float y, float z, float xRot, float yRot, float zRot)
{
	GLMat glMat;
	glMat.glTranslatef(x,y,z);
	glMat.glRotatef(zRot, 0,0,1);
	glMat.glRotatef(yRot, 0,1,0);
	glMat.glRotatef(xRot, 1,0,0);
	return glMat;
}

Point MatrixUtil::GetEulerXYZRot_In_Degrees(float* mat)
{
	Point xyzRot = GetEulerXYZRot_In_Radians(mat);
	xyzRot.x *= RAD_DEG;
	xyzRot.y *= RAD_DEG;
	xyzRot.z *= RAD_DEG;
	return xyzRot;
}

Point MatrixUtil::GetEulerXYZRot_In_Radians(float* r)
{
	Point xyzRot;
	xyzRot.x = atan2( r[6], r[10] );
	xyzRot.y = atan2( -r[2], sqrt(r[6]*r[6] + r[10]*r[10]) );
	xyzRot.z = atan2( r[1], r[0] );
	return xyzRot;
}

int MatrixUtil::InvertMatrix(float* src, float* inverse)
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