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

void Cam::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

void Cam::Init(int screenW, int screenH, float zNear, float zFar, float zNearPlaneW)
{
	SW = (float)screenW;
	SH = (float)screenH;

	_zNear  = zNear;
	_zFar = zFar;
	_zNearPlaneHalfW = zNearPlaneW/2.0f;

	_pivot = CVector3(0, 0, 0);
	_trans = CVector3(0, 0, -200.0f);
	_angle = CVector3(30, 0, 0);

	_viewType = 5;

	glViewport(0, 0, (int)SW, (int)SH);
	SetPerspectiveView();
}


void Cam::SetProjection()
{
	if(_isOrtho)
	{
		SetOrthoView();
	}
	else
	{
		SetPerspectiveView();
	}
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

	_left = -(-_trans.z *_zNearPlaneHalfW) / _zNear;
	_right = (-_trans.z *_zNearPlaneHalfW) / _zNear;
	_bottom = -((-_trans.z *_zNearPlaneHalfW) / _zNear) * (SH/SW);
	_top = ((-_trans.z *_zNearPlaneHalfW) / _zNear) * (SH/SW);

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
	
	glTranslatef(_trans.x, _trans.y, _trans.z);

	//glTranslatef(_pivot.x, _pivot.y, _pivot.z);

	glRotatef(_angle.x,1,0,0);
	glRotatef(_angle.y,0,1,0);

	glTranslatef(-_pivot.x, -_pivot.y, -_pivot.z);
}


bool Cam::UpdateCamera()
{
	if( Input::IsKeyPressed(VK_SHIFT) && Input::IsMiddleMousePressed())
	{
		float dx = (float)(Input::MX - Input::PrevMX);
		float dy = (float)(Input::MY - Input::PrevMY);

		float z = _trans.z;
		if(z < 0)
			z = -z;

		z /= 3000;

		_trans.x += dx*z;
		_trans.y += -dy*z;

		return true;
	}
	else if(Input::IsKeyPressed(VK_CONTROL) && Input::IsMiddleMousePressed())
	{
		_trans.z += (float)(Input::PrevMY - Input::MY) * 2.0f;

		return true;
	}
	else if(Input::IsMiddleMousePressed())
	{
		float dx = (float)(Input::MX - Input::PrevMX);
		float dy = (float)(Input::MY - Input::PrevMY);

		_angle.y += dx * 180.0f / (SW*0.5f);
		_angle.x += dy * 180.0f / (SH*0.5f);

		return true;
	}

	if(Input::IsScrollDown())
	{
		_trans.z -= 45.0f;
		return true;
	}
	else if(Input::IsScrollUp())
	{
		_trans.z += 45.0f;
		return true;
	}

	return false;
}

void Cam::SetPivot(CVector3 pivotPoint)
{
	_pivot = pivotPoint;
}

void Cam::SetTrans(CVector3 trans)
{
	_trans = trans;
}

void Cam::SetRot(CVector3 rot)
{
	_angle = rot;
}

void Cam::SetFrontView()
{
	_angle.Set(0,0,0);
	_viewType = 0;
}

void Cam::SetBackView()
{
	_angle.Set(0,180,0);
	_viewType = 1;
}

void Cam::SetLeftView()
{
	_angle.Set(0,90,0);
	_viewType = 2;
}

void Cam::SetRightView()
{
	_angle.Set(0,-90,0);
	_viewType = 3;
}

void Cam::SetTopView()
{
	_angle.Set(90,0,0);
	_viewType = 4;
}

void Cam::SetBottomView()
{
	_angle.Set(-90,0,0);
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
