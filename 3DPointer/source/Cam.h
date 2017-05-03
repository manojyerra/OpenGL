#ifndef Cam_H
#define Cam_H
#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "Math/GLMat.h"
#include "Math/Vector3.h"

#include <vector>
using namespace std;

class Cam
{
private:
	float _left;
	float _right;
	float _bottom;
	float _top;

	float _zNear;
	float _zFar;
	float _zNearPlaneHalfW;

	float _angleX;
	float _angleY;
	float _angleZ;

	float _transZ;
	float _transX;
	float _transY;

	bool _isOrtho;

	float SW;
	float SH;

	Cam();
	static Cam* _ref;

public:
	static Cam* GetInstance();
	void Init(int screenW, int screenH, float zNear, float zFar, float zNearPlaneW);
	void SetPerspectiveView();
	void SetOrthoView();
	void SetModelViewMatrix();
	bool UpdateCamera();

	//void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix);
	//vector<CVector3> Get2DPosOnScreenFrom3DPos(vector<CVector3>* pos3DVec, float* modelMatrix);
};

#endif
