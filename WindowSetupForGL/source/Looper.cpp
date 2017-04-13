#include "Looper.h"
#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;
}

void Looper::Update(float deltaTime)
{
}

void Looper::Draw()
{
	glClearColor(0,0,1,1);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0,0,_windowW,_windowH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _windowW, _windowH, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float x = 10;
	float y = 10;
	float w = _windowW-20;
	float h = _windowH-20;

	GLfloat ver[12];

	ver[0] = x;			ver[1] = y;
	ver[2] = x+w;		ver[3] = y;
	ver[4] = x;			ver[5] = y+h;
	ver[6] = x+w;		ver[7] = y;
	ver[8] = x;			ver[9] = y+h;
	ver[10] = x+w;		ver[11] = y+h;

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, ver);
	glDrawArrays(GL_TRIANGLES,0,6);
	glDisableClientState(GL_VERTEX_ARRAY);	
}

Looper::~Looper()
{
}
