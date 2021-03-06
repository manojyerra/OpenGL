#include "Looper.h"
#include "Input.h"
#include "Floor.h"
#include "Util/GLUtil.h"
#include "Math/Vector3.h"
#include "Math/TransformVertexBuf.h"
#include "math.h"
#include "Cam.h"
#include "DefinesAndIncludes.h"


Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = (float)windowWidth;
	_windowH = (float)windowHeight;

	_enablePhysics = false;
	_pausedPhysics = false;
	_shape = NULL;

	_rect.SetColor(0xff000077);

	SUISetup((int)_windowW, (int)_windowH);

	GLUtil::Init((int)_windowW, (int)_windowH);

	Cam::GetInstance()->Init((int)_windowW, (int)_windowH, 1.0f, 10000.0f, 0.2f);

	_floor = new Floor();

	_modelsMgr = new ModelsManager();

	//for(int j=0; j<50; j+=25)
	//{
	//	for(int i=0; i<50; i+=25)
	//	{
	//		if(rand()%2 == 0)
	//			_modelsMgr->Add("data/cat", -40.0f+i, 0.0f, -40.0f+j);
	//		else
	//			_modelsMgr->Add("data/barrel", -40.0f+i, 0.0f, -40.0f+j);
	//	}
	//}

	FLModel* model1 = _modelsMgr->Add("data/barrel");
	FLModel* model2 = _modelsMgr->Add("data/barrel", CVector3(5,0,0) );
	//FLModel* model3 = _modelsMgr->Add("data/barrel", CVector3(10,5,-100) );

	PhyManager::GetInstance();

	_floorBox = new PhyBox(0,-0.5,0, 60,1,60, 0);
	
	//_shape = Shape::GetBestFitBoundingShape( model->GetVerticesPointer(), model->GetNumVertices() * 3);
	//_shape = Shape::GetBoundingShape( model->GetVerticesPointer(), model->GetNumVertices() * 3, Shape::CYLINDER);
	//model->AddBoundingShape(_shape);

	_modelPropsFrame = new ModelPropsFrame((int)_windowW-250, 0, 250, 750, _modelsMgr);

	_mainFrame = new MainFrame(0,0,215,750, Cam::GetInstance(), _floor, _modelsMgr, _modelPropsFrame);
}

void Looper::Draw(float deltaTime)
{
	bool consumed = SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, deltaTime);
	Input::SetEnable( !consumed );

	_enablePhysics = _mainFrame->IsPhysicsEnabled();
	_pausedPhysics = _mainFrame->IsPhysicsPaused();

	if(_enablePhysics && !_pausedPhysics)
	{
		DrawOnPhysicsEnable(deltaTime);
		return;
	}

	Cam::GetInstance()->SetProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLUtil::SetLightPosition(0, 0, 0, GL_LIGHT0);

	Cam::GetInstance()->SetModelViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	if(Input::IsRightMousePressed() || (Input::IsMouseClicked() && Input::IsKeyPressed(VK_SHIFT)))
	{
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		FLModel* flModel = SelectModel(Input::MX, Input::MY);

		if(flModel)
		{
			if((Input::IsMouseClicked() && Input::IsKeyPressed(VK_SHIFT)))
			{
				_mainFrame->SetObjectAsPivot();
				_modelsMgr->ShowOnlySelectedObject( !_modelsMgr->IsShowingOnlySelectedObject() );
			}

			if(_mainFrame->IsSelectedObjectAsPivot() && flModel)
			{
				Cam::GetInstance()->SetPivot( flModel->GetPos()		);
				Cam::GetInstance()->SetRot	( CVector3(0,0,0)		);
				Cam::GetInstance()->SetTrans( CVector3(0,0,-300.0f)	);
			}

			_shape = NULL;
		}
	}

	if(Input::IsMouseDoubleClicked() && _modelsMgr->GetSelectedModel())
	{
		FLModel* model = _modelsMgr->GetSelectedModel();

		_shape = SelectBoundingShape(model, Input::MX, Input::MY);
	}

	if(_mainFrame->IsSelectedObjectAsPivot() && _pointer3D.IsHeldPointer() && Input::IsMouseReleased())
		Cam::GetInstance()->SetPivot( _modelsMgr->GetSelectedModel()->GetPos() );

	if(_modelsMgr->GetSelectedModel() && _mainFrame->IsShowingBorder())
	{
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_modelsMgr->GetSelectedModel()->CalcBorder();
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_modelsMgr->Draw();
	_floor->Draw();
	if(_enablePhysics && _pausedPhysics)
		_floorBox->Draw();


	bool bShapeAdded = false;

	if(!_enablePhysics)
	{
		UpdateSelectionRect();
		DrawRect(&_rect);

		bShapeAdded = CheckBoundingBoxAddition(_modelsMgr->GetSelectedModel(), &_rect);
	}

	if(!_enablePhysics || _pausedPhysics)
	{
	bool transChanged = false;
	bool rotChanged = false;


	if(_shape && _modelsMgr->GetSelectedModel())
	{
		glPushMatrix();
		glMultMatrixf(_modelsMgr->GetSelectedModel()->GetMat().m);

		_pointer3D.Draw(_shape->GetGLMatrix());

		if(_pointer3D.IsPointerDragged())
		{
			if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
			{
				_shape->SetPos( _pointer3D.pos );
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::ROTATE)
			{
				_shape->AddRotateInLocal('x', _pointer3D.rot.x);
				_shape->AddRotateInLocal('y', _pointer3D.rot.y);
				_shape->AddRotateInLocal('z', _pointer3D.rot.z);
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::SCALE)
			{				
				if(Input::IsKeyPressed((int)'U'))
				{
					float scaleSum = _pointer3D.scale.x + _pointer3D.scale.y + _pointer3D.scale.z;
				
					if(scaleSum < 3)	_shape->AddUniformScale(0.99);
					else				_shape->AddUniformScale(1.01);
				}
				else
				{
					CVector3 scale = CVector3(_pointer3D.scale.x-1, _pointer3D.scale.y-1, _pointer3D.scale.z-1);
					_shape->AddScale(scale);
				}
			}
		}

		glPopMatrix();
	}
	else if(_modelsMgr->GetSelectedModel())
	{
		_pointer3D.Draw(_modelsMgr->GetSelectedModel()->GetMat().m);

		if(Input::IsKeyPressed(VK_CONTROL))
		{
			if(Input::IsKeyPressedStill(VK_LEFT) || Input::IsKeyTyped(VK_LEFT))
			{
				if(_pointer3D.GetTransformationType() == Pointer3D::ROTATE)
				{
					float rotAmount = Input::IsKeyPressedStill(VK_LEFT) ? 5.0f : 1.0f;
					_modelsMgr->GetSelectedModel()->AddRotateInLocal(_pointer3D.GetInvisibleAxis(), rotAmount);
					rotChanged = true;
				}
				else if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
				{
					float transAmount = Input::IsKeyPressedStill(VK_LEFT) ? -0.1f : -0.01f;
					transAmount = transAmount * _pointer3D.GetSideAxisSign();
					_modelsMgr->GetSelectedModel()->AddTransInLocal(_pointer3D.GetSideAxis(), transAmount);
					transChanged = true;
				}
			}
			else if(Input::IsKeyPressedStill(VK_RIGHT) || Input::IsKeyTyped(VK_RIGHT))
			{
				if(_pointer3D.GetTransformationType() == Pointer3D::ROTATE)
				{
					float rotAmount = Input::IsKeyPressedStill(VK_RIGHT) ? -5.0f : -1.0f;
					_modelsMgr->GetSelectedModel()->AddRotateInLocal(_pointer3D.GetInvisibleAxis(), rotAmount);
					rotChanged = true;
				}
				else if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
				{
					float transAmount = Input::IsKeyPressedStill(VK_RIGHT) ? 0.1f : 0.01f;
					transAmount = transAmount * _pointer3D.GetSideAxisSign();
					_modelsMgr->GetSelectedModel()->AddTransInLocal(_pointer3D.GetSideAxis(), transAmount);
					transChanged = true;
				}
			}
			else if(Input::IsKeyPressedStill(VK_UP) || Input::IsKeyTyped(VK_UP))
			{
				if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
				{
					float transAmount = Input::IsKeyPressedStill(VK_UP) ? 0.1f : 0.01f;
					transAmount = transAmount * _pointer3D.GetTopAxisSign();
					_modelsMgr->GetSelectedModel()->AddTransInLocal( _pointer3D.GetTopAxis(), transAmount );
					transChanged = true;
				}
			}
			else if(Input::IsKeyPressedStill(VK_DOWN) || Input::IsKeyTyped(VK_DOWN))
			{
				if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
				{
					float transAmount = Input::IsKeyPressedStill(VK_DOWN) ? -0.1f : -0.01f;
					transAmount = transAmount * _pointer3D.GetTopAxisSign();
					_modelsMgr->GetSelectedModel()->AddTransInLocal( _pointer3D.GetTopAxis(), transAmount );
					transChanged = true;
				}
			}
		}

		if(_pointer3D.IsPointerDragged())
		{
			if(_pointer3D.GetTransformationType() == Pointer3D::TRANS)
			{
				_modelsMgr->GetSelectedModel()->SetPos( _pointer3D.pos );
				transChanged = true;
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::ROTATE)
			{
				_modelsMgr->GetSelectedModel()->AddRotateInLocal('x', _pointer3D.rot.x);
				_modelsMgr->GetSelectedModel()->AddRotateInLocal('y', _pointer3D.rot.y);
				_modelsMgr->GetSelectedModel()->AddRotateInLocal('z', _pointer3D.rot.z);
				rotChanged = true;
			}
			else if(_pointer3D.GetTransformationType() == Pointer3D::SCALE)
			{				
				if(Input::IsKeyPressed((int)'U'))
				{
					float scaleSum = _pointer3D.scale.x + _pointer3D.scale.y + _pointer3D.scale.z;

					if(scaleSum < 3)
					{
						_modelsMgr->GetSelectedModel()->AddUniformScale(0.99f);
					}
					else
					{
						_modelsMgr->GetSelectedModel()->AddUniformScale(1.01f);
					}
				}
				else
				{
					_modelsMgr->GetSelectedModel()->AddScale(_pointer3D.scale);
				}
			}
		}
	}
	
	if(transChanged)
		_modelPropsFrame->UpdateTransInfo( _modelsMgr->GetSelectedModel() );

	if(rotChanged)
		_modelPropsFrame->UpdateRotationInfo( _modelsMgr->GetSelectedModel() );

	if(bShapeAdded)
	{
		_modelPropsFrame->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
		_modelPropsFrame->ShowBoundingShapes(true);
	}
	}

	SUIDraw();
}

void Looper::DrawOnPhysicsEnable(float deltaTime)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cam::GetInstance()->SetProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLUtil::SetLightPosition(0, 0, 0, GL_LIGHT0);

	Cam::GetInstance()->SetModelViewMatrix();
	Cam::GetInstance()->UpdateCamera();

	UpdatePhysics(deltaTime);

	_modelsMgr->Draw();

	bool lightState = GLUtil::GLEnable(GL_LIGHTING, false);
	_floor->Draw();
	_floorBox->Draw();
	GLUtil::GLEnable(GL_LIGHTING, lightState);

	SUIDraw();
}

void Looper::UpdatePhysics(float deltaTime)
{
	PhyManager::GetInstance()->Update(deltaTime);

	if(Input::IsMouseClicked())			PhyManager::GetInstance()->OnClick(Input::MX, Input::MY);
	else if(Input::IsMouseReleased())	PhyManager::GetInstance()->OnRelease(Input::MX, Input::MY);
	else if(Input::IsMouseDragged())	PhyManager::GetInstance()->OnDrag(Input::MX, Input::MY);
}

bool Looper::CheckBoundingBoxAddition(FLModel* model, Rect* rect)
{
	bool added = false;

	if(model && rect->w > 0 && rect->h > 0)
	{
		int keys[5] = {'1','2','3','4','5'};

		if(Input::IsAnyKeyReleased(keys, 5))
		{
			if(Input::IsKeyReleased((int)'1'))		model->AddBestBoudingShapeByVerticesOnRect(rect);
			else if(Input::IsKeyReleased((int)'2'))	model->AddBoudingShapeByVerticesOnRect(rect, Shape::BOX);
			else if(Input::IsKeyReleased((int)'3'))	model->AddBoudingShapeByVerticesOnRect(rect, Shape::CONE);
			else if(Input::IsKeyReleased((int)'4'))	model->AddBoudingShapeByVerticesOnRect(rect, Shape::CYLINDER);
			else if(Input::IsKeyReleased((int)'5'))	model->AddBoudingShapeByVerticesOnRect(rect, Shape::SPHERE);

			model->ShowBoundingShapes(true);
			added = true;
		}
	}

	return added;
}

void Looper::UpdateSelectionRect()
{
	if(Input::IsMouseClicked() || Input::IsRightMouseClicked())
	{
		_rect.SetBounds(0,0,0,0);
	}
	else if(Input::IsMousePressed() && Input::IsKeyPressed(VK_CONTROL))
	{
		_rect.SetBoundsByPoints(Input::MouseClickX, Input::MouseClickY, Input::MX, Input::MY);
	}
}

void Looper::DrawRect(Rect* rect)
{
	if(_rect.w > 0 && _rect.h > 0)
	{
		state2D.Begin(0xff000055, 2.0f, 1.0f, true, false);
		_rect.Draw();
		state2D.End();
	}
}

FLModel* Looper::SelectModel(int mx, int my)
{
	unsigned int index = _modelsMgr->GetModelIndexByMousePos((float)mx, (float)my);

	if(index >= 0 && index < _modelsMgr->Size())
	{
		_modelsMgr->SetSelectedModelIndex( index );
		_modelPropsFrame->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
		return _modelsMgr->GetSelectedModel();
	}

	return NULL;
}

Shape* Looper::SelectBoundingShape(FLModel* model, float x, float y)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shape* shape = model->GetBoundingShapeIndexByMousePos(x, y);

	//if(shape)
	//{
	//	GLMat mat;
	//	mat.glMultMatrixf(model->GetMat().m);
	//	mat.glMultMatrixf(shape->GetGLMatrix());

	//	shape->SetGLMatrix(mat.m);
	//}

	return shape;
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

	if(_modelPropsFrame)
	{
		delete _modelPropsFrame;
		_modelPropsFrame = NULL;
	}

	if(_floor)
	{
		delete _floor;
		_floor = NULL;
	}

	Cam::DeleteInstance();
	PhyManager::DeleteInstance();

	SUIQuit();
}



	/*
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
	*/