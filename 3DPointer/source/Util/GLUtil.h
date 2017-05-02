#ifndef GLUtil_H
#define GLUtil_H
#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "Math/GLMat.h"
#include "Math/Vector3.h"

#include <vector>
using namespace std;

#define glColor(c) glColor4ub((c >> 24) & 255, (c >> 16) & 255, (c >> 8) & 255, (c) & 255)

class GLUtil
{
private:
	static float _viewX;
	static float _viewY;
	static float _viewW;
	static float _viewH;

	static float _angleX;
	static float _angleY;
	static float _angleZ;

	static float _transZ;
	static float _transX;
	static float _transY;

	static float SW;
	static float SH;

	static void SetFrustum(float left, float right, float bottom, float top, float zNear, float zFar);

public:
	static float _left;
	static float _right;
	static float _bottom;
	static float _top;
	static float _zNear;
	static float _zFar;

	static float _zNearPlaneHalfW;

	static void Init(int screenW, int screenH);
	static int GetWindowWidth();
	static int GetWindowHeight();

	static void SetViewport(float x, float y, float w, float h);
	static void Begin3DDraw();
	static void SetModelViewMatrix();
	static bool UpdateCamera();
	static void GLReadPixelsFromTopLeft(int x, int y, int width, int height, GLenum format, GLenum type, GLvoid *pixels);
	static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix);
	static vector<CVector3> Get2DPosOnScreenFrom3DPos(vector<CVector3>* pos3DVec, float* modelMatrix);


	/////////////////////////////////////////////////////////////////////


	static void GLClearColor(float r, float g, float b, float a, GLfloat* prevColor);
	static GLboolean GLEnable(unsigned int state, GLboolean enable);
	static void GLEnable(unsigned int* arr, int size, bool enable);
	static GLfloat GLLineWidth(GLfloat width);
	static unsigned int GLColor(unsigned int color);
	static unsigned int GetUInt(float r, float g, float b, float a);
	static GLfloat GLPointSize(GLfloat val);
	static GLenum GLDepthFunc(GLenum val);
	static GLMat GetModelViewMatrix();
	static GLMat GetProjectionMatrix();
	static void SetModelViewMatrix(GLMat mat);
	static void SetProjectionMatrix(GLMat mat);
	static void SetLightPosition(float x, float y, float z, unsigned int lightIndex);
	static unsigned int GenerateGLTextureID(int width, int height, int bytesPP, void* buffer);
};

#endif
