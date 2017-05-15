#ifndef Cam_H
#define Cam_H

#include "DefinesAndIncludes.h"
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

	CVector3 _pivot;
	CVector3 _angle;
	CVector3 _trans;

	bool _isOrtho;

	float SW;
	float SH;

	int _viewType;

	Cam();
	static Cam* _ref;

public:
	static Cam* GetInstance();
	void Init(int screenW, int screenH, float zNear, float zFar, float zNearPlaneW);

	void SetProjection();
	void SetPerspectiveView();
	void SetOrthoView();
	bool IsOrthoView();

	void SetModelViewMatrix();
	bool UpdateCamera();

	void SetPivot(CVector3 pivotPoint);
	void SetFrontView();
	void SetBackView();
	void SetLeftView();
	void SetRightView();
	void SetTopView();
	void SetBottomView();

	void ChangeView();
};

#endif
