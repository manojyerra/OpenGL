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

void GLMat::glScalef(float x, float y, float z, bool reverseOrder)
{
	float scaleMat[16];
	memset(scaleMat, 0, 16*sizeof(float));

	scaleMat[0] = x;
	scaleMat[5] = y;
	scaleMat[10] = z;
	scaleMat[15] = 1.0f;

	float result[16];

	if(reverseOrder == false)
		MultMat(m, scaleMat, result);
	else
		MultMat(scaleMat, m, result);

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

void GLMat::CopyRotation(float* mat)
{
	m[0] = mat[0];
	m[1] = mat[1];
	m[2] = mat[2];

	m[4] = mat[4];
	m[5] = mat[5];
	m[6] = mat[6];

	m[8] = mat[8];
	m[9] = mat[9];
	m[10] = mat[10];
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

void GLMat::GetGluLookAtParameters(float* m, float* gluLookAtParams)
{
	CVector3 sideVector(m[0], m[4], m[8]);
	CVector3 upVector(m[1], m[5], m[9]);
	CVector3 forwardVector(-m[2], -m[6], -m[10]);

	sideVector.Normalize();
	upVector.Normalize();
	forwardVector.Normalize();
	
	float rotMat[16];
	memcpy(rotMat, m, 16*sizeof(float));
	rotMat[12] = rotMat[13] = rotMat[14] = rotMat[3] = rotMat[7] = rotMat[11] = 0.0f;
	rotMat[15] = 1.0f;
	float rotInvert[16];
	InvertMatrix(rotMat, rotInvert);

	float transMat[16];
	memset(transMat, 0, 16*sizeof(float));
	transMat[0] = transMat[5] = transMat[10] = transMat[15] = 1.0f;
	MultMat(rotInvert, m, transMat);

	gluLookAtParams[0] = -transMat[12];
	gluLookAtParams[1] = -transMat[13];
	gluLookAtParams[2] = -transMat[14];

	gluLookAtParams[3] = -transMat[12] + forwardVector.x;
	gluLookAtParams[4] = -transMat[13] + forwardVector.y;
	gluLookAtParams[5] = -transMat[14] + forwardVector.z;
	
	gluLookAtParams[6] = upVector.x;
	gluLookAtParams[7] = upVector.y;
	gluLookAtParams[8] = upVector.z;
}

CVector3 GLMat::GetScale()
{
	float scaleX = sqrt( m[0]*m[0] + m[1]*m[1] + m[2]*m[2] );
	float scaleY = sqrt( m[4]*m[4] + m[5]*m[5] + m[6]*m[6] );
	float scaleZ = sqrt( m[8]*m[8] + m[9]*m[9] + m[10]*m[10] );

	return CVector3( scaleX, scaleY, scaleZ );
}

void GLMat::RemoveScale()
{
	CVector3 xVec(m[0], m[1], m[2]);
	CVector3 yVec(m[4], m[5], m[6]);
	CVector3 zVec(m[8], m[9], m[10]);

	xVec.Normalize();
	yVec.Normalize();
	zVec.Normalize();

	m[0] = xVec.x;
	m[1] = xVec.y;
	m[2] = xVec.z;

	m[4] = yVec.x;
	m[5] = yVec.y;
	m[6] = yVec.z;

	m[8] = zVec.x;
	m[9] = zVec.y;
	m[10] = zVec.z;
}

void GLMat::SetScale(CVector3 scale)
{
	CVector3 xVec(m[0], m[1], m[2]);
	CVector3 yVec(m[4], m[5], m[6]);
	CVector3 zVec(m[8], m[9], m[10]);

	xVec.SetLength( scale.x );
	yVec.SetLength( scale.y );
	zVec.SetLength( scale.z );

	m[0] = xVec.x;
	m[1] = xVec.y;
	m[2] = xVec.z;

	m[4] = yVec.x;
	m[5] = yVec.y;
	m[6] = yVec.z;

	m[8] = zVec.x;
	m[9] = zVec.y;
	m[10] = zVec.z;
}

void GLMat::SetPos(float x, float y, float z)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

void GLMat::SetPos(CVector3 pos)
{
	m[12] = pos.x;
	m[13] = pos.y;
	m[14] = pos.z;
}

CVector3 GLMat::GetPos()
{
	return CVector3(m[12], m[13], m[14]);
}

CVector3 GLMat::GetRotation()
{
	GLMat newMat;
	newMat.Copy(m);
	newMat.RemoveScale();

	return newMat.GetEulerXYZRot_In_Degrees();
}

void GLMat::SetRotation(CVector3 rot)
{
	CVector3 scale = GetScale();
	RemoveScale();

	GLMat newMat;
	
	newMat.glTranslatef( m[12], m[13], m[14] );
	newMat.glRotatef(rot.z, 0,0,1);
	newMat.glRotatef(rot.y, 0,1,0);
	newMat.glRotatef(rot.x, 1,0,0);
	newMat.glScalef(scale.x, scale.y, scale.z);

	Copy(newMat.m);
}

void GLMat::AddTransInWorld(float x, float y, float z)
{
	m[12] += x;
	m[13] += y;
	m[14] += z;
}

void GLMat::AddRotateInWorld(char axis, float angle)
{
	GLMat newRot;
	
	if(axis == 'x' || axis == 'X')	newRot.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	newRot.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	newRot.glRotatef(angle, 0,0,1);

	newRot.glMultMatrixf( m );

	Copy(newRot.m);
}

void GLMat::AddTransInLocal(char axis, float move)
{
	CVector3 vec;

	if(axis == 'x')			vec = CVector3( m[0], m[1], m[2] );
	else if(axis == 'y')	vec = CVector3( m[4], m[5], m[6] );
	else if(axis == 'z')	vec = CVector3( m[8], m[9], m[10] );
	else
	{
		return;
	}
	
	vec.Normalize();
	vec *= move;

	m[12] += vec.x;
	m[13] += vec.y;
	m[14] += vec.z;
}

void GLMat::AddRotateInLocal(char axis, float angle)
{
	CVector3 scale = GetScale();

	RemoveScale();

	GLMat rotMat;
	
	if(axis == 'x' || axis == 'X')	rotMat.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	rotMat.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	rotMat.glRotatef(angle, 0,0,1);

	glMultMatrixf(rotMat.Get());

	SetScale( scale );
}

CVector3 GLMat::GetEulerXYZRot_In_Degrees()
{
	CVector3 xyzRot = GetEulerXYZRot_In_Radians();
	xyzRot.x *= RAD_DEG;
	xyzRot.y *= RAD_DEG;
	xyzRot.z *= RAD_DEG;

	float decimX = xyzRot.x - (int)xyzRot.x;
	float decimY = xyzRot.y - (int)xyzRot.y;
	float decimZ = xyzRot.z - (int)xyzRot.z;

	xyzRot.x = ( (int)xyzRot.x + 360 ) % 360  + decimX;
	xyzRot.y = ( (int)xyzRot.y + 360 ) % 360  + decimY;
	xyzRot.z = ( (int)xyzRot.z + 360 ) % 360  + decimZ;

	return xyzRot;
}

CVector3 GLMat::GetEulerXYZRot_In_Radians()
{
	GLMat newMat;

	newMat.Copy(m);

	CVector3 xVec(m[0], m[1], m[2]);
	CVector3 yVec(m[4], m[5], m[6]);
	CVector3 zVec(m[8], m[9], m[10]);

	xVec.Normalize();
	yVec.Normalize();
	zVec.Normalize();

	newMat.m[0] = xVec.x;
	newMat.m[1] = xVec.y;
	newMat.m[2] = xVec.z;

	newMat.m[4] = yVec.x;
	newMat.m[5] = yVec.y;
	newMat.m[6] = yVec.z;

	newMat.m[8] = zVec.x;
	newMat.m[9] = zVec.y;
	newMat.m[10] = zVec.z;

	float* a = newMat.m;

	CVector3 xyzRot;
	xyzRot.x = atan2( a[6], a[10] );
	xyzRot.y = atan2( -a[2], sqrt(a[6]*a[6] + a[10]*a[10]) );
	xyzRot.z = atan2( a[1], a[0] );

	return xyzRot;
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