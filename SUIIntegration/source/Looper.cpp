#include "Looper.h"
#include "Input.h"
#include "Floor.h"
#include "Util/GLUtil.h"
#include "Math/Vector3.h"
#include "Math/TransformVertexBuf.h"
#include "math.h"

#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = (float)windowWidth;
	_windowH = (float)windowHeight;

	GLUtil::Init((int)_windowW, (int)_windowH);

	_modelsMgr = new ModelsManager();

	flModel = _modelsMgr->Add("data/cat", CVector3(0,0,0), CVector3(0, 0, 0));
	Shape* shape = Shape::GetBestFitBoundingShape(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	flModel->AddBoundingShape( shape );

	SUISetup(glUtil::GetWindowWidth(), glUtil::GetWindowHeight());

	_suiButton = new SUIButton("Button1", this);

	_suiFrame = new SUIFrame(0,0,300,300, SUIComponent::V_ALIGNMENT);
	_suiFrame->Add(_suiButton);
}

void Looper::Update(float deltaTime)
{	
	bool consumed = SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, deltaTime);
	Input::SetEnable( !consumed );
}

void Looper::Draw()
{
	glUtil::ClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glUtil::Clear();
	glUtil::Begin3DDraw();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glUtil::SetLightPosition(0, 0, 0, GL_LIGHT0);

	glUtil::SetModelViewMatrix();
	glUtil::UpdateCamera();

	Floor::Draw();
	flModel->Draw();

	SUIDraw();
}

void Looper::actionPerformed(SUIActionEvent SUIActionEvent)
{
	_suiButton->SetName("Button Clicked", SUIComponent::CENTER);
}

Looper::~Looper()
{
	if(_modelsMgr)
	{
		delete _modelsMgr;
		_modelsMgr = NULL;
	}

	delete _suiFrame;

	SUIQuit();
}
