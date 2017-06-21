#include "GameLoop.h"
#include "Platform/Platform.h"
#include "CommonLib/Input.h"
#include "CommonLib/UIFont.h"
#include "CommonLib/Cam.h"
#include "CommonLib/FileReader.h"
#include "PhysicsLib/PhysicsManager.h"
#include "MathLib/Point.h"

#include "Primitives/LinePrim.h"
#include "Primitives/TriPrim.h"
#include "Primitives/CirclePrim.h"
#include "Primitives/PolyPrim.h"
#include "Primitives/RectPolyPrim.h"

GameLoop::GameLoop()
{
	Platform::Init();

	_gameInit = new GameInit();

	_primVec.clear();
	_activePrim = NULL;
	_copyPrim = NULL;
	_activatePhysics = false;
	_compoundShapeID = 0;

	_myWindow = new MyWindow(1040,10);

	_physicsModeBtn = _myWindow->AddButton("Physics Mode", MyButton::MODE);
	_myWindow->NextRow();
	_editModeBtn = _myWindow->AddButton("Edit Mode", MyButton::MODE);
	_editModeBtn->On();

	_myWindow->NextRow();
	_myWindow->AddButton("Add Line");		_myWindow->NextRow();
	_myWindow->AddButton("Add Circle");		_myWindow->NextRow();
	_myWindow->AddButton("Add Triangle");	_myWindow->NextRow();
	_myWindow->AddButton("Add Rectangle");	_myWindow->NextRow();
	_myWindow->AddButton("Add Polygon 4");	_myWindow->NextRow();
	_myWindow->AddButton("Add Polygon 5");	_myWindow->NextRow();
	_myWindow->AddButton("Add Polygon 6");	_myWindow->NextRow();
	_myWindow->AddButton("Add Polygon 7");	_myWindow->NextRow();
	_myWindow->AddButton("Add Polygon 8");	_myWindow->NextRow();
	_myWindow->AddButton("Delete");			_myWindow->NextRow();
	_myWindow->AddButton("Save");			_myWindow->NextRow();
	_myWindow->AddButton("Load");			_myWindow->NextRow();

	_myWindow->NextRow();
	_compoundModeBtn = _myWindow->AddButton("Compound Mode", MyButton::MODE);
	_myWindow->NextRow();
	_myWindow->AddButton("Allow New Compound");
	_myWindow->NextRow();

	_dataModified = false;
	_currFilePath = "";
	_orthoX = 0;
	_orthoY = 0;
	_orthoW = Platform::GetWindowWidth();
	_orthoH = Platform::GetWindowHeight();

}


void GameLoop::Run(float deltaTime)
{
	Platform::DeviceYield(5);
	Update(deltaTime);
	Draw();
}

void GameLoop::Update(float deltaTime)
{
	Input::Update(deltaTime);
	
	MyButton* com = _myWindow->Update();
	if(!com)
		return;

	string label = com->GetLabel();


	if(label.compare("Edit Mode") == 0)
	{
		if(_editModeBtn->IsJustNowOn())
			DeActivatePhysics();
		else
			Edit();
	}
	else if(label.compare("Compound Mode") == 0)
	{
		EditInCompoundMode();
	}
	else if(label.compare("Physics Mode") == 0)
	{
		if(_physicsModeBtn->IsJustNowOn())
			ActivatePhysics();
		else
			EditInPhysicsMode(deltaTime);
	}

	// Options which can be used only in Edit mode...

	else if(_editModeBtn->IsOn())
	{
		if(label.compare("Save") == 0)
			WriteInFile(SaveFileDialog());
		else if(label.compare("Load") == 0)
			ReadFromFile(OpenFileDialog());
		else
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
}

void GameLoop::Edit()
{
	if(Input::IsMouseClicked())
		Select((float)Input::MouseX, (float)Input::MouseY);
	//else if(Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped('C'))
	//	Copy();
	//else if(Input::IsKeyPressed(KEYS_CONTROL) && Input::IsKeyTyped('V'))
	//	Paste();
	else if(Input::IsKeyPressed(KEYS_SHIFT) && Input::IsMousePressed())
	{
		float dy = (float)(Input::MouseY - Input::PrevMouseY);
		Cam::Z += 2*dy;
	}
	else if(Input::IsKeyPressed(KEYS_SHIFT) && Input::IsRightMousePressed())
	{
		float dx = (int)(Platform::MouseX() - Platform::PrevMousePosX);
		float dy = (int)(Platform::MouseY() - Platform::PrevMousePosY);

		Cam::X -= 3*dx;
		Cam::Y -= 3*dy;
	}
	else if(_activePrim)
	{
		if(_activePrim->Update())
		{
			_dataModified = true;
		}
	}
}

void GameLoop::EditInPhysicsMode(float deltaTime)
{
	PhysicsManager::GetInstance()->Update(deltaTime);
}

void GameLoop::AddOrDeleteOptions(string label)
{
	if(label.compare("Add Line") == 0)
	{
		LinePrim* prim = new LinePrim(_orthoX,_orthoY,_orthoX+50,_orthoY+50);
		AddPrim((Prim*)prim);
	}
	else if(label.compare("Add Rectangle") == 0)
	{
		RectPolyPrim* prim = new RectPolyPrim(_orthoX,_orthoY,75,50);
		AddPrim((Prim*)prim);
	}
	else if(label.compare("Add Circle") == 0)
	{
		CirclePrim* prim = new CirclePrim(25,25,25);
		AddPrim((Prim*)prim);
		prim->Move(_orthoX, _orthoY);
	}
	else if(label.compare("Add Triangle") == 0)
	{
		TriPrim* prim = new TriPrim(0,0,50,50,50,0);
		AddPrim((Prim*)prim);
		prim->Move(_orthoX, _orthoY);
	}
	else if(label.compare("Add Polygon 4") == 0)
	{
		vector<Point> vec;
		vec.push_back(Point(00, 30));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(50, 60));
		vec.push_back(Point(10, 60));

		PolyPrim* prim = new PolyPrim(vec);
		AddPrim((Prim*)prim);
		prim->Move(_orthoX, _orthoY);
	}
	else if(label.compare("Add Polygon 5") == 0)
	{
		vector<Point> vec;
		vec.push_back(Point(30, 00));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(60, 60));
		vec.push_back(Point(00, 60));
		vec.push_back(Point(00, 30));

		PolyPrim* prim = new PolyPrim(vec);
		AddPrim((Prim*)prim);
		prim->Move(_orthoX, _orthoY);
	}
	else if(label.compare("Add Polygon 6") == 0)
	{
		vector<Point> vec;
		vec.push_back(Point(30, 00));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(60, 60));
		vec.push_back(Point(30, 90));
		vec.push_back(Point(00, 60));
		vec.push_back(Point(00, 30));

		PolyPrim* prim = new PolyPrim(vec);
		AddPrim((Prim*)prim);
		prim->Move(_orthoX, _orthoY);
	}
	else if(label.compare("Add Polygon 7") == 0)
	{
		vector<Point> vec;
		vec.push_back(Point(30, 00));
		vec.push_back(Point(60, 30));
		vec.push_back(Point(60, 60));
		vec.push_back(Point(45, 90));
		vec.push_back(Point(15, 90));
		vec.push_back(Point(00, 60));
		vec.push_back(Point(00, 30));

		PolyPrim* prim = new PolyPrim(vec);
		AddPrim((Prim*)prim);
		prim->Move(_orthoX, _orthoY);
	}
	else if(label.compare("Add Polygon 8") == 0)
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

		PolyPrim* prim = new PolyPrim(vec);
		AddPrim((Prim*)prim);
		prim->Move(_orthoX, _orthoY);
	}
	else if(label.compare("Delete") == 0 || Input::IsKeyTyped(KEYS_DELETE))
	{
		for(int i=0;i<(int)_primVec.size();i++)
		{
			if(_primVec[i] == _activePrim)
			{
				delete _activePrim;
				_activePrim = NULL;
				_primVec.erase(_primVec.begin()+i);
				break;
			}
		}
	}
}

void GameLoop::EditInCompoundMode()
{
	if(Input::IsMouseClicked())
	{
		if( Select((float)Input::MouseX, (float)Input::MouseY) && _activePrim && Input::IsKeyPressed(KEYS_CONTROL) )
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

int GameLoop::GetNumCompoundPrims(int compoundShapeID)
{
	int count=0;

	for(int i=_primVec.size()-1; i>=0; i--)
	{
		if(_primVec[i]->GetCompoundShapeID() == compoundShapeID)
			count++;
	}
	return count;
}

bool GameLoop::Select(float x, float y)
{
	for(int i=_primVec.size()-1; i>=0; i--)
	{
		if(_primVec[i]->Contains(x,y))
		{
			if(_activePrim)
				_activePrim->SetSelect(false);

			_activePrim = _primVec[i];
			_activePrim->SetSelect(true);
			return true;
		}
	}
	return false;
}

bool GameLoop::Copy()
{
	if(_activePrim)
	{
		_copyPrim = _activePrim->Clone();
		return true;
	}
	return false;
}

bool GameLoop::Paste()
{
	if(_copyPrim)
	{
		if(_activePrim)
			_activePrim->SetSelect(false);

		_copyPrim->SetSelect(true);
		_copyPrim->Move(2,0);
		_primVec.push_back(_copyPrim);

		_activePrim = _copyPrim;
		return true;
	}
	return false;
}

void GameLoop::AddPrim(Prim* prim)
{
	if(_primVec.size()==0)
	{
		prim->SetSelect(true);
		_activePrim = prim;
	}
	_primVec.push_back((Prim*)prim);
}

void GameLoop::Draw()
{
	glClearColor(0.5,0.5,0.5,0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport( 0, 0, Platform::GetWindowWidth(), Platform::GetWindowHeight());

	float x = 0;
	float y = 0;
	float w = (float)Platform::GetWindowWidth();
	float h = (float)Platform::GetWindowHeight();
	float hByW = h/w;

	x += Cam::X;
	y += Cam::Y;

	x -= Cam::Z;
	y -= Cam::Z*hByW;

	w += 2*Cam::Z;
	h += 2*Cam::Z*hByW;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( x, x+w, y+h, y, -1, 1);

	_orthoX = x;
	_orthoY = y;
	_orthoW = w;
	_orthoH = h;

	if(_activatePhysics)
	{
		PhysicsManager::GetInstance()->Draw();
	}
	else
	{
		for(int i=0;i<(int)_primVec.size();i++)
			_primVec[i]->Draw();
	}

	_myWindow->Draw();
}

void GameLoop::ActivatePhysics()
{
	vector<PhyCompoundShape*> _compoundShapeVec;
	_compoundShapeVec.clear();

	for(int i=0;i<=_compoundShapeID;i++)
	{
		PhyCompoundShape* phyComShape = new PhyCompoundShape();
		_compoundShapeVec.push_back(phyComShape);
	}

	int bodyType = 0;

	for(int i=0;i<(int)_primVec.size();i++)
	{
		if(_primVec[i]->GetCompoundShapeID() == -1 )
		{
			if( _primVec[i]->ID == Prim::LINE)
			{
				LinePrim* linePrim = (LinePrim*)_primVec[i];

				if( linePrim->IsDynamicBody())
					bodyType = PhyShape::DYNAMIC_BODY;
				else if( linePrim->IsStaticBody())
					bodyType = PhyShape::STATIC_BODY;

				PhysicsManager::GetInstance()->AddLine(
					linePrim->_x[0], linePrim->_y[0], linePrim->_x[1], linePrim->_y[1], bodyType);
			}
			else if( _primVec[i]->ID == Prim::TRIANGLE)
			{
				TriPrim* triPrim = (TriPrim*)_primVec[i];

				if( triPrim->IsDynamicBody())
					bodyType = PhyShape::DYNAMIC_BODY;
				else if( triPrim->IsStaticBody())
					bodyType = PhyShape::STATIC_BODY;

				PhysicsManager::GetInstance()->AddTriangle( 
					triPrim->_x[0], triPrim->_y[0], triPrim->_x[1], triPrim->_y[1], triPrim->_x[2], triPrim->_y[2], bodyType);
			}
			else if( _primVec[i]->ID == Prim::CIRCLE)
			{
				CirclePrim* cirPrim = (CirclePrim*)_primVec[i];

				if( cirPrim->IsDynamicBody())
					bodyType = PhyShape::DYNAMIC_BODY;
				else if( cirPrim->IsStaticBody())
					bodyType = PhyShape::STATIC_BODY;

				PhysicsManager::GetInstance()->AddCircle(cirPrim->_cx, cirPrim->_cy, cirPrim->_radius, bodyType);
			}
			else if( _primVec[i]->ID == Prim::RECT_POLY_PRIM)
			{
				RectPolyPrim* prim = (RectPolyPrim*)_primVec[i];

				if( prim->IsDynamicBody())
					bodyType = PhyShape::DYNAMIC_BODY;
				else if( prim->IsStaticBody())
					bodyType = PhyShape::STATIC_BODY;

				PhysicsManager::GetInstance()->AddPolygon(prim->_vec, bodyType);
			}
			else if( _primVec[i]->ID == Prim::POLY)
			{
				PolyPrim* polyPrim = (PolyPrim*)_primVec[i];

				if( polyPrim->IsDynamicBody())
					bodyType = PhyShape::DYNAMIC_BODY;
				else if( polyPrim->IsStaticBody())
					bodyType = PhyShape::STATIC_BODY;

				PhysicsManager::GetInstance()->AddPolygon(polyPrim->_vec, bodyType);
			}
		}
		else if(_primVec[i]->GetCompoundShapeID() >= 0)
		{
			int compoundID = _primVec[i]->GetCompoundShapeID();

			if( _primVec[i]->ID == Prim::CIRCLE)
			{
				CirclePrim* circlePrim = (CirclePrim*)_primVec[i];
				_compoundShapeVec[compoundID]->AddCircle(circlePrim->_cx, circlePrim->_cy, circlePrim->_radius);
			}
			else if( _primVec[i]->ID == Prim::TRIANGLE)
			{
				TriPrim* triPrim = (TriPrim*)_primVec[i];
				_compoundShapeVec[compoundID]->AddTriangle(triPrim->_x[0], triPrim->_y[0], triPrim->_x[1], triPrim->_y[1], triPrim->_x[2], triPrim->_y[2]);
			}
			else if( _primVec[i]->ID == Prim::RECT_POLY_PRIM)
			{
				RectPolyPrim* rectPolyPrim = (RectPolyPrim*)_primVec[i];
				_compoundShapeVec[compoundID]->AddPoly(rectPolyPrim->_vec);
			}
			else if( _primVec[i]->ID == Prim::POLY)
			{
				PolyPrim* polyPrim = (PolyPrim*)_primVec[i];
				_compoundShapeVec[compoundID]->AddPoly(polyPrim->_vec);
			}

			if(_primVec[i]->IsStaticBody())
				_compoundShapeVec[compoundID]->SetBodyType(PhyShape::STATIC_BODY);
		}
	}

	for(int i=0;i<(int)_compoundShapeVec.size();i++)
	{
		if(_compoundShapeVec[i]->GetNumParts()>0)
			PhysicsManager::GetInstance()->AddCompoundShape(_compoundShapeVec[i]);
		else
		{
			delete _compoundShapeVec[i];
			_compoundShapeVec[i] = NULL;
		}
	}

	_compoundShapeVec.clear();

	_activatePhysics = true;
}


void GameLoop::DeActivatePhysics()
{
	PhysicsManager::GetInstance()->RemoveBodies();
	_activatePhysics = false;
}

void GameLoop::ReadFromFile(string filePath)
{
	if(filePath.length() == 0)
		return;

	CheckForSaveBeforeLoad();
	
	//clear all existing primitives before load new file.
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
	loadFile.LoadFile(filePath.c_str());
	
	vector<string> numPrimsVec = loadFile.GetTagData("NumPrims", true);
	int numPrims = atoi(numPrimsVec[0].c_str());

	vector<string> primDataVec = loadFile.GetTagData("Primitives", true);
	
	char str[128];

	for(int i=0; i<(int)primDataVec.size(); i++)
	{
		if(primDataVec[i].find("Circle") != -1)
		{
			int isDynamic;
			float cx,cy,r;
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f", str, &isDynamic, &cx, &cy, &r);

			CirclePrim* prim = new CirclePrim(cx,cy,r);
			AddPrim((Prim*)prim);

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;

		}
		else if(primDataVec[i].find("Triangle") != -1)
		{
			int isDynamic;
			float x[3],y[3];
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f %f %f %f", str, &isDynamic,	&x[0], &y[0],
																						&x[1], &y[1],
																						&x[2], &y[2]);

			TriPrim* prim = new TriPrim(0,0,0,0,0,0);
			AddPrim((Prim*)prim);

			for(int i=0; i<3; i++)
			{
				prim->_x[i] = x[i];
				prim->_y[i] = y[i];
			}

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;

		}
		else if(primDataVec[i].find("RectPoly") != -1)
		{
			int isDynamic;
			float x[4],y[4];
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f %f %f %f %f %f", str, &isDynamic,	&x[0], &y[0],
																								&x[1], &y[1],
																								&x[2], &y[2],
																								&x[3], &y[3]);

			vector<Point> vec;
			for(int i=0; i<4; i++)
				vec.push_back(Point(x[i], y[i]));

			RectPolyPrim* prim = new RectPolyPrim(vec);
			AddPrim((Prim*)prim);

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;
		}
		else if(primDataVec[i].find("Poly4") != -1)
		{
			int isDynamic;
			float x[4],y[4];
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f %f %f %f %f %f", str, &isDynamic,	&x[0], &y[0],
																								&x[1], &y[1],
																								&x[2], &y[2], 
																								&x[3], &y[3]);
			vector<Point> vec;
			for(int i=0; i<4; i++)
				vec.push_back(Point(x[i], y[i]));

			PolyPrim* prim = new PolyPrim(vec);
			AddPrim((Prim*)prim);

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;
		}
		else if(primDataVec[i].find("Poly5") != -1)
		{
			int isDynamic;
			float x[5],y[5];
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f %f %f %f %f %f %f %f", str, &isDynamic,	&x[0], &y[0],
																									&x[1], &y[1],
																									&x[2], &y[2],
																									&x[3], &y[3],
																									&x[4], &y[4]);
			vector<Point> vec;
			for(int i=0; i<5; i++)
				vec.push_back(Point(x[i], y[i]));

			PolyPrim* prim = new PolyPrim(vec);
			AddPrim((Prim*)prim);

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;
		}
		else if(primDataVec[i].find("Poly6") != -1)
		{
			int isDynamic;
			float x[6],y[6];
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f %f %f %f %f %f %f %f %f %f", str, &isDynamic,	&x[0], &y[0],
																											&x[1], &y[1],
																											&x[2], &y[2],
																											&x[3], &y[3],
																											&x[4], &y[4],
																											&x[5], &y[5]);
			vector<Point> vec;
			for(int i=0; i<6; i++)
				vec.push_back(Point(x[i], y[i]));

			PolyPrim* prim = new PolyPrim(vec);
			AddPrim((Prim*)prim);

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;
		}
		else if(primDataVec[i].find("Poly7") != -1)
		{
			int isDynamic;
			float x[7],y[7];
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f", str, &isDynamic,	&x[0], &y[0],
																												&x[1], &y[1],
																												&x[2], &y[2],
																												&x[3], &y[3],
																												&x[4], &y[4],
																												&x[5], &y[5],
																												&x[6], &y[6]);
			vector<Point> vec;
			for(int i=0; i<7; i++)
				vec.push_back(Point(x[i], y[i]));

			PolyPrim* prim = new PolyPrim(vec);
			AddPrim((Prim*)prim);

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;
		}
		else if(primDataVec[i].find("Poly8") != -1)
		{
			int isDynamic;
			float x[8],y[8];
			sscanf( primDataVec[i].c_str(),"%s %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", str, &isDynamic,	&x[0], &y[0],
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

			PolyPrim* prim = new PolyPrim(vec);
			AddPrim((Prim*)prim);

			if(isDynamic == 1)	prim->_isDynamicBody = true;
			else				prim->_isDynamicBody = false;
		}
	}

	_currFilePath = filePath;
	Platform::SetWindowTitle(_currFilePath);
}

void GameLoop::CheckForSaveBeforeLoad()
{
	if(_primVec.size() > 0 && _dataModified)
	{
		int answer = MessageBox(NULL, "Do you want to save", NULL, MB_OKCANCEL);

		if(answer == IDOK)
		{
			if(_currFilePath.length() > 0)
				WriteInFile(_currFilePath);
			else
				WriteInFile(SaveFileDialog());
		}
		_dataModified = false;
	}
}

void GameLoop::WriteInFile(string filePath)
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
		Prim* prim = _primVec[i];

		if(prim->ID == Prim::TRIANGLE)
		{
			TriPrim* triPrim = (TriPrim*)prim;

			int isDynamic = 1;
			if( triPrim->IsDynamicBody() == false)
				isDynamic = 0;

			sprintf(str, "Triangle");
			fprintf(fp, "\n%s %d %f %f %f %f %f %f",str, isDynamic,	triPrim->_x[0], triPrim->_y[0],
																	triPrim->_x[1], triPrim->_y[1],
																	triPrim->_x[2], triPrim->_y[2]);
		}
		else if(prim->ID == Prim::CIRCLE)
		{
			CirclePrim* cirPrim = (CirclePrim*)prim;

			int isDynamic = 1;
			if( cirPrim->IsDynamicBody() == false)
				isDynamic = 0;

			sprintf(str, "Circle");
			fprintf(fp, "\n%s %d %d %d %d",str, isDynamic, (int)cirPrim->_cx, (int)cirPrim->_cy, (int)cirPrim->_radius );
		}
		else if(prim->ID == Prim::RECT_POLY_PRIM)
		{
			RectPolyPrim* rectPolyPrim = (RectPolyPrim*)prim;

			int isDynamic = 1;
			if( rectPolyPrim->IsDynamicBody() == false)
				isDynamic = 0;

			sprintf(str, "RectPoly");
			fprintf(fp, "\n%s %d %f %f %f %f %f %f %f %f",str, isDynamic,	rectPolyPrim->_vec[0].x, rectPolyPrim->_vec[0].y,
																			rectPolyPrim->_vec[1].x, rectPolyPrim->_vec[1].y,
																			rectPolyPrim->_vec[2].x, rectPolyPrim->_vec[2].y,
																			rectPolyPrim->_vec[3].x, rectPolyPrim->_vec[3].y);
		}
		else if(prim->ID == Prim::POLY)
		{
			WritePolyInFile(fp, prim);
		}
	}

	fprintf(fp, "\n</Primitives>");

	fclose(fp);
}


void GameLoop::WritePolyInFile(FILE* fp, Prim* prim)
{
	PolyPrim* polyPrim = (PolyPrim*)prim;

	int isDynamic = 1;
	if( polyPrim->IsDynamicBody() == false)
		isDynamic = 0;

	char str[128];

	int size = polyPrim->_vec.size();
	if(size == 4)
	{
		sprintf(str, "Poly4");
		fprintf(fp, "\n%s %d %f %f %f %f %f %f %f %f",str, isDynamic,	polyPrim->_vec[0].x, polyPrim->_vec[0].y,
																		polyPrim->_vec[1].x, polyPrim->_vec[1].y,
																		polyPrim->_vec[2].x, polyPrim->_vec[2].y,
																		polyPrim->_vec[3].x, polyPrim->_vec[3].y);
	}
	else if(size == 5)
	{
		sprintf(str, "Poly5");
		fprintf(fp, "\n%s %d %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->_vec[0].x, polyPrim->_vec[0].y,
																			polyPrim->_vec[1].x, polyPrim->_vec[1].y,
																			polyPrim->_vec[2].x, polyPrim->_vec[2].y,
																			polyPrim->_vec[3].x, polyPrim->_vec[3].y,
																			polyPrim->_vec[4].x, polyPrim->_vec[4].y);
	}
	else if(size == 6)
	{
		sprintf(str, "Poly6");
		fprintf(fp, "\n%s %d %f %f %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->_vec[0].x, polyPrim->_vec[0].y,
																				polyPrim->_vec[1].x, polyPrim->_vec[1].y,
																				polyPrim->_vec[2].x, polyPrim->_vec[2].y,
																				polyPrim->_vec[3].x, polyPrim->_vec[3].y,
																				polyPrim->_vec[4].x, polyPrim->_vec[4].y,
																				polyPrim->_vec[5].x, polyPrim->_vec[5].y);
	}
	else if(size == 7)
	{
		sprintf(str, "Poly7");
		fprintf(fp, "\n%s %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->_vec[0].x, polyPrim->_vec[0].y,
																						polyPrim->_vec[1].x, polyPrim->_vec[1].y,
																						polyPrim->_vec[2].x, polyPrim->_vec[2].y,
																						polyPrim->_vec[3].x, polyPrim->_vec[3].y,
																						polyPrim->_vec[4].x, polyPrim->_vec[4].y,
																						polyPrim->_vec[5].x, polyPrim->_vec[5].y,
																						polyPrim->_vec[6].x, polyPrim->_vec[6].y);
	}
	else if(size == 8)
	{
		sprintf(str, "Poly8");
		fprintf(fp, "\n%s %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",str, isDynamic, polyPrim->_vec[0].x, polyPrim->_vec[0].y,
																							polyPrim->_vec[1].x, polyPrim->_vec[1].y,
																							polyPrim->_vec[2].x, polyPrim->_vec[2].y,
																							polyPrim->_vec[3].x, polyPrim->_vec[3].y,
																							polyPrim->_vec[4].x, polyPrim->_vec[4].y,
																							polyPrim->_vec[5].x, polyPrim->_vec[5].y,
																							polyPrim->_vec[6].x, polyPrim->_vec[6].y,
																							polyPrim->_vec[7].x, polyPrim->_vec[7].y);
	}
}


string GameLoop::OpenFileDialog(char *filter, HWND owner) 
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	 
	string fileNameStr;
	 
	if ( GetOpenFileName(&ofn) )
	fileNameStr = fileName;
	 
	return fileNameStr;
}


string GameLoop::SaveFileDialog(char *filter, HWND owner) 
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	string fileNameStr;

	if(GetSaveFileNameA(&ofn))
		fileNameStr = fileName;

	return fileNameStr;
}

GameLoop::~GameLoop()
{
	if(_myWindow)
	{
		delete _myWindow;
		_myWindow = NULL;
	}

	if(_copyPrim)
	{
		delete _copyPrim;
		_copyPrim = NULL;
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


	if(_gameInit)
	{
		delete _gameInit;
		_gameInit = NULL;
	}

	Platform::Terminate();
	Platform::DeviceExit();
}
