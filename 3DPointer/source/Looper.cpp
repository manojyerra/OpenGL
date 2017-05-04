#include "Looper.h"
#include "Input.h"
#include "Floor.h"
#include "Util/GLUtil.h"
#include "Math/Vector3.h"
#include "Math/TransformVertexBuf.h"
#include "math.h"
#include "Cam.h"

#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = (float)windowWidth;
	_windowH = (float)windowHeight;
	_rx = _ry = _rw = _rh = 0;

	SUISetup((int)_windowW, (int)_windowH);

	GLUtil::Init((int)_windowW, (int)_windowH);

	Cam::GetInstance()->Init(_windowW, _windowH, 1.0f, 10000.0f, 0.2f);

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

	FLModel* model = _modelsMgr->Add("data/barrel", 0, 0, 0);
	
	//shape = Shape::GetBestFitBoundingShape( model->GetVerticesPointer(), model->GetNumVertices() * 3);

	shape = Shape::GetBoundingShape( model->GetVerticesPointer(), model->GetNumVertices() * 3, Shape::CYLINDER);

	model->AddBoundingShape(shape);

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
	if(Input::IsKeyReleased((int)'A'))
	{
		if(Cam::GetInstance()->IsOrthoView())
			Cam::GetInstance()->SetPerspectiveView();
		else
			Cam::GetInstance()->SetOrthoView();
	}

	if(Cam::GetInstance()->IsOrthoView())
		Cam::GetInstance()->SetOrthoView();
	else
		Cam::GetInstance()->SetPerspectiveView();


	if(Input::IsKeyReleased((int)'F'))		Cam::GetInstance()->SetFrontView();
	else if(Input::IsKeyReleased((int)'G'))	Cam::GetInstance()->SetBackView();
	else if(Input::IsKeyReleased((int)'L'))	Cam::GetInstance()->SetLeftView();
	else if(Input::IsKeyReleased((int)'R'))	Cam::GetInstance()->SetRightView();
	else if(Input::IsKeyReleased((int)'T'))	Cam::GetInstance()->SetTopView();
	else if(Input::IsKeyReleased((int)'B'))	Cam::GetInstance()->SetBottomView();
	else if(Input::IsKeyReleased((int)'V'))	Cam::GetInstance()->ChangeView();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLUtil::SetLightPosition(0, 0, 0, GL_LIGHT0);

	Cam::GetInstance()->SetModelViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	if(Input::IsRightMousePressed())
	{
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SelectModel(Input::MX, Input::MY);
	}

	if(_modelsMgr->GetSelectedModel())
	{
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_modelsMgr->GetSelectedModel()->CalcBorder();
	}

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_modelsMgr->Draw();

	Floor::Draw();
	
	UpdateDrawRect();

	if(	Input::IsKeyReleased((int)'1') || 
		Input::IsKeyReleased((int)'2') ||
		Input::IsKeyReleased((int)'3') ||
		Input::IsKeyReleased((int)'4') ||
		Input::IsKeyReleased((int)'5'))
	{
		FLModel* selModel = _modelsMgr->GetSelectedModel();

		if( selModel && _rw > 0 && _rh > 0)
		{
			if(Input::IsKeyReleased((int)'1'))
			{
				selModel->AddBestBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh);
				selModel->ShowBoundingShapes(true);
				_modelPropsFrame->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'2'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::BOX);
				selModel->ShowBoundingShapes(true);
				_modelPropsFrame->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'3'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::CONE);
				selModel->ShowBoundingShapes(true);
				_modelPropsFrame->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'4'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::CYLINDER);
				selModel->ShowBoundingShapes(true);
				_modelPropsFrame->ShowBoundingShapes(true);
			}
			else if(Input::IsKeyReleased((int)'5'))
			{
				selModel->AddBoudingShapeByVerticesOnRect(_rx, _ry, _rw, _rh, Shape::SPHERE);
				selModel->ShowBoundingShapes(true);
				_modelPropsFrame->ShowBoundingShapes(true);
			}
		}
	}

	if(shape)
	{
		_pointer3D.Draw(shape->GetGLMatrix());

		if(_pointer3D.IsPointerDragged())
		{
			if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
			{
				shape->SetPos( _pointer3D.pos );
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::ROTATE)
			{
				shape->AddRotateInLocal('x', _pointer3D.rot.x);
				shape->AddRotateInLocal('y', _pointer3D.rot.y);
				shape->AddRotateInLocal('z', _pointer3D.rot.z);
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::SCALE)
			{				
				if(Input::IsKeyPressed((int)'U'))
				{
					float scaleSum = _pointer3D.scale.x + _pointer3D.scale.y + _pointer3D.scale.z;
				
					if(scaleSum > 0)		shape->AddUniformScale(1.01);
					else if(scaleSum < 0)	shape->AddUniformScale(0.99);
				}
				else
				{
					shape->AddScale(_pointer3D.scale);
				}
			}
		}
	}

	/*
	if(_modelsMgr->GetSelectedModel())
	{
		_pointer3D.Draw(_modelsMgr->GetSelectedModel()->GetMat().m);

		if(_pointer3D.IsPointerDragged())
		{
			if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
			{
				_modelsMgr->GetSelectedModel()->SetPos( _pointer3D.pos );
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::ROTATE)
			{
				_modelsMgr->GetSelectedModel()->AddRotateInLocal('x', _pointer3D.rot.x);
				_modelsMgr->GetSelectedModel()->AddRotateInLocal('y', _pointer3D.rot.y);
				_modelsMgr->GetSelectedModel()->AddRotateInLocal('z', _pointer3D.rot.z);
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::SCALE)
			{				
				if(Input::IsKeyPressed((int)'U'))
				{
					float scaleSum = _pointer3D.scale.x + _pointer3D.scale.y + _pointer3D.scale.z;
				
					if(scaleSum > 0)
					{
						_modelsMgr->GetSelectedModel()->AddUniformScale(1.01);
					}
					else if(scaleSum < 0)
					{
						_modelsMgr->GetSelectedModel()->AddUniformScale(0.99);
					}
				}
				else
				{
					_modelsMgr->GetSelectedModel()->AddScale(_pointer3D.scale);
				}
			}
		}
	}
	*/
	
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
		state2D.Begin(0xff000055, 2.0f, 1.0f, true, false);

		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(_rx,		_ry);
		glVertex2f(_rx+_rw, _ry);
		glVertex2f(_rx,		_ry+_rh);
		glVertex2f(_rx+_rw, _ry+_rh);
		glEnd();

		state2D.End();
	}
}

void Looper::SelectModel(int mx, int my)
{
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
