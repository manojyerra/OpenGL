#ifndef GLAndUtil_H
#define GLAndUtil_H
#pragma once

#define GLAndUtil util

class GLAndUtil
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

	static void Clear();
	static void ClearColor(float r, float g, float b, float a);
	static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D);
	static void Enable(unsigned int* arr, int size);
	static void Disable(unsigned int* arr, int size);
};

#endif
