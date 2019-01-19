#include "SUIChoice.h"
#include "SUIIncludes.h"
#include "SUIInput.h"
#include "SUIEvents.h"
#include "SUIManager.h"
#include "SUIFont.h"

namespace SUI
{
	SUIChoice::SUIChoice(int elementsToShow) : SUIComponent(SUI_CHOICE)
	{
		_selectedStr = "";
		_SUIList = new SUIList(elementsToShow);
		_SUIList->SetPos(_x, _y);
		_SUIList->SetWidth(_w);
		_SUIList->SetBgVisible(true);
		_SUIList->SetBorderVisible(true);
		_SUIList->SetBorderColor(10, 10, 155, 255);
		_SUIList->DisableScissor();
		_SUIList->AddActionListener((SUIActionListener*)(void*)0xccffccff);

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

	SUIChoice::~SUIChoice()
	{
		if (_SUIList)
		{
			delete _SUIList;
			_SUIList = NULL;
		}
	}

	void SUIChoice::Add(string name)
	{
		_SUIList->Add(name);
		_selectedStr = _SUIList->GetItem(_SUIList->GetSelectedIndex());
	}

	void SUIChoice::Move(float dx, float dy)
	{
		_x += dx;
		_y += dy;

		_SUIList->Move(dx, dy);
	}

	void SUIChoice::ResetBounds()
	{
		_SUIList->SetPos(_x, _y + _h);
		_SUIList->SetWidth(_w);

		_SUIList->ResetBounds();
	}

	bool SUIChoice::Contains(float x, float y)
	{
		return (x >= _x && x <= _x + _w && y >= _y && y <= _y + _h);
	}

	bool SUIChoice::IsDialogMode()
	{
		return _isListActive;
	}

	void SUIChoice::UpdateDialog()
	{
		if (SUIInput::IsMouseClicked())
		{
			if (_SUIList->Contains((float)SUIInput::MX, (float)SUIInput::MY) == false)
			{
				_isListActive = false;
				return;
			}
		}

		SUIEvents SUIEvents = _SUIList->UpdateByInput();

		if (SUIEvents.ACTION_PERFORMED)
		{
			_isListActive = false;
			_selectedStr = _SUIList->GetItem(_SUIList->GetSelectedIndex());
			_actionPerformed = true;
		}
	}

	void SUIChoice::DrawDialog()
	{
		_SUIList->Draw();
	}

	SUIEvents SUIChoice::UpdateByInput()
	{
		SUIEvents eventsVec;

		if (SUIInput::IsMouseClicked())
		{
			if (!_isListActive)
			{
				if (Contains((float)SUIInput::MX, (float)SUIInput::MY))
				{
					_isListActive = true;
					SUIManager::GetInstance()->SetDialogCom(this);
				}
			}
		}

		if (_actionPerformed)
		{
			_actionPerformed = false;
			eventsVec.ACTION_PERFORMED = true;
		}

		return eventsVec;
	}

	void SUIChoice::SetSelect(string itemName)
	{
		for (int i = 0; i < _SUIList->Size(); i++)
		{
			string str = _SUIList->GetItem(i);

			if (str.compare(itemName) == 0)
			{
				SetSelect(i);
				break;
			}
		}
	}

	void SUIChoice::SetSelect(int index)
	{
		if (index < 0)
			index = 0;

		_SUIList->SetSelect(index);
		_selectedStr = _SUIList->GetItem(_SUIList->GetSelectedIndex());
	}

	string SUIChoice::GetSelectedStr()
	{
		return _SUIList->GetSelectedStr();
	}

	string SUIChoice::GetItem(int index)
	{
		return _SUIList->GetItem(index);
	}

	int SUIChoice::GetSelectedIndex()
	{
		return _SUIList->GetSelectedIndex();
	}

	void SUIChoice::Draw()
	{
		if (_isBgVisible)
			DrawBackground();

		float fontSize = SUIFont::GetInstance()->GetFontSize();
		SUIFont::GetInstance()->HorBorder(_x, _x + _w);
		SUIFont::GetInstance()->SetColor(0xffffffff);
		SUIFont::GetInstance()->Begin();
		SUIFont::GetInstance()->DrawFromLeft(_selectedStr, _x + 1, _y + _h / 2, fontSize);
		SUIFont::GetInstance()->End();

		float triCY = _y + _h / 2.0f;
		float triCX = _x + _w - 10.0f;
		float triR = _h / 4.50f;

		if (triCX - triR > _x)
		{
			glColor4ub(0, 0, 0, 255);
			glBegin(GL_TRIANGLES);
			glVertex2f(triCX - triR, triCY - triR);
			glVertex2f(triCX + triR, triCY - triR);
			glVertex2f(triCX, triCY + triR);
			glEnd();
		}


		if (_isBorderVisible)
			DrawBorder();
	}
}