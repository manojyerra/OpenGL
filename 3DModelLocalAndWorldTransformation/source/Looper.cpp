#include "Looper.h"
#include "Input.h"
#include "GLAndUtil.h"
#include "Floor.h"

#include "math.h"
#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	GLAndUtil::Init(_windowW, _windowH);

	_flModel = new FLModel("data/cat", 0, 0, 0);
}


void Looper::Update(float deltaTime)
{
	FLModel* model = _flModel;

	if(Input::IsKeyPressed(VK_SHIFT))
	{
		if(Input::IsKeyPressed(VK_CONTROL))
		{
			if(Input::IsKeyPressed(VK_LEFT))		model->AddRotateInLocal('x', 1);
			else if(Input::IsKeyPressed(VK_RIGHT))	model->AddRotateInLocal('x', -1);
			else if(Input::IsKeyPressed(VK_UP))		model->AddRotateInLocal('y', 1);
			else if(Input::IsKeyPressed(VK_DOWN))	model->AddRotateInLocal('y', -1);
		}
		else
		{
			if(Input::IsKeyPressed(VK_LEFT))		model->AddTransInLocal('x', 0.3);
			else if(Input::IsKeyPressed(VK_RIGHT))	model->AddTransInLocal('x', -0.3);
			else if(Input::IsKeyPressed(VK_UP))		model->AddTransInLocal('y', 0.3);
			else if(Input::IsKeyPressed(VK_DOWN))	model->AddTransInLocal('y', -0.3);
		}
	}

	else if(Input::IsKeyPressed(VK_CONTROL))
	{
		if(Input::IsKeyPressed(VK_LEFT))			model->AddRotateInWorld('x', 1);
		else if(Input::IsKeyPressed(VK_RIGHT))		model->AddRotateInWorld('x', -1);
		else if(Input::IsKeyPressed(VK_UP))			model->AddRotateInWorld('y', 1);
		else if(Input::IsKeyPressed(VK_DOWN))		model->AddRotateInWorld('y', -1);
	}
	else
	{
		if(Input::IsKeyPressed(VK_LEFT))			model->AddTransInWorld(-0.3, 0, 0);
		else if(Input::IsKeyPressed(VK_RIGHT))		model->AddTransInWorld(0.3, 0, 0);
		else if(Input::IsKeyPressed(VK_UP))			model->AddTransInWorld(0, 0.3, 0);
		else if(Input::IsKeyPressed(VK_DOWN))		model->AddTransInWorld(0, -0.3, 0);
	}
}


void Looper::Draw()
{
	util::ClearColor(0.6, 0.6, 0.6, 1);
	util::Clear();
	util::Begin3DDraw();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat qaLightPos[] = {0, 0, 0, 1.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPos);

	util::SetModelViewMatrix();
	util::UpdateCamera();

	//_flModel->CalcBorder();
	//util::Clear();

	Floor::Draw();

	//_flModel->Draw();
	//_flModel->DrawBorder();

	_flModel->Draw();
}

Looper::~Looper()
{
	if(_flModel)
	{
		delete _flModel;
		_flModel = NULL;
	}
}
