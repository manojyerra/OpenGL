#include "FLModelBorder.h"
#include "Util/GLUtil.h"

FLModelBorder::FLModelBorder()
{
}

FLModelBorder::~FLModelBorder()
{
	_borderVec.clear();
}

void FLModelBorder::CalcBorder(float* verArr, U_INT  numIndices, U_INT  indicesType, 
												unsigned char* indicesPointer, float* mat, Rect* rect)
{
	_borderVec.clear();

	GLfloat clearColor[4];
	GLUtil::GLClearColor(1,1,1,1, clearColor);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLboolean light = GLUtil::GLEnable(GL_LIGHTING, false);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, false);
	GLboolean depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, false);
	GLboolean texture = GLUtil::GLEnable(GL_TEXTURE_2D, false);
	unsigned int prevColor = GLUtil::GLColor(0x000000ff);

	glPushMatrix();
	glMultMatrixf(mat);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verArr);
	glDrawElements(GL_TRIANGLES, numIndices, indicesType, indicesPointer);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	GLUtil::GLEnable(GL_LIGHTING, light);
	GLUtil::GLEnable(GL_BLEND, blend);
	GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);
	GLUtil::GLColor(prevColor);
	GLUtil::GLEnable(GL_TEXTURE_2D, texture);

	int x = rect->x;
	int y = rect->y;
	int w = rect->w;
	int h = rect->h;

	GLubyte* data = (GLubyte*)malloc(w*h*4);
	memset(data, 255, w*h*4);
	GLUtil::GLReadPixelsFromTopLeft(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int prevVal = 0;

	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			int pos = ((j*w) + i) * 4;

			if(i != 0 && data[pos] != prevVal)
			{
				_borderVec.push_back((float)(i + x));
				_borderVec.push_back((float)(h - j + y));
			}
			prevVal = data[pos];
		}
	}

	prevVal = 0;

	for(int i=0;i<w;i++)
	{
		for(int j=0;j<h;j++)
		{
			int pos = ((j*w) + i) * 4;

			if(j != 0 && data[pos] != prevVal)
			{
				_borderVec.push_back((float)(i + x));
				_borderVec.push_back((float)(h - j + y));
			}
			prevVal = data[pos];
		}
	}

	free(data);
}

void FLModelBorder::DrawBorder()
{
	if(_borderVec.size() > 0)
	{
		state2D.Begin(0xff0000ff, 1.0f, 1.0f, false, false);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &_borderVec[0]);
		glDrawArrays(GL_POINTS, 0, _borderVec.size()/2);
		glDisableClientState(GL_VERTEX_ARRAY);

		state2D.End();
	}
}
