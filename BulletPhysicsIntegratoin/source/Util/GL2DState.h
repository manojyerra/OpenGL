#ifndef GL2DState_H
#define GL2DState_H
#pragma once

#include "DefinesAndIncludes.h"

class GL2DState
{
private:
	float _projMat[16];
	float _modelMat[16];
	unsigned int _color;
	GLfloat _lineWidth;
	GLfloat _pointSize;

	GLboolean _depthTest;
	GLboolean _blend;
	GLboolean _lighting;
	GLboolean _lineSmooth;

public:
	GL2DState();
	
	void SetProjAndModelView();

	void Begin(unsigned int color, GLfloat lineWidth, GLfloat pointSize, GLboolean enableBlend, GLboolean lineSmooth);
	void End();
};

#endif
