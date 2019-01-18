#ifndef GLUtil_H
#define GLUtil_H
#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "Math/GLMat.h"

#define GLUtil glUtil
#define glColor(c) glColor4ub((c >> 24) & 255, (c >> 16) & 255, (c >> 8) & 255, (c) & 255)

class GLUtil
{
private:
	static float _viewX;
	static float _viewY;
	static float _viewW;
	static float _viewH;

	static float _left;
	static float _right;
	static float _bottom;
	static float _top;
	static float _zNear;
	static float _zFar;

	static float _zNearPlaneHalfW;

	static float _angleX;
	static float _angleY;
	static float _angleZ;

	static float _transZ;
	static float _transX;
	static float _transY;

	static float _clearR;
	static float _clearG;
	static float _clearB;
	static float _clearA;

	static float SW;
	static float SH;

	static void SetFrustum(float left, float right, float bottom, float top, float zNear, float zFar);

public:
	static void Init(int screenW, int screenH);

	static void SetViewport(float x, float y, float w, float h);
	static void Begin3DDraw();
	static void SetModelViewMatrix();
	static void Begin2DDraw();
	static bool UpdateCamera();
	static void SetLightPosition(float x, float y, float z, unsigned int lightIndex);

	static void Clear();
	static void ClearColor(float r, float g, float b, float a);
	static GLMat GetModelViewMatrix();
	static GLMat GetProjectionMatrix();
	static void SetModelViewMatrix(GLMat mat);
	static void SetProjectionMatrix(GLMat mat);

	static unsigned int GenerateGLTextureID(int width, int height, int bytesPP, void* buffer);

	//static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D);
	static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix);

	static void GLEnable(unsigned int* arr, int size, bool enable);
	static GLboolean GLEnable(unsigned int state, GLboolean enable);
	static GLfloat GLLineWidth(GLfloat width);
	static unsigned int GLColor(unsigned int color);
	static GLfloat GLPointSize(GLfloat val);
	static void GLClearColor(float r, float g, float b, float a, GLfloat* prevColor);
	static GLenum GLDepthFunc(GLenum val);
	static void GLReadPixelsFromTopLeft(int x, int y, int width, int height, GLenum format, GLenum type, GLvoid *pixels);

	static int GetWindowWidth();
	static int GetWindowHeight();
};

#endif
