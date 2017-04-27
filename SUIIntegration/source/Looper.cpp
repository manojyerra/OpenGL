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
	_windowW = windowWidth;
	_windowH = windowHeight;

	GLUtil::Init(_windowW, _windowH);

	_modelsMgr = new ModelsManager();

	flModel = _modelsMgr->Add("data/cat", CVector3(0,0,0), CVector3(0, 0, 0));
	Shape* shape = Shape::GetBestFitBoundingShape(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	flModel->AddBoundingShape( shape );

	SUISetup(glUtil::GetWindowWidth(), glUtil::GetWindowHeight());

	_suiFrame = new SuiFrame(100,100,300,500, SuiComponent::V_ALIGNMENT);
	_suiFrame->Add(new SuiButton("Button1"));
}

void Looper::Update(float deltaTime)
{	
	SuiInput::Update(Input::MX, Input::MY, Input::LEFT_BUTTON_DOWN, deltaTime);
}

void Looper::Draw()
{
	glUtil::ClearColor(0.6, 0.6, 0.6, 1);
	glUtil::Clear();
	glUtil::Begin3DDraw();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat qaLightPos[] = {0, 0, 0, 1.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPos);

	glUtil::SetModelViewMatrix();
	glUtil::UpdateCamera();

	Floor::Draw();

	flModel->Draw();

	SUIRun();
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
