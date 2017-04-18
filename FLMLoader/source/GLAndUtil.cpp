#include "GLAndUtil.h"
#include "Input.h"
#include <math.h>
#include <windows.h>
#include <gl/gl.h>

float GLAndUtil::_viewX = 0;
float GLAndUtil::_viewY = 0;
float GLAndUtil::_viewW = 0;
float GLAndUtil::_viewH = 0;

float GLAndUtil::_left = 0;
float GLAndUtil::_right = 0;
float GLAndUtil::_bottom = 0;
float GLAndUtil::_top = 0;
float GLAndUtil::_zNear = 0;
float GLAndUtil::_zFar = 0;

float GLAndUtil::_angleX = 0;
float GLAndUtil::_angleY = 0;
float GLAndUtil::_angleZ = 0;

float GLAndUtil::_transZ = 0;
float GLAndUtil::_transX = 0;
float GLAndUtil::_transY = 0;

float GLAndUtil::_clearR = 0;
float GLAndUtil::_clearG = 0;
float GLAndUtil::_clearB = 0;
float GLAndUtil::_clearA = 0;

float GLAndUtil::_zNearPlaneW = 0;

float GLAndUtil::SW = 0;
float GLAndUtil::SH = 0;

void GLAndUtil::Init(int screenW, int screenH)
{
	SW = screenW;
	SH = screenH;

	glShadeModel( GL_SMOOTH			);
	glCullFace	( GL_BACK			);
	glFrontFace	( GL_CCW			);
	glEnable	( GL_DEPTH_TEST		);
	glDisable	( GL_FOG			);
	glDisable	( GL_LIGHTING		);
	glDisable	( GL_CULL_FACE		);
	glEnable	( GL_BLEND			);
	glBlendFunc	( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable	( GL_LINE_SMOOTH	);
	//glHint		( GL_LINE_SMOOTH_HINT, GL_DONT_CARE	);

	_transZ = -158.0f;
	_transY = -8.7f;

	_angleX = 20;
	_angleY = 0;

	_zNearPlaneW = 0.1f;

	_viewX = 0.0f;
	_viewY = 0.0f;
	_viewW = SW;
	_viewH = SH;

	_clearR = 1.0f;
	_clearG = 1.0f;
	_clearB = 1.0f;
	_clearA = 1.0f;
}

void GLAndUtil::Begin3DDraw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	SetViewport(_viewX,_viewY,_viewW, _viewH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	SetFrustum(-_zNearPlaneW, _zNearPlaneW, -_zNearPlaneW*SH/SW, _zNearPlaneW*SH/SW, 1, 10000);
}

void GLAndUtil::SetModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(_transX,_transY,_transZ);
	glRotatef(_angleX,1,0,0);
	glRotatef(_angleY,0,1,0);
}

void GLAndUtil::Begin2DDraw()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0f);

	SetViewport(_viewX,_viewY,_viewW, _viewH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SW, SH, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLAndUtil::Clear()
{
	glClearColor(_clearR, _clearG, _clearB, _clearA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLAndUtil::ClearColor(float clearR, float clearG, float clearB, float clearA)
{
	_clearR = clearR;
	_clearG = clearG;
	_clearB = clearB;
	_clearA = clearA;

	glClearColor(clearR, clearG, clearB, clearA);
}

void GLAndUtil::SetZNearPlaneWidth(float width)
{
	_zNearPlaneW = width*0.5;
}

void GLAndUtil::SetViewport(float x, float y, float w, float h)
{
	_viewX = x;
	_viewY = y;
	_viewW = w;
	_viewH = h;

	glViewport(x, y, w, h);
}

void GLAndUtil::SetFrustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
	_left = left;
	_right = right;
	_bottom = bottom;
	_top = top;
	_zNear = zNear;
	_zFar = zFar;

	glFrustum(_left, _right, _bottom, _top, _zNear, _zFar);
}

bool GLAndUtil::UpdateCamera()
{
	if(Input::IsRightMousePressed())
	{
		float dx = Input::MX - Input::PrevMX;
		float dy = Input::MY - Input::PrevMY;

		float z = _transZ;
		if(z < 0)
			z = -z;

		z /= 3000;

		_transX += dx*z;
		_transY += -dy*z;
		return true;
	}
	else if(Input::IsMiddleMousePressed())
	{
		float dx = Input::MX - Input::PrevMX;
		float dy = Input::MY - Input::PrevMY;

		_angleY += dx * 180.0f / (SW*0.5);
		_angleX += dy * 180.0f / (SH*0.5);
		return true;
	}
	if(Input::IsKeyPressed(VK_SHIFT) && Input::IsMouseDragged())
	{
		_transZ += (Input::PrevMY - Input::MY) * 0.4;
		return true;
	}
	else if(Input::IsKeyPressed(VK_CONTROL) && Input::IsMouseDragged())
	{
		float dx = Input::MX - Input::PrevMX;
		float dy = Input::MY - Input::PrevMY;

		_angleY += dx * 180.0f / (SW*0.5);
		_angleX += dy * 180.0f / (SH*0.5);
		return true;
	}
	else if(Input::IsScrollDown())
	{
		Input::SCROLL_STATE = Input::SCROLL_NONE;
		_transZ -= 4.0f;
		return true;
	}
	else if(Input::IsScrollUp())
	{
		Input::SCROLL_STATE = Input::SCROLL_NONE;
		_transZ += 4.0f;
		return true;
	}

	return false;
}

/*

int GLAndUtil::GetTriNum(int x, int y)
{
	unsigned char data[4];
	glReadPixels( x, SH-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	if(data[0] == 255 && data[1] == 255 && data[2] == 255 && data[3] == 255)
		return -1;

	return (data[0] + data[1]*255 + data[2]*255*255);
}

Pos GLAndUtil::GetPosOnUVTriangle(Tri tri, float x, float y)
{
	Pos pos1 = GetPosOn2DScreen(tri.v1.x, tri.v1.y, tri.v1.z);
	Pos pos2 = GetPosOn2DScreen(tri.v2.x, tri.v2.y, tri.v2.z);
	Pos pos3 = GetPosOn2DScreen(tri.v3.x, tri.v3.y, tri.v3.z);

	Pos t1( tri.t1.u, tri.t1.v);
	Pos t2( tri.t2.u, tri.t2.v);
	Pos t3( tri.t3.u, tri.t3.v);

	Pos pos(x, y);
	Pos percent = Triangle2D::FindPercentages(pos1, pos2, pos3, pos);

	return Triangle2D::FindIntersectionByPercentages(t1, t2, t3, percent.x, percent.y);
}

Pos GLAndUtil::GetPosOn2DScreen(float x, float y, float z)
{
	Point p(x,y,z);

	p.SetRotationY(_angleY);
	p.SetRotationX(_angleX);

	p.x += _transX;
	p.y += _transY;
	p.z += _transZ;

	float frustumWidth = _right - _left;
	float frustumHeight = _top - _bottom;

	float dx = (p.x*-_zNear/p.z) - _left;
	float dy = (p.y*-_zNear/p.z) - _bottom;

	float xOnViewPort = _viewW * dx / frustumWidth;
	float yOnViewPort = _viewH * dy / frustumHeight;

	return Pos( _viewX + xOnViewPort, SH - (_viewY + yOnViewPort) );
}
*/

//float GameLoop::DistLine3D(Point p1, Point p2)
//{
//	return DistLine3D(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
//}
//
//float GameLoop::DistLine3D(float x1, float y1, float z1, float x2, float y2, float z2)
//{
//	return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );
//}

//void GameLoop::DrawRND()
//{
//	UpdateCamera();
//
//	glClearColor(0.3,0.3,0.3,1);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	SetFrustum(-0.2, 0.2, -0.2*_viewH/_viewW, 0.2*_viewH/_viewW, _zNear, 10000);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(_transX,_transY,_transZ);
//	glRotatef(_angleX,1,0,0);
//	glRotatef(_angleY,0,1,0);
//
//	ImageInfo* imgInfo = _objReader->GetSprite()->GetTexture();
//
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, imgInfo->textureID);
//
//	Point p1(0,0,-20);
//	Point p2(20,0,-20);
//	Point p3(0,40,-20);
//
//	float dist = DistLine3D(p1, p2);
//
//	float u1 = imgInfo->width/2.0f;
//	float v1 = imgInfo->height/2.0f;
//
//	float u2 = u1+dist*3;
//	float v2 = v1;
//
//	float distP1P3 = DistLine3D(p1, p3);
//	float distP2P3 = DistLine3D(p2, p3);
//
//	TwoPos twoPos = Triangle2D::GetP3(Pos(u1,v1), distP1P3*3, Pos(u2,v2), distP2P3*3);
//
//	float u3 = twoPos.p1.x;
//	float v3 = twoPos.p1.y;
//
//	glBegin(GL_TRIANGLES);
//		glTexCoord2f(u1/imgInfo->width, v1/imgInfo->height); glVertex3f(0,0,-20);
//		glTexCoord2f(u2/imgInfo->width, v2/imgInfo->height); glVertex3f(10,0,-20);
//		glTexCoord2f(u3/imgInfo->width, v3/imgInfo->height); glVertex3f(0,10,-20);
//		//glTexCoord2f(u1/imgInfo->width, v1/imgInfo->height); glVertex3f(0,0,-20);
//		//glTexCoord2f(u2/imgInfo->width, v2/imgInfo->height); glVertex3f(-10,0,-20);
//		//glTexCoord2f(u3/imgInfo->width, v3/imgInfo->height); glVertex3f(0,10,-20);
//	glEnd();
//
//	glDisable(GL_TEXTURE_2D);
//}
