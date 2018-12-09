#include "GLAndUtil.h"
#include "Input.h"
#include <math.h>


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

float GLAndUtil::_zNearPlaneHalfW = 0;

float GLAndUtil::SW = 0;
float GLAndUtil::SH = 0;

void GLAndUtil::Init(int screenW, int screenH)
{
	SW = screenW;
	SH = screenH;

	glShadeModel( GL_SMOOTH		);
	glCullFace	( GL_BACK		);
	glFrontFace	( GL_CCW		);
	
	glEnable	( GL_DEPTH_TEST	);
	glEnable	( GL_BLEND		);

	glDisable	( GL_FOG		);
	glDisable	( GL_LIGHTING	);
	glDisable	( GL_CULL_FACE	);
	
	glBlendFunc	( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_transZ = -280.0f;
	_transY = -8.0f;

	_angleX = 0;
	_angleY = 0;

	_zNear  = 1.0f;
	_zFar = 10000.0f;

	_zNearPlaneHalfW = 0.1f;

	_viewX = 0.0f;
	_viewY = 0.0f;
	_viewW = SW;
	_viewH = SH;

	_clearR = 1.0f;
	_clearG = 1.0f;
	_clearB = 1.0f;
	_clearA = 1.0f;
}


int GLAndUtil::GetWindowWidth()
{
	return SW;
}


int GLAndUtil::GetWindowHeight()
{
	return SH;
}


void GLAndUtil::Begin3DDraw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	SetViewport(_viewX,_viewY,_viewW, _viewH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	SetFrustum(-_zNearPlaneHalfW, _zNearPlaneHalfW, -_zNearPlaneHalfW*SH/SW, _zNearPlaneHalfW*SH/SW, _zNear, _zFar);


	//for ortho projection...

	//float _leftVal = -(-_transZ *_zNearPlaneHalfW) / _zNear;
	//float _rightVal = (-_transZ *_zNearPlaneHalfW) / _zNear;
	//float _bottomVal = -((-_transZ *_zNearPlaneHalfW) / _zNear) * (SH/SW);
	//float _topVal = ((-_transZ *_zNearPlaneHalfW) / _zNear) * (SH/SW);

	//glOrtho(_leftVal, _rightVal, _bottomVal, _topVal, _zNear, _zFar);
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

void GLAndUtil::ClearColor(float r, float g, float b, float a)
{
	_clearR = r;
	_clearG = g;
	_clearB = b;
	_clearA = a;

	glClearColor(_clearR, _clearG, _clearB, _clearA);
}

GLMat GLAndUtil::GetModelViewMatrix()
{
	GLMat mat;
	glGetFloatv(GL_MODELVIEW_MATRIX, mat.m);
	return mat;
}

GLMat GLAndUtil::GetProjectionMatrix()
{
	GLMat mat;
	glGetFloatv(GL_PROJECTION_MATRIX, mat.m);
	return mat;
}

void GLAndUtil::SetModelViewMatrix(GLMat mat)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(mat.m);
}

void GLAndUtil::SetProjectionMatrix(GLMat mat)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(mat.m);
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

void GLAndUtil::GLEnable(unsigned int* arr, int size, bool enable)
{
	if(enable)
	{
		for(int i=0; i<size; i++)
			glEnable(arr[i]);
	}
	else
	{
		for(int i=0; i<size; i++)
			glDisable(arr[i]);
	}
}

bool GLAndUtil::GLEnable(unsigned int state, bool enable)
{
	bool isEnabled = glIsEnabled(state);

	if(enable)
		glEnable(state);
	else
		glDisable(state);

	return isEnabled;
}

GLfloat GLAndUtil::GLLineWidth(GLfloat width)
{
	GLfloat lineWidth = 1;
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);
	glLineWidth(width);
	return lineWidth;
}

GLfloat GLAndUtil::GLPointSize(GLfloat val)
{
	GLfloat pointSize = 1.0f;
	glGetFloatv(GL_POINT_SIZE, &pointSize);
	glLineWidth(val);
	return pointSize;
}

unsigned int GLAndUtil::GLColor(unsigned int color)
{
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);

	glColor4ub((color >> 24) & 255, (color >> 16) & 255, (color >> 8) & 255, color & 255);

	int r = c[0]*255;
	int g = c[1]*255;
	int b = c[2]*255; 
	int a = c[3]*255;

	return (unsigned int)((r<<24) + (g<<16) + (b<<8) + a);
}

void GLAndUtil::GLClearColor(float r, float g, float b, float a, GLfloat* prevColor)
{
	glGetFloatv(GL_COLOR_CLEAR_VALUE, prevColor);
	glClearColor(r, g, b, a);
}

GLenum GLAndUtil::GLDepthFunc(GLenum val)
{	
	int returnVal = 0;
	glGetIntegerv(GL_DEPTH_FUNC, &returnVal);
	glDepthFunc(val);
	return returnVal;
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
		_transZ += (Input::PrevMY - Input::MY) * 2;
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


void GLAndUtil::Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D)
{
	GLfloat model[16]; 
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	float x = pos3D[0];
	float y = pos3D[1];
	float z = pos3D[2];

	float* a = model;

	float xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
	float yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
	float zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

	float xOnZNear = -_zNear * xWPos / zWPos;
	float yOnZNear = -_zNear * yWPos / zWPos;

	//if(_isOrtho)
	//{
	//	xOnZNear = xWPos;
	//	yOnZNear = yWPos;
	//}
	
	float zNearW = abs(_right-_left);
	float zNearH = abs(_top-_bottom);

	pos2D[0] = (( xOnZNear - _left ) / zNearW) * SW;
	pos2D[1] = SH - ((( yOnZNear - _bottom ) / zNearH) * SH);
}