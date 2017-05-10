#include "GLUtil.h"
#include "Input.h"
#include <math.h>

float GLUtil::SW = 0;
float GLUtil::SH = 0;

void GLUtil::Init(int screenW, int screenH)
{
	SW = (float)screenW;
	SH = (float)screenH;

	glShadeModel( GL_SMOOTH		);
	glCullFace	( GL_BACK		);
	glFrontFace	( GL_CCW		);
	
	glDisable	( GL_FOG		);
	glDisable	( GL_LIGHTING	);
	glDisable	( GL_CULL_FACE	);
	glDisable	( GL_LINE_SMOOTH);
	glHint		( GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glEnable	( GL_DEPTH_TEST	);
	glEnable	( GL_BLEND		);
	glBlendFunc	( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, GetWindowWidth(), GetWindowHeight());
}

int GLUtil::GetWindowWidth()
{
	return (int)SW;
}

int GLUtil::GetWindowHeight()
{
	return (int)SH;
}

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

void GLUtil::GLReadPixelsFromTopLeft(int x, int y, int width, int height, GLenum format, GLenum type, GLvoid *pixels)
{
	glReadPixels(x, GetWindowHeight() - y - height, width, height, format, type, pixels);
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

void GLUtil::GetProjectionValues(float* ll, float* rr, float* bb, float* tt, float* nn, float* ff)
{
	float m[16];
	glGetFloatv(GL_PROJECTION_MATRIX, m);

	GetProjectionValues(m, ll, rr, bb, tt, nn, ff);
}

void GLUtil::GetProjectionValues(float* projMat, float* ll, float* rr, float* bb, float* tt, float* nn, float* ff)
{
	float* m = projMat;

	float C1 = (m[10] + 1.0f) / (m[10] - 1.0f);

	float n = (m[14] * C1 - m[14]) / 2.0f;

	float f = n * (m[10] - 1.0f) / (m[10] + 1.0f);

	float C2 = 2 * n / m[0];

	float l = ( m[8] * C2 - C2 ) / 2.0f;

	float r = l * ( m[8] + 1 ) / ( m[8] - 1 );

	float C3 = 2 * n / m[5];

	float b = ( m[9] * C3 - C3 ) / 2.0f;

	float t = b * ( m[9] + 1 ) / ( m[9] - 1 );

	ll[0] = l;
	rr[0] = r;
	bb[0] = b;
	tt[0] = t;
	nn[0] = n;
	ff[0] = f;
}

void GLUtil::Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix)
{
	float m[16];
	glGetFloatv(GL_PROJECTION_MATRIX, m);

	Get2DPosOnScreenFrom3DPos(pos3D, pos2D, modelMatrix, m);
}

void GLUtil::Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix, float* projMatrix)
{
	float x = pos3D[0];
	float y = pos3D[1];
	float z = pos3D[2];

	float* a = modelMatrix;

	float xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
	float yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
	float zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

	float l,r,b,t,n,f;

	GetProjectionValues(projMatrix, &l, &r, &b, &t, &n, &f);

	float xOnZNear = -n * xWPos / zWPos;
	float yOnZNear = -n * yWPos / zWPos;

	if(f < 0)
	{
		xOnZNear = xWPos;
		yOnZNear = yWPos;
	}
	
	float zNearW = abs(r-l);
	float zNearH = abs(t-b);

	pos2D[0] = (( xOnZNear - l ) / zNearW) * SW;
	pos2D[1] = SH - ((( yOnZNear - b ) / zNearH) * SH);
}


vector<CVector3> GLUtil::Get2DPosOnScreenFrom3DPos(vector<CVector3>* pos3DVec, float* modelMatrix)
{
	vector<CVector3> vec2d;

	float l,r,b,t,n,f;

	GetProjectionValues(&l, &r, &b, &t, &n, &f);

	for(unsigned int i=0; i<pos3DVec->size(); i++)
	{
		float x = pos3DVec->at(i).x;
		float y = pos3DVec->at(i).y;
		float z = pos3DVec->at(i).z;

		float* a = modelMatrix;

		float xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
		float yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
		float zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

		float xOnZNear = -n * xWPos / zWPos;
		float yOnZNear = -n * yWPos / zWPos;

		if(f < 0)
		{
			xOnZNear = xWPos;
			yOnZNear = yWPos;
		}
	
		float zNearW = abs(r-l);
		float zNearH = abs(t-b);

		float x2D = (( xOnZNear - l ) / zNearW) * SW;
		float y2D = SH - ((( yOnZNear - b ) / zNearH) * SH);

		vec2d.push_back( CVector3(x2D, y2D, 0) );
	}

	return vec2d;
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
