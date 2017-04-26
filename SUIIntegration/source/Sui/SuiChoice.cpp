#include "SuiChoice.h"
#include "SuiIncludes.h"
#include "SuiInput.h"
#include "SuiEvents.h"
#include "SuiManager.h"
#include "SuiFont.h"

SuiChoice::SuiChoice(int elementsToShow) : SuiComponent(SUI_CHOICE)
{
	_selectedStr = "";
	_suiList = new SuiList(elementsToShow);
	_suiList->SetPos(_x,_y);
	_suiList->SetWidth(_w);
	_suiList->SetBgVisible(true);
	_suiList->SetBorderVisible(true);
	_suiList->SetBorderColor(10,10,155,255);
	_suiList->DisableScissor();
	_suiList->AddActionListener((SuiActionListener*)(void*)0xccffccff);

	_isBgVisible = true;
	_isBorderVisible = true;

	_bgR = 160;
	_bgG = 160;
	_bgB = 160;
	_bgA = 255;

	_h = 22;

	_isListActive = false;
	_actionPerformed = false;
	_actionListener = NULL;
	_mouseListener = NULL;
}

SuiChoice::~SuiChoice()
{
	if(_suiList)
	{
		delete _suiList;
		_suiList = NULL;
	}
}

void SuiChoice::Add(string name)
{
	_suiList->Add(name);
	_selectedStr = _suiList->GetItem(_suiList->GetSelectedIndex());
}

void SuiChoice::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	_suiList->Move(dx, dy);
}

void SuiChoice::ResetBounds()
{
	_suiList->SetPos(_x, _y+_h);
	_suiList->SetWidth(_w);

	_suiList->ResetBounds();
}

bool SuiChoice::Contains(float x, float y)
{
	return (x >= _x && x <= _x+_w && y >= _y && y <= _y+_h);
}

bool SuiChoice::IsDialogMode()
{
	return _isListActive;
}

void SuiChoice::UpdateDialog()
{
	if(SuiInput::IsMouseClicked())
	{
		if(_suiList->Contains((float)SuiInput::MX, (float)SuiInput::MY) == false)
		{
			_isListActive = false;
			return;
		}		
	}

	SuiEvents suiEvents = _suiList->UpdateByInput();
	
	if(suiEvents.ACTION_PERFORMED)
	{
		_isListActive = false;
		_selectedStr = _suiList->GetItem(_suiList->GetSelectedIndex());
		_actionPerformed = true;
	}
}

void SuiChoice::DrawDialog()
{
	_suiList->Draw();
}

SuiEvents SuiChoice::UpdateByInput()
{
	SuiEvents eventsVec;
	
	if(SuiInput::IsMouseClicked())
	{
		if(!_isListActive)
		{
			if(Contains(SuiInput::MX, SuiInput::MY))
			{
				_isListActive = true;
				SuiManager::GetInstance()->SetDialogCom(this);
			}
		}
	}

	if(_actionPerformed)
	{
		_actionPerformed = false;
		eventsVec.ACTION_PERFORMED = true;
	}

	return eventsVec;
}

void SuiChoice::SetSelect(string itemName)
{
	for(int i=0; i<_suiList->Size(); i++)
	{
		string str = _suiList->GetItem(i);

		if(str.compare(itemName) == 0)
		{
			SetSelect(i);
			break;
		}
	}
}

void SuiChoice::SetSelect(int index)
{
	if(index < 0)
		index = 0;

	_suiList->SetSelect(index);
	_selectedStr = _suiList->GetItem(_suiList->GetSelectedIndex());
}

string SuiChoice::GetSelectedStr()
{
	return _suiList->GetSelectedStr();
}

string SuiChoice::GetItem(int index)
{
	return _suiList->GetItem(index);
}

int SuiChoice::GetSelectedIndex()
{
	return _suiList->GetSelectedIndex();
}

void SuiChoice::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	float fontSize = SuiFont::GetInstance()->GetFontSize();
	SuiFont::GetInstance()->HorBorder(_x, _x+_w);
	SuiFont::GetInstance()->SetColor(0xffffffff);
	SuiFont::GetInstance()->Begin();
	SuiFont::GetInstance()->DrawFromLeft(_selectedStr, _x+1, _y+_h/2, fontSize);
	SuiFont::GetInstance()->End();

	float triCY = _y + _h/2.0f;
	float triCX = _x + _w - 10.0f;
	float triR = _h/4.50f;

	if(triCX - triR > _x )
	{
		glColor4ub(0, 0, 0, 255);
		glBegin(GL_TRIANGLES);
		glVertex2f(triCX-triR,	triCY-triR	);
		glVertex2f(triCX+triR,	triCY-triR	);
		glVertex2f(triCX,		triCY+triR	);
		glEnd();
	}


	if(_isBorderVisible)
		DrawBorder();
}