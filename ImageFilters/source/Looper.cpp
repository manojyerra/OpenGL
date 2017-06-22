#include "Looper.h"
#include "Input.h"
#include "Cam.h"
#include "Util/GLUtil.h"
#include "DefinesAndIncludes.h"


Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = (float)windowWidth;
	_windowH = (float)windowHeight;

	SUISetup((int)_windowW, (int)_windowH);

	_batchImageEdit = new BatchImageEdit(_windowW, _windowH);

	GLUtil::Init((int)_windowW, (int)_windowH);

	Cam::GetInstance()->Init((int)_windowW, (int)_windowH, 1.0f, 10000.0f, 0.2f);
}

void Looper::Draw(float deltaTime)
{
	SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, deltaTime);

	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam::GetInstance()->SetProjection();
	Cam::GetInstance()->SetModelViewMatrix();

	_batchImageEdit->Update();
	_batchImageEdit->Draw();

	SUIDraw();
}

Looper::~Looper()
{
	Cam::DeleteInstance();
	SUIQuit();
}
