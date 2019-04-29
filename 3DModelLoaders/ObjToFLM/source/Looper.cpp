#include "Looper.h"
#include "math.h"
#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	_objToFLM = new ObjToFLM("data/cottage");
}


void Looper::Update(float deltaTime)
{
}


void Looper::Draw()
{
}


Looper::~Looper()
{
	if(_objToFLM)
	{
		delete _objToFLM;
		_objToFLM = NULL;
	}
}
