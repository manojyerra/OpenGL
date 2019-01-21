#include "Looper.h"
#include "Input.h"
#include "math.h"
#include "SUIInput.h"
#include "SUIFrame.h"
#include "SUIIncludes.h"

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = (float)windowWidth;
	_windowH = (float)windowHeight;

	SUISetup(_windowW, _windowH);

	_suiFrame = new SUIFrame(0,0,300,300, SUIComponent::V_ALIGNMENT);

	_suiButton = new SUIButton("button");
	_suiFrame->Add(_suiButton);
}

void Looper::Update(float deltaTime)
{	
	bool consumed = SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, deltaTime);
	Input::SetEnable( !consumed );
}

void Looper::Draw()
{
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SUIDraw();
}

Looper::~Looper()
{
	delete _suiFrame;
	SUIQuit();
}
