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

	//flModel = _modelsMgr->Add("data/cat", CVector3(0,0,0), CVector3(0, 0, 0));
	//Shape* shape = Shape::GetBestFitBoundingShape(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	//flModel->AddBoundingShape(shape);

	for(int j=0; j<50; j+=25)
	{
		for(int i=0; i<50; i+=25)
		{
			if(rand()%2 == 0)
				_modelsMgr->Add("data/cat", -40+i, 0, -40+j);
			else
				_modelsMgr->Add("data/barrel", -40+i, 0, -40+j);
		}
	}

	SUISetup(glUtil::GetWindowWidth(), glUtil::GetWindowHeight());

	_mainFrame = NULL; //new MainFrame(0,0,200,500, this);
	_modelPropsFrame = new ModelPropsFrame(_windowW-200, 0, 200, 500, _modelsMgr);
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

	if(Input::IsRightMousePressed())
	{
		glUtil::ClearColor(1, 1, 1, 1);
		glUtil::Clear();

		int index = _modelsMgr->GetModelIndexByMousePos(Input::MX, Input::MY);

		if(index >= 0 && index < _modelsMgr->Size())
		{
			_modelsMgr->SetSelectedModelIndex(index);

			_modelPropsFrame->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
		}

		glUtil::ClearColor(0.6f, 0.6f, 0.6f, 1.0f);
		glUtil::Clear();
	}

	_modelsMgr->Draw();

	Floor::Draw();

	SUIDraw();
}

void Looper::actionPerformed(SUIActionEvent SUIActionEvent)
{
}

Looper::~Looper()
{
	if(_modelsMgr)
	{
		delete _modelsMgr;
		_modelsMgr = NULL;
	}

	if(_mainFrame)
	{
		delete _mainFrame;
		_mainFrame = NULL;
	}

	SUIQuit();
}
