#include "Editor.h"
#include "Sui/SuiInput.h"
#include "AppInfo.h"
#include "IO/FLM/FLMWriter.h"
#include "Util/UtilFuncs.h"
#include "Sui/SuiInput.h"
#include "MathLib/Vector3.h"
#include "Floor.h"
#include "Shapes/Box.h"
#include "Shapes/Cylinder.h"
#include "Shapes/Sphere.h"
#include "Shapes/Cone.h"
#include "Phy/PhyManager.h"
#include "Sui/SuiManager.h"
#include "Util/Meterial.h"

Editor::Editor(int windowWidth, int windowHeight)
{
	_baseWindowW = windowWidth;
	_baseWindowH = windowHeight;

	Cam::GetInstance()->Init(_baseWindowW, _baseWindowH);

	_mainFrame = new MainFrame(0,0,235, 500, this);
	_modelFrame = new ModelPropsFrame(_baseWindowW-235, 0, 235, 560, this);
	_eventsFrame = new EventsFrame(0,0,_baseWindowW, _baseWindowH);

	_viewCounter = Cam::GetInstance()->FRONT_VIEW;

	_selIndex = -1;
	_pointers = new Pointers();

	_enablePhysics = false;
	_drawBoundingShapes = _mainFrame->CanShowBoundingShapes();
	_drawObjects = _mainFrame->CanShowObjects();
	_wireFrame = false;
	_drawOnlySelModel = false;

	PhyManager::GetInstance();

	////
	//string str = "data/ground.obj";
	//ObjReader* objReader = new ObjReader(str, true, 0,0,0);
	//FLMStruct* FLMStruct = objReader->CreateFLMStruct();
	//
	//string savePath = str.replace(str.find(".obj"), 4, ".flm");

	//FLMWriter FLMWriter(FLMStruct);
	//FLMWriter.Write(savePath);

	//delete FLMStruct;
	//delete objReader;

	//FLMStruct = NULL;
	//objReader = NULL;

	int drawMode = Model3D::IMMEDIATE_MODE;

	//_modelVec.push_back(new Model3D("data/almara.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/barrel.flm", drawMode));
	////_modelVec.push_back(new Model3D("data/door01.flm", drawMode));
	////_modelVec.push_back(new Model3D("data/door02.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/dustbin.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/footRest_dustbin.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/gramphone.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/ground.flm", drawMode));

	////_modelVec.push_back(new Model3D("data/ground_door.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/highRiseStool.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/piggy.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/plant_vause.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/radio.flm", drawMode));
	////_modelVec.push_back(new Model3D("data/roof.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/sofaDoubble.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/sofaSingle_D_N.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/stool_barrel.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/table_old.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/tepoy.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/trophy.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/trophy_piggy.flm", drawMode));
	//_modelVec.push_back(new Model3D("data/vause.flm", drawMode));
	////_modelVec.push_back(new Model3D("data/wall.flm", drawMode));
	////_modelVec.push_back(new Model3D("data/window.flm", drawMode));

	_selIndex = -1;
	/////
}


void Editor::actionPerformed(SuiActionEvent e)
{
	SuiComponent* com = (SuiComponent*)e.GetComponent();
	string name = com->GetName();

	Cam* cam = Cam::GetInstance();

	if(name.compare("Load Models") == 0)
	{
		vector<string> strVec = UtilFuncs::OpenFileDialog("Models (*.flm;*.obj)\0*.flm;*.obj\0");
		
		for(int i=0;i<strVec.size();i++)
		{
			string str = strVec[i];

			if(str.length() > 0 && StringUtils::endsWith(str, ".obj"))
			{
				ObjReader* objReader = new ObjReader(str, true, 0,0,0);
				FLMStruct* FLMStruct = objReader->CreateFLMStruct();
				
				string savePath = str.replace(str.find(".obj"), 4, ".flm");

				FLMWriter FLMWriter(FLMStruct);
				FLMWriter.Write(savePath);

				delete FLMStruct;
				delete objReader;

				_modelVec.push_back(new Model3D(savePath, Model3D::IMMEDIATE_MODE));
				_selIndex = -1;//_modelVec.size()-1;
			}
			else if(str.length() > 0 && StringUtils::endsWith(str, ".flm"))
			{
				_modelVec.push_back(new Model3D(str, Model3D::IMMEDIATE_MODE));			
			}
		}
	}
	else if(name.compare("Save Models") == 0)
	{
		if(_modelVec.size() > 0)
		{
			for(int i=0;i<_modelVec.size();i++)
				_modelVec[i]->Write();

			MessageBox (NULL, "Saved Models...", "", MB_OK);
		}
		else
		{
			MessageBox (NULL, "There are no models to save...", "", MB_OK);
		}
	}
	else if(name.compare("Enable Physics") == 0)
	{
		SetPhysicsEnable(_mainFrame->IsPhysicsEnabled());
	}
	else if(name.compare("Projection") == 0)
	{
		Cam::GetInstance()->SetProjectionAsOrtho(_mainFrame->IsOrtho());
	}
	else if(name.compare("Show Objects") == 0)
	{
		_drawObjects = _mainFrame->CanShowObjects();
	}
	else if(name.compare("Show Bounding Shapes") == 0)
	{
		_drawBoundingShapes = _mainFrame->CanShowBoundingShapes();		
	}
	else if(name.compare("Pivot") == 0)
	{
		if(_mainFrame->IsOriginPivot())
			Cam::GetInstance()->SetAsOrigin(Point(0,0,0));
		else if(_selIndex >= 0 && _selIndex < _modelVec.size())
			Cam::GetInstance()->SetAsOrigin(_modelVec[_selIndex]->GetPos());
	}
	else if(name.compare("Meterial") == 0)
	{
		SuiChoice* meterialChoice = (SuiChoice*) com;

		int index = meterialChoice->GetSelectedIndex();
		Model3D* model3D = GetSelectedModel();
		if(model3D)
			model3D->SetMeterialID(meterialChoice->GetSelectedIndex(), meterialChoice->GetSelectedStr());
	}

	else if(name.compare("Top View") == 0)		cam->SetViewType(Cam::TOP_VIEW);
	else if(name.compare("Bottom View") == 0)	cam->SetViewType(Cam::BOTTOM_VIEW);
	else if(name.compare("Left View") == 0)		cam->SetViewType(Cam::LEFT_VIEW);
	else if(name.compare("Right View") == 0)	cam->SetViewType(Cam::RIGHT_VIEW);
	else if(name.compare("Front View") == 0)	cam->SetViewType(Cam::FRONT_VIEW);
	else if(name.compare("Back View") == 0)		cam->SetViewType(Cam::BACK_VIEW);

	else if(name.compare("Save") == 0)				{	SaveObject();		}
	else if(name.compare("Duplicate") == 0)			{	DuplicateObject();	}
	else if(name.compare("Delete") == 0)			{	DeleteObject();		}
	else if(name.compare("WireFrame On/Off") == 0)	{	WireFrameObject();	}
	else if(name.compare("Focus On/Off") == 0)		{	FocusObject();		}

	else if(IsSelected())
	{
		float move = 0.1f;
		float rot = 10.0f;

		if(SuiInput::IsKeyPressed(VK_SHIFT))
		{
			move *= 10;
			rot = 90;
		}
		else if(SuiInput::IsKeyPressed(VK_CONTROL))
		{
			move /= 10;
			rot = 1.0f;
		}

		else if(name.compare("+X Rot") == 0)	_modelVec[_selIndex]->RotInLocal('x', rot);
		else if(name.compare("-X Rot") == 0)	_modelVec[_selIndex]->RotInLocal('x', -rot);
		else if(name.compare("+Y Rot") == 0)	_modelVec[_selIndex]->RotInLocal('y', rot);
		else if(name.compare("-Y Rot") == 0)	_modelVec[_selIndex]->RotInLocal('y', -rot);
		else if(name.compare("+Z Rot") == 0)	_modelVec[_selIndex]->RotInLocal('z', rot);
		else if(name.compare("-Z Rot") == 0)	_modelVec[_selIndex]->RotInLocal('z', -rot);
	}
}

void Editor::Update(float deltaTime)
{
	if(SuiInput::IsAllEventsFired(_eventsFrame->GetPhysicsInput()))
	{
		SetPhysicsEnable(!_enablePhysics);
		_mainFrame->PhysicsOn(_enablePhysics);
	}

	if(_enablePhysics)
	{
		UpdatePhysics(deltaTime);
	}
	else
	{
		if(SuiInput::IsAllEventsFired(_eventsFrame->GetObjSaveInput()))					{	SaveObject();		}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetObjDuplicateInput()))		{	DuplicateObject();	}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetObjDeleteInput()))			{	DeleteObject();		}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetWireFrameInput()))			{	WireFrameObject();	}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetObjFocusInput()))			{	FocusObject();		}

		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetCameraViewInput()))
		{
			Cam::GetInstance()->SetProjectionAsOrtho(!Cam::GetInstance()->IsOrthoProjection());
			_mainFrame->SetAsOrtho(Cam::GetInstance()->IsOrthoProjection());
		}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetTopBottomViewInput()))
		{
			if(_viewCounter == Cam::TOP_VIEW)	_viewCounter = Cam::BOTTOM_VIEW;
			else								_viewCounter = Cam::TOP_VIEW;

			Cam::GetInstance()->SetProjectionAsOrtho(true);
			Cam::GetInstance()->SetViewType(_viewCounter);
			_mainFrame->SetAsOrtho(true);
		}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetFrontBackViewInput()))
		{
			if(_viewCounter == Cam::FRONT_VIEW)	_viewCounter = Cam::BACK_VIEW;
			else								_viewCounter = Cam::FRONT_VIEW;

			Cam::GetInstance()->SetProjectionAsOrtho(true);
			Cam::GetInstance()->SetViewType(_viewCounter);
			_mainFrame->SetAsOrtho(true);
		}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetLeftRightViewInput()))
		{
			if(_viewCounter == Cam::RIGHT_VIEW)	_viewCounter = Cam::LEFT_VIEW;
			else								_viewCounter = Cam::RIGHT_VIEW;

			Cam::GetInstance()->SetProjectionAsOrtho(true);
			Cam::GetInstance()->SetViewType(_viewCounter);
			_mainFrame->SetAsOrtho(true);
		}
		else if(SuiInput::IsAllEventsFired(_eventsFrame->GetBestBoundingInput()))
		{
			_modelFrame->AddBoundingShape("Best", GetSelectedModel());
		}
		else if(SuiInput::IsKeyTyped((int)'A'))
		{
			Model3D* model = GetSelectedModel();
			if(model)
			{
				float nearDist = 0;
				Point selModelPos = model->GetPos();
				Model3D* nearModel = NULL;

				for(int i=0;i<_modelVec.size();i++)
				{
					if(_modelVec[i] != model)
					{
						Point pos = _modelVec[i]->GetPos();
						float dist = sqrt( (selModelPos.x - pos.x)*(selModelPos.x - pos.x) + (selModelPos.y - pos.y)*(selModelPos.y - pos.y) + (selModelPos.z - pos.z)*(selModelPos.z - pos.z));
						if(nearModel == NULL || nearDist > dist)
						{
							nearDist = dist;
							nearModel = _modelVec[i];
						}
					}
				}

				if(nearModel)
				{
					float nearMW = abs(nearModel->GetFLMStruct()->maxX - nearModel->GetFLMStruct()->minX);
					float nearMH = abs(nearModel->GetFLMStruct()->maxY - nearModel->GetFLMStruct()->minY);
					float nearMD = abs(nearModel->GetFLMStruct()->maxZ - nearModel->GetFLMStruct()->minZ);

					float selMW = abs(model->GetFLMStruct()->maxX - model->GetFLMStruct()->minX);
					float selMH = abs(model->GetFLMStruct()->maxY - model->GetFLMStruct()->minY);
					float selMD = abs(model->GetFLMStruct()->maxZ - model->GetFLMStruct()->minZ);

					float distX = (nearModel->GetPos().x - model->GetPos().x);
					float distY = (nearModel->GetPos().y - model->GetPos().y);
					float distZ = (nearModel->GetPos().z - model->GetPos().z);

					if(abs(distX) - selMW/2 < nearMW/4)
					{
						model->SetPos(nearModel->GetPos().x, model->GetPos().y, model->GetPos().z);
					}
					else
					{
						if(distX < 0)
							model->SetPos(nearModel->GetPos().x + nearMW/2 + selMW/2, model->GetPos().y, model->GetPos().z);
						else
							model->SetPos(nearModel->GetPos().x - nearMW/2 - selMW/2, model->GetPos().y, model->GetPos().z);
					}



					if(abs(distY) - selMH/2 < nearMH/4)
					{
						model->SetPos(model->GetPos().x, nearModel->GetPos().y, model->GetPos().z);
					}
					else
					{
						if(distY < 0)
							model->SetPos(model->GetPos().x, nearModel->GetPos().y + nearMH/2 + selMH/2, model->GetPos().z);
						else
							model->SetPos(model->GetPos().x, nearModel->GetPos().y - nearMH/2 - selMH/2, model->GetPos().z);
					}



					if(abs(distZ) - selMD/2 < nearMD/4)
					{
						model->SetPos(model->GetPos().x, model->GetPos().y, nearModel->GetPos().z);
					}
					else
					{
						if(distZ < 0)
							model->SetPos(model->GetPos().x, model->GetPos().y, nearModel->GetPos().z + nearMD/2 + selMD/2);
						else
							model->SetPos(model->GetPos().x, model->GetPos().y, nearModel->GetPos().z - nearMD/2 - selMD/2);
					}
				}
			}
		}

		if(SuiInput::IsMouseReleased() && _mainFrame->IsOriginPivot() == false && IsSelected())
			Cam::GetInstance()->SetAsOrigin( _modelVec[_selIndex]->GetPos() );

		UpdateDrawRect();

		_pointers->SetModel(GetSelectedModel());
		_pointers->Update(Cam::GetInstance());

		UpdateByKeys();
		UpdateModelProps();
	}
}

void Editor::SaveObject()
{
	Model3D* model = GetSelectedModel();
	if(model)
	{
		model->Write();
		MessageBox (NULL, "Saved...", "", MB_OK);
	}
	else
	{
		MessageBox (NULL, "Model is not selected...", "", MB_OK);
	}
}

void Editor::DuplicateObject()
{
	Model3D* model = GetSelectedModel();
	if(model)
	{
		 Model3D* newModel = model->Clone();
		 newModel->SetPos( newModel->GetPos().x + 2, newModel->GetPos().y + 2, newModel->GetPos().z + 2 );
		_modelVec.push_back( newModel );
	}
}

void Editor::DeleteObject()
{
	Model3D* model = GetSelectedModel();
	if(model)
	{
		if(MessageBox(NULL, "Do you want delete model ? ", "", MB_OKCANCEL) == 1)
		{
			for(int i=0; i<_modelVec.size();i++)
			{
				if(_modelVec[i] == model)
				{
					_modelVec.erase(_modelVec.begin()+i);
					int status = remove(model->GetModelPath().c_str());

					delete model;
					model = NULL;

					break;
				}
			}
		}
	}
	else
	{
		MessageBox (NULL, "Model is not selected...", "", MB_OK);
	}
}

void Editor::WireFrameObject()
{
	Model3D* model = GetSelectedModel();

	if(model)
	{
		model->EnableWireFrame(!model->IsInWireFrame());
		model->SetDrawTexture(!model->IsInWireFrame());
	}
}

void Editor::FocusObject()
{
	if(_backUpModelVec.size() > 0 && _modelVec.size() == 1)
	{
		Model3D* model = _modelVec[0];
		_modelVec = _backUpModelVec;
		_backUpModelVec.clear();
		Cam::GetInstance()->_transZ += -8;

		for(int i=0;i<_modelVec.size();i++)
		{
			if(_modelVec[i] == model)
			{
				_selIndex = i;
				break;
			}
		}
	}
	else
	{
		Model3D* model = GetSelectedModel();
		if(model)
		{
			//Cam::GetInstance()->SetViewType(Cam::FRONT_VIEW);
			_mainFrame->SetAsOrtho(true);
			Cam::GetInstance()->SetProjectionAsOrtho(true);
			_mainFrame->SelectOriginAsModel();
			_mainFrame->BorderOn(false);
			Cam::GetInstance()->_transZ = -13;
			Cam::GetInstance()->_transY = 0;
			Cam::GetInstance()->_transX = 0;

			Cam::GetInstance()->SetAsOrigin(_modelVec[_selIndex]->GetPos());
			_backUpModelVec = _modelVec;
			_modelVec.clear();
			_modelVec.push_back(model);
			_selIndex = 0;
		}
	}
}

void Editor::UpdatePhysics(float deltaTime)
{
	PhyManager::GetInstance()->Update(deltaTime);

	if(SuiInput::IsMouseClicked())			PhyManager::GetInstance()->OnClick(SuiInput::MX, SuiInput::MY);
	else if(SuiInput::IsMouseReleased())	PhyManager::GetInstance()->OnRelease(SuiInput::MX, SuiInput::MY);
	else if(SuiInput::IsMouseDragged())		PhyManager::GetInstance()->OnDrag(SuiInput::MX, SuiInput::MY);
}

void Editor::UpdateByKeys()
{
	if(SuiInput::IsKeyTyped(VK_UP) || SuiInput::IsKeyTyped(VK_DOWN) || SuiInput::IsKeyTyped(VK_LEFT) || SuiInput::IsKeyTyped(VK_RIGHT) ||
		SuiInput::IsKeyPressedStill(VK_UP) || SuiInput::IsKeyPressedStill(VK_DOWN) || SuiInput::IsKeyPressedStill(VK_LEFT) || SuiInput::IsKeyPressedStill(VK_RIGHT))
	{
		MoveByArrows();
	}
}

void Editor::UpdateDrawRect()
{
	if(SuiInput::IsRightMouseClicked() || SuiInput::IsMouseClicked())
		rect = Rect();
	else if(SuiInput::IsRightMousePressed())
	{
		float x1 = SuiInput::MouseClickX;
		float y1 = SuiInput::MouseClickY;
		float x2 = SuiInput::MX;
		float y2 = SuiInput::MY;

		if(x1 > x2)
		{
			x1 = SuiInput::MX;
			x2 = SuiInput::MouseClickX;
		}

		if(y1 > y2)
		{
			y1 = SuiInput::MY;
			y2 = SuiInput::MouseClickY;
		}

		rect = Rect(x1, y1, x2-x1, y2-y1);
	}
}

void Editor::MoveByArrows()
{
	if(IsSelected() == false)
		return;

	CVector3 leftVec = _pointers->leftVec;
	CVector3 upVec = _pointers->upVec;
	CVector3 forVec = _pointers->forwardVec;

	float leftLen = leftVec.Length();
	float upLen = upVec.Length();
	float forLen = forVec.Length();

	float pi = 3.14159265;

	float leftAng = atan2( leftVec.y, leftVec.x ) * 180.0f / pi;
	float upAng = atan2( upVec.y, upVec.x ) * 180.0f / pi;
	float forAng = atan2( forVec.y, forVec.x ) * 180.0f / pi;

	if(leftAng < 0) leftAng = 360 + leftAng;
	if(upAng < 0) upAng = 360 + upAng;
	if(forAng < 0) forAng = 360 + forAng;

	float maxLen = 0;
	int type = 0;			// left = 1,	up = 2,		forward = 3
	int dir = 0;			// top = 1,		bottom = 2,	left = 3,	 right = 4
	float move = 0.0025;

	if(SuiInput::IsKeyTyped(VK_UP) || SuiInput::IsKeyTyped(VK_DOWN) || SuiInput::IsKeyPressedStill(VK_UP) || SuiInput::IsKeyPressedStill(VK_DOWN))
	{
		if( ((leftAng >= 45 && leftAng <= 135) || (leftAng >= 225 && leftAng <= 315)) && leftLen > maxLen)
		{
			maxLen = leftLen;
			type = 1;

			if(leftAng >= 45 && leftAng <= 135)	dir = 2;
			else								dir = 1;
		}

		if( ((upAng >= 45 && upAng <= 135) || (upAng >= 225 && upAng <= 315)) && upLen > maxLen)
		{
			maxLen = upLen;
			type = 2;

			if(upAng >= 45 && upAng <= 135)	dir = 2;
			else							dir = 1;
		}

		if( ((forAng >= 45 && forAng <= 135) || (forAng >= 225 && forAng <= 315)) && forLen > maxLen)
		{
			maxLen = forLen;
			type = 3;

			if(forAng >= 45 && forAng <= 135)	dir = 2;
			else								dir = 1;
		}

		Model3D* model = _modelVec[_selIndex];

		float moveUp = 0;
		float moveDown = 0;

		if(SuiInput::IsKeyPressedStill(VK_UP) || SuiInput::IsKeyTyped(VK_UP))
		{
			moveUp = move;
			moveDown = -move;
		}
		else if(SuiInput::IsKeyPressedStill(VK_DOWN) || SuiInput::IsKeyTyped(VK_DOWN))
		{
			moveUp = -move;
			moveDown = move;
		}

		if(type == 1 && dir == 1)	model->TransInLocal('x', moveUp);
		else if(type == 1 && dir == 2)	model->TransInLocal('x', moveDown);
		else if(type == 2 && dir == 1)	model->TransInLocal('y', moveUp);
		else if(type == 2 && dir == 2)	model->TransInLocal('y', moveDown);
		else if(type == 3 && dir == 1)	model->TransInLocal('z', moveUp);
		else if(type == 3 && dir == 2)	model->TransInLocal('z', moveDown);
	}
	else if(SuiInput::IsKeyTyped(VK_LEFT) || SuiInput::IsKeyTyped(VK_RIGHT) || SuiInput::IsKeyPressedStill(VK_LEFT) || SuiInput::IsKeyPressedStill(VK_RIGHT))
	{
		if( ((leftAng >= 135 && leftAng <= 225) || (leftAng >= 0 && leftAng <= 45) || (leftAng >= 315 && leftAng <= 360)) && leftLen > maxLen)
		{
			maxLen = leftLen;
			type = 1;

			if(leftAng >= 135 && leftAng <= 225)	dir = 3;
			else									dir = 4;
		}

		if( ((upAng >= 135 && upAng <= 225) || (upAng >= 0 && upAng <= 45) || (upAng >= 315 && upAng <= 360)) && upLen > maxLen)
		{
			maxLen = upLen;
			type = 2;

			if(upAng >= 135 && upAng <= 225)	dir = 3;
			else								dir = 4;
		}

		if( ((forAng >= 135 && forAng <= 225) || (forAng >= 0 && forAng <= 45) || (forAng >= 315 && forAng <= 360)) && forLen > maxLen)
		{
			maxLen = forLen;
			type = 3;

			if(forAng >= 135 && forAng <= 225)	dir = 3;
			else								dir = 4;
		}

		Model3D* model = _modelVec[_selIndex];

		float moveRight = 0;
		float moveLeft = 0;

		if(SuiInput::IsKeyPressedStill(VK_RIGHT) || SuiInput::IsKeyTyped(VK_RIGHT))
		{
			moveRight = move;
			moveLeft = -move;
		}
		else if(SuiInput::IsKeyPressedStill(VK_LEFT) || SuiInput::IsKeyTyped(VK_LEFT))
		{
			moveRight = -move;
			moveLeft = move;
		}

		if(type == 1 && dir == 4)	model->TransInLocal('x', moveRight);
		else if(type == 1 && dir == 3)	model->TransInLocal('x', moveLeft);
		else if(type == 2 && dir == 4)	model->TransInLocal('y', moveRight);
		else if(type == 2 && dir == 3)	model->TransInLocal('y', moveLeft);
		else if(type == 3 && dir == 4)	model->TransInLocal('z', moveRight);
		else if(type == 3 && dir == 3)	model->TransInLocal('z', moveLeft);
	}
}

bool Editor::IsSelected()
{
	return (_selIndex >= 0 && _selIndex < _modelVec.size());
}

Model3D* Editor::GetSelectedModel()
{
	if(IsSelected())
		return _modelVec[_selIndex];
	return NULL;
}

void Editor::Draw(float deltaTime)
{
	Cam::GetInstance()->Update(deltaTime);
	Cam::GetInstance()->SetView(_baseWindowW, _baseWindowH);

	if(_enablePhysics == false)
	{
		if(	SuiInput::IsAllEventsFired(_eventsFrame->GetObjSelectionInput()) && _pointers->GetActiveAxis() == Pointers::NONE && 
			SuiInput::IsKeyPressed((int)'X')==false)
		{
			SelectModel();
		}

		if(IsSelected() && _mainFrame->CanShowBorder())
			modelBorder.CalcBorder(_modelVec[_selIndex]);

		if(IsSelected() && SuiInput::IsKeyPressed((int)'X') && SuiInput::IsMouseReleased())
		{
			Model3D* model = GetSelectedModel();
			if(model)
			{
				int boundIndex = GetBoundingShapeIndex(GetSelectedModel());
				model->DeleteBoundingBox(boundIndex);
			}
		}

		else if(IsSelected() && SuiInput::IsKeyPressed((int)'Z') && SuiInput::IsMouseReleased())
		{
			Model3D* model = GetSelectedModel();
			if(model)
			{
				int triIndex = GetTriIndex(model);
				if(triIndex >= 0)
				{
					int size = 0;
					float* vertexBuf = model->GetConnectedVertex(triIndex, &size);

					BoundingBox _bBox;
					BoundingCylinder _bCylinder;
					BoundingSphere _bSphere;
					BoundingCone _bCone;

					_bBox.CalcBoundingBox(vertexBuf, size);
					_bCylinder.CalcBoundingCylinder(vertexBuf, size, _bBox.GetMat(), _bBox.GetScale());
					_bSphere.CalcBoundingSphere(vertexBuf, size);
					_bCone.CalcBoundingCone(vertexBuf, size, _bCylinder.GetMat(), _bCylinder.GetHeight());

					float boxVol = _bBox.GetVolume();
					float cylVol = _bCylinder.GetVolume();
					float sphereVol = _bSphere.GetVolume();
					float coneVol = _bCone.GetVolume();

					if(boxVol <= cylVol && boxVol <= sphereVol && boxVol <= coneVol)
						model->AddBoundingShape(Shape::BOX, _bBox.GetMat(), _bBox.GetScale());
					else if(cylVol <= boxVol && cylVol <= sphereVol && cylVol <= coneVol)
						model->AddBoundingShape(Shape::CYLINDER, _bCylinder.GetMat(), _bCylinder.GetScale());
					else if(coneVol <= boxVol && coneVol <= sphereVol && coneVol <= cylVol)
						model->AddBoundingShape(Shape::CONE, _bCone.GetMat(), _bCone.GetScale());
					else if(sphereVol <= boxVol && sphereVol <= coneVol && sphereVol <= cylVol)
						model->AddBoundingShape(Shape::SPHERE, _bSphere.GetMat(), _bSphere.GetScale());

					delete[] vertexBuf;
				}
			}
		}
	}

	glClearColor(0.6,0.4,0.6,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//glPushMatrix();
	//glMultMatrixf(mat);
	//glBegin(GL_TRIANGLES);
	//	glVertex3f(x1,y1,z1);
	//	glVertex3f(x2,y2,z2);
	//	glVertex3f(x3,y3,z3);
	//glEnd();
	//glPopMatrix();

	DrawFloor();

	for(int i=0;i<_modelVec.size();i++)
	{
		if(_modelVec[i]->GetGLTextureID() <= 0)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}

		if(_drawObjects)
		{
			_modelVec[i]->SetColor(255,255,255,255);
			_modelVec[i]->Draw();
		}

		if(_drawBoundingShapes)
			_modelVec[i]->DrawBoundingShapes();
		
		if(_modelVec[i]->GetGLTextureID() <= 0)
			glDisable(GL_LIGHTING);
	}

	if(_enablePhysics == false && IsSelected())
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);

		if(_mainFrame->CanShowBorder())
			modelBorder.DrawBorder();

		_pointers->Draw(_baseWindowW, _baseWindowH);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _baseWindowW, _baseWindowH, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float x = rect.x;
	float y = rect.y;
	float w = rect.w;
	float h = rect.h;

	GLfloat ver[12];

	ver[0] = x;			ver[1] = y;
	ver[2] = x+w;		ver[3] = y;
	ver[4] = x;			ver[5] = y+h;
	ver[6] = x+w;		ver[7] = y;
	ver[8] = x;			ver[9] = y+h;
	ver[10] = x+w;		ver[11] = y+h;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f( 1.0f, 0.0f, 0.0f, 0.3f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, ver);
	glDrawArrays(GL_TRIANGLES,0,6);
	glDisableClientState(GL_VERTEX_ARRAY);
	
}

void Editor::UpdateModelProps()
{
	if(_selIndex >= 0 && _modelVec.size() > _selIndex)
		_modelFrame->UpdateModelProps( _modelVec[_selIndex], rect);
}

void Editor::SetPhysicsEnable(bool enable)
{
	_enablePhysics = enable;

	Cam::GetInstance()->SetProjectionAsOrtho(false);
	_mainFrame->SetAsOrtho(false);

	for(int i=0;i<_modelVec.size();i++)
		_modelVec[i]->SetAsPhysicsObj(_enablePhysics);

	if(_enablePhysics == false)
		PhyManager::DeleteInstance();
}

void Editor::DrawFloor()
{
	Floor::SetVisible( true );
	Floor::SetAxisVisible( _mainFrame->IsAxisEnable() );
	Floor::SetGridVisible( _mainFrame->IsGridEnable() );
	Floor::SetGridLinesVisible( _mainFrame->IsGridLinesEnable() );
	Floor::Draw();
}

void Editor::SelectModel()
{
	GLfloat clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
	bool glDepthTest = glIsEnabled(GL_DEPTH_TEST);
	bool glLighting = glIsEnabled(GL_LIGHTING);
	bool glBlend = glIsEnabled(GL_BLEND);

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	
	for(int i=0;i<_modelVec.size();i++)
	{
		bool isDrawTexture = _modelVec[i]->IsTextureDrawing();
		bool isInWireFrame = _modelVec[i]->IsInWireFrame();

		_modelVec[i]->SetColor(i,0,0, 255);

		_modelVec[i]->SetDrawTexture(false);
		_modelVec[i]->EnableWireFrame(false);

		_modelVec[i]->Draw();

		_modelVec[i]->SetDrawTexture(isDrawTexture);
		_modelVec[i]->EnableWireFrame(isInWireFrame);
	}

	GLubyte data[4];
	AppInfo* appInfo = AppInfo::GetInstance();
	glReadPixels(SuiInput::MX_ON_WIN, appInfo->currWindowH - SuiInput::MY_ON_WIN, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	int colValSum = data[0];
	if(colValSum <= 255 && data[0] < _modelVec.size())
	{
		_selIndex = data[0];
		UpdateModelProps();
	}

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	if(glDepthTest)	glEnable(GL_DEPTH_TEST);	else glDisable(GL_DEPTH_TEST);
	if(glLighting)	glEnable(GL_LIGHTING);		else glDisable(GL_LIGHTING);
	if(glBlend)		glEnable(GL_BLEND);			else glDisable(GL_BLEND);

	if(_selIndex >= 0 && _mainFrame->IsOriginPivot() == false)
		Cam::GetInstance()->SetAsOrigin(_modelVec[_selIndex]->GetPos());
}

int Editor::GetBoundingShapeIndex(Model3D* model)
{
	if(model == NULL)
		return -1;

	GLfloat clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
	bool glDepthTest = glIsEnabled(GL_DEPTH_TEST);
	bool glLighting = glIsEnabled(GL_LIGHTING);
	bool glBlend = glIsEnabled(GL_BLEND);

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	
	for(int i=0; i<model->GetFLMStruct()->numBShapes; i++)
		model->DrawBoundingShape(i, i,0,0,255);

    GLubyte data[4];
	AppInfo* appInfo = AppInfo::GetInstance();
	glReadPixels(SuiInput::MX_ON_WIN, appInfo->currWindowH - SuiInput::MY_ON_WIN, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	int colValSum = data[0]; //+ data[1] + data[2];

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	if(glDepthTest)	glEnable(GL_DEPTH_TEST);	else glDisable(GL_DEPTH_TEST);
	if(glLighting)	glEnable(GL_LIGHTING);		else glDisable(GL_LIGHTING);
	if(glBlend)		glEnable(GL_BLEND);			else glDisable(GL_BLEND);

	if(colValSum > 255)
		return -1;

	return colValSum;
}

int Editor::GetTriIndex(Model3D* model)
{
	if(model == NULL)
		return -1;

	GLfloat clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
	bool glDepthTest = glIsEnabled(GL_DEPTH_TEST);
	bool glLighting = glIsEnabled(GL_LIGHTING);
	bool glBlend = glIsEnabled(GL_BLEND);

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	
	model->DrawWithUniqueColors();

    GLubyte data[4];
	AppInfo* appInfo = AppInfo::GetInstance();
	glReadPixels(SuiInput::MX_ON_WIN, appInfo->currWindowH - SuiInput::MY_ON_WIN, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	if(data[0] == 255 && data[1] == 255 && data[2] == 255)
		return -1;

	int colValSum = data[0] + 255*data[1] + 255*255*data[2];

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	if(glDepthTest)	glEnable(GL_DEPTH_TEST);	else glDisable(GL_DEPTH_TEST);
	if(glLighting)	glEnable(GL_LIGHTING);		else glDisable(GL_LIGHTING);
	if(glBlend)		glEnable(GL_BLEND);			else glDisable(GL_BLEND);

	return colValSum;
}

Editor::~Editor()
{
	if(_backUpModelVec.size() > 0 && _modelVec.size() == 1)
	{
		for(int i=0;i<_backUpModelVec.size();i++)
		{
			delete _backUpModelVec[i];
			_backUpModelVec[i] = NULL;
		}	
	}
	else
	{
		for(int i=0;i<_modelVec.size();i++)
		{
			delete _modelVec[i];
			_modelVec[i] = NULL;
		}
	}

	if(_modelFrame)
	{
		delete _modelFrame;
		_modelFrame = NULL;
	}
	if(_mainFrame)
	{
		delete _mainFrame;
		_mainFrame = NULL;
	}

	if(_pointers)
	{
		delete _pointers;
		_pointers = NULL;
	}

	if(_eventsFrame)
	{
		delete _eventsFrame;
		_eventsFrame = NULL;
	}

	//Meterial::GetInstance()->DeleteInstance();
	PhyManager::DeleteInstance();

	if(Cam::GetInstance())
		Cam::DeleteInstance();
}
