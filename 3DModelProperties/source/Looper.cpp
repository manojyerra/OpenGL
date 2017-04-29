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
	_rx = _ry = _rw = _rh = 0;

	GLUtil::Init((int)_windowW, (int)_windowH);

	_modelsMgr = new ModelsManager();

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
	_modelPropsFrame = new ModelPropsFrame((int)_windowW-200, 0, 200, 500, _modelsMgr);
}

void Looper::Update(float deltaTime)
{	
	bool consumed = SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, deltaTime);
	Input::SetEnable( !consumed );
}

void Looper::Draw()
{
	glUtil::Begin3DDraw();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glUtil::SetLightPosition(0, 0, 0, GL_LIGHT0);

	glUtil::SetModelViewMatrix();
	glUtil::UpdateCamera();

	if(Input::IsRightMousePressed())
	{
		SelectModel(Input::MX, Input::MY);
	}

	if(_modelsMgr->GetSelectedModel())
	{
		glUtil::ClearColor(1, 1, 1, 1);
		glUtil::Clear();
		_modelsMgr->GetSelectedModel()->CalcBorder();
	}

	glUtil::ClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glUtil::Clear();

	_modelsMgr->Draw();

	Floor::Draw();
	
	UpdateDrawRect();

	if(	
		Input::IsKeyReleased((int)'1') || 
		Input::IsKeyReleased((int)'2') ||
		Input::IsKeyReleased((int)'3') ||
		Input::IsKeyReleased((int)'4') ||
		Input::IsKeyReleased((int)'5')		
		)
	{
		FLModel* selModel = _modelsMgr->GetSelectedModel();

		if( selModel && _rw > 0 && _rh > 0)
		{
			if(Input::IsKeyReleased((int)'1'))
			{
				selModel->AddBestBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh);
				selModel->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'2'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::BOX);
				selModel->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'3'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::CONE);
				selModel->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'4'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::CYLINDER);
				selModel->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'5'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::SPHERE);
				selModel->ShowBoundingShapes(true);
			}
		}
	}

	SUIDraw();
}

void Looper::UpdateDrawRect()
{
	if(Input::IsMouseClicked() || Input::IsRightMouseClicked())
	{
		_rx = _ry = _rw = _rh = 0;
	}
	else if(Input::IsMousePressed() && Input::IsKeyPressed(VK_CONTROL))
	{
		float x1 = Input::MouseClickX;
		float y1 = Input::MouseClickY;
		float x2 = Input::MX;
		float y2 = Input::MY;

		if(x1 > x2)
		{
			x1 = Input::MX;
			x2 = Input::MouseClickX;
		}

		if(y1 > y2)
		{
			y1 = Input::MY;
			y2 = Input::MouseClickY;
		}

		_rx = x1;
		_ry = y1;
		_rw = x2-x1;
		_rh = y2-y1;
	}

	if( _rw > 0 && _rh > 0)
	{
		GLMat modelMat = glUtil::GetModelViewMatrix();
		GLMat projMat = glUtil::GetProjectionMatrix();
		bool depthTest = glUtil::GLEnable(GL_DEPTH_TEST, false);
		bool lighting = glUtil::GLEnable(GL_LIGHTING, false);
		bool blend = glUtil::GLEnable(GL_BLEND, true);
		unsigned int prevColor = glUtil::GLColor(0xff000055);
		GLfloat prevLineWidth = glUtil::GLLineWidth(2);

		GLUtil::Begin2DDraw();

		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(_rx,		_ry);
		glVertex2f(_rx+_rw, _ry);
		glVertex2f(_rx,		_ry+_rh);
		glVertex2f(_rx+_rw, _ry+_rh);
		glEnd();

		glUtil::GLLineWidth(prevLineWidth);
		glUtil::GLColor(prevColor);
		glUtil::GLEnable(GL_BLEND, blend);
		glUtil::GLEnable(GL_LIGHTING, depthTest);
		glUtil::SetProjectionMatrix(projMat);
		glUtil::SetModelViewMatrix(modelMat);
		glUtil::GLEnable(GL_DEPTH_TEST, depthTest);
	}
}

void Looper::SelectModel(int mx, int my)
{
	glUtil::ClearColor(1, 1, 1, 1);
	glUtil::Clear();

	unsigned int index = _modelsMgr->GetModelIndexByMousePos(mx, my);

	if(index >= 0 && index < _modelsMgr->Size())
	{
		_modelsMgr->SetSelectedModelIndex( index );
		_modelPropsFrame->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
	}
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


//void Looper::actionPerformed(SUIActionEvent SUIActionEvent)
//{
//}
