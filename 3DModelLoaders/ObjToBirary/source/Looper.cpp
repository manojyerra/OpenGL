#include "Looper.h"
#include "math.h"
#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	_objToBinary = new ObjToBinary("data/alien");
}


void Looper::Update(float deltaTime)
{
}


void Looper::Draw()
{
}


Looper::~Looper()
{
	if(_objToBinary)
	{
		delete _objToBinary;
		_objToBinary = NULL;
	}
}
