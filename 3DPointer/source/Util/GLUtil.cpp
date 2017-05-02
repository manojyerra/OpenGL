#include "GLUtil.h"
#include "Input.h"
#include <math.h>


float GLUtil::_viewX = 0;
float GLUtil::_viewY = 0;
float GLUtil::_viewW = 0;
float GLUtil::_viewH = 0;

float GLUtil::_left = 0;
float GLUtil::_right = 0;
float GLUtil::_bottom = 0;
float GLUtil::_top = 0;
float GLUtil::_zNear = 0;
float GLUtil::_zFar = 0;

float GLUtil::_angleX = 0;
float GLUtil::_angleY = 0;
float GLUtil::_angleZ = 0;

float GLUtil::_transZ = 0;
float GLUtil::_transX = 0;
float GLUtil::_transY = 0;

float GLUtil::_zNearPlaneHalfW = 0;

float GLUtil::SW = 0;
float GLUtil::SH = 0;

void GLUtil::Init(int screenW, int screenH)
{
	SW = (float)screenW;
	SH = (float)screenH;

	glShadeModel( GL_SMOOTH		);
	glCullFace	( GL_BACK		);
	glFrontFace	( GL_CCW		);
	
	glEnable	( GL_DEPTH_TEST	);

	glDisable	( GL_FOG		);
	glDisable	( GL_LIGHTING	);
	glDisable	( GL_CULL_FACE	);
	
	glEnable	( GL_BLEND		);
	glBlendFunc	( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable	(GL_LINE_SMOOTH);
	glHint		(GL_LINE_SMOOTH_HINT, GL_NICEST);

	_transZ = -850.0f;
	_transY = -8.0f;

	_angleX = 15;
	_angleY = -30;

	_zNear  = 1.0f;
	_zFar = 10000.0f;

	_zNearPlaneHalfW = 0.1f;

	_viewX = 0.0f;
	_viewY = 0.0f;
	_viewW = SW;
	_viewH = SH;
}

int GLUtil::GetWindowWidth()
{
	return (int)SW;
}

int GLUtil::GetWindowHeight()
{
	return (int)SH;
}

void GLUtil::SetViewport(float x, float y, float w, float h)
{
	_viewX = x;
	_viewY = y;
	_viewW = w;
	_viewH = h;

	glViewport((int)x, (int)y, (int)w, (int)h);
}

void GLUtil::Begin3DDraw()
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

void GLUtil::SetModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(_transX,_transY,_transZ);
	glRotatef(_angleX,1,0,0);
	glRotatef(_angleY,0,1,0);
}

bool GLUtil::UpdateCamera()
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
		_transZ -= 55.0f;

		return true;
	}
	else if(Input::IsScrollUp())
	{
		Input::SCROLL_STATE = Input::SCROLL_NONE;
		_transZ += 55.0f;

		return true;
	}

	return false;
}

void GLUtil::GLReadPixelsFromTopLeft(int x, int y, int width, int height, GLenum format, GLenum type, GLvoid *pixels)
{
	glReadPixels(x, GetWindowHeight() - y - height, width, height, format, type, pixels);
}

void GLUtil::SetFrustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
	_left = left;
	_right = right;
	_bottom = bottom;
	_top = top;
	_zNear = zNear;
	_zFar = zFar;

	glFrustum(_left, _right, _bottom, _top, _zNear, _zFar);
}

void GLUtil::Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix)
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

vector<CVector3> GLUtil::Get2DPosOnScreenFrom3DPos(vector<CVector3> pos3DVec, float* modelMatrix)
{
	vector<CVector3> vec2d;

	for(int i=0; i<pos3DVec.size(); i++)
	{
		float x = pos3DVec[i].x;
		float y = pos3DVec[i].y;
		float z = pos3DVec[i].z;

		float* a = modelMatrix;

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

		float x2D = (( xOnZNear - _left ) / zNearW) * SW;
		float y2D = SH - ((( yOnZNear - _bottom ) / zNearH) * SH);

		vec2d.push_back( CVector3(x2D, y2D, 0) );
	}

	return vec2d;
}



//////////////////////////////////////////////////////////////////////////////////////



void GLUtil::GLClearColor(float r, float g, float b, float a, GLfloat* prevColor)
{
	glGetFloatv(GL_COLOR_CLEAR_VALUE, prevColor);
	glClearColor(r, g, b, a);
}

GLboolean GLUtil::GLEnable(unsigned int state, GLboolean enable)
{
	GLboolean isEnabled = glIsEnabled(state);

	if(enable)
		glEnable(state);
	else
		glDisable(state);

	return isEnabled;
}

void GLUtil::GLEnable(unsigned int* arr, int size, bool enable)
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

GLfloat GLUtil::GLLineWidth(GLfloat width)
{
	GLfloat lineWidth = 1;
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);
	glLineWidth(width);
	return lineWidth;
}

unsigned int GLUtil::GLColor(unsigned int color)
{
	float c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);

	glColor(color);

	return GLUtil::GetUInt(c[0], c[1], c[2], c[3]);
}

unsigned int GLUtil::GetUInt(float r, float g, float b, float a)
{
	int rr = (int)(r*255);
	int gg = (int)(g*255);
	int bb = (int)(b*255); 
	int aa = (int)(a*255);

	return (unsigned int)((rr<<24) + (gg<<16) + (bb<<8) + aa);
}

GLfloat GLUtil::GLPointSize(GLfloat val)
{
	GLfloat pointSize = 1.0f;
	glGetFloatv(GL_POINT_SIZE, &pointSize);
	glPointSize(val);
	return pointSize;
}

GLenum GLUtil::GLDepthFunc(GLenum val)
{	
	int returnVal = 0;
	glGetIntegerv(GL_DEPTH_FUNC, &returnVal);
	glDepthFunc(val);
	return returnVal;
}

GLMat GLUtil::GetModelViewMatrix()
{
	GLMat mat;
	glGetFloatv(GL_MODELVIEW_MATRIX, mat.m);
	return mat;
}

GLMat GLUtil::GetProjectionMatrix()
{
	GLMat mat;
	glGetFloatv(GL_PROJECTION_MATRIX, mat.m);
	return mat;
}

void GLUtil::SetModelViewMatrix(GLMat mat)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(mat.m);
}

void GLUtil::SetProjectionMatrix(GLMat mat)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(mat.m);
}

void GLUtil::SetLightPosition(float x, float y, float z, unsigned int lightIndex)
{
	GLfloat qaLightPos[] = {x, y, z, 1.0f};
	glEnable(GL_LIGHTING);
	glEnable(lightIndex);
	glLightfv(lightIndex, GL_POSITION, qaLightPos);
}

unsigned int GLUtil::GenerateGLTextureID(int width, int height, int bytesPP, void* buffer)
{
	unsigned int textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(bytesPP == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPP == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPP == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_RGB8, GL_UNSIGNED_BYTE, buffer);
	else
		return 0;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	return textureID;
}