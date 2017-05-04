#include "Cam.h"
#include "Input.h"
#include <math.h>

Cam* Cam::_ref = NULL;

Cam::Cam()
{
}

Cam* Cam::GetInstance()
{
	if(_ref == NULL)
	{
		_ref = new Cam();
	}

	return _ref;
}

void Cam::Init(int screenW, int screenH, float zNear, float zFar, float zNearPlaneW)
{
	SW = (float)screenW;
	SH = (float)screenH;

	_zNear  = zNear;
	_zFar = zFar;
	_zNearPlaneHalfW = zNearPlaneW/2.0f;

	_transX = 0;
	_transZ = -250.0f;
	_transY = -8.0f;

	_angleX = 15;
	_angleY = -30;
	_angleZ = 0;

	_viewType = 5;

	glViewport(0, 0, SW, SH);
	SetPerspectiveView();
}


void Cam::SetPerspectiveView()
{
	_isOrtho = false;

	_left = -_zNearPlaneHalfW;
	_right = _zNearPlaneHalfW;
	_bottom = -_zNearPlaneHalfW*SH/SW;
	_top = _zNearPlaneHalfW*SH/SW;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(_left, _right, _bottom, _top, _zNear, _zFar);
}


void Cam::SetOrthoView()
{
	_isOrtho = true;

	_left = -(-_transZ *_zNearPlaneHalfW) / _zNear;
	_right = (-_transZ *_zNearPlaneHalfW) / _zNear;
	_bottom = -((-_transZ *_zNearPlaneHalfW) / _zNear) * (SH/SW);
	_top = ((-_transZ *_zNearPlaneHalfW) / _zNear) * (SH/SW);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(_left, _right, _bottom, _top, _zNear, _zFar);	
}


bool Cam::IsOrthoView()
{
	return _isOrtho;
}


void Cam::SetModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(_transX,_transY,_transZ);
	glRotatef(_angleX,1,0,0);
	glRotatef(_angleY,0,1,0);
}


bool Cam::UpdateCamera()
{
	if( Input::IsKeyPressed(VK_SHIFT) && Input::IsMiddleMousePressed())
	{
		float dx = (float)(Input::MX - Input::PrevMX);
		float dy = (float)(Input::MY - Input::PrevMY);

		float z = _transZ;
		if(z < 0)
			z = -z;

		z /= 3000;

		_transX += dx*z;
		_transY += -dy*z;

		return true;
	}
	else if(Input::IsKeyPressed(VK_CONTROL) && Input::IsMiddleMousePressed())
	{
		_transZ += (float)(Input::PrevMY - Input::MY) * 2.0f;

		return true;
	}
	else if(Input::IsMiddleMousePressed())
	{
		float dx = (float)(Input::MX - Input::PrevMX);
		float dy = (float)(Input::MY - Input::PrevMY);

		_angleY += dx * 180.0f / (SW*0.5f);
		_angleX += dy * 180.0f / (SH*0.5f);

		return true;
	}

	if(Input::IsScrollDown())
	{
		Input::SCROLL_STATE = Input::SCROLL_NONE;
		_transZ -= 45.0f;

		return true;
	}
	else if(Input::IsScrollUp())
	{
		Input::SCROLL_STATE = Input::SCROLL_NONE;
		_transZ += 45.0f;

		return true;
	}

	return false;
}

void Cam::SetFrontView()
{
	_angleX = 0;
	_angleY = 0;

	_viewType = 0;
}

void Cam::SetBackView()
{
	_angleX = 0;
	_angleY = 180;

	_viewType = 1;
}

void Cam::SetLeftView()
{
	_angleX = 0;
	_angleY = 90;

	_viewType = 2;
}

void Cam::SetRightView()
{
	_angleX = 0;
	_angleY = -90;

	_viewType = 3;
}

void Cam::SetTopView()
{
	_angleX = 90;
	_angleY = 0;

	_viewType = 4;
}

void Cam::SetBottomView()
{
	_angleX = -90;
	_angleY = 0;

	_viewType = 5;
}

void Cam::ChangeView()
{
	_viewType++;

	if(_viewType >= 6)
		_viewType = 0;

	if(_viewType == 0) SetFrontView();
	else if(_viewType == 1) SetBackView();
	else if(_viewType == 2) SetLeftView();
	else if(_viewType == 3) SetRightView();
	else if(_viewType == 4) SetTopView();
	else if(_viewType == 5) SetBottomView();
}


/*
void Cam::Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix)
{
	float x = pos3D[0];
	float y = pos3D[1];
	float z = pos3D[2];

	float* a = modelMatrix;

	float xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
	float yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
	float zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

	float xOnZNear = -_zNear * xWPos / zWPos;
	float yOnZNear = -_zNear * yWPos / zWPos;

	if(_isOrtho)
	{
		xOnZNear = xWPos;
		yOnZNear = yWPos;
	}
	
	float zNearW = abs(_right-_left);
	float zNearH = abs(_top-_bottom);

	pos2D[0] = (( xOnZNear - _left ) / zNearW) * SW;
	pos2D[1] = SH - ((( yOnZNear - _bottom ) / zNearH) * SH);
}

vector<CVector3> Cam::Get2DPosOnScreenFrom3DPos(vector<CVector3>* pos3DVec, float* modelMatrix)
{
	vector<CVector3> vec2d;

	for(int i=0; i<pos3DVec->size(); i++)
	{
		float x = pos3DVec->at(i).x;
		float y = pos3DVec->at(i).y;
		float z = pos3DVec->at(i).z;

		float* a = modelMatrix;

		float xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
		float yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
		float zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

		float xOnZNear = -_zNear * xWPos / zWPos;
		float yOnZNear = -_zNear * yWPos / zWPos;

		if(_isOrtho)
		{
			xOnZNear = xWPos;
			yOnZNear = yWPos;
		}
	
		float zNearW = abs(_right-_left);
		float zNearH = abs(_top-_bottom);

		float x2D = (( xOnZNear - _left ) / zNearW) * SW;
		float y2D = SH - ((( yOnZNear - _bottom ) / zNearH) * SH);

		vec2d.push_back( CVector3(x2D, y2D, 0) );
	}

	return vec2d;
}

*/
