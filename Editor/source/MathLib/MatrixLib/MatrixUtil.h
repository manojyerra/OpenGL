#ifndef MatrixUtil_H
#define MatrixUtil_H

#include "math.h"
#include "string.h"
#include "Def.h"
#include "GLMat.h"
#include "MathLib/Point.h"

class MatrixUtil
{
public:
	static void SetIdentityMatrix(float* m);
	static void GetTransArray(float xPos, float yPos, float zPos, float* result);
	static void GetXRotArray(float angleInDegrees, float* result);
	static void GetYRotArray(float angleInDegrees, float* result);
	static void GetZRotArray(float angleInDegrees, float* result);
	static void MultMat(float* a, float* b, float* result);
	static void GetXYZRotArray(float xAng, float yAng, float zAng, float* result);

	// Blender and GL matrix related functions...

	static GLMat GetBlenderToGLMat(float* pos, float* rot);
	static GLMat GetBlenderToGLMat(float x, float y, float z, float xRot, float yRot, float zRot);
	static GLMat GetGapMatrixFromBlenderToGL();
	static GLMat GetGLMat(float* pos, float* rot);
	static GLMat GetGLMat(float x, float y, float z, float xRot, float yRot, float zRot);

	static Point GetEulerXYZRot_In_Degrees(float* mat);
	static Point GetEulerXYZRot_In_Radians(float* mat);

	static int InvertMatrix(float* src, float* inverse);
};


#endif


/*
	def GetEulerXYZRot_In_Degrees(r, xyzRot):
		MatUtil.GetEulerXYZRot_In_Radians(r, xyzRot)
		xyzRot[0] *= Def.RAD_DEG
		xyzRot[1] *= Def.RAD_DEG
		xyzRot[2] *= Def.RAD_DEG

	def GetEulerXYZRot_In_Radians(r, xyzRot):
		xyzRot[0] = math.atan2( r[6], r[10] )
		xyzRot[1] = math.atan2( -r[2], math.sqrt(r[6]*r[6] + r[10]*r[10]) )
		xyzRot[2] = math.atan2( r[1], r[0] )
	
	def GetEulerYZXRot_In_Degrees(r, xyzRot):
		MatUtil.GetEulerYZXRot_In_Radians(r, xyzRot)
		xyzRot[0] *= Def.RAD_DEG
		xyzRot[1] *= Def.RAD_DEG
		xyzRot[2] *= Def.RAD_DEG
*/


/*

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

*/