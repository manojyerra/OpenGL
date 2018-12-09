#include "MyWindow.h"
#include "../CommonLib/Input.h"

MyWindow::MyWindow(float x, float y)
{
	_x = x;
	_y = y;
	_w = 0;
	_h = 0;

	_btnDefaultHeight = 25;

	_startX = 5;
	_startY = 30;

	_goToNextRow = false;

	_horGapBtwComponents = 5;
	_VerGapBtwComponents = 2;

	//_prevOnCom = GetActiveModeCom();
	_isMoving = false;
	_justNowOnBtn = NULL;
}


MyButton* MyWindow::Update()
{
	if(Input::IsMousePressed())
	{
		if(!_isMoving)
		{
			float x = (float)Input::MouseX;
			float y = (float)Input::MouseY;

			if(x>=_x&& x<=_x+_w && y>=_y && y<= _y+_startY)
			{
				_isMoving = true;
			}
		}
		else
		{
			float x1 = (float)Input::PrevMouseX;
			float y1 = (float)Input::PrevMouseY;
			float x2 = (float)Input::MouseX;
			float y2 = (float)Input::MouseY;

			MovePos(x2-x1, y2-y1);
			return NULL;
		}
	}
	else
	{
		_isMoving = false;
	}

	if(Input::IsMouseReleased())
	{
		MyButton* com = GetCom(Input::MouseX, Input::MouseY);

		if(com)
		{
			if(com->GetType() == MyButton::PROPERTY)
			{
				return com;
			}
			else if(com->GetType() == MyButton::ON_OFF)
			{
				if(com->IsOn())
					com->Off();
				else
				{
					com->On();
					com->JustNowOn();
				}
				return com;
			}
		}
	}

	return NULL;
}



MyButton* MyWindow::AddButton(string label, int type)
{
	MyButton* button = NULL;

	float _componentBorderGap = 10;

	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;

	float fontW = 16;
	float fontH = 16;

	if(_componentVec.size() == 0)
	{
		x = _x+_startX;
		y = _y+_startY;
		w = UIFont::GetInstance()->GetLength(label,fontH,0) + 2*_componentBorderGap;
		h = _btnDefaultHeight;

		button = new MyButton(MyButton::PINK, x, y, w, h);
	}
	else
	{
		w = UIFont::GetInstance()->GetLength(label,fontH,0) + 2*_componentBorderGap;
		h = _btnDefaultHeight;

		if(_goToNextRow)
		{
			x = _x+_startX;
			y = _y+_h;
			_goToNextRow = false;
		}
		else
		{
			MyButton* btn = _componentVec[_componentVec.size()-1];

			x = btn->GetX() + btn->GetWidth() + _horGapBtwComponents;
			y = btn->GetY();
		}

		button = new MyButton(MyButton::PINK, x, y, w, h);
	}

	// Finding MaxWidth and MaxHeight...
	if( x+w-_x > _w){
		_w = x+w+5 - _x; 
	}
	
	if( y+h-_y > _h){
		_h = y+h+_VerGapBtwComponents - _y;
	}

	button->SetLabel(label, 0x000000ff, 0);
	button->SetLabelFontSize(fontW, fontH);

	button->SetType(type);
	_componentVec.push_back(button);

	return button;
}

void MyWindow::NextRow()
{
	_goToNextRow = true;
}


MyButton* MyWindow::GetActiveDialog()
{
	for(int i=0; i<(int)_componentVec.size(); i++)
		if(_componentVec[i]->GetType() == MyButton::DIALOG && _componentVec[i]->IsOn())
			return _componentVec[i];
	return NULL;
}

MyButton* MyWindow::GetActiveModeCom()
{
	for(int i=0; i<(int)_componentVec.size(); i++)
		if(_componentVec[i]->GetType() == MyButton::MODE && _componentVec[i]->IsOn())
			return _componentVec[i];
	return NULL;
}

MyButton* MyWindow::GetCom(int x, int y)
{
	for(int i=0; i<(int)_componentVec.size(); i++)
		if(_componentVec[i]->Contains((float)x, (float)y))
			return _componentVec[i];
	return NULL;
}

void MyWindow::MovePos(float dx, float dy)
{
	_x += dx;
	_y += dy;

	for(int i=0;i<(int)_componentVec.size();i++){
		_componentVec[i]->SetPos(_componentVec[i]->GetX()+dx, _componentVec[i]->GetY()+dy);
	}
}

bool MyWindow::Contains(float x, float y)
{
	return ( x>= _x && x<= _x+_w && y>= _y && y<= _y+_h);
}

bool MyWindow::IsMoving()
{
	return _isMoving;
}

void MyWindow::Draw()
{
	for(int i=0;i<(int)_componentVec.size();i++){
		_componentVec[i]->Draw();
	}
}

MyWindow::~MyWindow()
{
	for(int i=0;i<(int)_componentVec.size();i++)
	{
		delete _componentVec[i];
		_componentVec[i] = NULL;
	}
	_componentVec.clear();
}
