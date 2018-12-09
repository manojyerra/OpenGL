#include "Editor.h"
#include "Platform.h"
#include "Input.h"
#include "UIFont.h"
#include "FileReader.h"
#include "PhysicsManager.h"
#include "Point.h"
#include "GameInfo.h"
#include "Vector3.h"

#include "EditLinePrim.h"
#include "EditRectPrim.h"
#include "EditCirclePrim.h"
#include "EditPolyPrim.h"

//https://www.youtube.com/watch?v=bBIXpu-D_Zo
//http://www.kerp.net/box2d/

Editor::Editor()
{
	_primVec.clear();
	_activePrim = NULL;
	_activatePhysics = false;
	_compoundShapeID = 0;
	_currFilePath = "";
	_showOptions = true;

	_undoRedo = new UndoRedo();
	_undoRedo->Copy(_primVec);

	_myWindow = new MyWindow(782,0);

	_physicsBtn		= _myWindow->AddButton("Apply Physics (P)", MyButton::ON_OFF);				_myWindow->NextRow();
	_addLineBtn		= _myWindow->AddButton("Add Line (Alt+L)");									_myWindow->NextRow();
	_addRectBtn		= _myWindow->AddButton("Add Rect (Alt+R)");									_myWindow->NextRow();
	_addCircleBtn	= _myWindow->AddButton("Add Circle (Alt+C)");								_myWindow->NextRow();
	_addPoly3Btn	= _myWindow->AddButton("Add Polygon 3 (Alt+3)");							_myWindow->NextRow();
	_addPoly4Btn	= _myWindow->AddButton("4");
	_addPoly5Btn	= _myWindow->AddButton("5");
	_addPoly6Btn	= _myWindow->AddButton("6");
	_addPoly7Btn	= _myWindow->AddButton("7");
	_addPoly8Btn	= _myWindow->AddButton("8");												_myWindow->NextRow();
	_lineLoopBtn	= _myWindow->AddButton("Line Loop (Ctrl+Q)", MyButton::ON_OFF);				_myWindow->NextRow();
	_lineSegLoopBtn = _myWindow->AddButton("LineSeg Loop (Ctrl+W)", MyButton::ON_OFF);			_myWindow->NextRow();
	_comLineLoopBtn = _myWindow->AddButton("Compound Line Loop (Ctrl+E)", MyButton::ON_OFF);	_myWindow->NextRow();
	_deleteBtn		= _myWindow->AddButton("Delete(DELETE)");									_myWindow->NextRow();
	_duplicateBtn	= _myWindow->AddButton("Duplicate (Ctrl+D)");								_myWindow->NextRow();
	_copyBtn		= _myWindow->AddButton("Copy (Ctrl+C)");									_myWindow->NextRow();
	_pasteBtn		= _myWindow->AddButton("Paste (Ctrl+V)");									_myWindow->NextRow();
	_selectAllBtn	= _myWindow->AddButton("Select All (Ctrl+A)");								_myWindow->NextRow();
	_deSelectAllBnt = _myWindow->AddButton("DeSelect All (ESCAPE)");							_myWindow->NextRow();
	_saveBtn		= _myWindow->AddButton("Save (Ctrl+S)");									_myWindow->NextRow();
	_loadBtn		= _myWindow->AddButton("Load (Ctrl+L)");									_myWindow->NextRow();
	_undoBtn		= _myWindow->AddButton("Undo (Ctrl+Z)");									_myWindow->NextRow();
	_redoBtn		= _myWindow->AddButton("Redo (Ctrl+Y)");									_myWindow->NextRow();
	_horGrid		= _myWindow->AddButton("HorGrid", MyButton::ON_OFF);
	_verGrid		= _myWindow->AddButton("VerGrid", MyButton::ON_OFF);

	PhysicsManager::GetInstance()->Init();
}

void Editor::Update(float deltaTime)
{
	MyButton* com = NULL;

	if (_showOptions)
	{
		com = _myWindow->Update();

		if (_myWindow->IsMoving())
			return;
	}

	if(Input::IsKeyTyped((int)'P'))
	{
		if(_physicsBtn->IsOn())		_physicsBtn->Off();
		else						_physicsBtn->On();
	}
	else if (Input::IsKeyTyped((int)'H'))
	{
		_showOptions = !_showOptions;
	}

	if(_physicsBtn->IsOn() && _physicsBtn->IsJustNowOn() == false)
	{
		AddCompoundLine();

		if(!_activatePhysics && Input::IsMouseReleased())
			ActivatePhysics();

		if(_activatePhysics)
			PhysicsManager::GetInstance()->Update(deltaTime);
	}
	else
	{
		if(_activatePhysics)
			DeActivatePhysics();

		if(CheckButtonEvents(com) == false)
			Edit();

		AddDataForUndo();
	}
}

bool Editor::CheckButtonEvents(MyButton* btn)
{
	if (btn && btn != _lineLoopBtn)			_lineLoopBtn->Off();
	if (btn && btn != _lineSegLoopBtn)		_lineSegLoopBtn->Off();
	if (btn && btn != _comLineLoopBtn)		_comLineLoopBtn->Off();

	if(btn == _addLineBtn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'L')))
	{
		EditLinePrim* prim = new EditLinePrim(0,0, 50,50);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addRectBtn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'R')))
	{
		EditRectPrim* prim = new EditRectPrim(0,0, 75,50);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addCircleBtn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'C')))
	{
		EditCirclePrim* prim = new EditCirclePrim(25,25,25);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addPoly3Btn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'3')))
	{
		vector<Point> vec;
		vec.push_back(Point(30, 0));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(0, 30));

		EditPolyPrim* prim = new EditPolyPrim(vec);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addPoly4Btn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'4')))
	{
		vector<Point> vec;
		vec.push_back(Point(0, 30));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(50, 60));
		vec.push_back(Point(10, 60));

		EditPolyPrim* prim = new EditPolyPrim(vec);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addPoly5Btn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'5')))
	{
		vector<Point> vec;
		vec.push_back(Point(30, 00));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(60, 60));
		vec.push_back(Point(00, 60));
		vec.push_back(Point(00, 30));

		EditPolyPrim* prim = new EditPolyPrim(vec);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addPoly6Btn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'6')))
	{
		vector<Point> vec;
		vec.push_back(Point(30, 00));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(60, 60));
		vec.push_back(Point(30, 90));
		vec.push_back(Point(00, 60));
		vec.push_back(Point(00, 30));

		EditPolyPrim* prim = new EditPolyPrim(vec);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addPoly7Btn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'7')))
	{
		vector<Point> vec;
		vec.push_back(Point(30, 00));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(60, 60));
		vec.push_back(Point(45, 90));
		vec.push_back(Point(15, 90));
		vec.push_back(Point(00, 60));
		vec.push_back(Point(00, 30));

		EditPolyPrim* prim = new EditPolyPrim(vec);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _addPoly8Btn || (Input::IsKeyPressed(KEYS_MENU) && Input::IsKeyTyped((int)'8')))
	{
		vector<Point> vec;
		vec.push_back(Point(15, 00));
		vec.push_back(Point(45, 00));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(60, 60));
		vec.push_back(Point(45, 90));
		vec.push_back(Point(15, 90));
		vec.push_back(Point(00, 60));
		vec.push_back(Point(00, 30));

		EditPolyPrim* prim = new EditPolyPrim(vec);
		AddPrim((EditPrim*)prim);
		DeSelectAll();
		ActivatePrim((EditPrim*)prim);
		return true;
	}
	else if(btn == _deleteBtn || Input::IsKeyTyped(KEYS_DELETE) )
	{
		Delete();
		return true;
	}
	else if(btn == _duplicateBtn || (Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'D')))
	{
		Duplicate();
		return true;
	}
	else if (btn == _copyBtn || (Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'C')))
	{
		Copy();
		return true;
	}
	else if (btn == _pasteBtn || (Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'V')))
	{
		Paste();
		return true;
	}
	else if(btn == _saveBtn || (Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'S')))
	{
		WriteInFile(SaveFileDialog());
		return true;
	}
	else if(btn == _loadBtn || (Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'L')))
	{
		ReadFromFile(OpenFileDialog());
		return true;
	}
	else if(btn == _undoBtn||
		(Input::IsKeyPressed(KEYS_CONTROL) && (Input::IsKeyTyped((int)'Z') || Input::IsKeyPressedStill((int)'Z'))))
	{
		Undo();
		return true;
	}
	else if(btn == _redoBtn || 
		(Input::IsKeyPressed(KEYS_CONTROL) && (Input::IsKeyTyped((int)'Y') || Input::IsKeyPressedStill((int)'Y'))))
	{
		Redo();
		return true;
	}
	else if(btn == _selectAllBtn || (Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'A')))
	{
		SelectAll();
		return true;
	}
	else if(btn == _deSelectAllBnt || Input::IsKeyTyped(KEYS_ESCAPE) )
	{
		DeSelectAll();
		_lineLoopBtn->Off();
		_lineSegLoopBtn->Off();
		_comLineLoopBtn->Off();

		return true;
	}
	else if(Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'Q'))
	{
		if(_lineLoopBtn->IsOn())	_lineLoopBtn->Off();
		else
		{
			_lineLoopBtn->On();
			_lineSegLoopBtn->Off();
			_comLineLoopBtn->Off();
		}

		return true;
	}
	else if(Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'W'))
	{
		if(_lineSegLoopBtn->IsOn())	_lineSegLoopBtn->Off();
		else
		{
			_lineLoopBtn->Off();
			_lineSegLoopBtn->On();
			_comLineLoopBtn->Off();
		}

		return true;
	}
	else if(Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped((int)'E'))
	{
		if(_comLineLoopBtn->IsOn())	_comLineLoopBtn->Off();
		else
		{
			_lineLoopBtn->Off();
			_lineSegLoopBtn->Off();
			_comLineLoopBtn->On();
		}

		return true;
	}

	return false;
}

void Editor::Edit()
{
	if(_comLineLoopBtn->IsOn())
	{
		MakeLineLoop(true);
		return;
	}
	else if(_lineLoopBtn->IsOn())
	{
		MakeLineLoop(false);
		return;
	}
	else if(_lineSegLoopBtn->IsOn())
	{
		if (Input::IsMouseClicked())
		{
			_pointsVec.clear();
			_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
		}
		else if (Input::IsMousePressed())
		{
			float mx = Input::MouseX;
			float my = Input::MouseY;

			float lastX = _pointsVec[_pointsVec.size() - 1].x;
			float lastY = _pointsVec[_pointsVec.size() - 1].y;

			float dist = sqrt((lastX - mx)*(lastX - mx) + (lastY - my)*(lastY - my));

			if (_pointsVec.size() == 1)
			{
				if(dist > 2)
					_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
			}
			else if (_pointsVec.size() > 1)
			{
				_pointsVec[_pointsVec.size() - 1].x = Input::MouseX;
				_pointsVec[_pointsVec.size() - 1].y = Input::MouseY;
			}
		}
		else if (Input::IsMouseReleased())
		{
			int pointsSize = _pointsVec.size();

			if (pointsSize > 1)
			{
				for (int i = 1; i<pointsSize; i++)
				{
					EditLinePrim* prim = new EditLinePrim(_pointsVec[i - 1].x, _pointsVec[i - 1].y, _pointsVec[i].x, _pointsVec[i].y);
					AddPrim((EditPrim*)prim);
				}
			}

			_pointsVec.clear();
		}

		if (Input::IsKeyTyped((int)'T'))
		{
			if (_pointsVec.size() > 1)
			{
				float mx = Input::MouseX;
				float my = Input::MouseY;

				float lastX = _pointsVec[_pointsVec.size() - 2].x;
				float lastY = _pointsVec[_pointsVec.size() - 2].y;

				float dist = sqrt((lastX - mx)*(lastX - mx) + (lastY - my)*(lastY - my));

				if (dist > 1)
					_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
			}
		}

		return;
	}
	else if(Input::IsKeyTyped((int)'C'))
	{
		for(int i=0;i<_primVec.size();i++)
		{
			if(_primVec[i]->IsInGroup())
				_primVec[i]->SetCompoundShapeID( -1 );
		}

		int id = GetNewCompoundID();

		for(int i=0;i<_primVec.size();i++)
		{
			if(_primVec[i]->IsInGroup())
				_primVec[i]->SetCompoundShapeID( id );
		}

		for(int i=0;i<_primVec.size();i++)
		{
			if(_primVec[i]->GetCompoundShapeID() >= 0)
				_primVec[i]->SetDynamicBody(true);
		}
	}
	else if(Input::IsKeyTyped((int)'V'))
	{
		for(int i=0;i<_primVec.size();i++)
		{
			if(_primVec[i]->IsInGroup())
				_primVec[i]->SetCompoundShapeID( -1 );
		}

		int maxCompoundID = GetMaxCompoundID();
		
		for(int j=0; j<maxCompoundID; j++)
		{
			bool comExist = IsCompoundIDExist(j);
			
			if(comExist == false)
			{
				for(int i=0;i<_primVec.size();i++)
				{
					if(_primVec[i]->GetCompoundShapeID() == maxCompoundID)
					{
						_primVec[i]->SetCompoundShapeID( j );
					}
				}

				break;
			}
		}
	}
	else if(Input::IsKeyTyped((int)'R'))
	{
		if(HasGroupSelection())
		{
			for(int i=0;i<_primVec.size();i++)
			{
				if(_primVec[i]->IsInGroup())
				{
					_primVec[i]->SetAngularVelocity(5.0f, true);
				}
			}
		}
		else if(_activePrim)
		{
			_activePrim->SetAngularVelocity(5.0f, true);
		}
	}
	else if(Input::IsKeyTyped(KEYS_SPACE))
	{
		if(_activePrim)
		{
			int comID = _activePrim->GetCompoundShapeID();

			if(comID >= 0)
			{
				for(int i=0;i<_primVec.size();i++)
				{
					if(_primVec[i]->GetCompoundShapeID() == comID)
						_primVec[i]->SetInGroup(true);
				}
			}
		}
	}
	else if (Input::IsKeyPressed(KEYS_SHIFT) && Input::IsMousePressed()) 
	{
		float dx = Input::MouseX - Input::PrevMouseX;
		float dy = Input::MouseY - Input::PrevMouseY;

		for(int i=0; i<_primVec.size(); i++)
			if(_primVec[i]->IsInGroup())
				_primVec[i]->Move(dx, dy);
	}
	else if( Input::IsKeyPressed(KEYS_CONTROL) && 
		(Input::IsKeyTyped(KEYS_ADD)		|| Input::IsKeyPressedStill(KEYS_ADD)		|| 
		Input::IsKeyTyped(KEYS_SUBTRACT)	|| Input::IsKeyPressedStill(KEYS_SUBTRACT)	||
		Input::IsKeyTyped(KEYS_UP)			|| Input::IsKeyPressedStill(KEYS_UP)		|| 
		Input::IsKeyTyped(KEYS_DOWN)		|| Input::IsKeyPressedStill(KEYS_DOWN))
		)
	{
		float sign = 1.0f;
		float speed = 4;

		if(		Input::IsKeyTyped(KEYS_SUBTRACT)|| Input::IsKeyPressedStill(KEYS_SUBTRACT)
			||	Input::IsKeyTyped(KEYS_DOWN)	|| Input::IsKeyPressedStill(KEYS_DOWN))
		{
			sign = -1.0f;
		}

		float currW = GameInfo::GetInstance()->CodedW();
		float currH = GameInfo::GetInstance()->CodedH();
		float newW = currW + sign * speed ;
		float newH = currH + sign * speed * currH/currW ;
		float factorX = newW / currW;
		float factorY = newH / currH;

		for(int i=0; i<_primVec.size(); i++)
		{
			if(_primVec[i]->IsInGroup() == false && _primVec[i] != _activePrim)
				continue;

			EditPrim* prim = _primVec[i];
			
			int numVertex = 0;
			vector<Point>* vec;

			if(prim->ID == EditPrim::POLY)
			{
				EditPolyPrim* polyPrim = (EditPolyPrim*)prim;
				numVertex = polyPrim->vec.size();
				vec = &polyPrim->vec;
			}
			else if(prim->ID == EditPrim::RECT)
			{
				EditRectPrim* rectPrim = (EditRectPrim*)prim;
				numVertex = rectPrim->vec.size();
				vec = &rectPrim->vec;
			}
			else if(prim->ID == EditPrim::LINE)
			{
				EditLinePrim* linePrim = (EditLinePrim*)prim;
				numVertex = linePrim->vec.size();
				vec = &linePrim->vec;
			}
			else if(prim->ID == EditPrim::CIRCLE)
			{
				EditCirclePrim* cirPrim = (EditCirclePrim*)prim;
				Point p1 = cirPrim->GetCenter();
				cirPrim->SetRadius( cirPrim->GetRadius() * factorX );
				p1.x = p1.x * factorX;
				p1.y = p1.y * factorY;
				cirPrim->SetCenter(p1.x, p1.y);
			}
			
			for(int j=0;j<numVertex;j++)
			{ 
				vec->at(j).x = vec->at(j).x * factorX;
				vec->at(j).y = vec->at(j).y * factorY;
			}

			if(prim->ID == EditPrim::RECT)
			{
				EditRectPrim* rectPrim = (EditRectPrim*)prim;
				rectPrim->SetRectPos();
			}
		}
	}
	else if ((Input::IsKeyPressed(KEYS_CONTROL)	|| Input::IsKeyPressed(KEYS_MENU) ) && 
		(Input::IsMouseClicked() || Input::IsMousePressed() || Input::IsMouseReleased()))
	{
		if (Input::IsMouseClicked())
		{
			_selP1 = Point(Input::MouseX, Input::MouseY);
			_selP2 = Point(Input::MouseX, Input::MouseY);
		}
		else if (Input::IsMousePressed())
		{
			_selP2 = Point(Input::MouseX, Input::MouseY);;
		}
		else if (Input::IsMouseReleased() || Input::IsKeyReleased(KEYS_CONTROL) || Input::IsKeyReleased(KEYS_MENU))
		{
			_selP1 = Point(0,0);
			_selP2 = Point(0,0);
		}

		RectPrim rectPrim = GetRect(_selP1, _selP2);

		if (rectPrim.w > 0 && rectPrim.h)
		{
			for (int i = 0; i < _primVec.size(); i++)
			{
				if (_primVec[i]->CollidesWithRect(rectPrim.x, rectPrim.y, rectPrim.w, rectPrim.h))
				{
					if(Input::IsKeyPressed(KEYS_CONTROL))			
					{
						_primVec[i]->SetInGroup(true);
						DeActiatePrim();
					}
					else if (Input::IsKeyPressed(KEYS_MENU))
						_primVec[i]->SetInGroup(false);
				}
			}
		}
	}
	else if(Input::IsKeyPressed(KEYS_CONTROL) && 
		(Input::IsKeyPressedStill(KEYS_LEFT)	|| Input::IsKeyPressedStill(KEYS_RIGHT) 
		|| Input::IsKeyTyped(KEYS_LEFT)			|| Input::IsKeyTyped(KEYS_RIGHT)))
	{

		if(Input::IsKeyTyped(KEYS_LEFT)			|| Input::IsKeyPressedStill(KEYS_LEFT))			AddRotation(1);
		else if(Input::IsKeyTyped(KEYS_RIGHT)	|| Input::IsKeyPressedStill(KEYS_RIGHT))		AddRotation(-1);

	}
	else if(Input::IsKeyPressedStill(KEYS_LEFT)		|| Input::IsKeyPressedStill(KEYS_RIGHT) 
			|| Input::IsKeyPressedStill(KEYS_UP)	|| Input::IsKeyPressedStill(KEYS_DOWN)
			|| Input::IsKeyTyped(KEYS_LEFT)			|| Input::IsKeyTyped(KEYS_RIGHT)
			|| Input::IsKeyTyped(KEYS_UP)			|| Input::IsKeyTyped(KEYS_DOWN))
	{

		float val = 1;

		if(Input::IsKeyPressed(KEYS_SHIFT))
			val *= 10;

		if(Input::IsKeyTyped(KEYS_LEFT)			|| Input::IsKeyPressedStill(KEYS_LEFT))		Move(-val, 0);
		else if(Input::IsKeyTyped(KEYS_RIGHT)	|| Input::IsKeyPressedStill(KEYS_RIGHT))	Move(val, 0);
		else if(Input::IsKeyTyped(KEYS_UP)		|| Input::IsKeyPressedStill(KEYS_UP))		Move(0, -val);
		else if(Input::IsKeyTyped(KEYS_DOWN)	|| Input::IsKeyPressedStill(KEYS_DOWN))		Move(0, val);
	}
	else if (Input::IsMouseClicked() && _myWindow->Contains(Input::MouseX, Input::MouseY) == false)
	{
		bool primActivated = ActivatePrim((float)Input::MouseX, (float)Input::MouseY);

		if(!primActivated)
		{
			if(_activePrim)
			{
				_activePrim->SetSelect(false);
				_activePrim = NULL;
			}
		}

		for (int i = 0; i < _primVec.size(); i++)
			_primVec[i]->SetInGroup(false);
	}
	else if(Input::IsRightMouseClicked())
	{
		if(_activePrim && _activePrim->Contains(Input::MouseX, Input::MouseY))
		{
			_activePrim->SetDynamicBody(!_activePrim->IsDynamicBody());
		}
		else
		{
			for (int i = 0; i < _primVec.size(); i++)
			{
				if(_primVec[i]->IsInGroup())
				{
					_primVec[i]->SetDynamicBody( !_primVec[i]->IsDynamicBody() );
				}
			}
		}

		for(int i=0;i<_primVec.size();i++)
		{
			if(_primVec[i]->GetCompoundShapeID() >= 0)
			{
				_primVec[i]->SetDynamicBody(true);
			}
		}
	}
	else if(_activePrim)
	{
		_activePrim->Update();
	}

	if(Input::IsKeyReleased(KEYS_CONTROL) || Input::IsKeyReleased(KEYS_MENU))
	{
		_selP1 = Point(0,0);
		_selP2 = Point(0,0);
	}
}

void Editor::AddCompoundLine()
{
	if (Input::IsMouseClicked())
	{
		_pointsVec.clear();
		_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
	}
	else if (Input::IsMousePressed())
	{
		float mx = Input::MouseX;
		float my = Input::MouseY;

		float lastX = _pointsVec[_pointsVec.size() - 1].x;
		float lastY = _pointsVec[_pointsVec.size() - 1].y;

		float dist = sqrt((lastX - mx)*(lastX - mx) + (lastY - my)*(lastY - my));

		if (dist > 7)
			_pointsVec.push_back(Point(mx, my));
	}
	else if (Input::IsMouseReleased())
	{
		int pointsSize = _pointsVec.size();

		int comID = GetNewCompoundID();

		if (pointsSize > 1)
		{
			PhyCompoundShape* phyComShape = new PhyCompoundShape();

			for (int i = 1; i<pointsSize; i++)
			{
				CVector3 nor(_pointsVec[i-1].y - _pointsVec[i - 0].y, _pointsVec[i - 0].x - _pointsVec[i-1].x);
				nor.Normalize();
				nor *= 4 / 2;

				vector<Point> vec1;
				vec1.push_back(Point(_pointsVec[i - 1].x - nor.x, _pointsVec[i - 1].y - nor.y));
				vec1.push_back(Point(_pointsVec[i - 0].x - nor.x, _pointsVec[i - 0].y - nor.y));
				vec1.push_back(Point(_pointsVec[i - 1].x + nor.x, _pointsVec[i - 1].y + nor.y));
				vec1.push_back(Point(_pointsVec[i - 0].x + nor.x, _pointsVec[i - 0].y + nor.y));

				EditPolyPrim* editPolyPrim = phyComShape->AddPoly(vec1, 3.5f);
				editPolyPrim->SetBgOpaque(true);
			}

			if (phyComShape->GetNumParts()>0)
				PhysicsManager::GetInstance()->AddCompoundShape(phyComShape);
		}

		_pointsVec.clear();
	}
}

void Editor::MakeLineLoop(bool makeCompound)
{
	if (Input::IsMouseClicked())
	{
		_pointsVec.clear();
		_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
	}
	else if (Input::IsMousePressed())
	{
		float mx = Input::MouseX;
		float my = Input::MouseY;

		float lastX = _pointsVec[_pointsVec.size() - 1].x;
		float lastY = _pointsVec[_pointsVec.size() - 1].y;

		float dist = sqrt((lastX - mx)*(lastX - mx) + (lastY - my)*(lastY - my));

		if (dist > 7)
			_pointsVec.push_back(Point(mx, my));
	}
	else if (Input::IsMouseReleased())
	{
		int pointsSize = _pointsVec.size();

		int comID = -1;

		if(makeCompound)
			comID = GetNewCompoundID();

		if (pointsSize > 1)
		{
			for (int i = 1; i<pointsSize; i++)
			{
				EditLinePrim* prim = new EditLinePrim(_pointsVec[i - 1].x, _pointsVec[i - 1].y, _pointsVec[i].x, _pointsVec[i].y);

				if(makeCompound)
					prim->SetCompoundShapeID(comID);

				AddPrim((EditPrim*)prim);
			}
		}

		_pointsVec.clear();
	}
}

bool Editor::ActivatePrim(float x, float y)
{
	int selIndex = -1;

	for(int i=_primVec.size()-1; i>=0; i--)
	{
		if(_primVec[i]->Contains(x,y))
		{
			ActivatePrim(_primVec[i]);
			selIndex = i;
		}
	}

	if(selIndex >= 0)
	{
		for(int i=selIndex; i<_primVec.size()-1; i++)
		{
			_primVec[i] = _primVec[i+1];
		}

		_primVec[_primVec.size()-1] = _activePrim;
	}

	return (selIndex >= 0);
}

void Editor::ActivatePrim(EditPrim* editPrim)
{
	if(_activePrim)
		_activePrim->SetSelect(false);

	_activePrim = editPrim;
	_activePrim->SetSelect(true);
}

void Editor::AddDataForUndo()
{
	vector<EditPrim*> lastData = _undoRedo->GetLastUndoData();

	if (lastData.size() != _primVec.size())
	{
		_undoRedo->Copy(_primVec);
	}
	else
	{
		for (int i = 0; i < (int)_primVec.size(); i++)
		{
			if (_primVec[i]->ID != lastData[i]->ID)
			{
				_undoRedo->Copy(_primVec);
				return;
			}
			else if (_primVec[i]->GetCompoundShapeID() != lastData[i]->GetCompoundShapeID())
			{
				_undoRedo->Copy(_primVec);
				return;
			}
			else if (_primVec[i]->IsDynamicBody() != lastData[i]->IsDynamicBody())
			{
				_undoRedo->Copy(_primVec);
				return;
			}
			else if (_primVec[i]->IsInGroup() != lastData[i]->IsInGroup())
			{
				_undoRedo->Copy(_primVec);
				return;
			}
			//else if (_primVec[i]->IsSelected() != lastData[i]->IsSelected())
			//{
			//	_undoRedo->Copy(_primVec);
			//	return;
			//}
			else
			{
				if (_primVec[i]->ID == EditPrim::POLY)
				{
					EditPolyPrim* prim1 = (EditPolyPrim*)_primVec[i];
					EditPolyPrim* prim2 = (EditPolyPrim*)lastData[i];

					if (!IsSame(prim1->vec, prim2->vec))
					{
						_undoRedo->Copy(_primVec);
						return;
					}
				}
				else if (_primVec[i]->ID == EditPrim::RECT)
				{
					EditRectPrim* prim1 = (EditRectPrim*)_primVec[i];
					EditRectPrim* prim2 = (EditRectPrim*)lastData[i];

					if (!IsSame(prim1->vec, prim2->vec))
					{
						_undoRedo->Copy(_primVec);
						return;
					}
				}
				else if (_primVec[i]->ID == EditPrim::LINE)
				{
					EditLinePrim* prim1 = (EditLinePrim*)_primVec[i];
					EditLinePrim* prim2 = (EditLinePrim*)lastData[i];

					if (!IsSame(prim1->vec, prim2->vec))
					{
						_undoRedo->Copy(_primVec);
						return;
					}
				}
				else if (_primVec[i]->ID == EditPrim::CIRCLE)
				{
					EditCirclePrim* prim1 = (EditCirclePrim*)_primVec[i];
					EditCirclePrim* prim2 = (EditCirclePrim*)lastData[i];

					if (prim1->GetCX() != prim2->GetCX() || prim1->GetCY() != prim2->GetCY() || prim1->GetRadius() != prim2->GetRadius())
					{
						_undoRedo->Copy(_primVec);
						return;
					}
				}
			}
		}
	}
}

bool Editor::IsSame(vector<Point> vec1, vector<Point> vec2)
{
	if (vec1.size() != vec2.size())
	{
		return false;
	}
	else
	{
		int len = vec1.size();

		for (int i = 0; i < len; i++)
		{
			if (vec1[i].x != vec2[i].x || vec1[i].y != vec2[i].y)
			{
				return false;
			}
		}
	}

	return true;
}

void Editor::Undo()
{
	for (int i = 0; i<(int)_primVec.size(); i++)
	{
		if (_primVec[i])
		{
			delete _primVec[i];
			_primVec[i] = NULL;
		}
	}
	_primVec.clear();

	vector<EditPrim*> undoVec = _undoRedo->UndoData();

	for (int i = 0; i < undoVec.size(); i++)
	{
		_primVec.push_back(undoVec[i]->Clone());
		_primVec[i]->SetSelect(false);
	}

	_activePrim = NULL;
}

void Editor::Redo()
{
	for (int i = 0; i<(int)_primVec.size(); i++)
	{
		if (_primVec[i])
		{
			delete _primVec[i];
			_primVec[i] = NULL;
		}
	}
	_primVec.clear();

	vector<EditPrim*> redoVec = _undoRedo->RedoData();

	for (int i = 0; i < redoVec.size(); i++)
	{
		_primVec.push_back(redoVec[i]->Clone());
		_primVec[i]->SetSelect(false);
	}

	_activePrim = NULL;
}

void Editor::DeActiatePrim()
{
	if(_activePrim == NULL)
		return;

	for(int i=_primVec.size()-1; i>=0; i--)
	{
		if(_primVec[i] == _activePrim)
		{
			if(_activePrim)
				_activePrim->SetSelect(false);

			_activePrim = NULL;
			break;
		}
	}
}

void Editor::DeSelectAll()
{
	for (int i = 0; i<(int)_primVec.size(); i++)
		_primVec[i]->SetInGroup(false);
}

void Editor::SelectAll()
{
	for (int i = 0; i<(int)_primVec.size(); i++)
		_primVec[i]->SetInGroup(true);
}

void Editor::Duplicate()
{
	if(HasGroupSelection())
	{
		vector<EditPrim*> newPrimsVec;

		for(int i=0; i<_primVec.size(); i++)
		{
			if (_primVec[i]->IsInGroup())
			{
				newPrimsVec.push_back( _primVec[i]->Clone() );
				_primVec[i]->SetInGroup(false);
			}
		}

		for(int i=0; i<newPrimsVec.size(); i++)
		{
			EditPrim* editPrim = newPrimsVec[i];
			editPrim->Move(3,5);
			editPrim->SetInGroup(true);
			AddPrim(editPrim);
		}
	}
	else
	{
		if(_activePrim)
		{
			_activePrim->SetSelect(false);

			EditPrim* newPrim = _activePrim->Clone();
			newPrim->SetSelect(true);
			newPrim->Move(3,5);
			_primVec.push_back(newPrim);

			_activePrim = newPrim;
		}
	}
}

void Editor::Copy()
{
	bool hasGroupSelection = HasGroupSelection();

	if (hasGroupSelection || _activePrim)
	{
		for (int i = 0; i < _copyPrimVec.size(); i++)
		{
			if (_copyPrimVec[i])
			{
				delete _copyPrimVec[i];
				_copyPrimVec[i] = NULL;
			}
		}

		_copyPrimVec.clear();
	}

	if(hasGroupSelection)
	{
		for (int i = 0; i<_primVec.size(); i++)
		{
			if (_primVec[i]->IsInGroup())
			{
				_copyPrimVec.push_back(_primVec[i]->Clone());
			}
		}
	}
	else if (_activePrim)
	{
		_copyPrimVec.push_back(_activePrim->Clone());
	}
}

void Editor::Paste()
{
	if (_copyPrimVec.size() > 0)
	{
		DeSelectAll();

		if (_activePrim)
		{
			_activePrim->SetSelect(false);
			_activePrim = NULL;
		}

		if (_copyPrimVec.size() == 1)
		{
			EditPrim* editPrim  = _copyPrimVec[0]->Clone();
			editPrim->SetSelect(true);
			AddPrim(editPrim);
			_activePrim = editPrim;
		}
		else
		{
			for (int i = 0; i < _copyPrimVec.size(); i++)
			{
				EditPrim* editPrim = _copyPrimVec[i]->Clone();
				editPrim->SetInGroup(true);
				editPrim->SetSelect(false);
				AddPrim(editPrim);
			}
		}
	}
}

void Editor::Delete()
{
	if(HasGroupSelection())
		DeleteSelectedGroup();
	else
		DeleteActivePrim();
}

void Editor::DeleteActivePrim()
{
	for (int i = 0; i<(int)_primVec.size(); i++)
	{
		if (_primVec[i] == _activePrim)
		{
			delete _activePrim;
			_activePrim = NULL;
			_primVec.erase(_primVec.begin() + i);
			break;
		}
	}
}

bool Editor::IsCompoundIDExist(int id)
{
	for (int i = 0; i<(int)_primVec.size(); i++)
	{
		if (_primVec[i]->GetCompoundShapeID() == id)
		{
			return true;
		}
	}

	return false;
}

void Editor::DeleteSelectedGroup()
{
	for (int i = 0; i<(int)_primVec.size(); i++)
	{
		if (_primVec[i]->IsInGroup())
		{
			if (_primVec[i] == _activePrim)
				_activePrim = NULL;

			delete _primVec[i];
			
			_primVec.erase(_primVec.begin() + i);
			i--;
		}
	}
}

int Editor::GetNewCompoundID()
{
	int compoundID = 0;
	bool runAgain = false;
	
	do
	{
		runAgain = false;

		for(int i=0; i<_primVec.size(); i++)
		{
			if(compoundID == _primVec[i]->GetCompoundShapeID())
			{
				compoundID++;
				runAgain = true;
			}
		}
	}
	while(runAgain);

	return compoundID;
}

int Editor::GetMaxCompoundID()
{
	int maxID = -1;

	for(int i=0; i<_primVec.size(); i++)
	{
		if(maxID < _primVec[i]->GetCompoundShapeID())
		{
			maxID = _primVec[i]->GetCompoundShapeID();
		}
	}

	return maxID;
}

void Editor::Move(float dx, float dy)
{
	if(HasGroupSelection())
	{
		for(int i=0; i<_primVec.size(); i++)
		{
			if (_primVec[i]->IsInGroup())
			{
				_primVec[i]->Move(dx, dy);
			}
		}		
	}
	else if(_activePrim)
	{
		_activePrim->Move(dx, dy);
	}
}

void Editor::AddRotation(float angleInDegrees)
{
	if(HasGroupSelection())
	{
		for(int i=0; i<_primVec.size(); i++)
		{
			if (_primVec[i]->IsInGroup())
			{
				_primVec[i]->AddRotation(angleInDegrees, Input::MouseX, Input::MouseY);
			}
		}		
	}
	else if(_activePrim)
	{
		_activePrim->AddRotation(angleInDegrees, Input::MouseX, Input::MouseY);
	}
}

void Editor::AddPrim(EditPrim* prim)
{
	if(_primVec.size()==0)
	{
		prim->SetSelect(true);
		_activePrim = prim;
	}
	_primVec.push_back((EditPrim*)prim);
}

bool Editor::HasGroupSelection()
{
	for (int i = 0; i<(int)_primVec.size(); i++)
		if (_primVec[i]->IsInGroup())
			return true;

	return false;
}

void Editor::Draw()
{
	if(_activatePhysics)
	{
		PhysicsManager::GetInstance()->Draw();
		DrawLineLoops();

		if(_showOptions)
			_myWindow->Draw();
	}
	else
	{
		if (_horGrid->IsOn())
		{
			float ow = GameInfo::GetInstance()->CodedW();
			float oh = GameInfo::GetInstance()->CodedW();

			float delta = oh / 40;

			AddData* addData = AddData::GetInstance();
			addData->glColor(0xffffff88);
			addData->glBegin(GL_LINES);
			for (int i = 0; i <= ow; i += delta)
			{
				addData->glVertex3f(0, i, 0);
				addData->glVertex3f(ow, i, 0);
			}
			addData->glEnd();
		}

		if (_verGrid->IsOn())
		{
			float ow = GameInfo::GetInstance()->CodedW();
			float oh = GameInfo::GetInstance()->CodedW();

			float delta = ow / 40;

			AddData* addData = AddData::GetInstance();
			addData->glColor(0xffffff88);
			addData->glBegin(GL_LINES);
			for (int i = 0; i <= oh; i += delta)
			{
				addData->glVertex3f(i, 0,  0);
				addData->glVertex3f(i, oh,  0);
			}
			addData->glEnd();
		}

		for(int i=0;i<(int)_primVec.size();i++)
			_primVec[i]->Draw();

		DrawLineLoops();

		RectPrim rectPrim = GetRect(_selP1, _selP2);

		if (rectPrim.w > 0 && rectPrim.h > 0)
		{
			rectPrim.SetBorderColor(0x0000ffff);
			rectPrim.SetColor(0x0000ff88);
			rectPrim.Draw();
		}

		if(_showOptions)
			_myWindow->Draw();
	}
	
	RectPrim::Draw(Input::MouseX, Input::MouseY, 2, 2, 0, 0xffffffff, NULL);
}

void Editor::DrawLineLoops()
{
	int numPoints = _pointsVec.size();

	if (numPoints > 1)
	{
		AddData* addData = AddData::GetInstance();
		addData->glColor(0xffffffff);
		addData->glBegin(GL_TRIANGLES);

		for (int i = 1; i<numPoints; i++)
		{
			CVector3 nor(_pointsVec[i - 1].y - _pointsVec[i].y, _pointsVec[i].x - _pointsVec[i - 1].x);
			nor.Normalize();
			nor *= 4 / 2;

			addData->glVertex3f(_pointsVec[i - 1].x - nor.x, _pointsVec[i - 1].y - nor.y, 0);
			addData->glVertex3f(_pointsVec[i - 0].x - nor.x, _pointsVec[i - 0].y - nor.y, 0);
			addData->glVertex3f(_pointsVec[i - 1].x + nor.x, _pointsVec[i - 1].y + nor.y, 0);
			addData->glVertex3f(_pointsVec[i - 0].x - nor.x, _pointsVec[i - 0].y - nor.y, 0);
			addData->glVertex3f(_pointsVec[i - 1].x + nor.x, _pointsVec[i - 1].y + nor.y, 0);
			addData->glVertex3f(_pointsVec[i - 0].x + nor.x, _pointsVec[i - 0].y + nor.y, 0);
		}

		addData->glEnd();
	}
}

RectPrim Editor::GetRect(Point p1, Point p2)
{
	float dx = _selP1.x - _selP2.x;
	float dy = _selP1.y - _selP2.y;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;

	if (dx >= 1 && dy >= 1)
	{
		float x = _selP1.x;
		float y = _selP1.y;

		if (x > _selP2.x)	x = _selP2.x;
		if (y > _selP2.y)	y = _selP2.y;

		return RectPrim(x, y, dx, dy, 0, 0x0000ff88);
	}

	return RectPrim(0,0,0,0, 0,0x0000ff88);
}

void Editor::ActivatePhysics()
{
	vector<PhyCompoundShape*> _compoundShapeVec;
	_compoundShapeVec.clear();

	int maxCompoundID = GetMaxCompoundID();

	if(maxCompoundID >= 0)
	{
		for(int i=0;i<=maxCompoundID;i++)
		{
			PhyCompoundShape* phyComShape = new PhyCompoundShape();
			_compoundShapeVec.push_back(phyComShape);
		}
	}

	int bodyType = 0;

	for(int i=0;i<(int)_primVec.size();i++)
	{
		if(_primVec[i]->GetCompoundShapeID() == -1 )
		{
			if( _primVec[i]->ID == EditPrim::LINE)
			{
				EditLinePrim* prim = (EditLinePrim*)_primVec[i];

				if(prim->IsDynamicBody())		bodyType = PhyShape::DYNAMIC_BODY;
				else if(prim->IsStaticBody())	bodyType = PhyShape::STATIC_BODY;

				CVector3 nor(prim->vec[0].y - prim->vec[1].y, prim->vec[1].x - prim->vec[0].x);
				nor.Normalize();
				nor *= prim->GetLineWidth()/2;

				vector<Point> vec1;
				vec1.push_back(Point(prim->vec[0].x - nor.x, prim->vec[0].y - nor.y));
				vec1.push_back(Point(prim->vec[1].x - nor.x, prim->vec[1].y - nor.y));
				vec1.push_back(Point(prim->vec[0].x + nor.x, prim->vec[0].y + nor.y));
				vec1.push_back(Point(prim->vec[1].x + nor.x, prim->vec[1].y + nor.y));

				PhyPoly* phyPoly = PhysicsManager::GetInstance()->AddPolygon(vec1, bodyType, 3.5f);
				phyPoly->SetColor(0,255,0,255);
				phyPoly->SetAngularVelocity(prim->GetAngularVelocity(), prim->IsAngularVelLoop());
			}
			else if( _primVec[i]->ID == EditPrim::CIRCLE)
			{
				EditCirclePrim* prim = (EditCirclePrim*)_primVec[i];

				if(prim->IsDynamicBody())		bodyType = PhyShape::DYNAMIC_BODY;
				else if(prim->IsStaticBody())	bodyType = PhyShape::STATIC_BODY;

				float radius = prim->GetRadius();

				if (radius < 0)
					radius = -radius;

				PhyCircle* phyCir = PhysicsManager::GetInstance()->AddCircle(prim->GetCX(), prim->GetCY(), radius, bodyType);
				phyCir->SetAngularVelocity(prim->GetAngularVelocity(), prim->IsAngularVelLoop());
			}
			else if( _primVec[i]->ID == EditPrim::RECT)
			{
				EditRectPrim* prim = (EditRectPrim*)_primVec[i];

				if(prim->IsDynamicBody())		bodyType = PhyShape::DYNAMIC_BODY;
				else if(prim->IsStaticBody())	bodyType = PhyShape::STATIC_BODY;

				PhyPoly* phyPoly = PhysicsManager::GetInstance()->AddPolygon(prim->vec, bodyType);
				phyPoly->SetAngularVelocity(prim->GetAngularVelocity(), prim->IsAngularVelLoop());
			}
			else if( _primVec[i]->ID == EditPrim::POLY)
			{
				EditPolyPrim* prim = (EditPolyPrim*)_primVec[i];

				if(prim->IsDynamicBody())		bodyType = PhyShape::DYNAMIC_BODY;
				else if(prim->IsStaticBody())	bodyType = PhyShape::STATIC_BODY;

				PhyPoly* phyPoly = PhysicsManager::GetInstance()->AddPolygon(prim->vec, bodyType);
				phyPoly->SetAngularVelocity(prim->GetAngularVelocity(), prim->IsAngularVelLoop());
			}
		}
		else if(_primVec[i]->GetCompoundShapeID() >= 0)
		{
			int compoundID = _primVec[i]->GetCompoundShapeID();

			if (_primVec[i]->ID == EditPrim::LINE)
			{
				EditLinePrim* prim = (EditLinePrim*)_primVec[i];

				CVector3 nor(prim->vec[0].y - prim->vec[1].y, prim->vec[1].x - prim->vec[0].x);
				nor.Normalize();
				nor *= prim->GetLineWidth()/2;

				vector<Point> vec1;
				vec1.push_back(Point(prim->vec[0].x - nor.x, prim->vec[0].y - nor.y));
				vec1.push_back(Point(prim->vec[1].x - nor.x, prim->vec[1].y - nor.y));
				vec1.push_back(Point(prim->vec[0].x + nor.x, prim->vec[0].y + nor.y));
				vec1.push_back(Point(prim->vec[1].x + nor.x, prim->vec[1].y + nor.y));

				EditPolyPrim* editPolyPrim = _compoundShapeVec[compoundID]->AddPoly(vec1, 3.5f);
				editPolyPrim->SetBgOpaque(true);
			}
			else if( _primVec[i]->ID == EditPrim::CIRCLE)
			{
				EditCirclePrim* circlePrim = (EditCirclePrim*)_primVec[i];

				float radius = circlePrim->GetRadius();

				if (radius < 0)
					radius = -radius;

				_compoundShapeVec[compoundID]->AddCircle(circlePrim->GetCX(), circlePrim->GetCY(), radius);
			}
			else if( _primVec[i]->ID == EditPrim::RECT)
			{
				EditRectPrim* rectPrim = (EditRectPrim*)_primVec[i];
				_compoundShapeVec[compoundID]->AddPoly(rectPrim->vec);
			}
			else if( _primVec[i]->ID == EditPrim::POLY)
			{
				EditPolyPrim* polyPrim = (EditPolyPrim*)_primVec[i];
				_compoundShapeVec[compoundID]->AddPoly(polyPrim->vec);
			}

			if(_primVec[i]->IsStaticBody())
				_compoundShapeVec[compoundID]->SetBodyType(PhyShape::STATIC_BODY);
		}
	}

	for(int i=0;i<(int)_compoundShapeVec.size();i++)
	{
		if(_compoundShapeVec[i]->GetNumParts()>0)
		{
			PhyCompoundShape* comShape = PhysicsManager::GetInstance()->AddCompoundShape(_compoundShapeVec[i]);
			comShape->SetAngularVelocity(3, true);
		}
		else
		{
			delete _compoundShapeVec[i];
			_compoundShapeVec[i] = NULL;
		}
	}

	_compoundShapeVec.clear();

	_activatePhysics = true;
}


void Editor::DeActivatePhysics()
{
	PhysicsManager::GetInstance()->RemoveBodies();
	_activatePhysics = false;
}

void Editor::ReadFromFile(string filePath)
{
	if(filePath.length() == 0)
		return;

	CheckForSaveBeforeLoad();
	
	//clear all existing primitives before loading new file.
	for(int i=0; i<(int)_primVec.size(); i++)
	{
		if(_primVec[i])
		{
			delete _primVec[i];
			_primVec[i] = NULL;
		}
	}
	_primVec.clear();

	FileReader loadFile;
	bool loadedFile = loadFile.LoadFileFromPath(filePath.c_str());
	
	if (loadedFile == false)
	{
		return;
	}

	vector<string> numPrimsVec = loadFile.GetTagData("NumPrims", true);
	int numPrims = atoi(numPrimsVec[0].c_str());

	vector<string> primDataVec = loadFile.GetTagData("Primitives", true);
	
	char str[128];

	for(int i=0; i<(int)primDataVec.size(); i++)
	{
		int isDynamic = 1;
		int compoundId = 0;

		if (primDataVec[i].find("Line") != -1)
		{
			float x1, y1, x2, y2;
			sscanf(primDataVec[i].c_str(), "%s %d %d %f %f %f %f", str, &isDynamic, &compoundId, &x1, &y1, &x2, &y2);

			EditLinePrim* prim = new EditLinePrim(x1, y1, x2, y2);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody(isDynamic == 1);
			prim->SetCompoundShapeID(compoundId);
		}
		else if(primDataVec[i].find("Circle") != -1)
		{
			float cx,cy,r;
			sscanf( primDataVec[i].c_str(),"%s %d %d %f %f %f", str, &isDynamic, &compoundId, &cx, &cy, &r);

			EditCirclePrim* prim = new EditCirclePrim(cx,cy,r);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody(isDynamic == 1);
			prim->SetCompoundShapeID(compoundId);
		}
		else if (primDataVec[i].find("Rect") != -1)
		{
			float x[4], y[4];
			sscanf(primDataVec[i].c_str(), "%s %d %d %f %f %f %f %f %f %f %f", str, &isDynamic, &compoundId, 
				&x[0], &y[0],
				&x[1], &y[1],
				&x[2], &y[2],
				&x[3], &y[3]);

			vector<Point> vec;
			for (int i = 0; i<4; i++)
				vec.push_back(Point(x[i], y[i]));

			EditRectPrim* prim = new EditRectPrim(vec);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody((isDynamic == 1));
			prim->SetCompoundShapeID(compoundId);
		}
		else if (primDataVec[i].find("Poly3") != -1)
		{
			float x[3], y[3];
			sscanf(primDataVec[i].c_str(), "%s %d %d %f %f %f %f %f %f", str, &isDynamic, &compoundId, 
				&x[0], &y[0],
				&x[1], &y[1],
				&x[2], &y[2]);

			vector<Point> vec;
			for (int i = 0; i<3; i++)
				vec.push_back(Point(x[i], y[i]));

			EditPolyPrim* prim = new EditPolyPrim(vec);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody((isDynamic == 1));
			prim->SetCompoundShapeID(compoundId);
		}
		else if(primDataVec[i].find("Poly4") != -1)
		{
			float x[4],y[4];
			sscanf( primDataVec[i].c_str(),"%s %d %d %f %f %f %f %f %f %f %f", str, &isDynamic, &compoundId, 
				&x[0], &y[0],
				&x[1], &y[1],
				&x[2], &y[2], 
				&x[3], &y[3]);

			vector<Point> vec;
			for(int i=0; i<4; i++)
				vec.push_back(Point(x[i], y[i]));

			EditPolyPrim* prim = new EditPolyPrim(vec);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody((isDynamic == 1));
			prim->SetCompoundShapeID(compoundId);
		}
		else if(primDataVec[i].find("Poly5") != -1)
		{
			float x[5],y[5];
			sscanf( primDataVec[i].c_str(),"%s %d %d %f %f %f %f %f %f %f %f %f %f", str, &isDynamic, &compoundId, 
				&x[0], &y[0],
				&x[1], &y[1],
				&x[2], &y[2],
				&x[3], &y[3],
				&x[4], &y[4]);

			vector<Point> vec;
			for(int i=0; i<5; i++)
				vec.push_back(Point(x[i], y[i]));

			EditPolyPrim* prim = new EditPolyPrim(vec);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody((isDynamic == 1));
			prim->SetCompoundShapeID(compoundId);
		}
		else if(primDataVec[i].find("Poly6") != -1)
		{
			float x[6],y[6];
			sscanf( primDataVec[i].c_str(),"%s %d %d %f %f %f %f %f %f %f %f %f %f %f %f", str, &isDynamic, &compoundId, 
				&x[0], &y[0],
				&x[1], &y[1],
				&x[2], &y[2],
				&x[3], &y[3],
				&x[4], &y[4],
				&x[5], &y[5]);

			vector<Point> vec;
			for(int i=0; i<6; i++)
				vec.push_back(Point(x[i], y[i]));

			EditPolyPrim* prim = new EditPolyPrim(vec);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody((isDynamic == 1));
			prim->SetCompoundShapeID(compoundId);
		}
		else if(primDataVec[i].find("Poly7") != -1)
		{
			float x[7],y[7];
			sscanf( primDataVec[i].c_str(),"%s %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f", str, &isDynamic, &compoundId,
				&x[0], &y[0],
				&x[1], &y[1],
				&x[2], &y[2],
				&x[3], &y[3],
				&x[4], &y[4],
				&x[5], &y[5],
				&x[6], &y[6]);
			
			vector<Point> vec;
			for(int i=0; i<7; i++)
				vec.push_back(Point(x[i], y[i]));

			EditPolyPrim* prim = new EditPolyPrim(vec);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody((isDynamic == 1));
			prim->SetCompoundShapeID(compoundId);
		}
		else if(primDataVec[i].find("Poly8") != -1)
		{
			float x[8],y[8];
			sscanf( primDataVec[i].c_str(),"%s %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", str, &isDynamic, &compoundId,
				&x[0], &y[0],
				&x[1], &y[1],
				&x[2], &y[2],
				&x[3], &y[3],
				&x[4], &y[4],
				&x[5], &y[5],
				&x[6], &y[6],
				&x[7], &y[7]);

			vector<Point> vec;
			for(int i=0; i<8; i++)
				vec.push_back(Point(x[i], y[i]));

			EditPolyPrim* prim = new EditPolyPrim(vec);
			AddPrim((EditPrim*)prim);

			prim->SetDynamicBody((isDynamic == 1));
			prim->SetCompoundShapeID(compoundId);
		}
	}

	_currFilePath = filePath;
	Platform::GetPlatform()->SetWindowTitle((char*)_currFilePath.c_str());
}

void Editor::CheckForSaveBeforeLoad()
{
	if(_primVec.size() > 0)
	{
		int answer = MessageBox(NULL, "Do you want to save the changes", NULL, MB_OKCANCEL);

		if(answer == 1)	//IDOK = 1
		{
			if(_currFilePath.length() > 0)
				WriteInFile(_currFilePath);
			else
				WriteInFile(SaveFileDialog());
		}
	}
}

void Editor::WriteInFile(string filePath)
{
	if(filePath.length() == 0)
		return;

	FILE* fp;
	fp = fopen(filePath.c_str(), "w");

	fprintf(fp, "\n<NumPrims>");
	fprintf(fp, "\n%d",_primVec.size());
	fprintf(fp, "\n</NumPrims>");

	fprintf(fp, "\n<Primitives>");

	char str[128];

	for(int i=0;i<(int)_primVec.size();i++)
	{
		EditPrim* prim = _primVec[i];

		if (prim->ID == EditPrim::LINE)
		{
			EditLinePrim* lprim = (EditLinePrim*)prim;

			int isDynamic = 1;
			if (prim->IsDynamicBody() == false)
				isDynamic = 0;

			sprintf(str, "Line");
			fprintf(fp, "\n%s %d %d %f %f %f %f", str, isDynamic, lprim->GetCompoundShapeID(),
													lprim->vec[0].x, lprim->vec[0].y, lprim->vec[1].x, lprim->vec[1].y);
		}
		else if(prim->ID == EditPrim::CIRCLE)
		{
			EditCirclePrim* cirPrim = (EditCirclePrim*)prim;

			int isDynamic = 1;
			if( cirPrim->IsDynamicBody() == false)
				isDynamic = 0;

			sprintf(str, "Circle");
			fprintf(fp, "\n%s %d %d %f %f %f",str, isDynamic, cirPrim->GetCompoundShapeID(), cirPrim->GetCX(), cirPrim->GetCY(), cirPrim->GetRadius() );
		}
		else if (prim->ID == EditPrim::RECT)
		{
			EditRectPrim* rectPrim = (EditRectPrim*)prim;

			int isDynamic = 1;
			if (rectPrim->IsDynamicBody() == false)
				isDynamic = 0;

			sprintf(str, "Rect");
			fprintf(fp, "\n%s %d %d %f %f %f %f %f %f %f %f", str, isDynamic, rectPrim->GetCompoundShapeID(), rectPrim->vec[0].x, rectPrim->vec[0].y,
				rectPrim->vec[1].x, rectPrim->vec[1].y,
				rectPrim->vec[2].x, rectPrim->vec[2].y,
				rectPrim->vec[3].x, rectPrim->vec[3].y);
		}
		else if(prim->ID == EditPrim::POLY)
		{
			WritePolyInFile(fp, prim);
		}
	}

	fprintf(fp, "\n</Primitives>");

	fclose(fp);
}


void Editor::WritePolyInFile(FILE* fp, EditPrim* prim)
{
	EditPolyPrim* polyPrim = (EditPolyPrim*)prim;

	int isDynamic = 1;
	if( polyPrim->IsDynamicBody() == false)
		isDynamic = 0;

	char str[128];

	int size = polyPrim->vec.size();

	if (size == 3)
	{
		sprintf(str, "Poly3");
		fprintf(fp, "\n%s %d %d %f %f %f %f %f %f", str, isDynamic, polyPrim->GetCompoundShapeID(),
			polyPrim->vec[0].x, polyPrim->vec[0].y,
			polyPrim->vec[1].x, polyPrim->vec[1].y,
			polyPrim->vec[2].x, polyPrim->vec[2].y);
	}
	else if(size == 4)
	{
		sprintf(str, "Poly4");
		fprintf(fp, "\n%s %d %d %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->GetCompoundShapeID(),
																			polyPrim->vec[0].x, polyPrim->vec[0].y,
																			polyPrim->vec[1].x, polyPrim->vec[1].y,
																			polyPrim->vec[2].x, polyPrim->vec[2].y,
																			polyPrim->vec[3].x, polyPrim->vec[3].y);
	}
	else if(size == 5)
	{
		sprintf(str, "Poly5");
		fprintf(fp, "\n%s %d %d %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->GetCompoundShapeID(), 
																			polyPrim->vec[0].x, polyPrim->vec[0].y,
																			polyPrim->vec[1].x, polyPrim->vec[1].y,
																			polyPrim->vec[2].x, polyPrim->vec[2].y,
																			polyPrim->vec[3].x, polyPrim->vec[3].y,
																			polyPrim->vec[4].x, polyPrim->vec[4].y);
	}
	else if(size == 6)
	{
		sprintf(str, "Poly6");
		fprintf(fp, "\n%s %d %d %f %f %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->GetCompoundShapeID(),
																				polyPrim->vec[0].x, polyPrim->vec[0].y,
																				polyPrim->vec[1].x, polyPrim->vec[1].y,
																				polyPrim->vec[2].x, polyPrim->vec[2].y,
																				polyPrim->vec[3].x, polyPrim->vec[3].y,
																				polyPrim->vec[4].x, polyPrim->vec[4].y,
																				polyPrim->vec[5].x, polyPrim->vec[5].y);
	}
	else if(size == 7)
	{
		sprintf(str, "Poly7");
		fprintf(fp, "\n%s %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->GetCompoundShapeID(),
																						polyPrim->vec[0].x, polyPrim->vec[0].y,
																						polyPrim->vec[1].x, polyPrim->vec[1].y,
																						polyPrim->vec[2].x, polyPrim->vec[2].y,
																						polyPrim->vec[3].x, polyPrim->vec[3].y,
																						polyPrim->vec[4].x, polyPrim->vec[4].y,
																						polyPrim->vec[5].x, polyPrim->vec[5].y,
																						polyPrim->vec[6].x, polyPrim->vec[6].y);
	}
	else if(size == 8)
	{
		sprintf(str, "Poly8");
		fprintf(fp, "\n%s %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->GetCompoundShapeID(),
																							polyPrim->vec[0].x, polyPrim->vec[0].y,
																							polyPrim->vec[1].x, polyPrim->vec[1].y,
																							polyPrim->vec[2].x, polyPrim->vec[2].y,
																							polyPrim->vec[3].x, polyPrim->vec[3].y,
																							polyPrim->vec[4].x, polyPrim->vec[4].y,
																							polyPrim->vec[5].x, polyPrim->vec[5].y,
																							polyPrim->vec[6].x, polyPrim->vec[6].y,
																							polyPrim->vec[7].x, polyPrim->vec[7].y);
	}
}


string Editor::OpenFileDialog(char *filter, void* owner)
{
	string fileNameStr;

#ifdef _PLATFORM_WIN32
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = (HWND)owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	 
	if ( GetOpenFileName(&ofn) )
	fileNameStr = fileName;
#endif

	return fileNameStr;
}


string Editor::SaveFileDialog(char *filter, void* owner)
{
	string fileNameStr;

#ifdef _PLATFORM_WIN32
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = (HWND)owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	if(GetSaveFileNameA(&ofn))
		fileNameStr = fileName;
#endif

	return fileNameStr;
}

Editor::~Editor()
{
	if(_myWindow)
	{
		delete _myWindow;
		_myWindow = NULL;
	}
	
	_activePrim = NULL;

	for(int i=0;i<(int)_primVec.size();i++)
	{
		if(_primVec[i])
		{
			delete _primVec[i];
			_primVec[i] = NULL;
		}
	}
	_primVec.clear();

	if(_undoRedo)
	{
		delete _undoRedo;
		_undoRedo = NULL;
	}


	for (int i = 0; i < _copyPrimVec.size(); i++)
	{
		if (_copyPrimVec[i])
		{
			delete _copyPrimVec[i];
			_copyPrimVec[i] = NULL;
		}
	}

	_copyPrimVec.clear();


	PhysicsManager::GetInstance()->Terminate();
}

/*
	else if (com == _lineLoop)
	{
		if(_activatePhysics)
			DeActivatePhysics();

		if (Input::IsMouseClicked())
		{
			_pointsVec.clear();
			_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
		}
		else if (Input::IsMousePressed())
		{
			float mx = Input::MouseX;
			float my = Input::MouseY;

			float lastX = _pointsVec[_pointsVec.size() - 1].x;
			float lastY = _pointsVec[_pointsVec.size() - 1].y;

			float dist = sqrt((lastX - mx)*(lastX - mx) + (lastY - my)*(lastY - my));

			if (dist > 7)
			{
				_pointsVec.push_back(Point(mx, my));
				//printLog("\n Total Points : %d ", _pointsVec.size());
			}
		}
		else if (Input::IsMouseReleased())
		{
			int pointsSize = _pointsVec.size();

			if (pointsSize > 1)
			{
				for (int i = 1; i<pointsSize; i++)
				{
					EditLinePrim* prim = new EditLinePrim(_pointsVec[i - 1].x, _pointsVec[i - 1].y, _pointsVec[i].x, _pointsVec[i].y);
					prim->SetDynamicBody(false);
					AddPrim((EditPrim*)prim);
				}
			}
		}
	}
	else if (com == _lineSegLoop)
	{
		if(_activatePhysics)
			DeActivatePhysics();

		if (Input::IsMouseClicked())
		{
			_pointsVec.clear();
			_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
		}
		else if (Input::IsMousePressed())
		{
			if (_pointsVec.size() == 1)
			{
				_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
			}
			else if (_pointsVec.size() > 1)
			{
				_pointsVec[_pointsVec.size() - 1].x = Input::MouseX;
				_pointsVec[_pointsVec.size() - 1].y = Input::MouseY;
			}
		}
		else if (Input::IsMouseReleased())
		{
			int pointsSize = _pointsVec.size();

			if (pointsSize > 1)
			{
				for (int i = 1; i<pointsSize; i++)
				{
					EditLinePrim* prim = new EditLinePrim(_pointsVec[i - 1].x, _pointsVec[i - 1].y, _pointsVec[i].x, _pointsVec[i].y);
					prim->SetDynamicBody(false);
					AddPrim((EditPrim*)prim);
				}
			}
		}

		if (Input::IsKeyReleased((int)'A'))
		{
			_pointsVec.push_back(Point(Input::MouseX, Input::MouseY));
		}
	}

	// Options which can be used only in Edit mode...

	else if(_editModeBtn->IsOn())
	{
			AddOrDeleteOptions(label);
	}
	else if(_compoundModeBtn->IsOn())
	{
		if(label.compare("Allow New Compound") == 0)
		{
			if(GetNumCompoundPrims(_compoundShapeID) > 0)
				_compoundShapeID++;
		}
		else
			AddOrDeleteOptions(label);
	}




void Editor::EditInCompoundMode()
{
	if(Input::IsMouseClicked())
	{
		if( ActivatePrim((float)Input::MouseX, (float)Input::MouseY) && _activePrim && Input::IsKeyPressed(KEYS_CONTROL) )
		{
			if( _activePrim->GetCompoundShapeID() == -1)
				_activePrim->SetCompoundShapeID(_compoundShapeID);
			else
				_activePrim->SetCompoundShapeID(-1);
		}
	}
	else if(Input::IsKeyTyped(KEYS_ADD) && _activePrim)
	{
		if(_activePrim->GetCompoundShapeID()+1 <= _compoundShapeID)
			_activePrim->SetCompoundShapeID( _activePrim->GetCompoundShapeID()+1 );
	}
	else if(Input::IsKeyTyped(KEYS_SUBTRACT) && _activePrim)
	{
		if(_activePrim->GetCompoundShapeID()-1 >= 0)
			_activePrim->SetCompoundShapeID( _activePrim->GetCompoundShapeID()-1 );
	}
	else if(_activePrim)
		_activePrim->Update();
}

int Editor::GetNumCompoundPrims(int compoundShapeID)
{
	int count=0;

	for(int i=_primVec.size()-1; i>=0; i--)
	{
		if(_primVec[i]->GetCompoundShapeID() == compoundShapeID)
			count++;
	}
	return count;
}


	//if(Input::IsKeyReleased((int)'Q'))
	//{
	//	if(_lineLoop->IsOn())
	//		_lineLoop->Off();
	//	else
	//	{
	//		_lineLoop->On();
	//		_lineSegLoop->Off();
	//	}
	//}
	//else if(Input::IsKeyReleased((int)'W'))
	//{
	//	if(_lineSegLoop->IsOn())
	//		_lineSegLoop->Off();
	//	else
	//	{
	//		_lineSegLoop->On();
	//		_lineLoop->Off();
	//	}
	//}
	//else 


				////////////////////// without vector... ///////////////////////////
				EditLinePrim* prim = (EditLinePrim*)_primVec[i];

				vector<Point> vec1;

				vec1.push_back(Point(prim->vec[0].x, prim->vec[0].y));
				vec1.push_back(Point(prim->vec[1].x, prim->vec[1].y));

				float dx = prim->vec[1].x - prim->vec[0].x;
				float dy = prim->vec[1].y - prim->vec[0].y;

				if (dx < 0) dx = -dx;
				if (dy < 0) dy = -dy;

				float add = 2;

				if (dy > dx)
				{
					vec1.push_back(Point(prim->vec[1].x + add, prim->vec[1].y));
					vec1.push_back(Point(prim->vec[0].x + add, prim->vec[0].y));
				}
				else
				{
					vec1.push_back(Point(prim->vec[1].x, prim->vec[1].y + add));
					vec1.push_back(Point(prim->vec[0].x, prim->vec[0].y + add));
				}

				_compoundShapeVec[compoundID]->AddPoly(vec1);


				///////////////////// actual line... //////////////////////////////
				//EditLinePrim* prim = (EditLinePrim*)_primVec[i];
				//_compoundShapeVec[compoundID]->AddLine(prim->vec[0].x, prim->vec[0].y, prim->vec[1].x, prim->vec[1].y);
*/