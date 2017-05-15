#ifndef GLUtil_H
#define GLUtil_H

#include "DefinesAndIncludes.h"
#include "Math/GLMat.h"
#include "Math/Vector3.h"

#include <vector>
using namespace std;

class GLUtil
{
private:
	static float SW;
	static float SH;

public:
	static void Init(int screenW, int screenH);
	static int GetWindowWidth();
	static int GetWindowHeight();

	static void GLClearColor(float r, float g, float b, float a, GLfloat* prevColor);
	static GLboolean GLEnable(unsigned int state, GLboolean enable);
	static void GLEnable(unsigned int* arr, int size, bool enable);
	static GLfloat GLLineWidth(GLfloat width);
	static unsigned int GLColor(unsigned int color);
	static unsigned int GetUInt(float r, float g, float b, float a);
	static GLfloat GLPointSize(GLfloat val);
	static GLenum GLDepthFunc(GLenum val);
	static GLMat GetModelViewMatrix();
	static GLMat GetProjectionMatrix();
	static void GLReadPixelsFromTopLeft(int x, int y, int width, int height, GLenum format, GLenum type, GLvoid *pixels);
	static void SetModelViewMatrix(GLMat mat);
	static void SetProjectionMatrix(GLMat mat);
	static void SetLightPosition(float x, float y, float z, unsigned int lightIndex);
	static void GetProjectionValues(float* l, float* r, float* b, float* t, float* n, float* f);
	static void GetProjectionValues(float* projMat, float* l, float* r, float* b, float* t, float* n, float* f);

	static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix);
	static void Get2DPosOnScreenFrom3DPos(float* pos3D, float* pos2D, float* modelMatrix, float* projMatrix);
	static vector<CVector3> Get2DPosOnScreenFrom3DPos(vector<CVector3>* pos3DVec, float* modelMatrix);
	static vector<float> GetVerticesOnRect(float* verArr, int numVertex, float* objMat, float x, float y, float w, float h);
	static void GetMinMaxPoints(vector<CVector3>* point3D, CVector3* min, CVector3* max);
	static unsigned int GenerateGLTextureID(int width, int height, int bytesPP, void* buffer);
};

#endif
