#include "MyWindow.h"
#include "../CommonLib/Input.h"

MyWindow::MyWindow(){}
MyWindow::~MyWindow(){}

void MyWindow::Init(float x, float y)
{
	_x = x;
	_y = y;
	_w = 0;
	_h = 0;

	_btnDefaultHeight = 24;

	_startX = 5;
	_startY = 30;

	_goToNextRow = false;

	_horGapBtwComponents = 4;
	_VerGapBtwComponents = 4;

	//_prevOnCom = GetActiveModeCom();
	_isMoving = false;
	_justNowOnBtn = NULL;
	_isOn = true;
}


Button* MyWindow::Update()
{
	if(_isOn == false)
		return NULL;

	if(Input::IsMousePressed())
	{
		if(!_isMoving)
		{
			float x = Input::MouseX;
			float y = Input::MouseY;

			if(x>=_x&& x<=_x+_w && y>=_y && y<= _y+_startY)
			{
				_isMoving = true;
			}
		}
		else
		{
			float x1 = Input::PrevMouseX;
			float y1 = Input::PrevMouseY;
			float x2 = Input::MouseX;
			float y2 = Input::MouseY;

			MovePos(x2-x1, y2-y1);
			return NULL;
		}
	}
	else
	{
		_isMoving = false;
	}

	Button* dialogBtn = GetActiveDialog();

	if(dialogBtn)
	{
		if(Input::IsMouseReleased())
		{
			if(dialogBtn->Contains(Input::MouseX, Input::MouseY))
			{
				dialogBtn->Off();
			}
		}
		return dialogBtn;
	}
	
	Button* activeCom = GetActiveModeCom();

	if(Input::IsMouseReleased())
	{
		Button* com = GetCom(Input::MouseX, Input::MouseY);
		if(com)
		{
			if(com->GetType() == Button::PROPERTY)
			{
				return com;
			}
			else if(com->GetType() == Button::ON_OFF)
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

			else if(com != activeCom)
			{
				com->On();				//action performed but need to check previouse component switched off or not.
				_justNowOnBtn = com;

				if(activeCom)
				{
					activeCom->Off();	//action performed here.. so sending this as return value.
					return activeCom;
				}
				else
				{
					return com;			// If there is not previous button in switch on state then return this.
				}
			}
		}
	}

	if(_justNowOnBtn)
	{
		Button* temp = _justNowOnBtn;
		_justNowOnBtn->JustNowOn();
		_justNowOnBtn = NULL;
		return temp;
	}

	return activeCom;
}

void MyWindow::On()
{
	_isOn = true;
}

void MyWindow::Off()
{
	_isOn = false;
}

bool MyWindow::IsOn()
{
	return _isOn;
}


Button* MyWindow::AddButton(string label, int type)
{
	Button* button = new Button();

	float _componentBorderGap = 10;

	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;

	if(_componentVec.size() == 0)
	{
		x = _x+_startX;
		y = _y+_startY;
		w = UIFont::GetInstance()->GetLength(label,0,18,18,-4) + 2*_componentBorderGap;
		h = _btnDefaultHeight;

		button->Init(Button::PINK, x, y, w, h);
	}
	else
	{
		w = UIFont::GetInstance()->GetLength(label,0,18,18,-4) + 2*_componentBorderGap;
		h = _btnDefaultHeight;

		if(_goToNextRow)
		{
			x = _x+_startX;
			y = _y+_h;
			_goToNextRow = false;
		}
		else
		{
			Button* btn = _componentVec[_componentVec.size()-1];

			x = btn->GetX() + btn->GetWidth() + _horGapBtwComponents;
			y = btn->GetY();
		}

		button->Init(Button::PINK, x, y, w, h);
	}

	// Finding MaxWidth and MaxHeight...
	if( x+w-_x > _w){
		_w = x+w+5 - _x; 
	}
	
	if( y+h-_y > _h){
		_h = y+h+_VerGapBtwComponents - _y;
	}

	button->SetLabel(label, 0x000000ff, -4);
	button->SetType(type);
	_componentVec.push_back(button);

	return button;
}

void MyWindow::NextRow()
{
	_goToNextRow = true;
}


Button* MyWindow::GetActiveDialog()
{
	for(int i=0; i<_componentVec.size(); i++)
		if(_componentVec[i]->GetType() == Button::DIALOG && _componentVec[i]->IsOn())
			return _componentVec[i];
	return NULL;
}

Button* MyWindow::GetActiveModeCom()
{
	for(int i=0; i<_componentVec.size(); i++)
		if(_componentVec[i]->GetType() == Button::MODE && _componentVec[i]->IsOn())
			return _componentVec[i];
	return NULL;
}

Button* MyWindow::GetCom(int x, int y)
{
	for(int i=0; i<_componentVec.size(); i++)
		if(_componentVec[i]->Contains(x, y))
			return _componentVec[i];
	return NULL;
}

void MyWindow::MovePos(float dx, float dy)
{
	_x += dx;
	_y += dy;

	for(int i=0;i<_componentVec.size();i++){
		_componentVec[i]->SetPos(_componentVec[i]->GetX()+dx, _componentVec[i]->GetY()+dy);
	}
}

bool MyWindow::Contains(float x, float y)
{
	return ( x>= _x && x<= _x+_w && y>= _y && y<= _y+_h);
}

void MyWindow::Draw()
{
	if(_isOn)
	{
		RectPrim::Draw(_x, _y, _w, _h, 220, 220, 220, 255, false);
		for(int i=0;i<_componentVec.size();i++){
			_componentVec[i]->Draw();
		}
	}
}

void MyWindow::Terminate()
{
	for(int i=0;i<_componentVec.size();i++)
	{
		_componentVec[i]->Terminate();
		delete _componentVec[i];
		_componentVec[i] = NULL;
	}
}
