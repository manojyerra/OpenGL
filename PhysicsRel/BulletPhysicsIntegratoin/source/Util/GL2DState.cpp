#include "GL2DState.h"
#include "GLUtil.h"

GL2DState::GL2DState()
{
	memset(_projMat, 0, 16*4);
	memset(_modelMat, 0, 16*4);

	_projMat[0] = _projMat[5] = _projMat[10] = _projMat[15] = 1.0f;
	_modelMat[0] = _modelMat[5] = _modelMat[10] = _modelMat[15] = 1.0f;

	_color = 0xffffffff;
	_lineWidth = 1.0f;
	_pointSize = 1.0f;

	_depthTest = false;
	_lighting = false;
	_blend = true;
	_lineSmooth = true;
}

void GL2DState::Begin(unsigned int color, GLfloat lineWidth, GLfloat pointSize, GLboolean enableBlend, GLboolean lineSmooth)
{
	glGetFloatv(GL_PROJECTION_MATRIX, _projMat);
	glGetFloatv(GL_MODELVIEW_MATRIX, _modelMat);

	_depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, false);
	_lighting = GLUtil::GLEnable(GL_LIGHTING, false);
	_blend = GLUtil::GLEnable(GL_BLEND, enableBlend);
	_lineSmooth = GLUtil::GLEnable(GL_LINE_SMOOTH, lineSmooth);

	_color = GLUtil::GLColor(color);
	_pointSize = GLUtil::GLPointSize(pointSize);
	_lineWidth = GLUtil::GLLineWidth(lineWidth);

	SetProjAndModelView();
}

void GL2DState::SetProjAndModelView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (GLdouble)GLUtil::GetWindowWidth(), (GLdouble)GLUtil::GetWindowHeight(), 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GL2DState::End()
{
	GLUtil::GLEnable(GL_DEPTH_TEST, _depthTest);
	GLUtil::GLEnable(GL_LIGHTING, _lighting);
	GLUtil::GLEnable(GL_BLEND, _blend);
	GLUtil::GLEnable(GL_LINE_SMOOTH, _lineSmooth);

	GLUtil::GLColor(_color);
	GLUtil::GLPointSize(_pointSize);
	GLUtil::GLLineWidth(_lineWidth);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(_projMat);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(_modelMat);
}
