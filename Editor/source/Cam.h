#ifndef Cam_H
#define Cam_H
#pragma once

#include "btBulletDynamicsCommon.h"
#include "MathLib/Point.h"

class Cam
{
private:
	static Cam* _ref;

	float _modelViewMat[16];
	float _projectionMat[16];

	float _angleX;
	float _angleY;

	bool _isOrtho;

	btVector3 _camPos;
	btVector3 _camLookAt;
	btVector3 _upVector;
	btVector3 _sideVector;
	btVector3 _forwardVector;

	Cam();
	~Cam();


public:
	static const int FRONT_VIEW = 0;
	static const int BACK_VIEW = 1;
	static const int LEFT_VIEW = 2;
	static const int RIGHT_VIEW = 3;
	static const int TOP_VIEW = 4;
	static const int BOTTOM_VIEW = 5;
	static const int NONE = 6;

	static Cam* GetInstance();
	static void DeleteInstance();

	Point _origin;

	//float _lookAtX;
	//float _lookAtY;
	//float _lookAtZ;

	float _transX;
	float _transZ;
	float _transY;

	float _zNearVal;
	float _zFarVal;
	float _zNearW;
	float _zNearH;
	float _zNearHorHalf;

	float _leftVal;
	float _rightVal;
	float _bottomVal;
	float _topVal;

	int _windowWidth;
	int _windowHeight;

	void Init(int windowWidth, int windowHeight);
	void Update(float deltaTime);
	void SetView(int windowWidth, int windowHeight);
	float* GetModelViewMatrix();
	float* GetProjectionMatrix();
	void GetPosOnScreen(float* ver, int numVertex, float* objectOrientationMat, float* onScreen);
	void GetPosOnScreen(float* ver, int numVertex, float* onScreen);
	void MultMat(float* a, float* b, float* result);
	void SetViewType(int viewType);
	void SetProjectionAsOrtho(bool isOrtho);
	bool IsOrthoProjection();
	void SetAsOrigin(Point origin);

	btVector3 getCameraPosition();
	btVector3 getCameraTargetPosition();
	btVector3 getUpVector();
	btVector3 getForwardVector();
	btVector3 getSideVector();

	void LookAt(btVector3& camPos, btVector3& camLookAt, btVector3& upVector, float* m);
	void __gluMakeIdentityd(float m[16]);
	int __gluInvertMatrixd(const float src[16], float inverse[16]);

	void GetGluLookAtParameters(float* m, float* gluLookAtParams);
};

#endif
